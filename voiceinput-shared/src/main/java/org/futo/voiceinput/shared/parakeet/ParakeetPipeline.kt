package org.futo.voiceinput.shared.parakeet

import kotlinx.coroutines.withContext
import org.futo.voiceinput.shared.ggml.ParakeetQnn
import org.futo.voiceinput.shared.ggml.inferenceContext
import kotlin.math.min

// The S25 Ultra's QNN HTP context binary is compiled for exactly these dimensions.
private const val SAMPLE_RATE = 16_000
private const val ENCODER_INPUT_SAMPLES = 96_000 // 6s @ 16kHz -> exactly 601 log-mel frames
private const val ENCODER_MEL_BINS = 128
private const val ENCODER_MEL_FRAMES = 601
private const val ENCODER_HIDDEN_DIM = 1024
private const val ENCODER_OUTPUT_FRAMES = 76

// onnx-asr's NemoConformerTdt default; caps same-frame emissions so the loop always terminates.
private const val MAX_TOKENS_PER_STEP = 10

/**
 * Full Parakeet-TDT-0.6B-v3 on-device ASR pipeline: CPU feature extraction (nemo128.onnx) ->
 * NPU encoder (QNN HTP context binary, via [ParakeetQnn]) -> CPU greedy TDT decode
 * (decoder_joint-model.onnx). Mirrors onnx-asr's NemoConformerTdt decoding loop exactly, since
 * that's the library the ONNX export was produced for.
 */
class ParakeetPipeline(
    featureExtractorPath: String,
    decoderJointPath: String,
    vocabPath: String,
    qnnBackendLibPath: String,
    qnnSystemLibPath: String,
    qnnContextBinaryPath: String
) {
    private val featureExtractor = ParakeetFeatureExtractor(featureExtractorPath)
    private val decoderJoint = ParakeetDecoderJoint(decoderJointPath)
    private val vocab = ParakeetVocab(vocabPath)
    private val encoder = ParakeetQnn(qnnBackendLibPath, qnnSystemLibPath, qnnContextBinaryPath)

    /** samples are mono float PCM in [-1, 1] at 16kHz, same format the local Whisper path consumes. */
    suspend fun transcribe(samples: FloatArray): String = withContext(inferenceContext) {
        val padded = padOrTruncate(samples, ENCODER_INPUT_SAMPLES)

        val (melFeatures, melFrameCount) = featureExtractor.run(padded)
        val encoderInput = toFixedMelGrid(melFeatures, melFrameCount)

        val encoderOutput = encoder.run(encoderInput)

        val tokens = greedyDecode(encoderOutput)
        vocab.decode(tokens).trim()
    }

    private fun greedyDecode(encoderOutput: FloatArray): List<Int> {
        val tokens = mutableListOf<Int>()
        var state1 = decoderJoint.zeroState()
        var state2 = decoderJoint.zeroState()
        var prevToken = vocab.blankId
        var emittedAtThisStep = 0
        var t = 0

        while (t < ENCODER_OUTPUT_FRAMES) {
            val frame = FloatArray(ENCODER_HIDDEN_DIM) { d -> encoderOutput[d * ENCODER_OUTPUT_FRAMES + t] }
            val result = decoderJoint.step(frame, prevToken, state1, state2)

            var bestToken = 0
            var bestTokenScore = Float.NEGATIVE_INFINITY
            for (i in 0 until vocab.vocabSize) {
                if (result.logits[i] > bestTokenScore) {
                    bestTokenScore = result.logits[i]
                    bestToken = i
                }
            }

            var bestStep = 0
            var bestStepScore = Float.NEGATIVE_INFINITY
            for (i in vocab.vocabSize until result.logits.size) {
                if (result.logits[i] > bestStepScore) {
                    bestStepScore = result.logits[i]
                    bestStep = i - vocab.vocabSize
                }
            }

            if (bestToken != vocab.blankId) {
                tokens.add(bestToken)
                prevToken = bestToken
                state1 = result.state1
                state2 = result.state2
                emittedAtThisStep += 1
            }

            if (bestStep > 0) {
                t += bestStep
                emittedAtThisStep = 0
            } else if (bestToken == vocab.blankId || emittedAtThisStep >= MAX_TOKENS_PER_STEP) {
                t += 1
                emittedAtThisStep = 0
            }
        }

        return tokens
    }

    private fun padOrTruncate(samples: FloatArray, target: Int): FloatArray {
        if (samples.size == target) return samples
        val out = FloatArray(target)
        System.arraycopy(samples, 0, out, 0, min(samples.size, target))
        return out
    }

    // Pads/truncates the mel-bin-major [128, melFrameCount] grid onto the fixed [128, 601] grid
    // the QNN context binary was compiled for.
    private fun toFixedMelGrid(melFeatures: FloatArray, melFrameCount: Int): FloatArray {
        if (melFrameCount == ENCODER_MEL_FRAMES) return melFeatures

        val out = FloatArray(ENCODER_MEL_BINS * ENCODER_MEL_FRAMES)
        val framesToCopy = min(melFrameCount, ENCODER_MEL_FRAMES)
        for (bin in 0 until ENCODER_MEL_BINS) {
            System.arraycopy(melFeatures, bin * melFrameCount, out, bin * ENCODER_MEL_FRAMES, framesToCopy)
        }
        return out
    }

    suspend fun close() = withContext(inferenceContext) {
        encoder.close()
        decoderJoint.close()
        featureExtractor.close()
    }
}

package org.futo.voiceinput.shared.parakeet

import ai.onnxruntime.OnnxTensor
import ai.onnxruntime.OrtEnvironment
import ai.onnxruntime.OrtSession
import ai.onnxruntime.TensorInfo
import java.nio.FloatBuffer
import java.nio.IntBuffer
import java.nio.LongBuffer

class InvalidParakeetOnnxModelException(path: String, cause: Throwable) :
    Exception("Could not load Parakeet ONNX model from $path", cause)

/**
 * Wraps `nemo128.onnx`: raw 16kHz waveform in, 128-bin log-mel features out. Runs on CPU via
 * ONNX Runtime (it's tiny — ~140KB — compared to the QNN-accelerated encoder it feeds).
 */
class ParakeetFeatureExtractor(onnxPath: String) {
    private val env = OrtEnvironment.getEnvironment()
    private val session = try {
        env.createSession(onnxPath, OrtSession.SessionOptions())
    } catch (e: Exception) {
        throw InvalidParakeetOnnxModelException(onnxPath, e)
    }

    /** Returns mel-bin-major flat features: index = melBin * frameCount + frameIndex. */
    fun run(samples: FloatArray): Pair<FloatArray, Int> {
        val waveforms = OnnxTensor.createTensor(env, FloatBuffer.wrap(samples), longArrayOf(1, samples.size.toLong()))
        val lens = OnnxTensor.createTensor(env, LongBuffer.wrap(longArrayOf(samples.size.toLong())), longArrayOf(1))

        waveforms.use { wv ->
            lens.use { ln ->
                session.run(mapOf("waveforms" to wv, "waveforms_lens" to ln)).use { result ->
                    val featuresTensor = result.get("features").get() as OnnxTensor
                    val featuresLenTensor = result.get("features_lens").get() as OnnxTensor

                    val buffer = featuresTensor.floatBuffer
                    val features = FloatArray(buffer.remaining())
                    buffer.get(features)

                    val frameCount = featuresLenTensor.longBuffer.get(0).toInt()
                    return features to frameCount
                }
            }
        }
    }

    fun close() = session.close()
}

/**
 * Wraps `decoder_joint-model.onnx`: the TDT prediction network + joint network, one step per
 * call. Runs on CPU via ONNX Runtime. Mirrors onnx-asr's NemoConformerTdt._decode step.
 */
class ParakeetDecoderJoint(onnxPath: String) {
    private val env = OrtEnvironment.getEnvironment()
    private val session = try {
        env.createSession(onnxPath, OrtSession.SessionOptions())
    } catch (e: Exception) {
        throw InvalidParakeetOnnxModelException(onnxPath, e)
    }

    val stateLayers: Int
    val stateDim: Int

    init {
        val state1Shape = (session.inputInfo["input_states_1"]!!.info as TensorInfo).shape
        stateLayers = state1Shape[0].toInt()
        stateDim = state1Shape[2].toInt()
    }

    class StepResult(val logits: FloatArray, val state1: FloatArray, val state2: FloatArray)

    fun zeroState(): FloatArray = FloatArray(stateLayers * stateDim)

    /** encoderFrame: one encoder time step (hiddenDim floats). state1/state2: [stateLayers,1,stateDim] flattened. */
    fun step(encoderFrame: FloatArray, prevToken: Int, state1: FloatArray, state2: FloatArray): StepResult {
        val encTensor = OnnxTensor.createTensor(env, FloatBuffer.wrap(encoderFrame), longArrayOf(1, encoderFrame.size.toLong(), 1))
        val targets = OnnxTensor.createTensor(env, IntBuffer.wrap(intArrayOf(prevToken)), longArrayOf(1, 1))
        val targetLength = OnnxTensor.createTensor(env, IntBuffer.wrap(intArrayOf(1)), longArrayOf(1))
        val st1 = OnnxTensor.createTensor(env, FloatBuffer.wrap(state1), longArrayOf(stateLayers.toLong(), 1, stateDim.toLong()))
        val st2 = OnnxTensor.createTensor(env, FloatBuffer.wrap(state2), longArrayOf(stateLayers.toLong(), 1, stateDim.toLong()))

        encTensor.use { enc ->
            targets.use { tg ->
                targetLength.use { tl ->
                    st1.use { s1 ->
                        st2.use { s2 ->
                            session.run(
                                mapOf(
                                    "encoder_outputs" to enc,
                                    "targets" to tg,
                                    "target_length" to tl,
                                    "input_states_1" to s1,
                                    "input_states_2" to s2
                                )
                            ).use { result ->
                                val logitsTensor = result.get("outputs").get() as OnnxTensor
                                val newState1Tensor = result.get("output_states_1").get() as OnnxTensor
                                val newState2Tensor = result.get("output_states_2").get() as OnnxTensor

                                val logitsBuf = logitsTensor.floatBuffer
                                val logits = FloatArray(logitsBuf.remaining())
                                logitsBuf.get(logits)

                                val s1Buf = newState1Tensor.floatBuffer
                                val newState1 = FloatArray(s1Buf.remaining())
                                s1Buf.get(newState1)

                                val s2Buf = newState2Tensor.floatBuffer
                                val newState2 = FloatArray(s2Buf.remaining())
                                s2Buf.get(newState2)

                                return StepResult(logits, newState1, newState2)
                            }
                        }
                    }
                }
            }
        }
    }

    fun close() = session.close()
}

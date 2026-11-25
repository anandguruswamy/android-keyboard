package org.futo.voiceinput.shared.ggml

import androidx.annotation.Keep
import kotlinx.coroutines.withContext

class InvalidParakeetModelException : Exception("The Parakeet QNN encoder could not be loaded")

// Wraps the Parakeet-TDT-0.6B-v3 NPU encoder, a pre-compiled QNN HTP context binary executed via
// the QAIRT SDK. Shares the single-thread inferenceContext defined in WhisperGGML.kt so NPU and
// CPU (GGML) inference never run concurrently against the same device resources.
@Keep
class ParakeetQnn(
    backendLibPath: String,
    systemLibPath: String,
    contextBinaryPath: String
) {
    private var handle: Long = 0L
    init {
        handle = openNative(backendLibPath, systemLibPath, contextBinaryPath)

        if (handle == 0L) {
            throw InvalidParakeetModelException()
        }
    }

    // audioSignal must have exactly the element count the encoder's input tensor expects
    // (e.g. 128 mel bins x 601 frames for the current Parakeet encoder build).
    suspend fun run(audioSignal: FloatArray): FloatArray = withContext(inferenceContext) {
        if (handle == 0L) {
            throw IllegalStateException("ParakeetQnn has already been closed, cannot run")
        }

        runNative(handle, audioSignal)
    }

    suspend fun close() = withContext(inferenceContext) {
        if (handle != 0L) {
            closeNative(handle)
        }
        handle = 0L
    }

    private external fun openNative(backendLibPath: String, systemLibPath: String, contextBinaryPath: String): Long
    private external fun runNative(handle: Long, audioSignal: FloatArray): FloatArray
    private external fun closeNative(handle: Long)
}

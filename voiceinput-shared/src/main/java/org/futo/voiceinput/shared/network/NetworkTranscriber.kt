package org.futo.voiceinput.shared.network

import org.json.JSONObject
import java.io.ByteArrayOutputStream
import java.io.DataOutputStream
import java.net.HttpURLConnection
import java.net.URL
import java.nio.ByteBuffer
import java.nio.ByteOrder
import kotlin.math.max
import kotlin.math.min

class NetworkTranscriptionException(message: String, cause: Throwable? = null) : Exception(message, cause)

/**
 * Talks to a LAN ASR server exposing the OpenAI-compatible
 * POST /v1/audio/transcriptions endpoint (FUTO Keyboard's homelab-gateway or orukeet directly).
 */
object NetworkTranscriber {
    private const val SAMPLE_RATE = 16000
    private const val CONNECT_TIMEOUT_MS = 5_000
    // Transcription itself can take a while on a loaded NPU; this is generous on purpose.
    private const val READ_TIMEOUT_MS = 60_000

    fun transcribe(baseUrl: String, samples: FloatArray, model: String = "whisper-1"): String {
        val wavBytes = encodeWav(samples)
        val boundary = "----futoKeyboardNetworkAsr${System.currentTimeMillis()}"

        val url = URL(baseUrl.trimEnd('/') + "/v1/audio/transcriptions")
        val connection = (url.openConnection() as HttpURLConnection).apply {
            requestMethod = "POST"
            doOutput = true
            connectTimeout = CONNECT_TIMEOUT_MS
            readTimeout = READ_TIMEOUT_MS
            setRequestProperty("Content-Type", "multipart/form-data; boundary=$boundary")
        }

        try {
            DataOutputStream(connection.outputStream).use { out ->
                writeFormField(out, boundary, "model", model)
                writeFileField(out, boundary, "file", "recording.wav", "audio/wav", wavBytes)
                out.writeBytes("--$boundary--\r\n")
            }

            val responseCode = connection.responseCode
            val body = (if (responseCode in 200..299) connection.inputStream else connection.errorStream)
                ?.bufferedReader()?.readText() ?: ""

            if (responseCode !in 200..299) {
                throw NetworkTranscriptionException("Network ASR server returned HTTP $responseCode: $body")
            }

            return parseResponseText(body)
        } catch (e: NetworkTranscriptionException) {
            throw e
        } catch (e: Exception) {
            throw NetworkTranscriptionException("Network ASR request failed: ${e.message}", e)
        } finally {
            connection.disconnect()
        }
    }

    private fun parseResponseText(body: String): String {
        return try {
            JSONObject(body).optString("text", "").trim()
        } catch (e: Exception) {
            // Some servers may respond with plain text when response_format=text
            body.trim()
        }
    }

    private fun writeFormField(out: DataOutputStream, boundary: String, name: String, value: String) {
        out.writeBytes("--$boundary\r\n")
        out.writeBytes("Content-Disposition: form-data; name=\"$name\"\r\n\r\n")
        out.writeBytes("$value\r\n")
    }

    private fun writeFileField(
        out: DataOutputStream,
        boundary: String,
        name: String,
        filename: String,
        contentType: String,
        bytes: ByteArray
    ) {
        out.writeBytes("--$boundary\r\n")
        out.writeBytes("Content-Disposition: form-data; name=\"$name\"; filename=\"$filename\"\r\n")
        out.writeBytes("Content-Type: $contentType\r\n\r\n")
        out.write(bytes)
        out.writeBytes("\r\n")
    }

    /** Encodes mono 16-bit PCM WAV at 16kHz from [-1, 1] float samples, matching what the local Whisper path consumes. */
    private fun encodeWav(samples: FloatArray): ByteArray {
        val pcm = ByteArray(samples.size * 2)
        val pcmBuffer = ByteBuffer.wrap(pcm).order(ByteOrder.LITTLE_ENDIAN)
        for (sample in samples) {
            val clamped = max(-1.0f, min(1.0f, sample))
            pcmBuffer.putShort((clamped * Short.MAX_VALUE).toInt().toShort())
        }

        val out = ByteArrayOutputStream()
        val header = ByteBuffer.allocate(44).order(ByteOrder.LITTLE_ENDIAN)

        val byteRate = SAMPLE_RATE * 2
        val dataSize = pcm.size

        header.put("RIFF".toByteArray())
        header.putInt(36 + dataSize)
        header.put("WAVE".toByteArray())
        header.put("fmt ".toByteArray())
        header.putInt(16) // PCM header size
        header.putShort(1) // PCM format
        header.putShort(1) // mono
        header.putInt(SAMPLE_RATE)
        header.putInt(byteRate)
        header.putShort(2) // block align (channels * bytes/sample)
        header.putShort(16) // bits per sample
        header.put("data".toByteArray())
        header.putInt(dataSize)

        out.write(header.array())
        out.write(pcm)
        return out.toByteArray()
    }
}

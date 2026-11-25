package org.futo.voiceinput.shared.parakeet

import android.content.Context
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.io.File
import java.io.IOException
import java.net.HttpURLConnection
import java.net.URL

data class ParakeetAsset(val remoteName: String, val localName: String)

/**
 * The six files the on-device Parakeet-TDT pipeline needs: the two CPU ONNX models, the vocab,
 * the QNN HTP context binary, and the two Qualcomm QNN runtime libraries the NPU encoder dlopens
 * at runtime (ParakeetQnn.kt). The QNN libs' real names/source are not settled yet — these are
 * the QAIRT SDK's conventional names, used as a placeholder until that's resolved.
 */
object ParakeetModelAssets {
    val ASSETS = listOf(
        ParakeetAsset("nemo128.onnx", "nemo128.onnx"),
        ParakeetAsset("decoder_joint-model.onnx", "decoder_joint-model.onnx"),
        ParakeetAsset("vocab.txt", "vocab.txt"),
        ParakeetAsset("parakeet-enc-601_ctx_qnn.bin", "parakeet-enc-601_ctx_qnn.bin"),
        ParakeetAsset("libQnnHtp.so", "libQnnHtp.so"),
        ParakeetAsset("libQnnSystem.so", "libQnnSystem.so")
    )

    fun directory(context: Context): File = File(context.filesDir, "parakeet")

    fun missingAssets(context: Context): List<ParakeetAsset> {
        val dir = directory(context)
        return ASSETS.filter { !File(dir, it.localName).exists() }
    }

    fun isComplete(context: Context): Boolean = missingAssets(context).isEmpty()

    fun localPath(context: Context, localName: String): String =
        File(directory(context), localName).absolutePath
}

class ParakeetDownloadException(assetName: String, cause: Throwable) :
    Exception("Failed to download Parakeet asset $assetName", cause)

data class ParakeetDownloadProgress(
    val assetIndex: Int,
    val assetCount: Int,
    val assetName: String,
    val fractionOfCurrentFile: Float
)

object ParakeetModelDownloader {
    /** Downloads every asset missing from context.filesDir/parakeet/ from "$baseUrl/<remoteName>". */
    suspend fun downloadMissing(
        context: Context,
        baseUrl: String,
        onProgress: (ParakeetDownloadProgress) -> Unit
    ) = withContext(Dispatchers.IO) {
        val dir = ParakeetModelAssets.directory(context)
        dir.mkdirs()

        val missing = ParakeetModelAssets.missingAssets(context)
        missing.forEachIndexed { index, asset ->
            onProgress(ParakeetDownloadProgress(index, missing.size, asset.localName, 0f))
            try {
                downloadOne(baseUrl, asset, dir) { fraction ->
                    onProgress(ParakeetDownloadProgress(index, missing.size, asset.localName, fraction))
                }
            } catch (e: Exception) {
                throw ParakeetDownloadException(asset.localName, e)
            }
        }
    }

    private fun downloadOne(baseUrl: String, asset: ParakeetAsset, dir: File, onProgress: (Float) -> Unit) {
        val url = URL("$baseUrl/${asset.remoteName}")
        val connection = url.openConnection() as HttpURLConnection
        connection.connectTimeout = 10_000
        connection.readTimeout = 30_000

        try {
            connection.connect()

            if (connection.responseCode != HttpURLConnection.HTTP_OK) {
                throw IOException("HTTP ${connection.responseCode} for $url")
            }

            val length = connection.contentLengthLong.coerceAtLeast(1L)
            val tempFile = File(dir, "${asset.localName}.part")

            connection.inputStream.use { input ->
                tempFile.outputStream().use { output ->
                    val buffer = ByteArray(65536)
                    var totalRead = 0L
                    var n: Int
                    while (input.read(buffer).also { n = it } >= 0) {
                        output.write(buffer, 0, n)
                        totalRead += n
                        onProgress((totalRead.toFloat() / length.toFloat()).coerceIn(0f, 1f))
                    }
                }
            }

            val finalFile = File(dir, asset.localName)
            if (!tempFile.renameTo(finalFile)) {
                throw IOException("Could not move downloaded file into place: ${asset.localName}")
            }
        } finally {
            connection.disconnect()
        }
    }
}

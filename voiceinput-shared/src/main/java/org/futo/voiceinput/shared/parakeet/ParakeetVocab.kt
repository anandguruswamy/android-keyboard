package org.futo.voiceinput.shared.parakeet

import java.io.File

class InvalidParakeetVocabException(path: String) : Exception("Could not load a usable Parakeet vocab from $path")

// Matches onnx-asr's NemoConformerTdt text assembly: leading/duplicate whitespace from the
// SentencePiece "▁" word-boundary marker collapses to a single space.
private val DECODE_SPACE_PATTERN = Regex("\\A\\s|\\s\\B|(\\s)\\b")

/**
 * Loads the Parakeet TDT SentencePiece vocab (`<token> <id>` per line, `<blk>` last) and
 * detokenizes greedy-decoded token ids back to text.
 */
class ParakeetVocab(path: String) {
    private val tokenById: Map<Int, String>
    val vocabSize: Int
    val blankId: Int

    init {
        val entries = File(path).readLines()
            .map { it.trimEnd('\n', '\r') }
            .filter { it.isNotEmpty() }
            .map { line ->
                val spaceIdx = line.lastIndexOf(' ')
                val token = line.substring(0, spaceIdx)
                val id = line.substring(spaceIdx + 1).trim().toInt()
                id to token
            }

        tokenById = entries.toMap()
        vocabSize = tokenById.size
        blankId = entries.firstOrNull { it.second == "<blk>" }?.first
            ?: throw InvalidParakeetVocabException(path)
    }

    fun decode(ids: List<Int>): String {
        val text = ids.joinToString("") { tokenById[it]?.replace('▁', ' ') ?: "" }
        return DECODE_SPACE_PATTERN.replace(text) { m -> if (m.groups[1] != null) " " else "" }
    }
}

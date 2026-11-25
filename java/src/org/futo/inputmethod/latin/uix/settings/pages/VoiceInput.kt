package org.futo.inputmethod.latin.uix.settings.pages

import android.view.inputmethod.InputMethodManager
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.LinearProgressIndicator
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.LocalResources
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.dp
import androidx.core.content.getSystemService
import kotlinx.coroutines.launch
import org.futo.inputmethod.latin.R
import org.futo.inputmethod.latin.uix.ANIMATE_BUBBLE
import org.futo.inputmethod.latin.uix.AUDIO_FOCUS
import org.futo.inputmethod.latin.uix.CAN_EXPAND_SPACE
import org.futo.inputmethod.latin.uix.DISALLOW_SYMBOLS
import org.futo.inputmethod.latin.uix.ENABLE_SOUND
import org.futo.inputmethod.latin.uix.PREFER_BLUETOOTH
import org.futo.inputmethod.latin.uix.NETWORK_ASR_URL
import org.futo.inputmethod.latin.uix.PARAKEET_ASR_URL
import org.futo.inputmethod.latin.uix.SYSTEM_VOICE_INPUT_PACKAGE
import org.futo.inputmethod.latin.uix.USE_NETWORK_ASR
import org.futo.inputmethod.latin.uix.USE_PARAKEET_ASR
import org.futo.inputmethod.latin.uix.USE_PERSONAL_DICT
import org.futo.inputmethod.latin.uix.USE_SYSTEM_VOICE_INPUT
import org.futo.inputmethod.latin.uix.USE_VAD_AUTOSTOP
import org.futo.inputmethod.latin.uix.settings.DropDownPickerSettingItem
import org.futo.inputmethod.latin.uix.settings.NavigationItemStyle
import org.futo.inputmethod.latin.uix.settings.ScreenTitle
import org.futo.inputmethod.latin.uix.settings.SettingTextField
import org.futo.inputmethod.latin.uix.settings.SettingToggleDataStore
import org.futo.inputmethod.latin.uix.settings.Tip
import org.futo.inputmethod.latin.uix.settings.UserSetting
import org.futo.inputmethod.latin.uix.settings.UserSettingsMenu
import org.futo.inputmethod.latin.uix.settings.useDataStore
import org.futo.inputmethod.latin.uix.settings.useDataStoreValue
import org.futo.inputmethod.latin.uix.settings.userSettingDecorationOnly
import org.futo.inputmethod.latin.uix.settings.userSettingNavigationItem
import org.futo.inputmethod.latin.uix.settings.userSettingToggleDataStore
import org.futo.voiceinput.shared.parakeet.ParakeetModelAssets
import org.futo.voiceinput.shared.parakeet.ParakeetModelDownloader

private val visibilityCheckNotSystemVoiceInput = @Composable {
    useDataStoreValue(USE_SYSTEM_VOICE_INPUT) == false
}

private data class VoiceIMEInfo(
    val builtin: Boolean,
    val name: String,
    val packageName: String,
)

@Composable
fun usePackageReadableName(pkg: String): String? {
    val context = LocalContext.current
    return remember(pkg) {
        try {
            context.packageManager.getPackageInfo(pkg, 0)
        } catch(e: Exception) {
            null
        }?.applicationInfo?.let {
            context.packageManager.getApplicationLabel(it).toString()
        }
    }
}

val VoiceInputMenu = UserSettingsMenu(
    title = R.string.voice_input_settings_title,
    navPath = "voiceInput", registerNavPath = true,
    settings = listOf(
        UserSetting(
            name = R.string.voice_input_settings_backend_system,
            searchTagList = listOf(
                R.string.voice_input_settings_disable_builtin_voice_input,
                R.string.voice_input_settings_disable_builtin_voice_input_subtitle
            )
        ) {
            val useExternal = useDataStore(USE_SYSTEM_VOICE_INPUT)
            val externalPkg = useDataStore(SYSTEM_VOICE_INPUT_PACKAGE)

            val context = LocalContext.current
            val res = LocalResources.current
            val options = remember(externalPkg.value) {
                val imm = context.getSystemService<InputMethodManager>()!!
                buildList {
                    add(VoiceIMEInfo(true, "", ""))
                    addAll(imm.enabledInputMethodList.filter { im ->
                        im.packageName == externalPkg.value ||
                            (0 until im.subtypeCount).map { im.getSubtypeAt(it) }
                                .any { it.mode.lowercase() == "voice" }
                    }.map {
                        VoiceIMEInfo(false, it.loadLabel(context.packageManager)?.toString() ?: it.packageName, it.packageName)
                    })
                }
            }

            val currOption = remember(externalPkg.value) {
                if(externalPkg.value == "") options[0] else
                options.find { it.packageName == externalPkg.value }
            }


            DropDownPickerSettingItem(
                stringResource(R.string.voice_input_settings_backend_system),
                options,
                currOption,
                {
                    useExternal.setValue(!it.builtin)
                    externalPkg.setValue(it.packageName ?: "")
                },
                {
                    if(it.builtin) res.getString(R.string.voice_input_settings_backend_system_internal)
                    else it.name
                }
            )

            if(useExternal.value && externalPkg.value.isNotEmpty()) {
                val privacyWhitelist = listOf(
                    "org.futo.voiceinput",
                    "org.futo.voiceinput.dev",
                    "dev.notune.transcribe",
                    "dev.soupslurpr.transcribro"
                )

                if(!privacyWhitelist.contains(externalPkg.value))
                    Tip(stringResource(R.string.voice_input_settings_backend_system_external_warning,
                        currOption?.name ?: externalPkg.value))
            }
        },

        //if(!systemVoiceInput.value) {
        userSettingToggleDataStore(
            title = R.string.voice_input_settings_indication_sounds,
            subtitle = R.string.voice_input_settings_indication_sounds_subtitle,
            setting = ENABLE_SOUND
        ).copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        /*
        userSettingToggleDataStore(
            title = R.string.voice_input_settings_verbose_progress,
            subtitle = R.string.voice_input_settings_verbose_progress_subtitle,
            setting = VERBOSE_PROGRESS
        ).copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),
         */

        userSettingToggleDataStore(
            title = R.string.voice_input_settings_use_personal_dict,
            subtitle = R.string.voice_input_settings_use_personal_dict_subtitle,
            setting = USE_PERSONAL_DICT
        ).copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        userSettingToggleDataStore(
            title = R.string.voice_input_settings_use_bluetooth_mic,
            subtitle = R.string.voice_input_settings_use_bluetooth_mic_subtitle,
            setting = PREFER_BLUETOOTH
        ).copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        userSettingToggleDataStore(
            title = R.string.voice_input_settings_audio_focus,
            subtitle = R.string.voice_input_settings_audio_focus_subtitle,
            setting = AUDIO_FOCUS
        ).copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        userSettingToggleDataStore(
            title = R.string.voice_input_settings_suppress_symbols,
            setting = DISALLOW_SYMBOLS
        ).copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        userSettingToggleDataStore(
            title = R.string.voice_input_settings_long_form,
            subtitle = R.string.voice_input_settings_long_form_subtitle,
            setting = CAN_EXPAND_SPACE
        ).copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        userSettingToggleDataStore(
            title = R.string.voice_input_settings_autostop_vad,
            subtitle = R.string.voice_input_settings_autostop_vad_subtitle,
            setting = USE_VAD_AUTOSTOP
        ).copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        userSettingToggleDataStore(
            title = R.string.voice_input_settings_animate_bubble,
            subtitle = R.string.voice_input_settings_animate_bubble_subtitle,
            setting = ANIMATE_BUBBLE
        ).copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        userSettingNavigationItem(
            title = R.string.voice_input_settings_change_models,
            subtitle = R.string.voice_input_settings_change_models_subtitle,
            style = NavigationItemStyle.Misc,
            navigateTo = "languages"
        ).copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),
        //}

        userSettingDecorationOnly {
            SettingToggleDataStore(
                title = "Network ASR (homelab)",
                setting = USE_NETWORK_ASR,
                subtitle = "Send recordings to a LAN server instead of running Whisper on-device"
            )
        }.copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        userSettingDecorationOnly {
            if (useDataStoreValue(USE_NETWORK_ASR)) {
                SettingTextField(
                    title = "Network ASR server URL",
                    placeholder = "http://192.168.8.234:8200",
                    field = NETWORK_ASR_URL
                )
            }
        }.copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        userSettingDecorationOnly {
            SettingToggleDataStore(
                title = "Parakeet NPU ASR (on-device)",
                setting = USE_PARAKEET_ASR,
                subtitle = "Run Parakeet-TDT fully on-device via the Hexagon NPU instead of Whisper or network ASR"
            )
        }.copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        userSettingDecorationOnly {
            if (useDataStoreValue(USE_PARAKEET_ASR)) {
                SettingTextField(
                    title = "Parakeet model server URL",
                    placeholder = "http://192.168.8.234:8200/parakeet",
                    field = PARAKEET_ASR_URL
                )
            }
        }.copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),

        userSettingDecorationOnly {
            if (useDataStoreValue(USE_PARAKEET_ASR)) {
                ParakeetDownloadStatus(useDataStoreValue(PARAKEET_ASR_URL))
            }
        }.copy(visibilityCheck = visibilityCheckNotSystemVoiceInput),
    )
)

@Composable
private fun ParakeetDownloadStatus(baseUrl: String) {
    val context = LocalContext.current
    val scope = rememberCoroutineScope()

    var missing by remember { mutableStateOf(ParakeetModelAssets.missingAssets(context)) }
    var isDownloading by remember { mutableStateOf(false) }
    var progressText by remember { mutableStateOf("") }
    var progressFraction by remember { mutableStateOf(0f) }
    var errorText by remember { mutableStateOf<String?>(null) }

    ScreenTitle("Parakeet model files")

    if (missing.isEmpty()) {
        Text(
            "All model files are downloaded.",
            modifier = Modifier.fillMaxWidth().padding(8.dp, 4.dp)
        )
    } else {
        Text(
            "${missing.size} file(s) still needed: ${missing.joinToString(", ") { it.localName }}",
            modifier = Modifier.fillMaxWidth().padding(8.dp, 4.dp)
        )

        if (isDownloading) {
            LinearProgressIndicator(
                progress = progressFraction,
                modifier = Modifier.fillMaxWidth().padding(8.dp, 4.dp)
            )
            Text(progressText, modifier = Modifier.fillMaxWidth().padding(8.dp, 0.dp))
        } else {
            Button(
                onClick = {
                    isDownloading = true
                    errorText = null
                    scope.launch {
                        try {
                            ParakeetModelDownloader.downloadMissing(context, baseUrl) { progress ->
                                progressFraction = if (progress.assetCount > 0)
                                    (progress.assetIndex + progress.fractionOfCurrentFile) / progress.assetCount
                                else 0f
                                progressText = "${progress.assetName} (${progress.assetIndex + 1}/${progress.assetCount})"
                            }
                        } catch (e: Exception) {
                            errorText = e.message ?: "Download failed"
                        } finally {
                            isDownloading = false
                            missing = ParakeetModelAssets.missingAssets(context)
                        }
                    }
                },
                modifier = Modifier.fillMaxWidth().padding(8.dp, 4.dp)
            ) {
                Text("Download Parakeet models")
            }
        }

        errorText?.let {
            Text(it, modifier = Modifier.fillMaxWidth().padding(8.dp, 0.dp))
        }
    }
}
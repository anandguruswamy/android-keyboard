#include <jni.h>
#include <memory>
#include <vector>

#include "defines.h"
#include "jni_common.h"
#include "jni_utils.h"
#include "org_futo_voiceinput_ParakeetQnn.h"
#include "qnn/parakeet_qnn_encoder.hpp"

struct ParakeetModelState {
    std::unique_ptr<qnnx::ParakeetQnnEncoder> encoder;
};

static jlong ParakeetQnn_open(JNIEnv *env, jclass clazz, jstring backend_lib_path,
                               jstring system_lib_path, jstring context_binary_path) {
    std::string backend_lib_path_str = jstring2string(env, backend_lib_path);
    std::string system_lib_path_str = jstring2string(env, system_lib_path);
    std::string context_binary_path_str = jstring2string(env, context_binary_path);

    auto *state = new ParakeetModelState();

    AKLOGI("ParakeetQnn: attempting to load context binary %s", context_binary_path_str.c_str());
    try {
        state->encoder = std::make_unique<qnnx::ParakeetQnnEncoder>(
                backend_lib_path_str, system_lib_path_str, context_binary_path_str);
    } catch (const std::exception &e) {
        AKLOGE("ParakeetQnn: failed to load encoder: %s", e.what());
        delete state;
        return 0L;
    }

    return reinterpret_cast<jlong>(state);
}

static jfloatArray ParakeetQnn_run(JNIEnv *env, jclass clazz, jlong handle,
                                    jfloatArray audio_signal_array) {
    auto *state = reinterpret_cast<ParakeetModelState *>(handle);
    if (!state || !state->encoder) {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"),
                       "ParakeetQnn handle is closed or invalid");
        return nullptr;
    }

    jsize num_samples = env->GetArrayLength(audio_signal_array);
    std::vector<float> audio_signal(static_cast<size_t>(num_samples));
    env->GetFloatArrayRegion(audio_signal_array, 0, num_samples, audio_signal.data());

    std::vector<float> output;
    try {
        output = state->encoder->run(audio_signal);
    } catch (const std::exception &e) {
        AKLOGE("ParakeetQnn: run failed: %s", e.what());
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), e.what());
        return nullptr;
    }

    jfloatArray result = env->NewFloatArray(static_cast<jsize>(output.size()));
    env->SetFloatArrayRegion(result, 0, static_cast<jsize>(output.size()), output.data());
    return result;
}

static void ParakeetQnn_close(JNIEnv *env, jclass clazz, jlong handle) {
    auto *state = reinterpret_cast<ParakeetModelState *>(handle);
    if (!state) return;

    delete state;
}

static const JNINativeMethod sMethods[] = {
        {
                const_cast<char *>("openNative"),
                const_cast<char *>("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)J"),
                reinterpret_cast<void *>(ParakeetQnn_open)
        },
        {
                const_cast<char *>("runNative"),
                const_cast<char *>("(J[F)[F"),
                reinterpret_cast<void *>(ParakeetQnn_run)
        },
        {
                const_cast<char *>("closeNative"),
                const_cast<char *>("(J)V"),
                reinterpret_cast<void *>(ParakeetQnn_close)
        }
};

namespace voiceinput {
    int register_ParakeetQnn(JNIEnv *env) {
        const char *const kClassPathName = "org/futo/voiceinput/shared/ggml/ParakeetQnn";
        return latinime::registerNativeMethods(env, kClassPathName, sMethods, NELEMS(sMethods));
    }
}

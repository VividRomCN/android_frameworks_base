/*
 * Copyright (C) 2012 - 2015 The MoKee OpenSource Project
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdio.h>
#include <jni.h>
#include <assert.h>
#include "androidjni.h"
#include "utils.h"

// #define DEBUG

#ifdef DEBUG
#include <android/log.h>
#define TAG_JNI "phonelocation"
#endif

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv *env, const char *className,
                                 JNINativeMethod *gMethods, int numMethods) {
    jclass clazz;
	
    clazz = (*env)->FindClass(env, className);
    if (clazz == NULL) {
#ifdef DEBUG
        __android_log_print(ANDROID_LOG_DEBUG, TAG_JNI, "class (%s) not exist!", className);
#endif
        return JNI_FALSE;
    }
    if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
#ifdef DEBUG
        __android_log_print(ANDROID_LOG_DEBUG, TAG_JNI, "method not exist!");
#endif
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 */
static int registerNatives(JNIEnv *env) {
    if (!registerNativeMethods(env, JNIREG_CLASS_UTIL, gMethods_Utils, sizeof(gMethods_Utils) / sizeof(gMethods_Utils[0])))
        return JNI_FALSE;
    if (!registerNativeMethods(env, JNIREG_CLASS_LOCATION, gMethods_Location, sizeof(gMethods_Location) / sizeof(gMethods_Location[0])))
        return JNI_FALSE;
    return JNI_TRUE;
}

/*
 * Set some test stuff up.
 *
 * Returns the JNI version on success, -1 on failure.
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    if ((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_4) != JNI_OK) {
#ifdef DEBUG
        __android_log_print(ANDROID_LOG_DEBUG, TAG_JNI, "GetEnv Error!");
#endif
        return -1;
    }
    assert(env != NULL);

    if (!registerNatives(env)) { 
#ifdef DEBUG
        __android_log_print(ANDROID_LOG_DEBUG, TAG_JNI, "registerNatives Error!");
#endif
		return -1; 
	}
#ifdef DEBUG
        __android_log_print(ANDROID_LOG_DEBUG, TAG_JNI, "registerNatives OK!");
#endif
    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

    return result;
}

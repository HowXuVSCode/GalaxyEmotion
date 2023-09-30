/*
 * Copyright (c) 2003, 2018, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#include <stdio.h>
#include <string.h>
#include "jvmti.h"
#include "agent_common.h"
#include "JVMTITools.h"

extern "C" {


#define PASSED 0
#define STATUS_FAILED 2

static jvmtiEnv *jvmti = NULL;
static jvmtiCapabilities caps;
static jint result = PASSED;
static jboolean printdump = JNI_FALSE;

#ifdef STATIC_BUILD
JNIEXPORT jint JNICALL Agent_OnLoad_localtab003(JavaVM *jvm, char *options, void *reserved) {
    return Agent_Initialize(jvm, options, reserved);
}
JNIEXPORT jint JNICALL Agent_OnAttach_localtab003(JavaVM *jvm, char *options, void *reserved) {
    return Agent_Initialize(jvm, options, reserved);
}
JNIEXPORT jint JNI_OnLoad_localtab003(JavaVM *jvm, char *options, void *reserved) {
    return JNI_VERSION_1_8;
}
#endif
jint Agent_Initialize(JavaVM *jvm, char *options, void *reserved) {
    jint res;
    jvmtiError err;

    if (options != NULL && strcmp(options, "printdump") == 0) {
        printdump = JNI_TRUE;
    }

    res = jvm->GetEnv((void **) &jvmti, JVMTI_VERSION_1_1);
    if (res != JNI_OK || jvmti == NULL) {
        printf("Wrong result of a valid call to GetEnv!\n");
        return JNI_ERR;
    }

    err = jvmti->GetPotentialCapabilities(&caps);
    if (err != JVMTI_ERROR_NONE) {
        printf("(GetPotentialCapabilities) unexpected error: %s (%d)\n",
               TranslateError(err), err);
        return JNI_ERR;
    }

    err = jvmti->AddCapabilities(&caps);
    if (err != JVMTI_ERROR_NONE) {
        printf("(AddCapabilities) unexpected error: %s (%d)\n",
               TranslateError(err), err);
        return JNI_ERR;
    }

    err = jvmti->GetCapabilities(&caps);
    if (err != JVMTI_ERROR_NONE) {
        printf("(GetCapabilities) unexpected error: %s (%d)\n",
               TranslateError(err), err);
        return JNI_ERR;
    }

    if (!caps.can_access_local_variables) {
        printf("Warning: Access to local variables is not implemented\n");
    }

    return JNI_OK;
}

JNIEXPORT jint JNICALL
Java_nsk_jvmti_GetLocalVariableTable_localtab003_check(JNIEnv *env, jclass cls) {
    jvmtiError err;
    jmethodID mid;
    jint entryCount;
    jvmtiLocalVariableEntry *table;
    int i;

    if (jvmti == NULL) {
        printf("JVMTI client was not properly loaded!\n");
        return STATUS_FAILED;
    }

    if (!caps.can_access_local_variables) return result;

    mid = env->GetMethodID(cls, "meth", "()I");
    if (mid == NULL) {
        printf("Cannot get method ID!\n");
        return STATUS_FAILED;
    }

    if (printdump == JNI_TRUE) {
        printf(">>> try class which has not local variable info ...\n");
    }
    err = jvmti->GetLocalVariableTable(mid, &entryCount, &table);
    if (err != JVMTI_ERROR_ABSENT_INFORMATION) {
        result = STATUS_FAILED;
        printf("Error expected: JVMTI_ERROR_ABSENT_INFORMATION,\n");
        printf("\tactual: %s (%d)\n", TranslateError(err), err);
        printf("  meth() local variable table (%d entries):%s\n",
               entryCount, (entryCount==0 ? " empty" : ""));
        for (i = 0; i < entryCount; i++) {
            printf("    name=\"%s\", sig=\"%s\"",
                table[i].name, table[i].signature);
            printf(", start_location=0x%x%08x",
                   (jint)(table[i].start_location >> 32),
                   (jint)table[i].start_location);
            printf(", length=%d", table[i].length);
            printf(", slot=%d\n", table[i].slot);
        }
    }

    if (printdump == JNI_TRUE) {
        printf(">>> ... done\n");
    }

    return result;
}

}
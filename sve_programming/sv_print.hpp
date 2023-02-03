#pragma once

#ifndef __ARM_FEATURE_SVE
#error arm_sve not suppport
#endif

#include <arm_sve.h>

void sv_print(svbool_t tp, int idx = -1);

void sv_print(svint8_t va, int idx = -1);

void sv_print(svuint8_t va, int idx = -1);

void sv_print(svint16_t va, int idx = -1);

void sv_print(svuint16_t va, int idx = -1);

void sv_print(svint32_t va, int idx = -1);

void sv_print(svuint32_t va, int idx = -1);

void sv_print(svint64_t va, int idx = -1);

void sv_print(svuint64_t va, int idx = -1);

void sv_print(svfloat32_t va, int idx = -1);

void sv_print(svfloat64_t va, int idx = -1);


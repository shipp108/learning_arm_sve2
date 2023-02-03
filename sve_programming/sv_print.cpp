
#include "sv_print.hpp"

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <vector>

#ifdef __ARM_FEATURE_SVE

void sv_print(svbool_t tp, int idx) {
    const int n = svcntb();
    std::vector<int8_t> b(n, 0);
    svst1_s8(tp, b.data(), svdup_n_s8(1));
    printf("svbool_t variable:\n\t");
    if (idx < 0 || idx >= n) {
        for (int i = 0; i < n; ++i) {
            printf("| %s ", (b[i] == 1) ? "T" : "F");
        }
    } else {
        printf("| %s ", (b[idx] == 1) ? "T" : "F");
    }
    printf("|\n");
}

void sv_print(svint8_t va, int idx) {
    const int n = svcntb();
    std::vector<int8_t> a(n);
    svbool_t tp = svptrue_b8();
    svst1_s8(tp, a.data(), va);
    printf("svint8_t variable:\n\t");
    if (idx < 0 || idx >= n) {
        for (int i = 0; i < n; ++i) {
            printf("| %+3d ", (int32_t)(a[i]));
        }
    } else {
        printf("| %+3d ", (int32_t)(a[idx]));
    }
    printf("|\n");
}

void sv_print(svuint8_t va, int idx) {
    const int n = svcntb();
    std::vector<uint8_t> a(n);
    svbool_t tp = svptrue_b8();
    svst1_u8(tp, a.data(), va);
    printf("svuint8_t variable:\n\t");
    if (idx < 0 || idx >= n) {
        for (int i = 0; i < n; ++i) {
            printf("| %3u ", (uint32_t)(a[i]));
        }
    } else {
        printf("| %3u ", (uint32_t)(a[idx]));
    }
    printf("|\n");
}

void sv_print(svint16_t va, int idx) {
    const int n = svcnth();
    std::vector<int16_t> a(n);
    svbool_t tp = svptrue_b16();
    svst1_s16(tp, a.data(), va);
    printf("svint16_t variable:\n\t");
    if (idx < 0 || idx >= n) {
        for (int i = 0; i < n; ++i) {
            printf("| %+5d ", a[i]);
        }
    } else {
        printf("| %+5d ", a[idx]);
    }
    printf("|\n");
}

void sv_print(svuint16_t va, int idx) {
    const int n = svcnth();
    std::vector<uint16_t> a(n);
    svbool_t tp = svptrue_b16();
    svst1_u16(tp, a.data(), va);
    printf("svuint16_t variable:\n\t");
    if (idx < 0 || idx >= n) {
        for (int i = 0; i < n; ++i) {
            printf("| %5u ", a[i]);
        }
    } else {
        printf("| %5u ", a[idx]);
    }
    printf("|\n");
}

void sv_print(svint32_t va, int idx) {
    const int n = svcntw();
    std::vector<int32_t> a(n);
    svbool_t tp = svptrue_b32();
    svst1_s32(tp, a.data(), va);
    printf("svint32_t variable:\n\t");
    if (idx < 0 || idx >= n) {
        for (int i = 0; i < n; ++i) {
            printf("| %+10d ", a[i]);
        }
    } else {
        printf("| %+10d ", a[idx]);
    }
    printf("|\n");
}

void sv_print(svuint32_t va, int idx) {
    const int n = svcntw();
    std::vector<uint32_t> a(n);
    svbool_t tp = svptrue_b32();
    svst1_u32(tp, a.data(), va);
    printf("svuint32_t variable:\n\t");
    if (idx < 0 || idx >= n) {
        for (int i = 0; i < n; ++i) {
            printf("| %10u ", a[i]);
        }
    } else {
        printf("| %10u ", a[idx]);
    }
    printf("|\n");
}

void sv_print(svint64_t va, int idx) {
    const int n = svcntd();
    std::vector<int64_t> a(n);
    svbool_t tp = svptrue_b64();
    svst1_s64(tp, a.data(), va);
    printf("svint64_t variable:\n\t");
    if (idx < 0 || idx >= n) {
        for (int i = 0; i < n; ++i) {
            printf("| %+" PRId64 " ", a[i]);
        }
    } else {
        printf("| %+" PRId64 " ", a[idx]);
    }
    printf("|\n");
}

void sv_print(svuint64_t va, int idx) {
    const int n = svcntd();
    std::vector<uint64_t> a(n);
    svbool_t tp = svptrue_b64();
    svst1_u64(tp, a.data(), va);
    printf("svuint64_t variable:\n\t");
    if (idx < 0 || idx >= n) {
        for (int i = 0; i < n; ++i) {
            printf("| %" PRIu64 " ", a[i]);
        }
    } else {
        printf("| %" PRIu64 " ", a[idx]);
    }
    printf("|\n");
}

void sv_print(svfloat32_t va, int idx) {
    const int n = svcntw();
    std::vector<float> a(n);
    svbool_t tp = svptrue_b32();
    svst1_f32(tp, a.data(), va);
    printf("svfloat32_t variable:\n\t");
    if (idx < 0 || idx >= n) {
        for (int i = 0; i < n; ++i) {
            printf("| %+.4f ", a[i]);
        }
    } else {
        printf("| %+.4f ", a[idx]);
    }
    printf("|\n");
}

void sv_print(svfloat64_t va, int idx) {
    const int n = svcntd();
    std::vector<double> a(n);
    svbool_t tp = svptrue_b64();
    svst1_f64(tp, a.data(), va);
    printf("svfloat64_t variable:\n\t");
    if (idx < 0 || idx >= n) {
        for (int i = 0; i < n; ++i) {
            printf("| %+.7lf ", a[i]);
        }
    } else {
        printf("| %+.7lf ", a[idx]);
    }
    printf("|\n");
}

#endif // __ARM_FEATURE_SVE

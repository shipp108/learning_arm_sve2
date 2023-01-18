#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <arm_neon.h>
#include <arm_sve.h>
#include <sys/time.h>

class Timer {
public:
    Timer() {
        Reset();
    }

    void Reset() {
        gettimeofday(&tv, NULL);
    }

    float CostMs() {
        struct timeval tv_e;
        gettimeofday(&tv_e, NULL);
        return ((tv_e.tv_sec - tv.tv_sec) * 1e3) + ((tv_e.tv_usec - tv.tv_usec) * 1e-3);
    }

private:
    struct timeval tv;
};

template<typename Tp>
uint8_t CastU8(Tp val) {
    uint8_t tmp = (val < 0) ? (0) : ((val > 255) ? 255 : (uint8_t)(val));
    return tmp;
}

void WriteRawFile(const char *fname, const uint8_t *data, int length) {
    FILE *fp = fopen(fname, "w+");
    if (fp) {
        fwrite(data, 1, length, fp);
        fclose(fp);
    }
}

int Cmp(const uint8_t *ref, const uint8_t *res, size_t length) {
    size_t err_cnt = 0;
    for (size_t i = 0; i < length; ++i) {
        if (ref[i] != res[i]) {
            err_cnt++;
        }
    }
    return err_cnt;
}

void RGB2GrayC(const uint8_t *rgb, uint8_t *gray, int width, int height) {
    const size_t total_sz = width * height;
    for (size_t idx = 0; idx < total_sz; ++idx) {
        const size_t off = idx * 3;
        uint16_t temp = rgb[off] * 77 + rgb[off + 1] * 150 + rgb[off + 2] * 29;
        gray[idx] = temp >> 8;
    }
}

void RGB2GrayNeon(const uint8_t *rgb, uint8_t *gray, int width, int height) {
    uint8x8_t w_r = vdup_n_u8(77);
    uint8x8_t w_g = vdup_n_u8(150);
    uint8x8_t w_b = vdup_n_u8(29);

    // Note: total_sz should gt 8
    const size_t total_sz = width * height;
    const size_t align_sz = total_sz & (~7);
    size_t idx = 0;
    for (; idx < align_sz; idx += 8) {
    LOOP_BODY:
        uint8x8x3_t src = vld3_u8(rgb + idx * 3);
        uint16x8_t temp = vmull_u8(src.val[0], w_r);
        temp = vmlal_u8(temp, src.val[1], w_g);
        temp = vmlal_u8(temp, src.val[2], w_b);
        uint8x8_t result = vshrn_n_u16(temp, 8);
        vst1_u8(gray + idx, result);
    }
    if (align_sz != total_sz) {
        idx = total_sz - 8;
        goto LOOP_BODY;
    }
}

void RGB2GraySVE(const uint8_t *rgb, uint8_t *gray, int width, int height) {
    const size_t total_sz = width * height;
    size_t vl = svcntb();
    for (size_t idx = 0; idx < total_sz; idx += vl) {
        svbool_t predicate = svwhilelt_b8(idx, total_sz);
        svuint8x3_t src = svld3_u8(predicate, rgb + idx * 3);
        svuint8_t val = svget3_u8(src, 0);
        svuint16_t tmp_b = svmullb_n_u16(val, 77);
        svuint16_t tmp_t = svmullt_n_u16(val, 77);

        val = svget3_u8(src, 1);
        tmp_b = svmlalb_n_u16(tmp_b, val, 150);
        tmp_t = svmlalt_n_u16(tmp_t, val, 150);

        val = svget3_u8(src, 2);
        tmp_b = svmlalb_n_u16(tmp_b, val, 29);
        tmp_t = svmlalt_n_u16(tmp_t, val, 29);

        val = svqshrnb_n_u16(tmp_b, 8);
        val = svqshrnt_n_u16(val, tmp_t, 8);
        svst1_u8(predicate, gray + idx, val);
    }
}

int main() {
    //int width = 640;
    int width = 641;
    int height = 480;

    const size_t total_sz = width * height;

    uint8_t *rgb = nullptr;
    uint8_t *gray_c = nullptr;
    uint8_t *gray_n = nullptr;
    uint8_t *gray_s = nullptr;

    posix_memalign((void**)(&rgb), 128, 3 * total_sz);
    posix_memalign((void**)(&gray_c), 128, total_sz);
    posix_memalign((void**)(&gray_n), 128, total_sz);
    posix_memalign((void**)(&gray_s), 128, total_sz);

    // init rgb
    {
        for (int h = 0; h < height; ++h) {
            const float g = (h * 255.f) / height;
            const uint8_t bg = CastU8(g);
            const int h_off = h * width * 3;
            for (int w = 0; w < width; ++w) {
                float r = (w * 255.f) / width;
                float b = r * g / 255.f;
                int off = h_off + w * 3;
                rgb[off] = CastU8(r);
                rgb[off + 1] = bg;
                rgb[off + 2] = CastU8(b);
            }
        }
    }
    memset(gray_c, 0, total_sz);
    memset(gray_n, 0, total_sz);
    memset(gray_s, 0, total_sz);

    Timer t;
    RGB2GrayC(rgb, gray_c, width, height);
    float cost0 = t.CostMs();

    t.Reset();
    RGB2GrayNeon(rgb, gray_n, width, height);
    float cost1 = t.CostMs();

    t.Reset();
    RGB2GraySVE(rgb, gray_s, width, height);
    float cost2 = t.CostMs();

    int err_neon = Cmp(gray_c, gray_n, total_sz);
    int err_sve = Cmp(gray_c, gray_s, total_sz);

    printf("Process done.\n");
    printf("neon err: %d\nsve  err: %d\n", err_neon, err_sve);
    //printf("time cost: \n\tc    : %f ms\n\tneon : %f ms\n\tsve  : %f ms\n", cost0, cost1, cost2);

    //WriteRawFile("rgb_641x480x3", rgb, total_sz * 3);
    //WriteRawFile("gray_c_641x480", gray_c, total_sz);
    //WriteRawFile("gray_neon_641x480", gray_n, total_sz);
    //WriteRawFile("gray_sve_641x480", gray_s, total_sz);

    free(rgb);
    free(gray_c);
    free(gray_n);
    free(gray_s);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hist.h"

#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <arm_sve.h>

void print_hist(const char *name, uint32_t *hist) {
    printf("%s:\n", name);
    for (size_t i = 0; i < 256; ++i) {
        printf("| %3zu: %4d ", i, hist[i]);
        if ((i + 1) % 8 == 0) {
            printf("|\n");
        }
    }
}

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


int main() {
    srand(time(NULL));

    const size_t size = 4000 * 3000;
    uint8_t *data = nullptr;
    uint32_t *hist1 = nullptr;
    uint32_t *hist2 = nullptr;
    uint32_t *hist3 = nullptr;

    #if 1
    if (posix_memalign((void**)&data, 128, size)) {
        fprintf(stderr, "posix_memalign failed\n");
        return -1;
    }

    if (posix_memalign((void**)&hist1, 128, 256 * sizeof(uint32_t))) {
        fprintf(stderr, "posix_memalign failed\n");
        return -1;
    }
    if (posix_memalign((void**)&hist2, 128, 256 * sizeof(uint32_t))) {
        fprintf(stderr, "posix_memalign failed\n");
        return -1;
    }
    if (posix_memalign((void**)&hist3, 128, 256 * sizeof(uint32_t))) {
        fprintf(stderr, "posix_memalign failed\n");
        return -1;
    }
    #else
    data = (uint8_t *)malloc(size);
    hist1 = (uint32_t *)malloc(256 * sizeof(uint32_t));
    hist2 = (uint32_t *)malloc(256 * sizeof(uint32_t));
    hist3 = (uint32_t *)malloc(256 * sizeof(uint32_t));
    #endif

    for (size_t i = 0; i < size; ++i) {
        data[i] = rand() & 0xFF;
    }

    // warmup
    histogram_c(hist1, data, size);
    histogram_c(hist1, data, size);
    histogram_c(hist1, data, size);

    int loop_cnt = 1;
    Timer t;
    for (int i = 0; i < loop_cnt; ++i) {
        memset(hist1, 0, sizeof(uint32_t) * 256);
        histogram_c(hist1, data, size);
    }
    float cost0 = t.CostMs();

    printf("vlen: %lu\n", svcntb() * 8);

    t.Reset();
    for (int i = 0; i < loop_cnt; ++i) {
        memset(hist2, 0, sizeof(uint32_t) * 256);
        histogram_sve_gather(hist2, data, size);
    }
    float cost1 = t.CostMs();

    t.Reset();
    for (int i = 0; i < loop_cnt; ++i) {
        memset(hist3, 0, sizeof(uint32_t) * 256);
        histogram_sve_common(hist3, data, size);
    }
    float cost2 = t.CostMs();

    printf("c     cost: %f ms\n", cost0 / loop_cnt);
    printf("sve 0 cost: %f ms\n", cost1 / loop_cnt);
    printf("sve 1 cost: %f ms\n", cost2 / loop_cnt);

    //print_hist("hist c", hist1);
    //print_hist("hist sve slow", hist2);
    //print_hist("hist sve common", hist3);

    bool cmp_success = true;
    for (int i = 0; i < 256; ++i) {
        if (hist1[i] != hist2[i] || hist1[i] != hist3[i]) {
            printf("diff: %d %d %d\n", hist1[i], hist2[i], hist3[i]);
            cmp_success = false;
        }
    }
    if (cmp_success) {
        printf("cmp success\n");
    }

    free(data);
    free(hist1);
    free(hist2);
    free(hist3);

    return 0;
}
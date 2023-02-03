#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <malloc.h>

#include <arm_sve.h>

#include "sv_print.hpp"

void TestLoadWithFFR() {
    const int page_size  = getpagesize();
    const int numbers = page_size;
    const size_t total_bytes = numbers * sizeof(uint8_t);

    #define USING_MMAP 1
    #ifdef USING_MMAP
    uint8_t *ptr = (uint8_t*)mmap(NULL, total_bytes,
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if(ptr == MAP_FAILED){
        printf("Mapping Failed\n");
        return;
    }
    #else // USING_MMAP
    uint8_t *ptr = (uint8_t*)malloc(total_bytes);
    #endif // USING_MMAP

    for(int i = 0; i < numbers; i++) {
        ptr[i] = i & 0xFF;
    }

    svbool_t pg = svptrue_b8();
    // valid address
    {
        printf("**************************************\n");
        printf("LDFF test: all success\n");
        int start_idx = numbers - svcntb();

        svsetffr();
        svuint8_t x = svldff1_u8(pg, ptr + start_idx);
        printf("data: ");
        sv_print(x);

        svbool_t ffr = svrdffr();
        printf("ffr : ");
        sv_print(ffr);
    }

    // with invalid address
    {
        printf("\n**************************************\n");
        printf("LDFF test: with half faulting\n");
        int start_idx = numbers - svcntb() / 2;

        svsetffr();
        svuint8_t x = svldff1_u8(pg, ptr + start_idx);
        printf("data: ");
        sv_print(x);

        svbool_t ffr = svrdffr();
        printf("ffr : ");
        sv_print(ffr);
    }

    // first invalid address, should using ldnf(non-fault load)
    {
        printf("\n**************************************\n");
        printf("LDNF test: null faulting in first\n");

        svsetffr();
        svuint8_t x = svldnf1_u8(pg, NULL);
        printf("data: ");
        sv_print(x);

        svbool_t ffr = svrdffr();
        printf("ffr : ");
        sv_print(ffr);
    }

    #ifdef USING_MMAP
    int err = munmap(ptr, total_bytes);
    if(err != 0){
        printf("UnMapping Failed\n");
    }
    #else
    free(ptr);
    #endif
}

int main(int argc, char *argv[])
{
    TestLoadWithFFR();

    return 0;
}
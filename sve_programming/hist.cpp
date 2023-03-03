
#include "hist.h"
#include <stdio.h>
#include <arm_sve.h>

void histogram_c(uint32_t *hist, uint8_t *data, size_t sz) {
    for (size_t i = 0; i < sz; ++i) {
        hist[data[i]]++;
    }
}

void histogram_sve_gather(uint32_t *hist, uint8_t *data, size_t sz) {
    uint64_t vl = svcntw();
    for (size_t i = 0; i < sz; i += vl) {
        svbool_t vpred = svwhilelt_b32(i, sz);
        svuint32_t vd = svld1ub_u32(vpred, data + i);
        svuint32_t vh = svld1_gather_index(vpred, hist, vd);
        svuint32_t vc = svhistcnt_z(vpred, vd, vd);
        vh = svadd_m(vpred, vh, vc);
        svst1_scatter_index(vpred, hist, vd, vh);
    }
}

void histogram_sve_common(uint32_t *hist, uint8_t *data, size_t sz) {
    svuint8_t vbidx_vl = svindex_u8(0, 1); // (0, 1, 2, ..., 16, ...), quadword, valid sz = 16

    svbool_t vb_mask = svcmplt(svptrue_b8(), vbidx_vl, 16);
    svbool_t vw_mask = svcmplt(svptrue_b32(), svindex_u32(0, 1), 4);
    const size_t sz_align = (sz & (~15)) / 4;
    for (size_t i = 0; i < sz_align; i += 16) {
        svuint8x4_t vd4 = svld4(vb_mask, data + (i * 4));

        for (size_t j = 0; j < 16; ++j) {
            const uint8_t hist_off = j * 16;
            uint32_t *hist_ptr = hist + hist_off;
            // (0, 1, 2, ..., 16, ...) + (j * 16)
            svuint8_t vbidx_t_vl = svadd_z(svptrue_b8(), vbidx_vl, hist_off);

            svuint8_t vtmp0 = svhistseg(vbidx_t_vl, svget4(vd4, 0));
            svuint8_t vtmp1 = svhistseg(vbidx_t_vl, svget4(vd4, 1));
            svuint8_t vtmp2 = svhistseg(vbidx_t_vl, svget4(vd4, 2));
            svuint8_t vtmp3 = svhistseg(vbidx_t_vl, svget4(vd4, 3));

            // (16 * 4 = 64) < (max<u8>)
            vtmp0 = svadd_z(svptrue_b8(), vtmp0, vtmp1);
            vtmp2 = svadd_z(svptrue_b8(), vtmp2, vtmp3);
            svuint8_t vr = svadd_z(svptrue_b8(), vtmp0, vtmp2);

            svuint32x2_t vhist0 = svld2(vw_mask, hist_ptr);
            svuint32x2_t vhist1 = svld2(vw_mask, hist_ptr + 8);

            svuint16_t vhr = svunpklo(vr);
            svuint32_t vw0 = svaddwb(svget2(vhist0, 0), vhr);
            svuint32_t vw1 = svaddwt(svget2(vhist0, 1), vhr);
            vhr = svunpklo(svext(vr, vr, 8));
            svuint32_t vw2 = svaddwb(svget2(vhist1, 0), vhr);
            svuint32_t vw3 = svaddwt(svget2(vhist1, 1), vhr);

            vhist0 = svset2(vhist0, 0, vw0);
            vhist0 = svset2(vhist0, 1, vw1);
            vhist1 = svset2(vhist1, 0, vw2);
            vhist1 = svset2(vhist1, 1, vw3);

            svst2(vw_mask, hist_ptr, vhist0);
            svst2(vw_mask, hist_ptr + 8, vhist1);
        }
    }
    for (size_t i = sz_align * 4; i < sz; ++i) {
        hist[data[i]]++;
    }
}

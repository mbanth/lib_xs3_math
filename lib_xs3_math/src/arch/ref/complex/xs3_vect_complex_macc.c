// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"


////////////////////////////////////////
//      16-Bit Multiplication         //
////////////////////////////////////////


headroom_t xs3_vect_complex_s16_macc(
    int16_t acc_real[],
    int16_t acc_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t bc_sat)
{
    for(int k = 0; k < length; k++){

        acc_real[k] = vlashr16( acc_real[k], acc_shr );
        acc_imag[k] = vlashr16( acc_imag[k], acc_shr );
        
        complex_s32_t B = {b_real[k], b_imag[k]};
        complex_s32_t C = {c_real[k], c_imag[k]};

        struct{ int64_t re; int64_t im; } P = {
            ((int64_t)B.re * C.re) - B.im * C.im,
            ((int64_t)B.re * C.im) + B.im * C.re,
        };

        P.re = ROUND_SHR(P.re, bc_sat);
        P.im = ROUND_SHR(P.im, bc_sat);

        acc_real[k] = vladd16( acc_real[k], SAT(16)(P.re));
        acc_imag[k] = vladd16( acc_imag[k], SAT(16)(P.im));
    }

    return xs3_vect_complex_s16_headroom(acc_real, acc_imag, length);
}

headroom_t xs3_vect_complex_s16_nmacc(
    int16_t acc_real[],
    int16_t acc_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t bc_sat)
{
    for(int k = 0; k < length; k++){

        acc_real[k] = vlashr16( acc_real[k], acc_shr );
        acc_imag[k] = vlashr16( acc_imag[k], acc_shr );
        
        complex_s32_t B = {b_real[k], b_imag[k]};
        complex_s32_t C = {c_real[k], c_imag[k]};

        struct{ int64_t re; int64_t im; } P = {
            ((int64_t)B.re * C.re) - B.im * C.im,
            ((int64_t)B.re * C.im) + B.im * C.re,
        };

        P.re = ROUND_SHR(P.re, bc_sat);
        P.im = ROUND_SHR(P.im, bc_sat);

        acc_real[k] = vlsub16( acc_real[k], SAT(16)(P.re));
        acc_imag[k] = vlsub16( acc_imag[k], SAT(16)(P.im));
    }

    return xs3_vect_complex_s16_headroom(acc_real, acc_imag, length);
}







////////////////////////////////////////
//      32-Bit Multiplication         //
////////////////////////////////////////


headroom_t xs3_vect_complex_s32_macc(
    complex_s32_t acc[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    for(int k = 0; k < length; k++){
        
        complex_s32_t B = {
            ASHR(32)(b[k].re, b_shr), 
            ASHR(32)(b[k].im, b_shr),
        };
        complex_s32_t C = {
            ASHR(32)(c[k].re, c_shr), 
            ASHR(32)(c[k].im, c_shr),
        };

        int64_t q1 = ROUND_SHR( ((int64_t)B.re) * C.re, 30 );
        int64_t q2 = ROUND_SHR( ((int64_t)B.im) * C.im, 30 );
        int64_t q3 = ROUND_SHR( ((int64_t)B.re) * C.im, 30 );
        int64_t q4 = ROUND_SHR( ((int64_t)B.im) * C.re, 30 );

        acc[k].re = vladd32( vlashr32( acc[k].re, acc_shr ), SAT(32)(q1 - q2) );
        acc[k].im = vladd32( vlashr32( acc[k].im, acc_shr ), SAT(32)(q3 + q4) );
    }

    return xs3_vect_complex_s32_headroom(acc, length);
}

headroom_t xs3_vect_complex_s32_nmacc(
    complex_s32_t acc[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    for(int k = 0; k < length; k++){
        
        complex_s32_t B = {
            ASHR(32)(b[k].re, b_shr), 
            ASHR(32)(b[k].im, b_shr),
        };
        complex_s32_t C = {
            ASHR(32)(c[k].re, c_shr), 
            ASHR(32)(c[k].im, c_shr),
        };

        int64_t q1 = ROUND_SHR( ((int64_t)B.re) * C.re, 30 );
        int64_t q2 = ROUND_SHR( ((int64_t)B.im) * C.im, 30 );
        int64_t q3 = ROUND_SHR( ((int64_t)B.re) * C.im, 30 );
        int64_t q4 = ROUND_SHR( ((int64_t)B.im) * C.re, 30 );

        acc[k].re = vlsub32( vlashr32( acc[k].re, acc_shr ), SAT(32)(q1 - q2) );
        acc[k].im = vlsub32( vlashr32( acc[k].im, acc_shr ), SAT(32)(q3 + q4) );
    }

    return xs3_vect_complex_s32_headroom(acc, length);
}




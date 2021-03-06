/*
 * Copyright (c) 2007 Wayne Meissner. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * Neither the name of the project nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifdef __sparc
    #define fix_mem_access __asm("ta 6")
#else
    #define fix_mem_access
#endif

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef signed long sL;
typedef unsigned long uL;
typedef float f32;
typedef double f64;
typedef unsigned long ulong;

#define ADD(T) T add_##T(T arg1, T arg2) { return arg1 + arg2; }
#define SUB(T) T sub_##T(T arg1, T arg2) { return arg1 - arg2; }
#define MUL(T) T mul_##T(T arg1, T arg2) { return arg1 * arg2; }
#define DIV(T) T div_##T(T arg1, T arg2) { return arg1 / arg2; }
#define RET(T) T ret_##T(T arg1) { return arg1; }
#define SET(T) static T T##_;void set_##T(T arg1) { T##_ = arg1; }
#define GET(T) T get_##T() { return T##_; }
typedef char* ptr;
#define TEST(T) ADD(T) SUB(T) MUL(T) DIV(T) RET(T) SET(T) GET(T)
TEST(s8);
TEST(u8);
TEST(s16);
TEST(u16);
TEST(s32);
TEST(u32);
TEST(s64);
TEST(u64);
TEST(float);
TEST(double);
TEST(long);
TEST(ulong);

#define ADD2(R, T1, T2) R add_##T1##T2##_##R(T1 arg1, T2 arg2) { return arg1 + arg2; }
#define SUB2(R, T1, T2) R sub_##T1##T2##_##R(T1 arg1, T2 arg2) { return arg1 - arg2; }
#define MUL2(R, T1, T2) R mul_##T1##T2##_##R(T1 arg1, T2 arg2) { return arg1 * arg2; }
#define DIV2(R, T1, T2) R div_##T1##T2##_##R(T1 arg1, T2 arg2) { return arg1 / arg2; }

#define T2__(R, T1, T2) ADD2(R, T1, T2) SUB2(R, T1, T2) MUL2(R, T1, T2) DIV2(R, T1, T2)
#define T2_(R, T1) \
    T2__(R, T1, s8) T2__(R, T1, u8) \
    T2__(R, T1, s16) T2__(R, T1, u16) \
    T2__(R, T1, s32) T2__(R, T1, u32) \
    T2__(R, T1, sL) T2__(R, T1, uL) \
    T2__(R, T1, s64) T2__(R, T1, u64) \

#define TEST2(R) \
    T2_(R, s8) T2_(R, u8) T2_(R, s16) T2_(R, u16) T2_(R, s32) T2_(R, u32) \
    T2_(R, sL) T2_(R, uL) T2_(R, s64) T2_(R, u64)

#ifdef notyet
TEST2(s32)
TEST2(u32)
TEST2(s64)
TEST2(u64)
#endif

#define ADD3(R, T1, T2, T3) R add_##T1##T2##T3##_##R(T1 arg1, T2 arg2, T3 arg3) { return arg1 + arg2 + arg3; }
#define pack_f32(buf, v) do { *(float *)(buf) = v; } while(0)
#define pack_f64(buf, v) do { *(double *)(buf) = v; } while(0)
#define pack_int(buf, v) do { *(buf) = v; } while(0)
#define pack_s8 pack_int
#define pack_u8 pack_int
#define pack_s16 pack_int
#define pack_u16 pack_int
#define pack_s32 pack_int
#define pack_u32 pack_int
#define pack_s64 pack_int
#define pack_u64 pack_int
#define pack_sL pack_int
#define pack_uL pack_int

#define PACK3(R, T1, T2, T3) void pack_##T1##T2##T3##_##R(T1 arg1, T2 arg2, T3 arg3, R* r) { \
    fix_mem_access; \
    pack_##T1(&r[0], arg1); \
    pack_##T2(&r[1], arg2); \
    pack_##T3(&r[2], arg3); \
}

#define T3___(R, T1, T2, T3) PACK3(R, T1, T2, T3) /* SUB2(R, T1, T2) MUL2(R, T1, T2) DIV2(R, T1, T2) */
#define T3__(R, T1, T2) \
    T3___(R, T1, T2, s8) T3___(R, T1, T2, u8) \
    T3___(R, T1, T2, s16) T3___(R, T1, T2, u16) \
    T3___(R, T1, T2, s32) T3___(R, T1, T2, u32) \
    T3___(R, T1, T2, sL) T3___(R, T1, T2, uL) \
    T3___(R, T1, T2, s64) T3___(R, T1, T2, u64) \
    T3___(R, T1, T2, f32) T3___(R, T1, T2, f64) \

#define T3_(R, T1) \
    T3__(R, T1, s8) T3__(R, T1, u8) \
    T3__(R, T1, s16) T3__(R, T1, u16) \
    T3__(R, T1, s32) T3__(R, T1, u32) \
    T3__(R, T1, sL) T3__(R, T1, uL) \
    T3__(R, T1, s64) T3__(R, T1, u64) \
    T3__(R, T1, f32) T3__(R, T1, f64) \

#define TEST3(R) \
    T3_(R, s8) T3_(R, u8) T3_(R, s16) T3_(R, u16) T3_(R, s32) T3_(R, u32) \
    T3_(R, sL) T3_(R, uL) T3_(R, s64) T3_(R, u64) T3_(R, f32) T3_(R, f64)

TEST3(s64)


/*
 * Copyright (c) 2017, deadcafe.beef@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither the name of the project nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stddef.h>

#include "cpuid.h"

enum cpuid_reg_e {
        CPUID_REG_EAX = 0,
        CPUID_REG_EBX,
        CPUID_REG_ECX,
        CPUID_REG_EDX,

        CPUID_REG_NB,
};

struct cpuid_s {
        unsigned leaf;
        unsigned sub_leaf;
        unsigned reg[CPUID_REG_NB];
};

#ifndef ARRAYOF
# define ARRAYOF(_a)	(sizeof(_a)/sizeof(_a[0]))
#endif

#define CPUID_SUB_LEAF_UNSPEC   0
#define CPUID_BASIC             0x0U
#define CPUID_EXT               0x80000000U
#define CPUID_INVALID		(unsigned) (-1)

static inline int
cpuid_exec(struct cpuid_s *cpuid,
           const unsigned leaf,
           unsigned sub_leaf)
{
        int ret = -1;

        __asm__ __volatile__ ("cpuid\n\t"
                              : "=a" (cpuid->reg[CPUID_REG_EAX]),
                                "=b" (cpuid->reg[CPUID_REG_EBX]),
                                "=c" (cpuid->reg[CPUID_REG_ECX]),
                                "=d" (cpuid->reg[CPUID_REG_EDX])
                              : "a" (leaf),
                                "c" (sub_leaf)
                              );

        /* all Zero then invalid */
        if (cpuid->reg[CPUID_REG_EAX] |
            cpuid->reg[CPUID_REG_EBX] |
            cpuid->reg[CPUID_REG_ECX] |
            cpuid->reg[CPUID_REG_EDX]) {
                cpuid->leaf = leaf;
                cpuid->sub_leaf = sub_leaf;
                ret = 0;
        } else {
                cpuid->leaf = CPUID_INVALID;
                cpuid->sub_leaf = CPUID_INVALID;
        }
        return ret;
}

/*
 *
 */
static unsigned
cpuid_reg_read(struct cpuid_s *cpuid,
               unsigned leaf,
               unsigned sub_leaf,
               enum cpuid_reg_e reg_id)
{
        unsigned ret = 0;

        if (cpuid->leaf == leaf && cpuid->sub_leaf == sub_leaf) {
                ret = cpuid->reg[reg_id];
        } else {
                /* read MAX CPUID_BASIC or CPUID_EXT */
                if (!cpuid_exec(cpuid, CPUID_EXT & leaf, 0)) {

                        if (cpuid->reg[CPUID_REG_EAX] >= leaf) {
                                if (!cpuid_exec(cpuid, leaf, sub_leaf)) {
                                        ret = cpuid->reg[reg_id];
                                }
                        }
                }
        }
        return ret;
}

struct cpuid_list {
        const char *name;
        unsigned flag;
        unsigned leaf;
        unsigned sub_leaf;
        enum cpuid_reg_e reg;
        unsigned mask;
};

static const struct cpuid_list cpuid_list[CPUID_NB] = {
        [CPUID_BIT_AESNI] = { /* CPUID.01H:ECX.AESNI[bit 25] */
                .name     = "AESNI",
                .flag     = CPUID_FLAGS_AESNI,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
                .reg      = CPUID_REG_ECX,
                .mask     = (1u << 25),
        },
        [CPUID_BIT_PCLMULQDQ] = { /* CPUID.01H:ECX.PCLMULQDQ[bit 1] */
                .name     = "PCLMULQDQ",
                .flag     = CPUID_FLAGS_PCLMULQDQ,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
                .reg      = CPUID_REG_ECX,
                .mask     = (1u << 1),
        },
        [CPUID_BIT_SSE] = { /* CPUID.01H:ECX.SSE[bit 0] */
                .name     = "SSE",
                .flag     = CPUID_FLAGS_SSE,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
                .reg      = CPUID_REG_ECX,
                .mask     = (1u << 0),
        },
        [CPUID_BIT_AVX] = { /* CPUID.01H:ECX.AVX[bit 28] */
                .name     = "AVX",
                .flag     = CPUID_FLAGS_AVX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
                .reg      = CPUID_REG_ECX,
                .mask     = (1u << 28),
        },
        [CPUID_BIT_AVX2] = { /* CPUID.07H:EBX.AVX2[bit 5] */
                .name     = "AVX2",
                .flag     = CPUID_FLAGS_AVX2,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
                .reg      = CPUID_REG_EBX,
                .mask     = (1u << 5),
        },
        [CPUID_BIT_AVX512F] = { /* CPUID.07H:ECX.AVX512F[bit 16] */
                .name     = "AVX512F",
                .flag     = CPUID_FLAGS_AVX512F,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
                .reg      = CPUID_REG_ECX,
                .mask     = (1u << 16),
        },
        [CPUID_BIT_SHANI] = { /* CPUID.07H:ECX.SHANI[bit 29] */
                .name     = "SHANI",
                .flag     = CPUID_FLAGS_SHANI,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
                .reg      = CPUID_REG_ECX,
                .mask     = (1u << 29),
        },
};

/*
 *
 */
unsigned
cpuid_flags_read(unsigned flags)
{
        unsigned ret = 0;
        struct cpuid_s cpuid = {
                .leaf     = CPUID_INVALID,
                .sub_leaf = CPUID_INVALID,
        };

        for (unsigned i = 0; i < ARRAYOF(cpuid_list); i++) {
                if (cpuid_list[i].flag & flags) {
                        unsigned reg;
                        reg = cpuid_reg_read(&cpuid,
                                             cpuid_list[i].leaf,
                                             cpuid_list[i].sub_leaf,
                                             cpuid_list[i].reg);
                        if (reg & cpuid_list[i].mask)
                                ret |= cpuid_list[i].flag;
                }
        }
        return ret;
}

const char *
cpuid_name(enum cpuid_bit_e id)
{
        if (id < 0 || id >= CPUID_NB)
                return NULL;
        return cpuid_list[id].name;
}

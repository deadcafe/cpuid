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

#include <stddef.h>
#include <string.h>

#include "cpuid.h"

enum cpuid_reg_e {
        CPUID_REG_EAX = 0,
        CPUID_REG_EBX,
        CPUID_REG_ECX,
        CPUID_REG_EDX,

        CPUID_REG_NB,
};

struct cpuid_attr {
        const char *name;
        unsigned bit;
        enum cpuid_reg_e reg;
        unsigned leaf;
        unsigned sub_leaf;
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


/*
 * cpuids 
 */
static const struct cpuid_attr cpuid_attr[] = {
        {
                .name     = "fpu",
                .bit      = 0,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "vme",
                .bit      = 1,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "de",
                .bit      = 2,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pse",
                .bit      = 3,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "tsc",
                .bit      = 4,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "msr",
                .bit      = 5,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pae",
                .bit      = 6,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "mce",
                .bit      = 7,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "cx8",
                .bit      = 8,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "apic",
                .bit      = 9,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "sep",
                .bit      = 11,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "mtrr",
                .bit      = 12,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pge",
                .bit      = 13,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "mca",
                .bit      = 14,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "cmov",
                .bit      = 15,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pat",
                .bit      = 16,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pse-36",
                .bit      = 17,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "psn",
                .bit      = 18,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "clfsh",
                .bit      = 19,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "ds",
                .bit      = 21,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "acpi",
                .bit      = 22,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "mmx",
                .bit      = 23,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "fxsr",
                .bit      = 24,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "sse",
                .bit      = 25,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "sse2",
                .bit      = 26,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "ss",
                .bit      = 27,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "htt",
                .bit      = 28,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "tm",
                .bit      = 29,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "ia64",
                .bit      = 30,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pbe",
                .bit      = 31,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },

        {
                .name     = "sse3",
                .bit      = 0,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pclmulqdq",
                .bit      = 1,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "dtes64",
                .bit      = 2,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "monitor",
                .bit      = 3,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "ds-cpl",
                .bit      = 4,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "vmx",
                .bit      = 5,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "smx",
                .bit      = 6,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "est",
                .bit      = 7,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "tm2",
                .bit      = 8,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "ssse3",
                .bit      = 9,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "cnxt-id",
                .bit      = 10,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "sdbg",
                .bit      = 11,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "fma",
                .bit      = 12,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "cx16",
                .bit      = 13,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "xtpr",
                .bit      = 14,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pdcm",
                .bit      = 15,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pcid",
                .bit      = 17,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "dca",
                .bit      = 18,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "sse4.1",
                .bit      = 19,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "sse4.2",
                .bit      = 20,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "x2apic",
                .bit      = 21,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "movbe",
                .bit      = 22,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "popcnt",
                .bit      = 23,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "tsc-deadline",
                .bit      = 24,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "aes",
                .bit      = 25,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "xsave",
                .bit      = 26,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "osxsave",
                .bit      = 27,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx",
                .bit      = 28,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "f16c",
                .bit      = 29,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "rdrnd",
                .bit      = 30,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "hypervisor",
                .bit      = 31,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x01,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },

        {
                .name     = "fsgsbase",
                .bit      = 0,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "sgx",
                .bit      = 2,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "bmi1",
                .bit      = 3,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "hle",
                .bit      = 4,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx2",
                .bit      = 5,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "smep",
                .bit      = 7,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "bmi2",
                .bit      = 8,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "erms",
                .bit      = 9,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "invpcid",
                .bit      = 10,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "rtm",
                .bit      = 11,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pqm",
                .bit      = 12,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "mpx",
                .bit      = 14,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pq",
                .bit      = 15,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx512f",
                .bit      = 16,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx512dq",
                .bit      = 17,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "rdseed",
                .bit      = 18,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "adx",
                .bit      = 19,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "smap",
                .bit      = 20,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx512ifma",
                .bit      = 21,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pcommit",
                .bit      = 22,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "clfushopt",
                .bit      = 23,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "clwb",
                .bit      = 24,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "intel_pt",
                .bit      = 25,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx512pf",
                .bit      = 26,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx512er",
                .bit      = 27,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx512cd",
                .bit      = 28,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "sha",
                .bit      = 29,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx512bw",
                .bit      = 30,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx512vl",
                .bit      = 31,
                .reg      = CPUID_REG_EBX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },


        {
                .name     = "prefetchwt1",
                .bit      = 0,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx512vbmi",
                .bit      = 1,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "umip",
                .bit      = 2,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "pku",
                .bit      = 3,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "ospke",
                .bit      = 4,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx512vpopvntdq",
                .bit      = 14,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "rdpid",
                .bit      = 22,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "sgx_lc",
                .bit      = 30,
                .reg      = CPUID_REG_ECX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },


        {
                .name     = "avx512_4vnniw",
                .bit      = 2,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },
        {
                .name     = "avx512_4fmaps",
                .bit      = 3,
                .reg      = CPUID_REG_EDX,
                .leaf     = CPUID_BASIC | 0x07,
                .sub_leaf = CPUID_SUB_LEAF_UNSPEC,
        },


        {/* terminator */
                .name     = NULL,
        },
};

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

/*
 * max number of names: 32 names
 */
unsigned
cpuid_flags_read(const char **names)
{
        unsigned bits = 0;
        struct cpuid_s cpuid = {
                .leaf     = CPUID_INVALID,
                .sub_leaf = CPUID_INVALID,
        };

        for (unsigned n = 0; names[n]; n++) {
                for (unsigned i = 0; cpuid_attr[i].name; i++) {
                        unsigned reg;
                        
                        if (strcmp(names[n], cpuid_attr[i].name))
                                continue;

                        reg = cpuid_reg_read(&cpuid,
                                             cpuid_attr[i].leaf,
                                             cpuid_attr[i].sub_leaf,
                                             cpuid_attr[i].reg);
                        if (reg & (1u << cpuid_attr[i].bit))
                                bits |= (1u << n);
                }
        }
        return bits;
}

#ifndef _CPUID_H_
#define _CPUID_H_

enum cpuid_bit_e {
        CPUID_BIT_AESNI = 0,
        CPUID_BIT_PCLMULQDQ,
        CPUID_BIT_SSE,
        CPUID_BIT_AVX,
        CPUID_BIT_AVX2,
        CPUID_BIT_AVX512F,
        CPUID_BIT_SHANI,

        CPUID_NB,
};

#define CPUID_FLAGS_AESNI	(1u << CPUID_BIT_AESNI)
#define CPUID_FLAGS_PCLMULQDQ	(1u << CPUID_BIT_PCLMULQDQ)
#define CPUID_FLAGS_SSE		(1u << CPUID_BIT_SSE)
#define CPUID_FLAGS_AVX		(1u << CPUID_BIT_AVX)
#define CPUID_FLAGS_AVX2	(1u << CPUID_BIT_AVX2)
#define CPUID_FLAGS_AVX512F	(1u << CPUID_BIT_AVX512F)
#define CPUID_FLAGS_SHANI	(1u << CPUID_BIT_SHANI)


extern unsigned cpuid_flags_read(unsigned flags);
extern const char *cpuid_name(enum cpuid_bit_e);

#endif /* !_CPUID_H_ */

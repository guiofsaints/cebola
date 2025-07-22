#include <stdio.h>
#include <stdint.h>
#include <string.h>

//
//	arm NEON / C integer scalers for ARMv7 devices
//	args/	src :	src offset		address of top left corner
//		dst :	dst offset		address	of top left corner
//		sw  :	src width		pixels
//		sh  :	src height		pixels
//		sp  :	src pitch (stride)	bytes	if 0, (src width * [2|4]) is used
//		dp  :	dst pitch (stride)	bytes	if 0, (src width * [2|4] * multiplier) is used
//
//	** NOTE **
//	since 32bit aligned addresses need to be processed for NEON scalers,
//	x-offset and stride pixels must be even# in the case of 16bpp,
//	if odd#, then handled by the C scaler
//

//
//	C scalers
//
void scale1x_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp, uint32_t ymul) {
	if (!sw||!sh||!ymul) return;
	uint32_t swl = sw*sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*1; }
	if ((ymul == 1)&&(swl == sp)&&(sp == dp)) memcpy(dst, src, sp*sh);
	else {
		for (; sh>0; sh--, src=(uint8_t*)src+sp) {
			for (uint32_t i=ymul; i>0; i--, dst=(uint8_t*)dst+dp) memcpy(dst, src, swl);
		}
	}
}

void scale1x1_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale1x_c16(src, dst, sw, sh, sp, dp, 1); }
void scale1x2_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale1x_c16(src, dst, sw, sh, sp, dp, 2); }
void scale1x3_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale1x_c16(src, dst, sw, sh, sp, dp, 3); }
void scale1x4_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale1x_c16(src, dst, sw, sh, sp, dp, 4); }

void scale1x_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp, uint32_t ymul) {
	if (!sw||!sh||!ymul) return;
	uint32_t swl = sw*sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*1; }
	if ((ymul == 1)&&(swl == sp)&&(sp == dp)) memcpy(dst, src, sp*sh);
	else {
		for (; sh>0; sh--, src=(uint8_t*)src+sp) {
			for (uint32_t i=ymul; i>0; i--, dst=(uint8_t*)dst+dp) memcpy(dst, src, swl);
		}
	}
}

void scale1x1_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale1x_c32(src, dst, sw, sh, sp, dp, 1); }
void scale1x2_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale1x_c32(src, dst, sw, sh, sp, dp, 2); }
void scale1x3_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale1x_c32(src, dst, sw, sh, sp, dp, 3); }
void scale1x4_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale1x_c32(src, dst, sw, sh, sp, dp, 4); }

void scale2x_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp, uint32_t ymul) {
	if (!sw||!sh||!ymul) return;
	uint32_t x, dx, pix, dpix1, dpix2, swl = sw*sizeof(uint16_t);
	if (!sp) { sp = swl; } swl*=2; if (!dp) { dp = swl; }
	for (; sh>0; sh--, src=(uint8_t*)src+sp) {
		uint32_t *s = (uint32_t* __restrict)src;
		uint32_t *d = (uint32_t* __restrict)dst;
		for (x=dx=0; x<(sw/2); x++, dx+=2) {
			pix = s[x];
			dpix1=(pix & 0x0000FFFF)|(pix<<16);
			dpix2=(pix & 0xFFFF0000)|(pix>>16);
			d[dx] = dpix1; d[dx+1] = dpix2;
		}
		if (sw&1) {
			uint16_t *s16 = (uint16_t*)s;
			uint16_t pix16 = s16[x*2];
			d[dx] = pix16|(pix16<<16);
		}
		void* __restrict dstsrc = dst; dst = (uint8_t*)dst+dp;
		for (uint32_t i=ymul-1; i>0; i--, dst=(uint8_t*)dst+dp) memcpy(dst, dstsrc, swl);
	}
}

void scale2x1_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale2x_c16(src, dst, sw, sh, sp, dp, 1); }
void scale2x2_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale2x_c16(src, dst, sw, sh, sp, dp, 2); }
void scale2x3_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale2x_c16(src, dst, sw, sh, sp, dp, 3); }
void scale2x4_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale2x_c16(src, dst, sw, sh, sp, dp, 4); }

void scale2x_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp, uint32_t ymul) {
	if (!sw||!sh||!ymul) return;
	uint32_t x, dx, pix, swl = sw*sizeof(uint32_t);
	if (!sp) { sp = swl; } swl*=2; if (!dp) { dp = swl; }
	for (; sh>0; sh--, src=(uint8_t*)src+sp) {
		uint32_t *s = (uint32_t* __restrict)src;
		uint32_t *d = (uint32_t* __restrict)dst;
		for (x=dx=0; x<sw; x++, dx+=2) {
			pix = s[x];
			d[dx] = pix; d[dx+1] = pix;
		}
		void* __restrict dstsrc = dst; dst = (uint8_t*)dst+dp;
		for (uint32_t i=ymul-1; i>0; i--, dst=(uint8_t*)dst+dp) memcpy(dst, dstsrc, swl);
	}
}

void scale2x1_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale2x_c32(src, dst, sw, sh, sp, dp, 1); }
void scale2x2_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale2x_c32(src, dst, sw, sh, sp, dp, 2); }
void scale2x3_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale2x_c32(src, dst, sw, sh, sp, dp, 3); }
void scale2x4_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale2x_c32(src, dst, sw, sh, sp, dp, 4); }

void scale4x_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp, uint32_t ymul) {
	if (!sw||!sh||!ymul) return;
	uint32_t x, dx, pix, dpix1, dpix2, swl = sw*sizeof(uint16_t);
	if (!sp) { sp = swl; } swl*=4; if (!dp) { dp = swl; }
	for (; sh>0; sh--, src=(uint8_t*)src+sp) {
		uint32_t *s = (uint32_t* __restrict)src;
		uint32_t *d = (uint32_t* __restrict)dst;
		for (x=dx=0; x<(sw/2); x++, dx+=4) {
			pix = s[x];
			dpix1=(pix & 0x0000FFFF)|(pix<<16);
			dpix2=(pix & 0xFFFF0000)|(pix>>16);
			d[dx] = dpix1; d[dx+1] = dpix1; d[dx+2] = dpix2; d[dx+3] = dpix2;
		}
		if (sw&1) {
			uint16_t *s16 = (uint16_t*)s;
			uint16_t pix16 = s16[x*2];
			dpix1 = pix16|(pix16<<16);
			d[dx] = dpix1; d[dx+1] = dpix1;
		}
		void* __restrict dstsrc = dst; dst = (uint8_t*)dst+dp;
		for (uint32_t i=ymul-1; i>0; i--, dst=(uint8_t*)dst+dp) memcpy(dst, dstsrc, swl);
	}
}

void scale4x1_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale4x_c16(src, dst, sw, sh, sp, dp, 1); }
void scale4x2_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale4x_c16(src, dst, sw, sh, sp, dp, 2); }
void scale4x3_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale4x_c16(src, dst, sw, sh, sp, dp, 3); }
void scale4x4_c16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale4x_c16(src, dst, sw, sh, sp, dp, 4); }

void scale4x_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp, uint32_t ymul) {
	if (!sw||!sh||!ymul) return;
	uint32_t x, dx, pix, swl = sw*sizeof(uint32_t);
	if (!sp) { sp = swl; } swl*=4; if (!dp) { dp = swl; }
	for (; sh>0; sh--, src=(uint8_t*)src+sp) {
		uint32_t *s = (uint32_t* __restrict)src;
		uint32_t *d = (uint32_t* __restrict)dst;
		for (x=dx=0; x<sw; x++, dx+=4) {
			pix = s[x];
			d[dx] = pix; d[dx+1] = pix; d[dx+2] = pix; d[dx+3] = pix;
		}
		void* __restrict dstsrc = dst; dst = (uint8_t*)dst+dp;
		for (uint32_t i=ymul-1; i>0; i--, dst=(uint8_t*)dst+dp) memcpy(dst, dstsrc, swl);
	}
}

void scale4x1_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale4x_c32(src, dst, sw, sh, sp, dp, 1); }
void scale4x2_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale4x_c32(src, dst, sw, sh, sp, dp, 2); }
void scale4x3_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale4x_c32(src, dst, sw, sh, sp, dp, 3); }
void scale4x4_c32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	scale4x_c32(src, dst, sw, sh, sp, dp, 4); }

//
//	memcpy_neon (dst/src must be aligned 4, size must be aligned 2)
//
static void memcpy_neon(void* dst, void* src, uint32_t size) {
	asm volatile (
	"	bic r4, %[sz], #127	;"
	"	add r3, %[s], %[sz]	;"	// r3 = endofs
	"	add r4, %[s], r4	;"	// r4 = s128ofs
	"	cmp %[s], r4		;"
	"	beq 2f			;"
	"1:	vldmia %[s]!, {q8-q15}	;"	// 128 bytes
	"	vstmia %[d]!, {q8-q15}	;"
	"	cmp %[s], r4		;"
	"	bne 1b			;"
	"2:	cmp %[s], r3		;"
	"	beq 7f			;"
	"	tst %[sz], #64		;"
	"	beq 3f			;"
	"	vldmia %[s]!, {q8-q11}	;"	// 64 bytes
	"	vstmia %[d]!, {q8-q11}	;"
	"	cmp %[s], r3		;"
	"	beq 7f			;"
	"3:	tst %[sz], #32		;"
	"	beq 4f			;"
	"	vldmia %[s]!, {q12-q13}	;"	// 32 bytes
	"	vstmia %[d]!, {q12-q13}	;"
	"	cmp %[s], r3		;"
	"	beq 7f			;"
	"4:	tst %[sz], #16		;"
	"	beq 5f			;"
	"	vldmia %[s]!, {q14}	;"	// 16 bytes
	"	vstmia %[d]!, {q14}	;"
	"	cmp %[s], r3		;"
	"	beq 7f			;"
	"5:	tst %[sz], #8		;"
	"	beq 6f			;"
	"	vldmia %[s]!, {d30}	;"	// 8 bytes
	"	vstmia %[d]!, {d30}	;"
	"	cmp %[s], r3		;"
	"	beq 7f			;"
	"6:	ldrh r4, [%[s]],#2	;"	// rest
	"	strh r4, [%[d]],#2	;"
	"	cmp %[s], r3		;"
	"	bne 6b			;"
	"7:				"
	: [s]"+r"(src), [d]"+r"(dst)
	: [sz]"r"(size)
	: "r3","r4","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

//
//	NEON scalers
//

void scale1x1_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw*sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*1; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale1x1_c16(src,dst,sw,sh,sp,dp); return; }
	if ((swl == sp)&&(sp == dp)) memcpy_neon(dst, src, sp*sh);
	else for (; sh>0; sh--, src=(uint8_t*)src+sp, dst=(uint8_t*)dst+dp) memcpy_neon(dst, src, swl);
}

void scale1x2_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw*sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale1x2_c16(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl128 = swl & ~127;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*2 - swl;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x128bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9  = 2x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q15}	;"	// 128 bytes
	"	vstmia %1!, {q8-q15}	;"
	"	vstmia r9!, {q8-q15}	;"
	"	cmp %0, lr		;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 8f			;"
	"	tst %3, #64		;"
	"	beq 4f			;"
	"	vldmia %0!, {q8-q11}	;"	// 64 bytes
	"	vstmia %1!, {q8-q11}	;"
	"	vstmia r9!, {q8-q11}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"4:	tst %3, #32		;"
	"	beq 5f			;"
	"	vldmia %0!, {q12-q13}	;"	// 32 bytes
	"	vstmia %1!, {q12-q13}	;"
	"	vstmia r9!, {q12-q13}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"5:	tst %3, #16		;"
	"	beq 6f			;"
	"	vldmia %0!, {q14}	;"	// 16 bytes
	"	vstmia %1!, {q14}	;"
	"	vstmia r9!, {q14}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"6:	tst %3, #8		;"
	"	beq 7f			;"
	"	vldmia %0!, {d30}	;"	// 8 bytes
	"	vstmia %1!, {d30}	;"
	"	vstmia r9!, {d30}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"7:	ldr lr, [%0],#4		;"	// 4 bytes
	"	str lr, [%1],#4		;"
	"	str lr, [r9]		;"
	"8:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl128), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale1x3_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw*sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale1x3_c16(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl128 = swl & ~127;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*3 - swl;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x128bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9  = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q15}	;"	// 128 bytes
	"	vstmia %1!, {q8-q15}	;"
	"	vstmia r9!, {q8-q15}	;"
	"	vstmia r10!, {q8-q15}	;"
	"	cmp %0, lr		;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 8f			;"
	"	tst %3, #64		;"
	"	beq 4f			;"
	"	vldmia %0!, {q8-q11}	;"	// 64 bytes
	"	vstmia %1!, {q8-q11}	;"
	"	vstmia r9!, {q8-q11}	;"
	"	vstmia r10!, {q8-q11}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"4:	tst %3, #32		;"
	"	beq 5f			;"
	"	vldmia %0!, {q12-q13}	;"	// 32 bytes
	"	vstmia %1!, {q12-q13}	;"
	"	vstmia r9!, {q12-q13}	;"
	"	vstmia r10!, {q12-q13}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"5:	tst %3, #16		;"
	"	beq 6f			;"
	"	vldmia %0!, {q14}	;"	// 16 bytes
	"	vstmia %1!, {q14}	;"
	"	vstmia r9!, {q14}	;"
	"	vstmia r10!, {q14}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"6:	tst %3, #8		;"
	"	beq 7f			;"
	"	vldmia %0!, {d30}	;"	// 8 bytes
	"	vstmia %1!, {d30}	;"
	"	vstmia r9!, {d30}	;"
	"	vstmia r10!, {d30}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"7:	ldr lr, [%0],#4		;"	// 4 bytes
	"	str lr, [%1],#4		;"
	"	str lr, [r9]		;"
	"	str lr, [r10]		;"
	"8:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl128), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale1x4_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw*sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale1x4_c16(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl128 = swl & ~127;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*4 - swl;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x128bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9  = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	add r11, r10, %7	;"	// r11 = 4x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q15}	;"	// 128 bytes
	"	vstmia %1!, {q8-q15}	;"
	"	vstmia r9!, {q8-q15}	;"
	"	vstmia r10!, {q8-q15}	;"
	"	vstmia r11!, {q8-q15}	;"
	"	cmp %0, lr		;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 8f			;"
	"	tst %3, #64		;"
	"	beq 4f			;"
	"	vldmia %0!, {q8-q11}	;"	// 64 bytes
	"	vstmia %1!, {q8-q11}	;"
	"	vstmia r9!, {q8-q11}	;"
	"	vstmia r10!, {q8-q11}	;"
	"	vstmia r11!, {q8-q11}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"4:	tst %3, #32		;"
	"	beq 5f			;"
	"	vldmia %0!, {q12-q13}	;"	// 32 bytes
	"	vstmia %1!, {q12-q13}	;"
	"	vstmia r9!, {q12-q13}	;"
	"	vstmia r10!, {q12-q13}	;"
	"	vstmia r11!, {q12-q13}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"5:	tst %3, #16		;"
	"	beq 6f			;"
	"	vldmia %0!, {q14}	;"	// 16 bytes
	"	vstmia %1!, {q14}	;"
	"	vstmia r9!, {q14}	;"
	"	vstmia r10!, {q14}	;"
	"	vstmia r11!, {q14}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"6:	tst %3, #8		;"
	"	beq 7f			;"
	"	vldmia %0!, {d30}	;"	// 8 bytes
	"	vstmia %1!, {d30}	;"
	"	vstmia r9!, {d30}	;"
	"	vstmia r10!, {d30}	;"
	"	vstmia r11!, {d30}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"7:	ldr lr, [%0],#4		;"	// 4 bytes
	"	str lr, [%1],#4		;"
	"	str lr, [r9]		;"
	"	str lr, [r10]		;"
	"	str lr, [r11]		;"
	"8:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl128), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","r11","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale1x1_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw*sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*1; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale1x1_c32(src,dst,sw,sh,sp,dp); return; }
	if ((swl == sp)&&(sp == dp)) memcpy_neon(dst, src, sp*sh);
	else for (; sh>0; sh--, src=(uint8_t*)src+sp, dst=(uint8_t*)dst+dp) memcpy_neon(dst, src, swl);
}

void scale1x2_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw*sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale1x2_c32(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl128 = swl & ~127;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*2 - swl;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x128bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9  = 2x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q15}	;"	// 128 bytes
	"	vstmia %1!, {q8-q15}	;"
	"	cmp %0, lr		;"
	"	vstmia r9!, {q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 8f			;"
	"	tst %3, #64		;"
	"	beq 4f			;"
	"	vldmia %0!, {q8-q11}	;"	// 64 bytes
	"	vstmia %1!, {q8-q11}	;"
	"	cmp %0, r8		;"
	"	vstmia r9!, {q8-q11}	;"
	"	beq 8f			;"
	"4:	tst %3, #32		;"
	"	beq 5f			;"
	"	vldmia %0!, {q12-q13}	;"	// 32 bytes
	"	vstmia %1!, {q12-q13}	;"
	"	cmp %0, r8		;"
	"	vstmia r9!, {q12-q13}	;"
	"	beq 8f			;"
	"5:	tst %3, #16		;"
	"	beq 6f			;"
	"	vldmia %0!, {q14}	;"	// 16 bytes
	"	vstmia %1!, {q14}	;"
	"	cmp %0, r8		;"
	"	vstmia r9!, {q14}	;"
	"	beq 8f			;"
	"6:	tst %3, #8		;"
	"	beq 7f			;"
	"	vldmia %0!, {d30}	;"	// 8 bytes
	"	vstmia %1!, {d30}	;"
	"	cmp %0, r8		;"
	"	vstmia r9!, {d30}	;"
	"	beq 8f			;"
	"7:	ldr lr, [%0],#4		;"	// 4 bytes
	"	str lr, [%1],#4		;"
	"	str lr, [r9]		;"
	"8:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl128), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale1x3_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw*sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale1x3_c32(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl128 = swl & ~127;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*3 - swl;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x128bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9  = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q15}	;"	// 128 bytes
	"	vstmia %1!, {q8-q15}	;"
	"	vstmia r9!, {q8-q15}	;"
	"	vstmia r10!, {q8-q15}	;"
	"	cmp %0, lr		;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 8f			;"
	"	tst %3, #64		;"
	"	beq 4f			;"
	"	vldmia %0!, {q8-q11}	;"	// 64 bytes
	"	vstmia %1!, {q8-q11}	;"
	"	vstmia r9!, {q8-q11}	;"
	"	vstmia r10!, {q8-q11}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"4:	tst %3, #32		;"
	"	beq 5f			;"
	"	vldmia %0!, {q12-q13}	;"	// 32 bytes
	"	vstmia %1!, {q12-q13}	;"
	"	vstmia r9!, {q12-q13}	;"
	"	vstmia r10!, {q12-q13}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"5:	tst %3, #16		;"
	"	beq 6f			;"
	"	vldmia %0!, {q14}	;"	// 16 bytes
	"	vstmia %1!, {q14}	;"
	"	vstmia r9!, {q14}	;"
	"	vstmia r10!, {q14}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"6:	tst %3, #8		;"
	"	beq 7f			;"
	"	vldmia %0!, {d30}	;"	// 8 bytes
	"	vstmia %1!, {d30}	;"
	"	vstmia r9!, {d30}	;"
	"	vstmia r10!, {d30}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"7:	ldr lr, [%0],#4		;"	// 4 bytes
	"	str lr, [%1],#4		;"
	"	str lr, [r9]		;"
	"	str lr, [r10]		;"
	"8:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl128), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale1x4_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw*sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale1x4_c32(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl128 = swl & ~127;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*4 - swl;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x128bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9  = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	add r11, r10, %7	;"	// r11 = 4x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q15}	;"	// 128 bytes
	"	vstmia %1!, {q8-q15}	;"
	"	vstmia r9!, {q8-q15}	;"
	"	vstmia r10!, {q8-q15}	;"
	"	vstmia r11!, {q8-q15}	;"
	"	cmp %0, lr		;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 8f			;"
	"	tst %3, #64		;"
	"	beq 4f			;"
	"	vldmia %0!, {q8-q11}	;"	// 64 bytes
	"	vstmia %1!, {q8-q11}	;"
	"	vstmia r9!, {q8-q11}	;"
	"	vstmia r10!, {q8-q11}	;"
	"	vstmia r11!, {q8-q11}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"4:	tst %3, #32		;"
	"	beq 5f			;"
	"	vldmia %0!, {q12-q13}	;"	// 32 bytes
	"	vstmia %1!, {q12-q13}	;"
	"	vstmia r9!, {q12-q13}	;"
	"	vstmia r10!, {q12-q13}	;"
	"	vstmia r11!, {q12-q13}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"5:	tst %3, #16		;"
	"	beq 6f			;"
	"	vldmia %0!, {q14}	;"	// 16 bytes
	"	vstmia %1!, {q14}	;"
	"	vstmia r9!, {q14}	;"
	"	vstmia r10!, {q14}	;"
	"	vstmia r11!, {q14}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"6:	tst %3, #8		;"
	"	beq 7f			;"
	"	vldmia %0!, {d30}	;"	// 8 bytes
	"	vstmia %1!, {d30}	;"
	"	vstmia r9!, {d30}	;"
	"	vstmia r10!, {d30}	;"
	"	vstmia r11!, {d30}	;"
	"	cmp %0, r8		;"
	"	beq 8f			;"
	"7:	ldr lr, [%0],#4		;"	// 4 bytes
	"	str lr, [%1],#4		;"
	"	str lr, [r9]		;"
	"	str lr, [r10]		;"
	"	str lr, [r11]		;"
	"8:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl128), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","r11","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale2x1_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*2; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale2x1_c16(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl64 = swl & ~63;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp - swl*2;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x64bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q11}	;"	// 32 pixels 64 bytes
	"	vdup.16 d0, d23[3]	;"
	"	vdup.16 d1, d23[2]	;"
	"	vext.16 d31, d1,d0,#2	;"
	"	vdup.16 d0, d23[1]	;"
	"	vdup.16 d1, d23[0]	;"
	"	vext.16 d30, d1,d0,#2	;"
	"	vdup.16 d0, d22[3]	;"
	"	vdup.16 d1, d22[2]	;"
	"	vext.16 d29, d1,d0,#2	;"
	"	vdup.16 d0, d22[1]	;"
	"	vdup.16 d1, d22[0]	;"
	"	vext.16 d28, d1,d0,#2	;"
	"	vdup.16 d0, d21[3]	;"
	"	vdup.16 d1, d21[2]	;"
	"	vext.16 d27, d1,d0,#2	;"
	"	vdup.16 d0, d21[1]	;"
	"	vdup.16 d1, d21[0]	;"
	"	vext.16 d26, d1,d0,#2	;"
	"	vdup.16 d0, d20[3]	;"
	"	vdup.16 d1, d20[2]	;"
	"	vext.16 d25, d1,d0,#2	;"
	"	vdup.16 d0, d20[1]	;"
	"	vdup.16 d1, d20[0]	;"
	"	vext.16 d24, d1,d0,#2	;"
	"	vdup.16 d0, d19[3]	;"
	"	vdup.16 d1, d19[2]	;"
	"	vext.16 d23, d1,d0,#2	;"
	"	vdup.16 d0, d19[1]	;"
	"	vdup.16 d1, d19[0]	;"
	"	vext.16 d22, d1,d0,#2	;"
	"	vdup.16 d0, d18[3]	;"
	"	vdup.16 d1, d18[2]	;"
	"	vext.16 d21, d1,d0,#2	;"
	"	vdup.16 d0, d18[1]	;"
	"	vdup.16 d1, d18[0]	;"
	"	vext.16 d20, d1,d0,#2	;"
	"	vdup.16 d0, d17[3]	;"
	"	vdup.16 d1, d17[2]	;"
	"	vext.16 d19, d1,d0,#2	;"
	"	vdup.16 d0, d17[1]	;"
	"	vdup.16 d1, d17[0]	;"
	"	vext.16 d18, d1,d0,#2	;"
	"	vdup.16 d0, d16[3]	;"
	"	vdup.16 d1, d16[2]	;"
	"	vext.16 d17, d1,d0,#2	;"
	"	vdup.16 d0, d16[1]	;"
	"	vdup.16 d1, d16[0]	;"
	"	vext.16 d16, d1,d0,#2	;"
	"	cmp %0, lr		;"
	"	vstmia %1!, {q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"	tst %3, #32		;"
	"	beq 4f			;"
	"	vldmia %0!,{q8-q9}	;"	// 16 pixels
	"	vdup.16 d0, d19[3]	;"
	"	vdup.16 d1, d19[2]	;"
	"	vext.16 d23, d1,d0,#2	;"
	"	vdup.16 d0, d19[1]	;"
	"	vdup.16 d1, d19[0]	;"
	"	vext.16 d22, d1,d0,#2	;"
	"	vdup.16 d0, d18[3]	;"
	"	vdup.16 d1, d18[2]	;"
	"	vext.16 d21, d1,d0,#2	;"
	"	vdup.16 d0, d18[1]	;"
	"	vdup.16 d1, d18[0]	;"
	"	vext.16 d20, d1,d0,#2	;"
	"	vdup.16 d0, d17[3]	;"
	"	vdup.16 d1, d17[2]	;"
	"	vext.16 d19, d1,d0,#2	;"
	"	vdup.16 d0, d17[1]	;"
	"	vdup.16 d1, d17[0]	;"
	"	vext.16 d18, d1,d0,#2	;"
	"	vdup.16 d0, d16[3]	;"
	"	vdup.16 d1, d16[2]	;"
	"	vext.16 d17, d1,d0,#2	;"
	"	vdup.16 d0, d16[1]	;"
	"	vdup.16 d1, d16[0]	;"
	"	vext.16 d16, d1,d0,#2	;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {q8-q11}	;"
	"	beq 5f			;"
	"4:	ldrh lr, [%0],#2	;"	// rest
	"	orr lr, lr, lsl #16	;"
	"	cmp %0, r8		;"
	"	str lr, [%1],#4		;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl64), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","lr","q0","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale2x2_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*2; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale2x2_c16(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl64 = swl & ~63;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*2 - swl*2;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x64bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9 = 2x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q11}	;"	// 32 pixels 64 bytes
	"	vdup.16 d0, d23[3]	;"
	"	vdup.16 d1, d23[2]	;"
	"	vext.16 d31, d1,d0,#2	;"
	"	vdup.16 d0, d23[1]	;"
	"	vdup.16 d1, d23[0]	;"
	"	vext.16 d30, d1,d0,#2	;"
	"	vdup.16 d0, d22[3]	;"
	"	vdup.16 d1, d22[2]	;"
	"	vext.16 d29, d1,d0,#2	;"
	"	vdup.16 d0, d22[1]	;"
	"	vdup.16 d1, d22[0]	;"
	"	vext.16 d28, d1,d0,#2	;"
	"	vdup.16 d0, d21[3]	;"
	"	vdup.16 d1, d21[2]	;"
	"	vext.16 d27, d1,d0,#2	;"
	"	vdup.16 d0, d21[1]	;"
	"	vdup.16 d1, d21[0]	;"
	"	vext.16 d26, d1,d0,#2	;"
	"	vdup.16 d0, d20[3]	;"
	"	vdup.16 d1, d20[2]	;"
	"	vext.16 d25, d1,d0,#2	;"
	"	vdup.16 d0, d20[1]	;"
	"	vdup.16 d1, d20[0]	;"
	"	vext.16 d24, d1,d0,#2	;"
	"	vdup.16 d0, d19[3]	;"
	"	vdup.16 d1, d19[2]	;"
	"	vext.16 d23, d1,d0,#2	;"
	"	vdup.16 d0, d19[1]	;"
	"	vdup.16 d1, d19[0]	;"
	"	vext.16 d22, d1,d0,#2	;"
	"	vdup.16 d0, d18[3]	;"
	"	vdup.16 d1, d18[2]	;"
	"	vext.16 d21, d1,d0,#2	;"
	"	vdup.16 d0, d18[1]	;"
	"	vdup.16 d1, d18[0]	;"
	"	vext.16 d20, d1,d0,#2	;"
	"	vdup.16 d0, d17[3]	;"
	"	vdup.16 d1, d17[2]	;"
	"	vext.16 d19, d1,d0,#2	;"
	"	vdup.16 d0, d17[1]	;"
	"	vdup.16 d1, d17[0]	;"
	"	vext.16 d18, d1,d0,#2	;"
	"	vdup.16 d0, d16[3]	;"
	"	vdup.16 d1, d16[2]	;"
	"	vext.16 d17, d1,d0,#2	;"
	"	vdup.16 d0, d16[1]	;"
	"	vdup.16 d1, d16[0]	;"
	"	vext.16 d16, d1,d0,#2	;"
	"	cmp %0, lr		;"
	"	vstmia %1!, {q8-q15}	;"
	"	vstmia r9!, {q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"	tst %3, #32		;"
	"	beq 4f			;"
	"	vldmia %0!,{q8-q9}	;"	// 16 pixels
	"	vdup.16 d0, d19[3]	;"
	"	vdup.16 d1, d19[2]	;"
	"	vext.16 d23, d1,d0,#2	;"
	"	vdup.16 d0, d19[1]	;"
	"	vdup.16 d1, d19[0]	;"
	"	vext.16 d22, d1,d0,#2	;"
	"	vdup.16 d0, d18[3]	;"
	"	vdup.16 d1, d18[2]	;"
	"	vext.16 d21, d1,d0,#2	;"
	"	vdup.16 d0, d18[1]	;"
	"	vdup.16 d1, d18[0]	;"
	"	vext.16 d20, d1,d0,#2	;"
	"	vdup.16 d0, d17[3]	;"
	"	vdup.16 d1, d17[2]	;"
	"	vext.16 d19, d1,d0,#2	;"
	"	vdup.16 d0, d17[1]	;"
	"	vdup.16 d1, d17[0]	;"
	"	vext.16 d18, d1,d0,#2	;"
	"	vdup.16 d0, d16[3]	;"
	"	vdup.16 d1, d16[2]	;"
	"	vext.16 d17, d1,d0,#2	;"
	"	vdup.16 d0, d16[1]	;"
	"	vdup.16 d1, d16[0]	;"
	"	vext.16 d16, d1,d0,#2	;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {q8-q11}	;"
	"	vstmia r9!, {q8-q11}	;"
	"	beq 5f			;"
	"4:	ldrh lr, [%0],#2	;"	// rest
	"	orr lr, lr, lsl #16	;"
	"	cmp %0, r8		;"
	"	str lr, [%1],#4		;"
	"	str lr, [r9],#4		;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl64), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","lr","q0","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale2x3_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*2; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale2x3_c16(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl64 = swl & ~63;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*3 - swl*2;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x64bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9  = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q11}	;"	// 32 pixels 64 bytes
	"	vdup.16 d0, d23[3]	;"
	"	vdup.16 d1, d23[2]	;"
	"	vext.16 d31, d1,d0,#2	;"
	"	vdup.16 d0, d23[1]	;"
	"	vdup.16 d1, d23[0]	;"
	"	vext.16 d30, d1,d0,#2	;"
	"	vdup.16 d0, d22[3]	;"
	"	vdup.16 d1, d22[2]	;"
	"	vext.16 d29, d1,d0,#2	;"
	"	vdup.16 d0, d22[1]	;"
	"	vdup.16 d1, d22[0]	;"
	"	vext.16 d28, d1,d0,#2	;"
	"	vdup.16 d0, d21[3]	;"
	"	vdup.16 d1, d21[2]	;"
	"	vext.16 d27, d1,d0,#2	;"
	"	vdup.16 d0, d21[1]	;"
	"	vdup.16 d1, d21[0]	;"
	"	vext.16 d26, d1,d0,#2	;"
	"	vdup.16 d0, d20[3]	;"
	"	vdup.16 d1, d20[2]	;"
	"	vext.16 d25, d1,d0,#2	;"
	"	vdup.16 d0, d20[1]	;"
	"	vdup.16 d1, d20[0]	;"
	"	vext.16 d24, d1,d0,#2	;"
	"	vdup.16 d0, d19[3]	;"
	"	vdup.16 d1, d19[2]	;"
	"	vext.16 d23, d1,d0,#2	;"
	"	vdup.16 d0, d19[1]	;"
	"	vdup.16 d1, d19[0]	;"
	"	vext.16 d22, d1,d0,#2	;"
	"	vdup.16 d0, d18[3]	;"
	"	vdup.16 d1, d18[2]	;"
	"	vext.16 d21, d1,d0,#2	;"
	"	vdup.16 d0, d18[1]	;"
	"	vdup.16 d1, d18[0]	;"
	"	vext.16 d20, d1,d0,#2	;"
	"	vdup.16 d0, d17[3]	;"
	"	vdup.16 d1, d17[2]	;"
	"	vext.16 d19, d1,d0,#2	;"
	"	vdup.16 d0, d17[1]	;"
	"	vdup.16 d1, d17[0]	;"
	"	vext.16 d18, d1,d0,#2	;"
	"	vdup.16 d0, d16[3]	;"
	"	vdup.16 d1, d16[2]	;"
	"	vext.16 d17, d1,d0,#2	;"
	"	vdup.16 d0, d16[1]	;"
	"	vdup.16 d1, d16[0]	;"
	"	vext.16 d16, d1,d0,#2	;"
	"	cmp %0, lr		;"
	"	vstmia %1!, {q8-q15}	;"
	"	vstmia r9!, {q8-q15}	;"
	"	vstmia r10!, {q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"	tst %3, #32		;"
	"	beq 4f			;"
	"	vldmia %0!,{q8-q9}	;"	// 16 pixels
	"	vdup.16 d0, d19[3]	;"
	"	vdup.16 d1, d19[2]	;"
	"	vext.16 d23, d1,d0,#2	;"
	"	vdup.16 d0, d19[1]	;"
	"	vdup.16 d1, d19[0]	;"
	"	vext.16 d22, d1,d0,#2	;"
	"	vdup.16 d0, d18[3]	;"
	"	vdup.16 d1, d18[2]	;"
	"	vext.16 d21, d1,d0,#2	;"
	"	vdup.16 d0, d18[1]	;"
	"	vdup.16 d1, d18[0]	;"
	"	vext.16 d20, d1,d0,#2	;"
	"	vdup.16 d0, d17[3]	;"
	"	vdup.16 d1, d17[2]	;"
	"	vext.16 d19, d1,d0,#2	;"
	"	vdup.16 d0, d17[1]	;"
	"	vdup.16 d1, d17[0]	;"
	"	vext.16 d18, d1,d0,#2	;"
	"	vdup.16 d0, d16[3]	;"
	"	vdup.16 d1, d16[2]	;"
	"	vext.16 d17, d1,d0,#2	;"
	"	vdup.16 d0, d16[1]	;"
	"	vdup.16 d1, d16[0]	;"
	"	vext.16 d16, d1,d0,#2	;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {q8-q11}	;"
	"	vstmia r9!, {q8-q11}	;"
	"	vstmia r10!, {q8-q11}	;"
	"	beq 5f			;"
	"4:	ldrh lr, [%0],#2	;"	// rest
	"	orr lr, lr, lsl #16	;"
	"	cmp %0, r8		;"
	"	str lr, [%1],#4		;"
	"	str lr, [r9],#4		;"
	"	str lr, [r10],#4	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl64), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","lr","q0","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale2x4_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*2; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale2x3_c16(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl64 = swl & ~63;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*4 - swl*2;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x64bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9  = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	add r11, r10, %7	;"	// r11 = 4x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q11}	;"	// 32 pixels 64 bytes
	"	vdup.16 d0, d23[3]	;"
	"	vdup.16 d1, d23[2]	;"
	"	vext.16 d31, d1,d0,#2	;"
	"	vdup.16 d0, d23[1]	;"
	"	vdup.16 d1, d23[0]	;"
	"	vext.16 d30, d1,d0,#2	;"
	"	vdup.16 d0, d22[3]	;"
	"	vdup.16 d1, d22[2]	;"
	"	vext.16 d29, d1,d0,#2	;"
	"	vdup.16 d0, d22[1]	;"
	"	vdup.16 d1, d22[0]	;"
	"	vext.16 d28, d1,d0,#2	;"
	"	vdup.16 d0, d21[3]	;"
	"	vdup.16 d1, d21[2]	;"
	"	vext.16 d27, d1,d0,#2	;"
	"	vdup.16 d0, d21[1]	;"
	"	vdup.16 d1, d21[0]	;"
	"	vext.16 d26, d1,d0,#2	;"
	"	vdup.16 d0, d20[3]	;"
	"	vdup.16 d1, d20[2]	;"
	"	vext.16 d25, d1,d0,#2	;"
	"	vdup.16 d0, d20[1]	;"
	"	vdup.16 d1, d20[0]	;"
	"	vext.16 d24, d1,d0,#2	;"
	"	vdup.16 d0, d19[3]	;"
	"	vdup.16 d1, d19[2]	;"
	"	vext.16 d23, d1,d0,#2	;"
	"	vdup.16 d0, d19[1]	;"
	"	vdup.16 d1, d19[0]	;"
	"	vext.16 d22, d1,d0,#2	;"
	"	vdup.16 d0, d18[3]	;"
	"	vdup.16 d1, d18[2]	;"
	"	vext.16 d21, d1,d0,#2	;"
	"	vdup.16 d0, d18[1]	;"
	"	vdup.16 d1, d18[0]	;"
	"	vext.16 d20, d1,d0,#2	;"
	"	vdup.16 d0, d17[3]	;"
	"	vdup.16 d1, d17[2]	;"
	"	vext.16 d19, d1,d0,#2	;"
	"	vdup.16 d0, d17[1]	;"
	"	vdup.16 d1, d17[0]	;"
	"	vext.16 d18, d1,d0,#2	;"
	"	vdup.16 d0, d16[3]	;"
	"	vdup.16 d1, d16[2]	;"
	"	vext.16 d17, d1,d0,#2	;"
	"	vdup.16 d0, d16[1]	;"
	"	vdup.16 d1, d16[0]	;"
	"	vext.16 d16, d1,d0,#2	;"
	"	cmp %0, lr		;"
	"	vstmia %1!, {q8-q15}	;"
	"	vstmia r9!, {q8-q15}	;"
	"	vstmia r10!, {q8-q15}	;"
	"	vstmia r11!, {q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"	tst %3, #32		;"
	"	beq 4f			;"
	"	vldmia %0!,{q8-q9}	;"	// 16 pixels
	"	vdup.16 d0, d19[3]	;"
	"	vdup.16 d1, d19[2]	;"
	"	vext.16 d23, d1,d0,#2	;"
	"	vdup.16 d0, d19[1]	;"
	"	vdup.16 d1, d19[0]	;"
	"	vext.16 d22, d1,d0,#2	;"
	"	vdup.16 d0, d18[3]	;"
	"	vdup.16 d1, d18[2]	;"
	"	vext.16 d21, d1,d0,#2	;"
	"	vdup.16 d0, d18[1]	;"
	"	vdup.16 d1, d18[0]	;"
	"	vext.16 d20, d1,d0,#2	;"
	"	vdup.16 d0, d17[3]	;"
	"	vdup.16 d1, d17[2]	;"
	"	vext.16 d19, d1,d0,#2	;"
	"	vdup.16 d0, d17[1]	;"
	"	vdup.16 d1, d17[0]	;"
	"	vext.16 d18, d1,d0,#2	;"
	"	vdup.16 d0, d16[3]	;"
	"	vdup.16 d1, d16[2]	;"
	"	vext.16 d17, d1,d0,#2	;"
	"	vdup.16 d0, d16[1]	;"
	"	vdup.16 d1, d16[0]	;"
	"	vext.16 d16, d1,d0,#2	;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {q8-q11}	;"
	"	vstmia r9!, {q8-q11}	;"
	"	vstmia r10!, {q8-q11}	;"
	"	vstmia r11!, {q8-q11}	;"
	"	beq 5f			;"
	"4:	ldrh lr, [%0],#2	;"	// rest
	"	orr lr, lr, lsl #16	;"
	"	cmp %0, r8		;"
	"	str lr, [%1],#4		;"
	"	str lr, [r9],#4		;"
	"	str lr, [r10],#4	;"
	"	str lr, [r11],#4	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl64), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","r11","lr","q0","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale2x1_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*2; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale2x1_c32(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl64 = swl & ~63;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp - swl*2;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr = x64bytes offset
	"	add r8, %0, %3		;"	// r8 = lineend offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q11}	;"	// 16 pixels 64 bytes
	"	vdup.32 d31, d23[1]	;"
	"	vdup.32 d30, d23[0]	;"
	"	vdup.32 d29, d22[1]	;"
	"	vdup.32 d28, d22[0]	;"
	"	vdup.32 d27, d21[1]	;"
	"	vdup.32 d26, d21[0]	;"
	"	vdup.32 d25, d20[1]	;"
	"	vdup.32 d24, d20[0]	;"
	"	vdup.32 d23, d19[1]	;"
	"	vdup.32 d22, d19[0]	;"
	"	vdup.32 d21, d18[1]	;"
	"	vdup.32 d20, d18[0]	;"
	"	vdup.32 d19, d17[1]	;"
	"	vdup.32 d18, d17[0]	;"
	"	vdup.32 d17, d16[1]	;"
	"	vdup.32 d16, d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!, {q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldr lr, [%0],#4		;"	// rest
	"	vdup.32 d16, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {d16}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl64), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale2x2_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*2; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale2x2_c32(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl64 = swl & ~63;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*2 - swl*2;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr = x64bytes offset
	"	add r8, %0, %3		;"	// r8 = lineend offset
	"	add r9, %1, %7		;"	// r9 = 2x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q11}	;"	// 16 pixels 64 bytes
	"	vdup.32 d31, d23[1]	;"
	"	vdup.32 d30, d23[0]	;"
	"	vdup.32 d29, d22[1]	;"
	"	vdup.32 d28, d22[0]	;"
	"	vdup.32 d27, d21[1]	;"
	"	vdup.32 d26, d21[0]	;"
	"	vdup.32 d25, d20[1]	;"
	"	vdup.32 d24, d20[0]	;"
	"	vdup.32 d23, d19[1]	;"
	"	vdup.32 d22, d19[0]	;"
	"	vdup.32 d21, d18[1]	;"
	"	vdup.32 d20, d18[0]	;"
	"	vdup.32 d19, d17[1]	;"
	"	vdup.32 d18, d17[0]	;"
	"	vdup.32 d17, d16[1]	;"
	"	vdup.32 d16, d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!, {q8-q15}	;"
	"	vstmia r9!, {q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldr lr, [%0],#4		;"	// rest
	"	vdup.32 d16, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {d16}	;"
	"	vstmia r9!, {d16}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl64), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale2x3_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*2; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale2x3_c32(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl64 = swl & ~63;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*3 - swl*2;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr = x64bytes offset
	"	add r8, %0, %3		;"	// r8 = lineend offset
	"	add r9, %1, %7		;"	// r9 = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q11}	;"	// 16 pixels 64 bytes
	"	vdup.32 d31, d23[1]	;"
	"	vdup.32 d30, d23[0]	;"
	"	vdup.32 d29, d22[1]	;"
	"	vdup.32 d28, d22[0]	;"
	"	vdup.32 d27, d21[1]	;"
	"	vdup.32 d26, d21[0]	;"
	"	vdup.32 d25, d20[1]	;"
	"	vdup.32 d24, d20[0]	;"
	"	vdup.32 d23, d19[1]	;"
	"	vdup.32 d22, d19[0]	;"
	"	vdup.32 d21, d18[1]	;"
	"	vdup.32 d20, d18[0]	;"
	"	vdup.32 d19, d17[1]	;"
	"	vdup.32 d18, d17[0]	;"
	"	vdup.32 d17, d16[1]	;"
	"	vdup.32 d16, d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!, {q8-q15}	;"
	"	vstmia r9!, {q8-q15}	;"
	"	vstmia r10!, {q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldr lr, [%0],#4		;"	// rest
	"	vdup.32 d16, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {d16}	;"
	"	vstmia r9!, {d16}	;"
	"	vstmia r10!, {d16}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl64), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale2x4_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*2; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale2x4_c32(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl64 = swl & ~63;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*4 - swl*2;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr = x64bytes offset
	"	add r8, %0, %3		;"	// r8 = lineend offset
	"	add r9, %1, %7		;"	// r9 = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	add r11, r10, %7	;"	// r11 = 4x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!, {q8-q11}	;"	// 16 pixels 64 bytes
	"	vdup.32 d31, d23[1]	;"
	"	vdup.32 d30, d23[0]	;"
	"	vdup.32 d29, d22[1]	;"
	"	vdup.32 d28, d22[0]	;"
	"	vdup.32 d27, d21[1]	;"
	"	vdup.32 d26, d21[0]	;"
	"	vdup.32 d25, d20[1]	;"
	"	vdup.32 d24, d20[0]	;"
	"	vdup.32 d23, d19[1]	;"
	"	vdup.32 d22, d19[0]	;"
	"	vdup.32 d21, d18[1]	;"
	"	vdup.32 d20, d18[0]	;"
	"	vdup.32 d19, d17[1]	;"
	"	vdup.32 d18, d17[0]	;"
	"	vdup.32 d17, d16[1]	;"
	"	vdup.32 d16, d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!, {q8-q15}	;"
	"	vstmia r9!, {q8-q15}	;"
	"	vstmia r10!, {q8-q15}	;"
	"	vstmia r11!, {q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldr lr, [%0],#4		;"	// rest
	"	vdup.32 d16, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {d16}	;"
	"	vstmia r9!, {d16}	;"
	"	vstmia r10!, {d16}	;"
	"	vstmia r11!, {d16}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl64), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","r11","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale4x1_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*4; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale4x1_c16(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl32 = swl & ~31;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp - swl*4;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x32bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!,{q8-q9}	;"	// 16 pixels 32 bytes
	"	vdup.16 d31,d19[3]	;"
	"	vdup.16 d30,d19[2]	;"
	"	vdup.16 d29,d19[1]	;"
	"	vdup.16 d28,d19[0]	;"
	"	vdup.16 d27,d18[3]	;"
	"	vdup.16 d26,d18[2]	;"
	"	vdup.16 d25,d18[1]	;"
	"	vdup.16 d24,d18[0]	;"
	"	vdup.16 d23,d17[3]	;"
	"	vdup.16 d22,d17[2]	;"
	"	vdup.16 d21,d17[1]	;"
	"	vdup.16 d20,d17[0]	;"
	"	vdup.16 d19,d16[3]	;"
	"	vdup.16 d18,d16[2]	;"
	"	vdup.16 d17,d16[1]	;"
	"	vdup.16 d16,d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!,{q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldrh lr, [%0],#2	;"	// rest
	"	vdup.16 d16, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {d16}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl32), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale4x2_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*4; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale4x2_c16(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl32 = swl & ~31;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*2 - swl*4;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x32bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9  = 2x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!,{q8-q9}	;"	// 16 pixels 32 bytes
	"	vdup.16 d31,d19[3]	;"
	"	vdup.16 d30,d19[2]	;"
	"	vdup.16 d29,d19[1]	;"
	"	vdup.16 d28,d19[0]	;"
	"	vdup.16 d27,d18[3]	;"
	"	vdup.16 d26,d18[2]	;"
	"	vdup.16 d25,d18[1]	;"
	"	vdup.16 d24,d18[0]	;"
	"	vdup.16 d23,d17[3]	;"
	"	vdup.16 d22,d17[2]	;"
	"	vdup.16 d21,d17[1]	;"
	"	vdup.16 d20,d17[0]	;"
	"	vdup.16 d19,d16[3]	;"
	"	vdup.16 d18,d16[2]	;"
	"	vdup.16 d17,d16[1]	;"
	"	vdup.16 d16,d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!,{q8-q15}	;"
	"	vstmia r9!,{q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldrh lr, [%0],#2	;"	// rest
	"	vdup.16 d16, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {d16}	;"
	"	vstmia r9!, {d16}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl32), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale4x3_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*4; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale4x3_c16(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl32 = swl & ~31;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*3 - swl*4;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x32bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9  = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!,{q8-q9}	;"	// 16 pixels 32 bytes
	"	vdup.16 d31,d19[3]	;"
	"	vdup.16 d30,d19[2]	;"
	"	vdup.16 d29,d19[1]	;"
	"	vdup.16 d28,d19[0]	;"
	"	vdup.16 d27,d18[3]	;"
	"	vdup.16 d26,d18[2]	;"
	"	vdup.16 d25,d18[1]	;"
	"	vdup.16 d24,d18[0]	;"
	"	vdup.16 d23,d17[3]	;"
	"	vdup.16 d22,d17[2]	;"
	"	vdup.16 d21,d17[1]	;"
	"	vdup.16 d20,d17[0]	;"
	"	vdup.16 d19,d16[3]	;"
	"	vdup.16 d18,d16[2]	;"
	"	vdup.16 d17,d16[1]	;"
	"	vdup.16 d16,d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!,{q8-q15}	;"
	"	vstmia r9!,{q8-q15}	;"
	"	vstmia r10!,{q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldrh lr, [%0],#2	;"	// rest
	"	vdup.16 d16, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {d16}	;"
	"	vstmia r9!, {d16}	;"
	"	vstmia r10!, {d16}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl32), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale4x4_n16(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint16_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*4; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale4x4_c16(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl32 = swl & ~31;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*4 - swl*4;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr  = x32bytes offset
	"	add r8, %0, %3		;"	// r8  = lineend offset
	"	add r9, %1, %7		;"	// r9  = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	add r11, r10, %7	;"	// r11 = 4x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!,{q8-q9}	;"	// 16 pixels 32 bytes
	"	vdup.16 d31,d19[3]	;"
	"	vdup.16 d30,d19[2]	;"
	"	vdup.16 d29,d19[1]	;"
	"	vdup.16 d28,d19[0]	;"
	"	vdup.16 d27,d18[3]	;"
	"	vdup.16 d26,d18[2]	;"
	"	vdup.16 d25,d18[1]	;"
	"	vdup.16 d24,d18[0]	;"
	"	vdup.16 d23,d17[3]	;"
	"	vdup.16 d22,d17[2]	;"
	"	vdup.16 d21,d17[1]	;"
	"	vdup.16 d20,d17[0]	;"
	"	vdup.16 d19,d16[3]	;"
	"	vdup.16 d18,d16[2]	;"
	"	vdup.16 d17,d16[1]	;"
	"	vdup.16 d16,d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!,{q8-q15}	;"
	"	vstmia r9!,{q8-q15}	;"
	"	vstmia r10!,{q8-q15}	;"
	"	vstmia r11!,{q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldrh lr, [%0],#2	;"	// rest
	"	vdup.16 d16, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {d16}	;"
	"	vstmia r9!, {d16}	;"
	"	vstmia r10!, {d16}	;"
	"	vstmia r11!, {d16}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl32), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","r11","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale4x1_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*4; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale4x1_c32(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl32 = swl & ~31;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp - swl*4;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr = x32bytes offset
	"	add r8, %0, %3		;"	// r8 = lineend offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!,{q8-q9}	;"	// 8 pixels 32 bytes
	"	vdup.32 q15,d19[1]	;"
	"	vdup.32 q14,d19[0]	;"
	"	vdup.32 q13,d18[1]	;"
	"	vdup.32 q12,d18[0]	;"
	"	vdup.32 q11,d17[1]	;"
	"	vdup.32 q10,d17[0]	;"
	"	vdup.32 q9,d16[1]	;"
	"	vdup.32 q8,d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!,{q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldr lr, [%0],#4		;"	// rest
	"	vdup.32 q8, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {q8}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl32), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale4x2_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*4; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale4x2_c32(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl32 = swl & ~31;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*2 - swl*4;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr = x32bytes offset
	"	add r8, %0, %3		;"	// r8 = lineend offset
	"	add r9, %1, %7		;"	// r9 = 2x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!,{q8-q9}	;"	// 8 pixels 32 bytes
	"	vdup.32 q15,d19[1]	;"
	"	vdup.32 q14,d19[0]	;"
	"	vdup.32 q13,d18[1]	;"
	"	vdup.32 q12,d18[0]	;"
	"	vdup.32 q11,d17[1]	;"
	"	vdup.32 q10,d17[0]	;"
	"	vdup.32 q9,d16[1]	;"
	"	vdup.32 q8,d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!,{q8-q15}	;"
	"	vstmia r9!,{q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldr lr, [%0],#4		;"	// rest
	"	vdup.32 q8, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {q8}	;"
	"	vstmia r9!, {q8}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl32), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale4x3_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*4; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale4x3_c32(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl32 = swl & ~31;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*3 - swl*4;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr = x32bytes offset
	"	add r8, %0, %3		;"	// r8 = lineend offset
	"	add r9, %1, %7		;"	// r9 = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!,{q8-q9}	;"	// 8 pixels 32 bytes
	"	vdup.32 q15,d19[1]	;"
	"	vdup.32 q14,d19[0]	;"
	"	vdup.32 q13,d18[1]	;"
	"	vdup.32 q12,d18[0]	;"
	"	vdup.32 q11,d17[1]	;"
	"	vdup.32 q10,d17[0]	;"
	"	vdup.32 q9,d16[1]	;"
	"	vdup.32 q8,d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!,{q8-q15}	;"
	"	vstmia r9!,{q8-q15}	;"
	"	vstmia r10!,{q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldr lr, [%0],#4		;"	// rest
	"	vdup.32 q8, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {q8}	;"
	"	vstmia r9!, {q8}	;"
	"	vstmia r10!, {q8}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl32), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

void scale4x4_n32(void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
	if (!sw||!sh) return;
	uint32_t swl = sw * sizeof(uint32_t);
	if (!sp) { sp = swl; } if (!dp) { dp = swl*4; }
	if ( ((uintptr_t)src&3)||((uintptr_t)dst&3)||(sp&3)||(dp&3) ) { scale4x4_c32(src,dst,sw,sh,sp,dp); return; }
	uint32_t swl32 = swl & ~31;
	uint32_t sadd = sp - swl;
	uint32_t dadd = dp*4 - swl*4;
	uint8_t* finofs = (uint8_t*)src + (sp*sh);
	asm volatile (
	"1:	add lr, %0, %2		;"	// lr = x32bytes offset
	"	add r8, %0, %3		;"	// r8 = lineend offset
	"	add r9, %1, %7		;"	// r9 = 2x line offset
	"	add r10, r9, %7		;"	// r10 = 3x line offset
	"	add r11, r10, %7	;"	// r11 = 4x line offset
	"	cmp %0, lr		;"
	"	beq 3f			;"
	"2:	vldmia %0!,{q8-q9}	;"	// 8 pixels 32 bytes
	"	vdup.32 q15,d19[1]	;"
	"	vdup.32 q14,d19[0]	;"
	"	vdup.32 q13,d18[1]	;"
	"	vdup.32 q12,d18[0]	;"
	"	vdup.32 q11,d17[1]	;"
	"	vdup.32 q10,d17[0]	;"
	"	vdup.32 q9,d16[1]	;"
	"	vdup.32 q8,d16[0]	;"
	"	cmp %0, lr		;"
	"	vstmia %1!,{q8-q15}	;"
	"	vstmia r9!,{q8-q15}	;"
	"	vstmia r10!,{q8-q15}	;"
	"	vstmia r11!,{q8-q15}	;"
	"	bne 2b			;"
	"3:	cmp %0, r8		;"
	"	beq 5f			;"
	"4:	ldr lr, [%0],#4		;"	// rest
	"	vdup.32 q8, lr		;"
	"	cmp %0, r8		;"
	"	vstmia %1!, {q8}	;"
	"	vstmia r9!, {q8}	;"
	"	vstmia r10!, {q8}	;"
	"	vstmia r11!, {q8}	;"
	"	bne 4b			;"
	"5:	add %0, %0, %4		;"
	"	add %1, %1, %5		;"
	"	cmp %0, %6		;"
	"	bne 1b			"
	: "+r"(src), "+r"(dst)
	: "r"(swl32), "r"(swl), "r"(sadd), "r"(dadd), "r"(finofs), "r"(dp)
	: "r8","r9","r10","r11","lr","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}

/* Bridge to NEON scalers for Retroarch video driver */
void scale1x1_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale1x1_n16(src, dst, sw, sh, sp, dp); }
void scale1x2_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale1x2_n16(src, dst, sw, sh, sp, dp); }
void scale1x3_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale1x3_n16(src, dst, sw, sh, sp, dp); }
void scale1x4_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale1x4_n16(src, dst, sw, sh, sp, dp); }
void scale1x1_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale1x1_n32(src, dst, sw, sh, sp, dp); }
void scale1x2_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale1x2_n32(src, dst, sw, sh, sp, dp); }
void scale1x3_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale1x3_n32(src, dst, sw, sh, sp, dp); }
void scale1x4_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale1x4_n32(src, dst, sw, sh, sp, dp); }
void scale2x1_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale2x1_n16(src, dst, sw, sh, sp, dp); }
void scale2x2_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale2x2_n16(src, dst, sw, sh, sp, dp); }
void scale2x3_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale2x3_n16(src, dst, sw, sh, sp, dp); }
void scale2x4_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale2x4_n16(src, dst, sw, sh, sp, dp); }
void scale2x1_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale2x1_n32(src, dst, sw, sh, sp, dp); }
void scale2x2_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale2x2_n32(src, dst, sw, sh, sp, dp); }
void scale2x3_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale2x3_n32(src, dst, sw, sh, sp, dp); }
void scale2x4_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale2x4_n32(src, dst, sw, sh, sp, dp); }
void scale4x1_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale4x1_n16(src, dst, sw, sh, sp, dp); }
void scale4x2_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale4x2_n16(src, dst, sw, sh, sp, dp); }
void scale4x3_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale4x3_n16(src, dst, sw, sh, sp, dp); }
void scale4x4_16(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale4x4_n16(src, dst, sw, sh, sp, dp); }
void scale4x1_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale4x1_n32(src, dst, sw, sh, sp, dp); }
void scale4x2_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale4x2_n32(src, dst, sw, sh, sp, dp); }
void scale4x3_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale4x3_n32(src, dst, sw, sh, sp, dp); }
void scale4x4_32(void* data, void* __restrict src, void* __restrict dst, uint32_t sw, uint32_t sh, uint32_t sp, uint32_t dp) {
     scale4x4_n32(src, dst, sw, sh, sp, dp); }

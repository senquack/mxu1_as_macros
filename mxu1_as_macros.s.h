# mxu1_as_macros.s.h
#
# MIPS Ingenic XBurst MXU1 rev2 SIMD helper macros for GNU GAS
#
# Copyright (c) 2019 Daniel Silsby (senquack)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


################################################################################
#  These macros take the place of the unofficial patched Ingenic binutils, and
# the syntax there and here is interchangeable. Users of a toolchain that
# contains that unofficial Ingenic binutils patch should *not* specify '-mmxu'
# on the compiler/assembler command line when using this header.
#
# To use this header:
#  1.) In a GAS .S or .s assembly file, add following at top of file:
#      .include "mxu1_as_macros.s.h"
#  2.) In a GCC C/C++ file using inline asm, add following at top of file:
#      __asm__ __volatile__(".include \"mxu1_as_macros.s.h\" \n");
#
#  Because of limitations in GAS macro expansion, and the fact that GAS MIPS
# register names like '$a1' do not evaluate to integer expressions, you cannot
# use '.equ' to generically define the regs your code uses.
# You cannot, for instance, do this when using this header:
#     .equ R_SRCPTR, $a1
#     lxw  $t0, R_SRCPTR, $t1, 0                # (GIVES MACRO SYNTAX ERROR)
# What you *can* do instead, is use a C preprocessor #define, which of course
# requires any standalone ASM code to be in a .S file (not .s):
#     #define R_SRCPTR $a1
#     lxw  $t0, R_SRCPTR, $t1, 0                # (WORKS)
# An alternative for inline ASM in C/C++ is like so:
#     #define R_SRCPTR "$a1"
#     asm("lxw  $t0, " R_SRCPTR ", $t1, 0 \n"); # (WORKS)
################################################################################


.ifndef MXU1_AS_MACROS_S_H
.equiv  MXU1_AS_MACROS_S_H, 1

# Named MIPS GPR specifiers, i.e. $zero..$ra
.equiv GPR_$zero, 0
.equiv GPR_$at,   1
.equiv GPR_$v0,   2
.equiv GPR_$v1,   3
.equiv GPR_$a0,   4
.equiv GPR_$a1,   5
.equiv GPR_$a2,   6
.equiv GPR_$a3,   7
.equiv GPR_$t0,   8
.equiv GPR_$t1,   9
.equiv GPR_$t2,   10
.equiv GPR_$t3,   11
.equiv GPR_$t4,   12
.equiv GPR_$t5,   13
.equiv GPR_$t6,   14
.equiv GPR_$t7,   15
.equiv GPR_$s0,   16
.equiv GPR_$s1,   17
.equiv GPR_$s2,   18
.equiv GPR_$s3,   19
.equiv GPR_$s4,   20
.equiv GPR_$s5,   21
.equiv GPR_$s6,   22
.equiv GPR_$s7,   23
.equiv GPR_$t8,   24
.equiv GPR_$t9,   25
.equiv GPR_$k0,   26
.equiv GPR_$k1,   27
.equiv GPR_$gp,   28
.equiv GPR_$sp,   29
.equiv GPR_$fp,   30
.equiv GPR_$s8,   GPR_$fp
.equiv GPR_$ra,   31

# Unnamed MIPS GPRs, i.e. $0..$31
.irp i, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
  .equiv GPR_$\i, \i
.endr

# MXU regs xr0..xr16
# For use with s32i2m,s32m2i MXU<->GPR reg transfer opcodes (can access xr16)
.irp i, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
  .equiv MXU_I2M_M2I_xr\i, \i
.endr

# MXU regs xr0..xr15
# For use with all other MXU opcodes (4-bit fields can only access xr0..15)
.irp i, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
  .equiv MXU_xr\i, \i
.endr

# MXU pattern names ptn0..ptn7, 0..7
.irp i, 0,1,2,3,4,5,6,7
  .equiv MXU_PTN_ptn\i, \i
  .equiv MXU_PTN_\i, \i
.endr

# MXU patterns for 's16mad' opcode (one-channel add)
.equiv MXU_APTN1_A,     0
.equiv MXU_APTN1_S,     1
.equiv MXU_APTN1_0,     0
.equiv MXU_APTN1_1,     1

# MXU patterns for 's16mad' opcode (one-channel multiply)
.equiv MXU_MPTN2_HH,    0
.equiv MXU_MPTN2_LL,    1
.equiv MXU_MPTN2_HL,    2
.equiv MXU_MPTN2_LH,    3
.equiv MXU_MPTN2_0,     0
.equiv MXU_MPTN2_1,     1
.equiv MXU_MPTN2_2,     2
.equiv MXU_MPTN2_3,     3

# MXU patterns for dual-channel add
.equiv MXU_APTN2_AA,    0
.equiv MXU_APTN2_AS,    1
.equiv MXU_APTN2_SA,    2
.equiv MXU_APTN2_SS,    3
.equiv MXU_APTN2_0,     0
.equiv MXU_APTN2_1,     1
.equiv MXU_APTN2_2,     2
.equiv MXU_APTN2_3,     3

# MXU patterns for dual-channel multiply
.equiv MXU_OPTN2_WW,    0
.equiv MXU_OPTN2_LW,    1
.equiv MXU_OPTN2_HW,    2
.equiv MXU_OPTN2_XW,    3
.equiv MXU_OPTN2_0,     0
.equiv MXU_OPTN2_1,     1
.equiv MXU_OPTN2_2,     2
.equiv MXU_OPTN2_3,     3


.macro MXU_CHECK_BOUNDS val:req, lo_bound:req, hi_bound:req
  .if (\val) < (\lo_bound) || (\val) > (\hi_bound)
    .error "MXU opcode field out of range [ \lo_bound .. \hi_bound ] : \val"
  .endif
.endm

.macro MXU_CHECK_OFFSET val:req, byte_alignment:req, lo_bound:req, hi_bound:req
  .if (\val) & (\byte_alignment - 1)
    .error "MXU opcode immediate offset misaligned: \val"
  .endif
  .if (\val) < (\lo_bound) || (\val) > (\hi_bound)
    .error "MXU opcode immediate offset out of range [ \lo_bound .. \hi_bound ] : \val"
  .endif
.endm

.macro MXU_CHECK_PATTERN ptn:req, lo_bound:req, hi_bound:req
  .if (MXU_PTN_\ptn) < (\lo_bound) || (MXU_PTN_\ptn) > (\hi_bound)
    .error "MXU opcode pattern field out of range [ ptn\lo_bound .. ptn\hi_bound ] : \ptn"
  .endif
.endm


# XXX: The Ingenic MXU PDF dated June 2, 2017 containing MXU encodings table
#  shows the wrong encoding for d16mule, and their binutils patch fails to
#  parse or assemble it right. The opcode as encoded here has been tested to
#  function correctly.
#  'X1000_M200_XBurst_ISA_MXU_PM.pdf' is the name of the errant PDF doc.
#
.macro d16mul      xra:req, xrb:req, xrc:req, xrd:req, optn2:req
  .word 0x70000008 | (MXU_OPTN2_\optn2 << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16mulf     xra:req, xrb:req, xrc:req, optn2:req
  .word 0x70000009 | (MXU_OPTN2_\optn2 << 22) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16mule     xra:req, xrb:req, xrc:req, xrd:req, optn2:req
  .word 0x71000009 | (MXU_OPTN2_\optn2 << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16mac      xra:req, xrb:req, xrc:req, xrd:req, aptn2:req, optn2:req
  .word 0x7000000a | (MXU_APTN2_\aptn2 << 24) | (MXU_OPTN2_\optn2 << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16macf     xra:req, xrb:req, xrc:req, xrd:req, aptn2:req, optn2:req
  .word 0x7000000b | (MXU_APTN2_\aptn2 << 24) | (MXU_OPTN2_\optn2 << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16madl     xra:req, xrb:req, xrc:req, xrd:req, aptn2:req, optn2:req
  .word 0x7000000c | (MXU_APTN2_\aptn2 << 24) | (MXU_OPTN2_\optn2 << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s16mad      xra:req, xrb:req, xrc:req, xrd:req, aptn1:req, mptn2:req
  .word 0x7000000d | (MXU_APTN1_\aptn1 << 24) | (MXU_MPTN2_\mptn2 << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q16add      xra:req, xrb:req, xrc:req, xrd:req, aptn2:req, optn2:req
  .word 0x7000000e | (MXU_APTN2_\aptn2 << 24) | (MXU_OPTN2_\optn2 << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16mace     xra:req, xrb:req, xrc:req, xrd:req, aptn2:req, optn2:req
  .word 0x7000000f | (MXU_APTN2_\aptn2 << 24) | (MXU_OPTN2_\optn2 << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm


.macro q8mul       xra:req, xrb:req, xrc:req, xrd:req
  .word 0x70000038 | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8mulsu     xra:req, xrb:req, xrc:req, xrd:req
  .word 0x70800038 | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8mac       xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x7000003a | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8macsu     xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x7080003a | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8madl      xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x7000003c | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm


.macro q8movz      xra:req, xrb:req, xrc:req
  .word 0x70000039 | (0 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8movn      xra:req, xrb:req, xrc:req
  .word 0x70000039 | (1 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16movz     xra:req, xrb:req, xrc:req
  .word 0x70000039 | (2 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16movn     xra:req, xrb:req, xrc:req
  .word 0x70000039 | (3 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32movz     xra:req, xrb:req, xrc:req
  .word 0x70000039 | (4 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32movn     xra:req, xrb:req, xrc:req
  .word 0x70000039 | (5 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm


.macro q16scop     xra:req, xrb:req, xrc:req, xrd:req
  .word 0x7000003b | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32sfl      xra:req, xrb:req, xrc:req, xrd:req, ptn:req
  MXU_CHECK_PATTERN \ptn, 0, 3
  .word 0x7000003d | (MXU_PTN_\ptn << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8sad       xra:req, xrb:req, xrc:req, xrd:req
  .word 0x7000003e | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm


.macro d32add      xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x70000018 | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d32addc     xra:req, xrb:req, xrc:req, xrd:req
  .word 0x70400018 | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d32acc      xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x70000019 | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d32accm     xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x70400019 | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d32asum     xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x70800019 | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q16acc      xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x7000001b | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q16accm     xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x7040001b | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16asum     xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x7080001b | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8adde      xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x7000001c | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d8sum       xra:req, xrb:req, xrc:req
  .word 0x7040001c | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d8sumc      xra:req, xrb:req, xrc:req
  .word 0x7080001c | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8acce      xra:req, xrb:req, xrc:req, xrd:req, aptn2:req
  .word 0x7000001d | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm


.macro s32cps      xra:req, xrb:req, xrc:req
  .word 0x70000007 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16cps      xra:req, xrb:req, xrc:req
  .word 0x70080007 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8abd       xra:req, xrb:req, xrc:req
  .word 0x70100007 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q16sat      xra:req, xrb:req, xrc:req
  .word 0x70180007 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32slt      xra:req, xrb:req, xrc:req
  .word 0x70000006 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16slt      xra:req, xrb:req, xrc:req
  .word 0x70040006 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16avg      xra:req, xrb:req, xrc:req
  .word 0x70080006 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16avgr     xra:req, xrb:req, xrc:req
  .word 0x700c0006 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8avg       xra:req, xrb:req, xrc:req
  .word 0x70100006 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8avgr      xra:req, xrb:req, xrc:req
  .word 0x70140006 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8add       xra:req, xrb:req, xrc:req, aptn2:req
  .word 0x701c0006 | (MXU_APTN2_\aptn2 << 24) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32max      xra:req, xrb:req, xrc:req
  .word 0x70000003 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32min      xra:req, xrb:req, xrc:req
  .word 0x70040003 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16max      xra:req, xrb:req, xrc:req
  .word 0x70080003 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d16min      xra:req, xrb:req, xrc:req
  .word 0x700c0003 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8max       xra:req, xrb:req, xrc:req
  .word 0x70100003 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8min       xra:req, xrb:req, xrc:req
  .word 0x70140003 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8slt       xra:req, xrb:req, xrc:req
  .word 0x70180003 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q8sltu      xra:req, xrb:req, xrc:req
  .word 0x701c0003 | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm


.macro d32sll      xra:req, xrb:req, xrc:req, xrd:req, sft4:req
  MXU_CHECK_BOUNDS \sft4, 0, 15
  .word 0x70000030 | ((\sft4) << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d32slr      xra:req, xrb:req, xrc:req, xrd:req, sft4:req
  MXU_CHECK_BOUNDS \sft4, 0, 15
  .word 0x70000031 | ((\sft4) << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d32sarl     xra:req, xrb:req, xrc:req, sft4:req
  MXU_CHECK_BOUNDS \sft4, 0, 15
  .word 0x70000032 | ((\sft4) << 22) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro d32sar      xra:req, xrb:req, xrc:req, xrd:req, sft4:req
  MXU_CHECK_BOUNDS \sft4, 0, 15
  .word 0x70000033 | ((\sft4) << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q16sll      xra:req, xrb:req, xrc:req, xrd:req, sft4:req
  MXU_CHECK_BOUNDS \sft4, 0, 15
  .word 0x70000034 | ((\sft4) << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q16slr      xra:req, xrb:req, xrc:req, xrd:req, sft4:req
  MXU_CHECK_BOUNDS \sft4, 0, 15
  .word 0x70000035 | ((\sft4) << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro q16sar      xra:req, xrb:req, xrc:req, xrd:req, sft4:req
  MXU_CHECK_BOUNDS \sft4, 0, 15
  .word 0x70000037 | ((\sft4) << 22) | (MXU_\xrd << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm


.macro d32sllv     xra:req, xrd:req, rs:req
  .word 0x70000036 | (GPR_\rs << 21) | (0 << 18) | (MXU_\xrd << 14) | (MXU_\xra << 10)
.endm
.macro d32slrv     xra:req, xrd:req, rs:req
  .word 0x70000036 | (GPR_\rs << 21) | (1 << 18) | (MXU_\xrd << 14) | (MXU_\xra << 10)
.endm
.macro d32sarv     xra:req, xrd:req, rs:req
  .word 0x70000036 | (GPR_\rs << 21) | (3 << 18) | (MXU_\xrd << 14) | (MXU_\xra << 10)
.endm
.macro q16sllv     xra:req, xrd:req, rs:req
  .word 0x70000036 | (GPR_\rs << 21) | (4 << 18) | (MXU_\xrd << 14) | (MXU_\xra << 10)
.endm
.macro q16slrv     xra:req, xrd:req, rs:req
  .word 0x70000036 | (GPR_\rs << 21) | (5 << 18) | (MXU_\xrd << 14) | (MXU_\xra << 10)
.endm
.macro q16sarv     xra:req, xrd:req, rs:req
  .word 0x70000036 | (GPR_\rs << 21) | (7 << 18) | (MXU_\xrd << 14) | (MXU_\xra << 10)
.endm


.macro s32madd     xra:req, xrd:req, rs:req, rt:req
  .word 0x70008000 | (GPR_\rs << 21) | (GPR_\rt << 16) | (MXU_\xrd << 10) | (MXU_\xra << 6)
.endm
.macro s32maddu    xra:req, xrd:req, rs:req, rt:req
  .word 0x70008001 | (GPR_\rs << 21) | (GPR_\rt << 16) | (MXU_\xrd << 10) | (MXU_\xra << 6)
.endm
.macro s32msub     xra:req, xrd:req, rs:req, rt:req
  .word 0x70008004 | (GPR_\rs << 21) | (GPR_\rt << 16) | (MXU_\xrd << 10) | (MXU_\xra << 6)
.endm
.macro s32msubu    xra:req, xrd:req, rs:req, rt:req
  .word 0x70008005 | (GPR_\rs << 21) | (GPR_\rt << 16) | (MXU_\xrd << 10) | (MXU_\xra << 6)
.endm


# Note: Ingenic MXU docs fail to mention that, like 's32madd', 's32msub' , etc,
#  the HI and LO CPU registers are stained by 's32mul' and 's32mulu'.
.macro s32mul      xra:req, xrd:req, rs:req, rt:req
  .word 0x70000026 | (GPR_\rs << 21) | (GPR_\rt << 16) | (MXU_\xrd << 10) | (MXU_\xra << 6)
.endm
.macro s32mulu     xra:req, xrd:req, rs:req, rt:req
  .word 0x70004026 | (GPR_\rs << 21) | (GPR_\rt << 16) | (MXU_\xrd << 10) | (MXU_\xra << 6)
.endm
.macro s32extr     xra:req, xrd:req, rs:req, bits5:req
  MXU_CHECK_BOUNDS \bits5, 1, 31
  .word 0x70008026 | (GPR_\rs << 21) | ((\bits5) << 16) | (MXU_\xrd << 10) | (MXU_\xra << 6)
.endm
.macro s32extrv    xra:req, xrd:req, rs:req, rt:req
  .word 0x7000c026 | (GPR_\rs << 21) | (GPR_\rt << 16) | (MXU_\xrd << 10) | (MXU_\xra << 6)
.endm


# XXX: The Ingenic MXU PDF dated June 2, 2017 containing MXU encodings table
#  appears to list this opcode group in the wrong order with regard to the 3-bit
#  minor field at bit 18. The order we use here instead matches their unofficial
#  unofficial binutils patch as well as the 'mxu_as' script from CI20 MPlayer.
#  'X1000_M200_XBurst_ISA_MXU_PM.pdf' is the name of the errant PDF doc.
#
.macro d32sarw     xra:req, xrb:req, xrc:req, rs:req
  .word 0x70000027 | (GPR_\rs << 21) | (0 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32aln      xra:req, xrb:req, xrc:req, rs:req
  .word 0x70000027 | (GPR_\rs << 21) | (1 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32alni     xra:req, xrb:req, xrc:req, ptn:req
  MXU_CHECK_PATTERN \ptn, 0, 4
  .word 0x70000027 | (MXU_PTN_\ptn << 23) | (2 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32lui      xra:req, imm8:req, ptn:req
  MXU_CHECK_PATTERN \ptn, 0, 7
  .if (\imm8) < 0
    MXU_CHECK_BOUNDS \imm8, -128, 127
  .else 
    MXU_CHECK_BOUNDS \imm8, 0, 255
  .endif
  .word 0x70000027 | (MXU_PTN_\ptn << 23) | (3 << 18) | (((\imm8) & 0xff) << 10) | (MXU_\xra << 6)
.endm
.macro s32nor      xra:req, xrb:req, xrc:req
  .word 0x70000027 | (4 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32and      xra:req, xrb:req, xrc:req
  .word 0x70000027 | (5 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32or       xra:req, xrb:req, xrc:req
  .word 0x70000027 | (6 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm
.macro s32xor      xra:req, xrb:req, xrc:req
  .word 0x70000027 | (7 << 18) | (MXU_\xrc << 14) | (MXU_\xrb << 10) | (MXU_\xra << 6)
.endm


.macro s32m2i      xra:req, rt:req
  .word 0x7000002e | (GPR_\rt << 16) | (MXU_I2M_M2I_\xra << 6)
.endm
.macro s32i2m      xra:req, rt:req
  .word 0x7000002f | (GPR_\rt << 16) | (MXU_I2M_M2I_\xra << 6)
.endm


.macro s32lddv     xra:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000012 | (GPR_\rs << 21) | (GPR_\rt << 16) | ((\strd2) << 14) | (MXU_\xra << 6)
.endm
.macro s32lddvr    xra:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000412 | (GPR_\rs << 21) | (GPR_\rt << 16) | ((\strd2) << 14) | (MXU_\xra << 6)
.endm
.macro s32stdv     xra:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000013 | (GPR_\rs << 21) | (GPR_\rt << 16) | ((\strd2) << 14) | (MXU_\xra << 6)
.endm
.macro s32stdvr    xra:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000413 | (GPR_\rs << 21) | (GPR_\rt << 16) | ((\strd2) << 14) | (MXU_\xra << 6)
.endm
.macro s32ldiv     xra:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000016 | (GPR_\rs << 21) | (GPR_\rt << 16) | ((\strd2) << 14) | (MXU_\xra << 6)
.endm
.macro s32ldivr    xra:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000416 | (GPR_\rs << 21) | (GPR_\rt << 16) | ((\strd2) << 14) | (MXU_\xra << 6)
.endm
.macro s32sdiv     xra:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000017 | (GPR_\rs << 21) | (GPR_\rt << 16) | ((\strd2) << 14) | (MXU_\xra << 6)
.endm
.macro s32sdivr    xra:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000417 | (GPR_\rs << 21) | (GPR_\rt << 16) | ((\strd2) << 14) | (MXU_\xra << 6)
.endm


.macro s32ldd      xra:req, rs:req, imm12:req
  MXU_CHECK_OFFSET \imm12, 4, -2048, 2047
  .word 0x70000010 | (GPR_\rs << 21) | (((\imm12) & 0xffc) << 8) | (MXU_\xra << 6)
.endm
.macro s32lddr     xra:req, rs:req, imm12:req
  MXU_CHECK_OFFSET \imm12, 4, -2048, 2047
  .word 0x70100010 | (GPR_\rs << 21) | (((\imm12) & 0xffc) << 8) | (MXU_\xra << 6)
.endm
.macro s32std      xra:req, rs:req, imm12:req
  MXU_CHECK_OFFSET \imm12, 4, -2048, 2047
  .word 0x70000011 | (GPR_\rs << 21) | (((\imm12) & 0xffc) << 8) | (MXU_\xra << 6)
.endm
.macro s32stdr     xra:req, rs:req, imm12:req
  MXU_CHECK_OFFSET \imm12, 4, -2048, 2047
  .word 0x70100011 | (GPR_\rs << 21) | (((\imm12) & 0xffc) << 8) | (MXU_\xra << 6)
.endm
.macro s32ldi      xra:req, rs:req, imm12:req
  MXU_CHECK_OFFSET \imm12, 4, -2048, 2047
  .word 0x70000014 | (GPR_\rs << 21) | (((\imm12) & 0xffc) << 8) | (MXU_\xra << 6)
.endm
.macro s32ldir     xra:req, rs:req, imm12:req
  MXU_CHECK_OFFSET \imm12, 4, -2048, 2047
  .word 0x70100014 | (GPR_\rs << 21) | (((\imm12) & 0xffc) << 8) | (MXU_\xra << 6)
.endm
.macro s32sdi      xra:req, rs:req, imm12:req
  MXU_CHECK_OFFSET \imm12, 4, -2048, 2047
  .word 0x70000015 | (GPR_\rs << 21) | (((\imm12) & 0xffc) << 8) | (MXU_\xra << 6)
.endm
.macro s32sdir     xra:req, rs:req, imm12:req
  MXU_CHECK_OFFSET \imm12, 4, -2048, 2047
  .word 0x70100015 | (GPR_\rs << 21) | (((\imm12) & 0xffc) << 8) | (MXU_\xra << 6)
.endm


# XXX: The Ingenic binutils patch fails to disassemble certain immediate offsets
#  for s16 loads/stores, so don't be alarmed if you see oddness there.
#  It appears to simply be a bug in their disassembler, happily.
#
.macro s8ldd       xra:req, rs:req, imm8:req, ptn:req
  MXU_CHECK_OFFSET  \imm8, 1, -128, 127
  MXU_CHECK_PATTERN \ptn, 0, 7
  .word 0x70000022 | (GPR_\rs << 21) | (MXU_PTN_\ptn << 18) | (((\imm8) & 0xff) << 10) | (MXU_\xra << 6)
.endm
.macro s8std       xra:req, rs:req, imm8:req, ptn:req
  MXU_CHECK_OFFSET  \imm8, 1, -128, 127
  MXU_CHECK_PATTERN \ptn, 0, 3
  .word 0x70000023 | (GPR_\rs << 21) | (MXU_PTN_\ptn << 18) | (((\imm8) & 0xff) << 10) | (MXU_\xra << 6)
.endm
.macro s8ldi       xra:req, rs:req, imm8:req, ptn:req
  MXU_CHECK_OFFSET  \imm8, 1, -128, 127
  MXU_CHECK_PATTERN \ptn, 0, 7
  .word 0x70000024 | (GPR_\rs << 21) | (MXU_PTN_\ptn << 18) | (((\imm8) & 0xff) << 10) | (MXU_\xra << 6)
.endm
.macro s8sdi       xra:req, rs:req, imm8:req, ptn:req
  MXU_CHECK_OFFSET  \imm8, 1, -128, 127
  MXU_CHECK_PATTERN \ptn, 0, 3
  .word 0x70000025 | (GPR_\rs << 21) | (MXU_PTN_\ptn << 18) | (((\imm8) & 0xff) << 10) | (MXU_\xra << 6)
.endm
.macro s16ldd      xra:req, rs:req, imm10:req, ptn:req
  MXU_CHECK_OFFSET  \imm10, 2, -512, 511
  MXU_CHECK_PATTERN \ptn, 0, 3
  .word 0x7000002a | (GPR_\rs << 21) | (MXU_PTN_\ptn << 19) | (((\imm10) & 0x3fe) << 9) | (MXU_\xra << 6)
.endm
.macro s16std      xra:req, rs:req, imm10:req, ptn:req
  MXU_CHECK_OFFSET  \imm10, 2, -512, 511
  MXU_CHECK_PATTERN \ptn, 0, 1
  .word 0x7000002b | (GPR_\rs << 21) | (MXU_PTN_\ptn << 19) | (((\imm10) & 0x3fe) << 9) | (MXU_\xra << 6)
.endm
.macro s16ldi      xra:req, rs:req, imm10:req, ptn:req
  MXU_CHECK_OFFSET  \imm10, 2, -512, 511
  MXU_CHECK_PATTERN \ptn, 0, 3
  .word 0x7000002c | (GPR_\rs << 21) | (MXU_PTN_\ptn << 19) | (((\imm10) & 0x3fe) << 9) | (MXU_\xra << 6)
.endm
.macro s16sdi      xra:req, rs:req, imm10:req, ptn:req
  MXU_CHECK_OFFSET \imm10, 2, -512, 511
  MXU_CHECK_PATTERN \ptn, 0, 1
  .word 0x7000002d | (GPR_\rs << 21) | (MXU_PTN_\ptn << 19) | (((\imm10) & 0x3fe) << 9) | (MXU_\xra << 6)
.endm


.macro lxw         rd:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000028 | (GPR_\rs << 21) | (GPR_\rt << 16) | (GPR_\rd << 11) | ((\strd2) << 9) | (3 << 6)
.endm
.macro lxh         rd:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000028 | (GPR_\rs << 21) | (GPR_\rt << 16) | (GPR_\rd << 11) | ((\strd2) << 9) | (1 << 6)
.endm
.macro lxhu        rd:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000028 | (GPR_\rs << 21) | (GPR_\rt << 16) | (GPR_\rd << 11) | ((\strd2) << 9) | (5 << 6)
.endm
.macro lxb         rd:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000028 | (GPR_\rs << 21) | (GPR_\rt << 16) | (GPR_\rd << 11) | ((\strd2) << 9) | (0 << 6)
.endm
.macro lxbu        rd:req, rs:req, rt:req, strd2:req
  MXU_CHECK_BOUNDS \strd2, 0, 2
  .word 0x70000028 | (GPR_\rs << 21) | (GPR_\rt << 16) | (GPR_\rd << 11) | ((\strd2) << 9) | (4 << 6)
.endm


.endif # MXU1_AS_MACROS_S_H

# vim:shiftwidth=2:expandtab:syntax=asm

/*
 * Copyright (c) 1997, 2018, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef CPU_X86_C2_MACROASSEMBLER_X86_HPP
#define CPU_X86_C2_MACROASSEMBLER_X86_HPP

// C2_MacroAssembler contains high-level macros for C2

public:
  Assembler::AvxVectorLen vector_length_encoding(int vlen_in_bytes);

  void pminmax(int opcode, BasicType elem_bt, XMMRegister dst, XMMRegister src,
               XMMRegister tmp = xnoreg);
  void vpminmax(int opcode, BasicType elem_bt,
                XMMRegister dst, XMMRegister src1, XMMRegister src2,
                int vlen_enc);

  void vminmax_fp(int opcode, BasicType elem_bt,
                  XMMRegister dst, XMMRegister a, XMMRegister b,
                  XMMRegister tmp, XMMRegister atmp, XMMRegister btmp,
                  int vlen_enc);
  void evminmax_fp(int opcode, BasicType elem_bt,
                   XMMRegister dst, XMMRegister a, XMMRegister b,
                   KRegister ktmp, XMMRegister atmp, XMMRegister btmp,
                   int vlen_enc);

  void vextendbd(bool sign, XMMRegister dst, XMMRegister src, int vector_len);
  void vextendwd(bool sign, XMMRegister dst, XMMRegister src, int vector_len);

  void vshiftd(int opcode, XMMRegister dst, XMMRegister shift);
  void vshiftd(int opcode, XMMRegister dst, XMMRegister src, XMMRegister shift, int vlen_enc);
  void vshiftw(int opcode, XMMRegister dst, XMMRegister shift);
  void vshiftw(int opcode, XMMRegister dst, XMMRegister src, XMMRegister shift, int vlen_enc);
  void vshiftq(int opcode, XMMRegister dst, XMMRegister shift);
  void vshiftq(int opcode, XMMRegister dst, XMMRegister src, XMMRegister shift, int vlen_enc);

  void varshiftd(int opcode, XMMRegister dst, XMMRegister src, XMMRegister shift, int vlen_enc);
  void varshiftw(int opcode, XMMRegister dst, XMMRegister src, XMMRegister shift, int vlen_enc);
  void varshiftq(int opcode, XMMRegister dst, XMMRegister src, XMMRegister shift, int vlen_enc, XMMRegister vtmp = xnoreg);
  void varshiftbw(int opcode, XMMRegister dst, XMMRegister src, XMMRegister shift, int vector_len, XMMRegister vtmp, Register scratch);
  void evarshiftb(int opcode, XMMRegister dst, XMMRegister src, XMMRegister shift, int vector_len, XMMRegister vtmp, Register scratch);

  void insert(BasicType typ, XMMRegister dst, Register val, int idx);
  void vinsert(BasicType typ, XMMRegister dst, XMMRegister src, Register val, int idx);
  void vgather(BasicType typ, XMMRegister dst, Register base, XMMRegister idx, XMMRegister mask, int vector_len);
  void evgather(BasicType typ, XMMRegister dst, KRegister mask, Register base, XMMRegister idx, int vector_len);
  void evscatter(BasicType typ, Register base, XMMRegister idx, KRegister mask, XMMRegister src, int vector_len);

  // extract
  void extract(BasicType typ, Register dst, XMMRegister src, int idx);
  XMMRegister get_lane(BasicType typ, XMMRegister dst, XMMRegister src, int elemindex);
  void get_elem(BasicType typ, Register dst, XMMRegister src, int elemindex);
  void get_elem(BasicType typ, XMMRegister dst, XMMRegister src, int elemindex, Register tmp = noreg, XMMRegister vtmp = xnoreg);

  // vector test
  void vectortest(int bt, int vlen, XMMRegister src1, XMMRegister src2,
                  XMMRegister vtmp1 = xnoreg, XMMRegister vtmp2 = xnoreg);

  // blend
  void evpcmp(BasicType typ, KRegister kdmask, KRegister ksmask, XMMRegister src1, AddressLiteral adr, int comparison, int vector_len, Register scratch = rscratch1);
  void evpblend(BasicType typ, XMMRegister dst, KRegister kmask, XMMRegister src1, XMMRegister src2, bool merge, int vector_len);

  void load_vector_mask(XMMRegister dst, XMMRegister src, int vlen_in_bytes, BasicType elem_bt);
  void load_iota_indices(XMMRegister dst, Register scratch, int vlen_in_bytes);

  // Reductions for vectors of bytes, shorts, ints, longs, floats, and doubles.

  // dst = src1  reduce(op, src2) using vtmp as temps
  void reduceI(int opcode, int vlen, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
#ifdef _LP64
  void reduceL(int opcode, int vlen, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
#endif // _LP64

  // dst = reduce(op, src2) using vtmp as temps
  void reduce_fp(int opcode, int vlen,
                 XMMRegister dst, XMMRegister src,
                 XMMRegister vtmp1, XMMRegister vtmp2 = xnoreg);
  void reduceB(int opcode, int vlen, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void mulreduceB(int opcode, int vlen, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduceS(int opcode, int vlen, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduceFloatMinMax(int opcode, int vlen, bool is_dst_valid,
                         XMMRegister dst, XMMRegister src,
                         XMMRegister tmp, XMMRegister atmp, XMMRegister btmp, XMMRegister xmm_0, XMMRegister xmm_1 = xnoreg);
  void reduceDoubleMinMax(int opcode, int vlen, bool is_dst_valid,
                          XMMRegister dst, XMMRegister src,
                          XMMRegister tmp, XMMRegister atmp, XMMRegister btmp, XMMRegister xmm_0, XMMRegister xmm_1 = xnoreg);
 private:
  void reduceF(int opcode, int vlen, XMMRegister dst, XMMRegister src, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduceD(int opcode, int vlen, XMMRegister dst, XMMRegister src, XMMRegister vtmp1, XMMRegister vtmp2);

  // Int Reduction
  void reduce2I (int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce4I (int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce8I (int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce16I(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);

  // Byte Reduction
  void reduce8B (int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce16B(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce32B(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce64B(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void mulreduce8B (int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void mulreduce16B(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void mulreduce32B(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void mulreduce64B(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);

  // Short Reduction
  void reduce4S (int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce8S (int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce16S(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce32S(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);

  // Long Reduction
#ifdef _LP64
  void reduce2L(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce4L(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce8L(int opcode, Register dst, Register src1, XMMRegister src2, XMMRegister vtmp1, XMMRegister vtmp2);
#endif // _LP64

  // Float Reduction
  void reduce2F (int opcode, XMMRegister dst, XMMRegister src, XMMRegister vtmp);
  void reduce4F (int opcode, XMMRegister dst, XMMRegister src, XMMRegister vtmp);
  void reduce8F (int opcode, XMMRegister dst, XMMRegister src, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce16F(int opcode, XMMRegister dst, XMMRegister src, XMMRegister vtmp1, XMMRegister vtmp2);

  // Double Reduction
  void reduce2D(int opcode, XMMRegister dst, XMMRegister src, XMMRegister vtmp);
  void reduce4D(int opcode, XMMRegister dst, XMMRegister src, XMMRegister vtmp1, XMMRegister vtmp2);
  void reduce8D(int opcode, XMMRegister dst, XMMRegister src, XMMRegister vtmp1, XMMRegister vtmp2);

  // Base reduction instruction
  void reduce_operation_128(BasicType typ, int opcode, XMMRegister dst, XMMRegister src);
  void reduce_operation_256(BasicType typ, int opcode, XMMRegister dst, XMMRegister src1, XMMRegister src2);
#endif // CPU_X86_C2_MACROASSEMBLER_X86_HPP

//===-- SBFSelectionDAGInfo.cpp - SBF SelectionDAG Info -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the SBFSelectionDAGInfo class.
//
//===----------------------------------------------------------------------===//

#include "SBFTargetMachine.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/IR/DerivedTypes.h"
#include "SBFSelectionDAGInfo.h"

using namespace llvm;

#define DEBUG_TYPE "sbf-selectiondag-info"

SDValue SBFSelectionDAGInfo::EmitTargetCodeForMemcpy(
    SelectionDAG &DAG, const SDLoc &dl, SDValue Chain, SDValue Dst, SDValue Src,
    SDValue Size, Align Alignment, bool isVolatile, bool AlwaysInline,
    MachinePointerInfo DstPtrInfo, MachinePointerInfo SrcPtrInfo) const {
  // Requires the copy size to be a constant.
  ConstantSDNode *ConstantSize = dyn_cast<ConstantSDNode>(Size);
  if (!ConstantSize)
    return SDValue();

  unsigned CopyLen = ConstantSize->getZExtValue();
  // If the alignment is greater than 8, we can only store and load 8 bytes at a
  // time.
  uint64_t BytesPerOp = std::min(Alignment.value(), static_cast<uint64_t>(8));
  unsigned StoresNumEstimate =
      alignTo(CopyLen, Alignment) >> Log2_64(BytesPerOp);
  // Impose the same copy length limit as MaxStoresPerMemcpy.
  if (StoresNumEstimate > getCommonMaxStoresPerMemFunc())
    return SDValue();

  SDVTList VTs = DAG.getVTList(MVT::Other, MVT::Glue);

  Dst = DAG.getNode(SBFISD::MEMCPY, dl, VTs, Chain, Dst, Src,
                    DAG.getConstant(CopyLen, dl, MVT::i64),
                    DAG.getConstant(Alignment.value(), dl, MVT::i64));

  return Dst.getValue(0);
}

//===-- SBF.h - Top-level interface for SBF representation ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SBF_SBF_H
#define LLVM_LIB_TARGET_SBF_SBF_H

#include "MCTargetDesc/SBFMCTargetDesc.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class SBFTargetMachine;
class PassRegistry;

ModulePass *createSBFCheckAndAdjustIR();

FunctionPass *createSBFISelDag(SBFTargetMachine &TM);
FunctionPass *createSBFMISimplifyPatchablePass();
FunctionPass *createSBFMIPeepholePass();
FunctionPass *createSBFMIPeepholeTruncElimPass();
FunctionPass *createSBFMIPreEmitPeepholePass();
FunctionPass *createSBFMIPreEmitCheckingPass();

void initializeSBFCheckAndAdjustIRPass(PassRegistry&);
void initializeSBFDAGToDAGISelPass(PassRegistry &);
void initializeSBFMIPeepholePass(PassRegistry&);
void initializeSBFMIPeepholeTruncElimPass(PassRegistry &);
void initializeSBFMIPreEmitCheckingPass(PassRegistry&);
void initializeSBFMIPreEmitPeepholePass(PassRegistry &);
void initializeSBFMISimplifyPatchablePass(PassRegistry &);

class SBFAbstractMemberAccessPass
    : public PassInfoMixin<SBFAbstractMemberAccessPass> {
  SBFTargetMachine *TM;

public:
  SBFAbstractMemberAccessPass(SBFTargetMachine *TM) : TM(TM) {}
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  static bool isRequired() { return true; }
};

class SBFPreserveDITypePass : public PassInfoMixin<SBFPreserveDITypePass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  static bool isRequired() { return true; }
};

class SBFIRPeepholePass : public PassInfoMixin<SBFIRPeepholePass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  static bool isRequired() { return true; }
};

class SBFAdjustOptPass : public PassInfoMixin<SBFAdjustOptPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);
};
} // namespace llvm

#endif

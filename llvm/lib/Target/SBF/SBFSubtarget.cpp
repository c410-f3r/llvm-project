//===-- SBFSubtarget.cpp - SBF Subtarget Information ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the SBF specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "SBFSubtarget.h"
#include "SBF.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/TargetParser/Host.h"

using namespace llvm;

#define DEBUG_TYPE "sbf-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "SBFGenSubtargetInfo.inc"

void SBFSubtarget::anchor() {}

SBFSubtarget &SBFSubtarget::initializeSubtargetDependencies(const Triple &TT,
                                                            StringRef CPU,
                                                            StringRef FS) {
  initializeEnvironment(TT);
  initSubtargetFeatures(CPU, FS);
  return *this;
}

void SBFSubtarget::initializeEnvironment(const Triple &TT) {
  assert(TT.getArch() == Triple::sbf && "expected Triple::sbf");
  HasJmpExt = false;
  HasAlu32 = false;
  UseDwarfRIS = false;

  // SBFv2 features
  HasDynamicFrames = false;
  DisableNeg = false;
  ReverseSubImm = false;
  NoLddw = false;
  CallxRegSrc = false;
  HasPqrClass = false;
  NewCallConvention = false;
}

void SBFSubtarget::initSubtargetFeatures(StringRef CPU, StringRef FS) {
  ParseSubtargetFeatures(CPU, /*TuneCPU*/ CPU, FS);

  if (CPU == "v2") {
    HasJmpExt = true;
  }

  if (CPU == "v3") {
    HasJmpExt = true;
    HasAlu32 = true;
  }

  if (CPU == "sbfv2") {
    if (!HasDynamicFrames)
      report_fatal_error("sbfv2 requires dynamic-frames\n", false);
  }
}

SBFSubtarget::SBFSubtarget(const Triple &TT, const std::string &CPU,
                           const std::string &FS, const TargetMachine &TM)
    : SBFGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS), InstrInfo(),
      FrameLowering(initializeSubtargetDependencies(TT, CPU, FS)),
      TLInfo(TM, *this) {
  assert(TT.getArch() == Triple::sbf && "expected Triple::sbf");
}

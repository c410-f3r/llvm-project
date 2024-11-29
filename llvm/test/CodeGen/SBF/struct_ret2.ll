; RUN: llc < %s -march=sbf | FileCheck %s

; Function Attrs: nounwind uwtable
define { i64, i32 } @foo(i32 %a, i32 %b, i32 %c) #0 {
; CHECK-LABEL: foo:
; CHECK: call bar
; CHECK: ldxdw r1, [r10 - 16]
; CHECK: ldxw r2, [r10 - 8]
; CHECK: stxw [r6 + 8], r2
; CHECK: stxdw [r6 + 0], r1
entry:
  %call = tail call { i64, i32 } @bar(i32 %a, i32 %b, i32 %c, i32 1, i32 2) #3
  ret { i64, i32 } %call
}

declare { i64, i32 } @bar(i32, i32, i32, i32, i32) #1

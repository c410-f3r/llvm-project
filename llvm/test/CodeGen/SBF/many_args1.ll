; RUN: llc -O2 -march=sbf < %s | FileCheck %s

; Function Attrs: nounwind uwtable
define i32 @foo(i32 %a, i32 %b, i32 %c) #0 {
; CHECK-LABEL: foo:
; CHECK: mov64 r4, 2
; CHECK: stxdw [r10 - 4096], r4
; CHECK: mov64 r4, 3
; CHECK: stxdw [r10 - 4088], r4
; CHECK: mov64 r5, r10
; CHECK: mov64 r4, 1
; CHECK: call bar
entry:
  %call = tail call i32 @bar(i32 %a, i32 %b, i32 %c, i32 1, i32 2, i32 3) #3
  ret i32 %call
}

declare i32 @bar(i32, i32, i32, i32, i32, i32) #1

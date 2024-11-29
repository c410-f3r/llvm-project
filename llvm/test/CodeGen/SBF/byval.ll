; RUN: llc -O2 -march=sbf < %s | FileCheck %s

%struct.S = type { [10 x i32] }

; Function Attrs: nounwind uwtable
define void @bar(i32 %a) #0 {
; CHECK-LABEL: bar:
; CHECK: lddw r2, 8589934593
; CHECK: stxdw [r10 - 40], r2
; CHECK: mov64 r2, r10
; CHECK: add64 r2, -40
; CHECK: call foo
entry:
  %.compoundliteral = alloca %struct.S, align 8
  %arrayinit.begin = getelementptr inbounds %struct.S, %struct.S* %.compoundliteral, i64 0, i32 0, i64 0
  store i32 1, i32* %arrayinit.begin, align 8
  %arrayinit.element = getelementptr inbounds %struct.S, %struct.S* %.compoundliteral, i64 0, i32 0, i64 1
  store i32 2, i32* %arrayinit.element, align 4
  %arrayinit.element2 = getelementptr inbounds %struct.S, %struct.S* %.compoundliteral, i64 0, i32 0, i64 2
  store i32 3, i32* %arrayinit.element2, align 8
  %arrayinit.start = getelementptr inbounds %struct.S, %struct.S* %.compoundliteral, i64 0, i32 0, i64 3
  %scevgep4 = bitcast i32* %arrayinit.start to i8*
  call void @llvm.memset.p0i8.i64(i8* align 4 %scevgep4, i8 0, i64 28, i1 false)
  call void @foo(i32 %a, %struct.S* byval(%struct.S) align 8 %.compoundliteral) #3
  ret void
}

declare void @foo(i32, %struct.S* byval(%struct.S) align 8) #1

; Function Attrs: nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture, i8, i64, i1) #3

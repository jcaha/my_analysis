# my_analysis
本次实验使用**LLVM**生成C语言程序中间代码(IR)。原始代码如下
```
int a = 10;
int test(int b){
	int *p = &a;
	*p = b+1;
	return a; 
}
```
使用以下命令，生成LLVM IR。需要注意的是，`-O1`参数用来选择优化方法，生成SSA中间代码。
```
clang -c test.c -S -O1 -emit-llvm -o test.ll
```
得到的test.ll文件中间代码部分如下。
```
@a = local_unnamed_addr global i32 10, align 4

; Function Attrs: norecurse nounwind uwtable
define i32 @test(i32 %b) local_unnamed_addr #0 {
entry:
  %add = add nsw i32 %b, 1
  store i32 %add, i32* @a, align 4, !tbaa !2
  ret i32 %add
}
```
IR中test1函数内部，对变量`a`的内存位置上，进行了重复赋值。

`@a = local_unnamed_addr global i32 10, align 4`把值`10`赋给全局变量`@a`

`store i32 %add, i32* @a, align 4, !tbaa !2`第二次对`@a`进行赋值

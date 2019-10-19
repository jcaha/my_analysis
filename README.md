# my_analysis
本次实验使用**LLVM**生成C语言程序中间代码(IR)。原始代码如下
```
int test1(int a){
	int* p = &a;
	*p = *p + 1;
	return *p;
}

int test2(int a){
	int p = a;
	p = p + 1;
	return p;
}
```
其中，两个函数进行实验对照。test2可以完全转化SSA，而test1只能转化部分SSA。
使用以下命令，生成LLVM IR
```
clang -c test.c -S -emit-llvm -o test.ll
```
得到的test.ll文件中间代码部分如下。
```
; Function Attrs: noinline nounwind uwtable
define dso_local i32 @test1(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32*, align 8
  store i32 %0, i32* %2, align 4
  store i32* %2, i32** %3, align 8
  %4 = load i32*, i32** %3, align 8
  %5 = load i32, i32* %4, align 4
  %6 = add nsw i32 %5, 1
  %7 = load i32*, i32** %3, align 8
  store i32 %6, i32* %7, align 4
  %8 = load i32*, i32** %3, align 8
  %9 = load i32, i32* %8, align 4
  ret i32 %9
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @test2(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %4 = load i32, i32* %2, align 4
  store i32 %4, i32* %3, align 4
  %5 = load i32, i32* %3, align 4
  %6 = add nsw i32 %5, 1
  store i32 %6, i32* %3, align 4
  %7 = load i32, i32* %3, align 4
  ret i32 %7
}
```
IR中test1函数内部，对内存位置`%2`上的数据，进行了重复赋值。

`store i32 %0, i32* %2, align 4`把值`%0`赋给地址`%2`

`store i32* %2, i32** %3, align 8`存储地址`%2`的内存位置，作为值赋给地址`%3`

`%7 = load i32*, i32** %3, align 8`从地址`%3`中取出内存位置(即上句的`%2`)，命名为`%7`

`store i32 %6, i32* %7, align 4`向内存位置`%7`(即内存位置`%2`)，赋值数据`%6`
这样，对于内存位置`%2`，前后两次重复赋值两次数据，分别是`%0`，`%6`。

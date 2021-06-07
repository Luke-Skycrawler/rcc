# RCC (C-like Compiler, R for either remarkable or retarded)

---

> Authors: [Haoyang Shi](https://github.com/Luke-Skycrawler), [Tinghao Xie](http://vtu.life)

This repository contains our course project for *Compiler Principle* Curriculum at ZJU.

### Difference with C

[x] type system: char, int, double and n-dimensional array type; Pointers and struct type is not supported in this version.
[x] no controled jumps, gotos and labels , i.e. break, continue and switch statements are not supported.
[x] pre-compile MARCO not supported
[x] `scanf` and `printf` are automaticly declared and linked with libc in runtime
[x] calling convention of `scanf` modified. e.g. you shall use `scanf("%d",i)` to read the value into variable i and drop the `&` symbol. 
[x] `for` loop snippet is switched to pascal-like `for(i: 0 to n){}`, where i is only seen within the scope of this loop 
[x] unary operators not supported

try out the test samples to get a better understanding of the gramma. 
### Prerequsite

* flex 2.5+
* bison 3.0+
* clang 7.0+
* llvm 7.0+

which is easily accessible via apt and other package managers.  

It has been successfully tested with
* flex 2.6.4 + bison 3.0.4 on Ubuntu 18.04 (x86_64)
* flex 2.5.35 + bison 3.7.6 on MacOS (x86_64)

### Install
Clean the directory with:
```bash
make clean
```

Install with:
```bash
make
```
### Usage

```bash
./rcc src_file
./a.out
```
The generated ELF object file and executable are named output.o and a.out respectively by default. 

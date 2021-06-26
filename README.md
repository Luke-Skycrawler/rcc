# RCC (C-like Compiler, R for either remarkable or retarded)

---

> Authors: [Haoyang Shi](https://github.com/Luke-Skycrawler), [Tinghao Xie](http://vtu.life)

[This repository](https://github.com/Luke-Skycrawler/rcc) contains our course project for *Compiler Principle* at ZJU.

### Differences with C

* type system: char, int, double and n-dimensional array type; Pointer type is not supported in this version.
* no controled jumps, gotos and labels , i.e. break, continue and switch statements are not supported.
* pre-compile MARCO not supported
* `scanf` and `printf` are automaticly declared and linked with libc in runtime
* calling convention of `scanf` modified. e.g. you shall use `scanf("%d",i)` to read the value into variable i and drop the `&` symbol. 
* `for` loop switched to pascal-like `for(i: 0 to n){}` and `for(i: n downto 0){}` snippet where i is only seen within the scope of this loop 
* unary operators not supported

Try out the test samples to get a better understanding of the gramma. 

### More Features

- [x] Advanced self-defined types (nested struct and arrays)
- [x] MACRO support (`#define`/`#define f(X)`,`#ifdef`/`ifndef` and nested)
- [ ] Error detection and recovery (primary)

### Prerequsite

* flex 2.5+
* bison 3.0+
* clang 7.0+
* llvm 7.0+

which is easily accessible via apt and other package managers.  

It has been successfully tested with
* flex 2.6.4 + bison 3.0.4 + llvm-12 on Ubuntu 18.04 (x86_64)
* flex 2.5.35 + bison 3.7.6 + llvm-12 on MacOS (x86_64)

### Install

Clean the directory with:
```bash
make clean
```

Install with:
```bash
make
```

If you want to install with a specific version of bison, install with:
```bash
make BISON=[YOUR-BISON-PATH]
```

If you are installing RCC with LLVM12 on MacOS, install with:
```bash
make DEFINE='-D MACOS'
```

### Usage

```bash
./rcc src_file
./a.out
```
The generated ELF object file and executable are named `output.o` and `a.out` respectively by default. 

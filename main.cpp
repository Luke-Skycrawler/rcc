#include "AST.hpp"
#include "RccGlobal.hpp"
#include "rcc.tab.hpp"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;
using namespace llvm::sys;

LLVMContext context;
IRBuilder<> builder(context);
Module *topModule;
// std::map<std::string, std::string> function_bindings;
std::map<std::string, AllocaInst*> bindings;
std::map<std::string, std::vector<Nconstant*>*> dimensionBindings;
Node *root;
bool type_error_alarm = true;

int main(int argc, char **argv)
{
    RccGlobal global(argc > 1 ? argv[1] : "");
    if (argc == 2)
        freopen(argv[1], "r", stdin);
    topModule = new Module("retarded", context);
    yy::rcc parser(global);
    int v = parser.parse();
    if (v == 0 && root)
    {
        root->codeGen();
        topModule->print(errs(), nullptr);
        printf("\n//--------------------visual-------------------\n");
        root->printNode(0);
        printf("\n//--------------------visual-------------------\n");
    }
    else
        printf("terminated\n");

    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    auto TargetTriple = sys::getDefaultTargetTriple();

    topModule->setTargetTriple(TargetTriple);

    std::string Error;
    auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target)
    {
        errs() << Error;
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto RM = Optional<Reloc::Model>();
    auto targetMachine =
        Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    topModule->setDataLayout(targetMachine->createDataLayout());

    auto Filename = "output.o";
    std::error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

    if (EC)
    {
        errs() << "Could not open file: " << EC.message();
        return 1;
    }

    legacy::PassManager pass;
    #ifdef MACOS
    auto fileType = CodeGenFileType::CGFT_ObjectFile;
    #endif
    #ifndef MACOS
    auto fileType = TargetMachine::CodeGenFileType::CGFT_ObjectFile;
    #endif

    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType))
    {
        errs() << "Target machine can't emit a file of this type";
        return 1;
    }

    pass.run(*topModule);
    dest.flush();

    outs() << "Wrote " << Filename << "\n";
    system("g++ output.o");
    return v;
}

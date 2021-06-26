cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Test VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(EXTRA_INCLUDES "/d/Program Files/llvm/include")
# set(EXTRA_LIBS "/d/Program Files/llvm/lib")
# # should we use our own math functions
# option(USE_MYMATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings
# to the source code
# configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the MathFunctions library
# if(USE_MYMATH)
#   add_subdirectory(MathFunctions)
#   list(APPEND EXTRA_LIBS MathFunctions)
#   list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
# endif()
# add the executable
list(APPEND EXTRA_LIBS "D:/Program Files/llvm/lib")
list(APPEND EXTRA_INCLUDES "D:/Program Files/llvm/include")
add_executable(Test rcc.tab.cpp lex.yy.c codegen.cpp llvmGlobal.cpp )

target_link_libraries(Test PUBLIC ${EXTRA_LIBS})

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Test PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )

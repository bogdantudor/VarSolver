cmake_minimum_required(VERSION 3.16)
project(VarSolver)

#if(MSVC)
#    target_compile_options(VarSolver PRIVATE /W4 /WX)
#else()
#    target_compile_options(VarSolver PRIVATE -Wall -Wextra -pedantic -Werror)
#endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(VarSolver main.cpp Solver.cpp Solver.h logging.h)

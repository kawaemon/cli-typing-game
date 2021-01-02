cmake_minimum_required(VERSION 3.10)

# set the project name
project(TypingGame)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:Shift_JIS>")

# add the executable
add_executable(TypingGame main.c)

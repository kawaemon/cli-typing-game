cmake_minimum_required(VERSION 3.10)

project(TypingGame)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:Shift_JIS>")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/Wall>")

add_executable(TypingGame src/assert.c src/main.c src/string.c src/win_terminal.c src/roma.c src/char_vector.c src/word.c)

# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project(sudoku VERSION 2.0.4 DESCRIPTION "An sudoku library")

option(EXAMPLE "Compile the example binaries.")
option(DEBUG "Compile with debugging information.")
option(PROFILE "Compile with profiling information.")


# Set required standard to C11.
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Debugging CFLAGS.  Turn optimizations off; turn debugging symbols on.
if(DEBUG)
    add_definitions(-DDEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
else()
    add_definitions(-DNDEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif()

# Profiling CFLAGS.  Turn profiling information on.
if(CMAKE_COMPILER_IS_GNUCC AND PROFILE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif()

# Creates and names a library, sets it as SHARED, and provides the relative
# paths to its source code.
# Gradle automatically packages shared libraries with your APK.
add_library( # Sets the name of the library.
sudoku

# Sets the library as a shared library.
SHARED

# Provides a relative path to your source file(s).
bits/src/arr.c
bits/src/board.c
bits/src/dbg.c
bits/src/generate.c
bits/src/rand.c
bits/src/read.c
bits/src/solve.c)
set_target_properties(sudoku PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(sudoku PROPERTIES SOVERSION ${PROJECT_VERSION})
set_target_properties(sudoku PROPERTIES PUBLIC_HEADER include/libsudoku.h)
target_include_directories(sudoku PRIVATE bits/include)

if(EXAMPLE)
    add_executable (libsudoku_example ${CMAKE_CURRENT_SOURCE_DIR}/example/main.c)
    target_include_directories(libsudoku_example PRIVATE include)
    target_link_libraries(libsudoku_example PRIVATE sudoku)
endif()

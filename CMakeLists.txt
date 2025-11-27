# Minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Project name and language
project(ChessGame LANGUAGES C)

# -------------------------
# C Standard
# -------------------------
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# -------------------------
# Include directories
# -------------------------
include_directories(include)

# -------------------------
# Gather all source files
# -------------------------
file(GLOB SRC_FILES "src/*.c")

# -------------------------
# Create executable
# -------------------------
add_executable(ChessGame ${SRC_FILES})

# -------------------------
# Compiler flags (optional)
# -------------------------
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ChessGame PRIVATE -Wall -Wextra -Wpedantic -Werror)
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(ChessGame PRIVATE /W4 /WX)
endif()

# -------------------------
# Output directory for executable
# -------------------------
set_target_properties(ChessGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build"
)

# -------------------------
# Optional: run target
# -------------------------
add_custom_target(run
    COMMAND ChessGame
    DEPENDS ChessGame
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/build"
)

# -------------------------
# Optional: clean target
# -------------------------
# CMake handles clean automatically with: cmake --build . --target clean
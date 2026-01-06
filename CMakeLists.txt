cmake_minimum_required(VERSION 3.10)
project(newton_fractal CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(ISPC_EXECUTABLE ispc REQUIRED)

set(ISPC_SOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/fractal.ispc)
set(GENERATED_HEADER ${CMAKE_CURRENT_BINARY_DIR}/fractal.ispc.h)
set(GENERATED_OBJECT ${CMAKE_CURRENT_BINARY_DIR}/fractal.o)

add_custom_command(
    OUTPUT ${GENERATED_OBJECT} ${GENERATED_HEADER}
    COMMAND ${ISPC_EXECUTABLE}
            -o ${GENERATED_OBJECT}
            -h ${GENERATED_HEADER}
            ${ISPC_SOURCE_FILE}
            --target=host
    DEPENDS ${ISPC_SOURCE_FILE}
    COMMENT "Compiling ISPC file: ${ISPC_SOURCE_FILE}"
)

add_custom_target(IspcCompilation
    DEPENDS ${GENERATED_OBJECT} ${GENERATED_HEADER}
)

add_executable(newton_fractal
    src/main.cpp
)
target_link_libraries(newton_fractal PRIVATE ${GENERATED_OBJECT})

add_dependencies(newton_fractal IspcCompilation)

target_include_directories(newton_fractal PUBLIC
    libs
    ${CMAKE_CURRENT_BINARY_DIR}
)

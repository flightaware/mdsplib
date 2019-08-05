cmake_minimum_required(VERSION 3.1...3.15)

project(mdsplib LANGUAGES C)

IF (MSVC)
    # MSVC is logs many inline warnings
    add_definitions("/wd4710")
    add_definitions("/wd4711")
    add_definitions("/wd4068")
ENDIF()

list(APPEND SRCS src/antoi.c)
list(APPEND SRCS src/charcmp.c)
list(APPEND SRCS src/decode_metar.c)
list(APPEND SRCS src/decode_metar_remark.c)
list(APPEND SRCS src/fracpart.c)
list(APPEND SRCS src/local.h)
list(APPEND SRCS src/metar_structs.h)
list(APPEND SRCS src/print_decoded_metar.c)
list(APPEND SRCS src/stspack2.c)
list(APPEND SRCS src/stspack3.c)

add_library(metar STATIC ${SRCS})

add_executable(metar_test src/metar_test.c)

target_link_libraries(metar_test PRIVATE metar)

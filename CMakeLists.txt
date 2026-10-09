# SPDX-License-Identifier: MPL-2.0
# ESP-IDF component; host verification uses the independent Makefile.
set(coexist_sources
    src/accessors.c src/api.c src/core.c src/external.c src/hardware.c
    src/policy.c src/rom_data.c src/rom_dispatch.c src/scheduler.c)
# Retain the exact public declaration inputs supplied to this implementation.
# sdkconfig, esp_err, GPIO/SOC and ROM timer declarations come from real IDF.
configure_file("${CMAKE_CURRENT_LIST_DIR}/spec/public-sdk/esp_coexist_adapter.h"
    "${CMAKE_CURRENT_BINARY_DIR}/public/private/esp_coexist_adapter.h" COPYONLY)
idf_component_register(SRCS ${coexist_sources}
    INCLUDE_DIRS include spec/public-sdk "${CMAKE_CURRENT_BINARY_DIR}/public"
    REQUIRES esp_coex
    LDFRAGMENTS linker.lf)
set_target_properties(${COMPONENT_LIB} PROPERTIES OUTPUT_NAME coexist)
target_compile_definitions(${COMPONENT_LIB} PRIVATE COEX_TARGET=1)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Wextra -Werror
    -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes)
# Target layout checks must compile with the very same compiler and headers.
target_sources(${COMPONENT_LIB} PRIVATE tests/layout.c)

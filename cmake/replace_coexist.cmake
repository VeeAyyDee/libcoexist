# SPDX-License-Identifier: MPL-2.0
# Include after project() in the consuming ESP-IDF project.
# The libcoexist component must already be in EXTRA_COMPONENT_DIRS.
if(NOT TARGET coexist)
    message(FATAL_ERROR "Pinned ESP-IDF imported coexist archive target is missing")
endif()
idf_component_get_property(_coex_source_target libcoexist COMPONENT_LIB)
if(NOT TARGET "${_coex_source_target}")
    message(FATAL_ERROR "Original-source libcoexist component was not registered")
endif()
get_target_property(_coex_binary_dir "${_coex_source_target}" BINARY_DIR)
set(_coex_archive "${_coex_binary_dir}/libcoexist.a")
set_target_properties(coexist PROPERTIES IMPORTED_LOCATION "${_coex_archive}")
# Do not add a dependency from the imported target to the component: IDF's
# esp_coex/Wi-Fi/component relationships can turn that into a cycle. The source
# component is already part of the application's component build graph.
message(STATUS "Using original-source libcoexist archive: ${_coex_archive}")
include("${CMAKE_CURRENT_LIST_DIR}/replace_rom_bindings.cmake")

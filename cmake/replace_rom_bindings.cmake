# SPDX-License-Identifier: MPL-2.0
# The SDK uses absolute ROM assignments, which override C definitions even
# when our archive is selected. Generate a build-local SDK script that omits
# only the 14 functions supplied here. Keep all ROM data cells/dependencies.
if(NOT IDF_TARGET STREQUAL "esp32s3" OR
   NOT IDF_VERSION_MAJOR EQUAL 6 OR NOT IDF_VERSION_MINOR EQUAL 0 OR
   NOT IDF_VERSION_PATCH EQUAL 3)
    message(FATAL_ERROR "libcoexist ROM binding replacement is pinned to ESP32-S3 IDF 6.0.3")
endif()
idf_build_get_property(_coex_idf_path IDF_PATH)
set(_coex_rom_original "${_coex_idf_path}/components/esp_rom/esp32s3/ld/esp32s3.rom.ld")
file(SHA256 "${_coex_rom_original}" _coex_rom_hash)
if(NOT _coex_rom_hash STREQUAL "67b760f9ac1736bc5ab4182508ea5d3197e0908b6f2d3ae39f29fc853b9fbea9")
    message(FATAL_ERROR "SDK ROM linker script differs from audited version; review libcoexist bindings first")
endif()
file(READ "${_coex_rom_original}" _coex_rom_text)
set(_coex_rom_replaced
    coex_bt_release coex_bt_request coex_core_pti_get coex_core_release
    coex_core_request coex_core_status_get coex_event_duration_get
    coex_hw_timer_disable coex_hw_timer_enable coex_hw_timer_set
    coex_schm_interval_set coex_schm_lock coex_schm_unlock coex_wifi_release)
foreach(_coex_name IN LISTS _coex_rom_replaced)
    set(_coex_pattern "\n${_coex_name}[ \t]*=[ \t]*0x[0-9a-fA-F]+")
    string(REGEX MATCHALL "${_coex_pattern}" _coex_matches "${_coex_rom_text}")
    list(LENGTH _coex_matches _coex_count)
    if(NOT _coex_count EQUAL 1)
        message(FATAL_ERROR "Expected exactly one ROM assignment for ${_coex_name}")
    endif()
    string(REGEX REPLACE "${_coex_pattern};"
        "\n/* ${_coex_name} supplied by original-source libcoexist.a */"
        _coex_rom_text "${_coex_rom_text}")
endforeach()
set(_coex_rom_patched "${CMAKE_BINARY_DIR}/libcoexist-esp32s3.rom.ld")
file(WRITE "${_coex_rom_patched}" "${_coex_rom_text}")
idf_component_get_property(_coex_rom_target esp_rom COMPONENT_LIB)
get_target_property(_coex_rom_options ${_coex_rom_target} INTERFACE_LINK_OPTIONS)
set(_coex_new_options)
set(_coex_option_replaced FALSE)
foreach(_coex_option IN LISTS _coex_rom_options)
    if(_coex_option STREQUAL "SHELL:-T esp32s3.rom.ld")
        list(APPEND _coex_new_options "SHELL:-T \"${_coex_rom_patched}\"")
        set(_coex_option_replaced TRUE)
    else()
        list(APPEND _coex_new_options "${_coex_option}")
    endif()
endforeach()
if(NOT _coex_option_replaced)
    message(FATAL_ERROR "Could not find the expected SDK ROM linker option")
endif()
set_property(TARGET ${_coex_rom_target} PROPERTY INTERFACE_LINK_OPTIONS "${_coex_new_options}")
idf_build_set_property(__LINK_DEPENDS "${_coex_rom_patched}" APPEND)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${_coex_rom_original}")
message(STATUS "Using build-local ROM bindings for 14 libcoexist replacements; SDK files unchanged")

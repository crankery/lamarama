# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.1)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(lama C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Preserve packed assets in flash; only the generated C source lives in build/.
find_package(Python3 REQUIRED COMPONENTS Interpreter)
file(GLOB MONK_ASSETS CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/assets/monk-*.bin")
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/monk_assets.c
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/tools/embed_monk.py
            ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/monk_assets.c
    DEPENDS ${MONK_ASSETS} ${CMAKE_CURRENT_LIST_DIR}/tools/embed_monk.py
    VERBATIM
)

# Add executable. Default name is the project name, version 0.1

add_executable(lama
    dac.c
    audio_debug.c
    cv_input.c
    cv_pitch.c
    vowel_controls.c
    calibration.c
    settings.c
    settings_flash.c
    settings_format.c
    voice_control.c
    input.c
    display.c
    monk.c
    menu.c
    routing.c
    ui_controls.c
    ${CMAKE_CURRENT_BINARY_DIR}/monk_assets.c
    lama.c
    midi.c
    midi_uart.c
    messenger.c
    soundCore.c
    voice.c
)

option(LAMA_AUDIO_DEBUG "Send audio diagnostics over UART0 TX GP0 once/sec" ON)
if(LAMA_AUDIO_DEBUG)
    target_compile_definitions(lama PRIVATE
        LAMA_AUDIO_DEBUG=1
    )
endif()

target_compile_options(lama PRIVATE
    -mfpu=fpv5-sp-d16
    -mfloat-abi=softfp
)

pico_set_program_name(lama "lama")
pico_set_program_version(lama "0.1")

# Generate PIO header
pico_generate_pio_header(lama ${CMAKE_CURRENT_LIST_DIR}/dac.pio)
pico_generate_pio_header(lama ${CMAKE_CURRENT_LIST_DIR}/mcp3208.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(lama 0)
pico_enable_stdio_usb(lama 0)

# Add the standard library to the build
target_link_libraries(lama
        pico_stdlib
        pico_multicore
        pico_flash)

# Add the standard include files to the build
target_include_directories(lama PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(lama 
        hardware_flash
        hardware_adc
        hardware_clocks
        hardware_uart
        hardware_spi
        hardware_i2c
        hardware_dma
        hardware_pio
        hardware_timer
        hardware_watchdog
        )

pico_add_extra_outputs(lama)


add_custom_command(TARGET lama POST_BUILD
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/tools/check_flash_size.py
            ${CMAKE_CURRENT_BINARY_DIR}/lama.bin
    VERBATIM
)

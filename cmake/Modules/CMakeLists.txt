cmake_minimum_required(VERSION 3.22)

project(Modules)
add_library(Modules INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(Modules INTERFACE 
	USE_HAL_DRIVER
	STM32H563xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(Modules INTERFACE
    ../../Modules/Inc
)

target_sources(Modules INTERFACE
    ../../Modules/Src/w25q64.c
    ../../Modules/Src/audio_module.c
)

target_link_directories(Modules INTERFACE
)

target_link_libraries(Modules INTERFACE
)

# Validate that Modules code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()
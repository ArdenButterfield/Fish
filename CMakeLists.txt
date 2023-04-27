# 3.24.1 is bundled in Visual Studio 2022 v17.4
# 3.24.1 is also bundled in CLion as of 2023
cmake_minimum_required(VERSION 3.24.1)

# This is the name of your plugin
# This cannot have spaces (but PRODUCT_NAME can)
set(PROJECT_NAME "Fish")

# Set the plugin formats you'll be building here.
# Valid formats: AAX Unity VST AU AUv3 Standalone
set(FORMATS AU VST3 AUv3)

# This must be set before the project() call
# see: https://cmake.org/cmake/help/latest/variable/CMAKE_OSX_DEPLOYMENT_TARGET.html
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Support macOS down to High Sierra")

# Reads in our VERSION file and sticks in it CURRENT_VERSION variable
# Be sure the file has no newlines!
file(STRINGS VERSION CURRENT_VERSION)

# For simplicity, the name of the project is also the name of the target
project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

# By default we don't want Xcode schemes to be made for modules, etc
set(CMAKE_XCODE_GENERATE_SCHEME OFF)

# Building universal binaries on macOS increases build time
# This is set on CI but not during local dev
if ((DEFINED ENV{CI}) AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
  message("Building for Apple Silicon and x86_64")
  set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)
endif ()

# Adds all the module sources so they appear correctly in the IDE
# Must be set before JUCE is added as a sub-dir (or any targets are made)
# https://github.com/juce-framework/JUCE/commit/6b1b4cf7f6b1008db44411f2c8887d71a3348889
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

# Create a /Modules directory in the IDE with the JUCE Module code
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

# JUCE is setup as a submodule in the /JUCE folder
# Locally, you'll need to run `git submodule update --init --recursive` once
# and `git submodule update --remote --merge` to keep it up to date
# On Github Actions, it's managed by actions/checkout
add_subdirectory(JUCE)

# Check the readme at `docs/CMake API.md` in the JUCE repo for full config
juce_add_plugin("${PROJECT_NAME}"
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    COMPANY_NAME Wildergarden
    BUNDLE_ID com.wildergarden.wildergarden
    # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
    # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
    # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD TRUE # On MacOS, plugin will be copied to /Users/you/Library/Audio/Plug-Ins/
    PLUGIN_MANUFACTURER_CODE Awbb               # This has to be one uppercase, rest lower for AU formats
    PLUGIN_CODE FI01                            # A unique four-character plugin id with at least one upper-case character
    FORMATS "${FORMATS}"
    PRODUCT_NAME "${PROJECT_NAME}")        # The name of the final executable, which can differ from the target name

# C++20, please
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_20)

# Manually list all .h and .cpp files for the plugin
set(SourceFiles
    Source/stk-filters/Stk.cpp
    Source/stk-filters/BiQuad.h
    Source/stk-filters/Stk.h
    Source/stk-filters/BiQuad.cpp
    Source/stk-filters/Filter.h
    Source/PluginEditor.h
    Source/PluginProcessor.h
    Source/MP3Processor.cpp
    Source/filterCalc/FilterCalc.cpp
    Source/filterCalc/FilterCalc.h
    Source/downsampler.h
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/MP3Processor.h
    Source/LookAndFeel.h
    Source/RingBuffer.h
    Source/upsampler.h
    Source/BinaryData.h
    )
target_include_directories("${PROJECT_NAME}"
        PRIVATE
        Source/lib/lame/include)

target_sources("${PROJECT_NAME}" PRIVATE ${SourceFiles})

# No, we don't want our source buried in extra nested folders
set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")

# The Xcode source tree should uhhh, still look like the source tree, yo
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "" FILES ${SourceFiles})

# Setup our binary data as a target
juce_add_binary_data(Assets SOURCES
    Images/fish-ghost-0.png
    Images/fish-ghost-1.png
    Images/fish-ghost-2.png
    Images/fish-ghost-3.png
    Images/fish-ghost-4.png
    Images/fish-ghost-5.png
    Images/fish-ghost-6.png
    Images/fish-ghost-7.png
    Images/fish-ghost-8.png
    Images/fish-ghost-9.png
    Images/fish-ghost-10.png
    Images/fish-ghost-11.png
    Images/fish-ghost-12.png
    Images/fish-ghost-13.png
    Images/fish-ghost-14.png
    Images/fish-ghost-15.png
    Images/fish-ghost-16.png
    Images/fish-ghost-17.png
    Images/fish-ghost-18.png
    Images/fish-ghost-19.png
    Images/fish-ghost-20.png
    Images/fish-ghost-21.png
    Images/fish-ghost-22.png
    Images/fish-ghost-23.png
    Images/fish-ghost-24.png
    Images/fish-ghost-25.png
    Images/fish-ghost-26.png
    Images/fish-ghost-27.png
    Images/fish-ghost-28.png
    Images/fish-ghost-29.png
    Images/fish-ghost-30.png
    Images/fish-ghost-31.png
    Images/fish-ghost-32.png
    Images/fish-ghost-33.png
    Images/fish-ghost-34.png
    Images/fish-unique-0.png
    Images/fish-unique-1.png
    Images/fish-unique-2.png
    Images/fish-unique-3.png
    Images/fish-unique-4.png
    Images/fish-unique-5.png
    Images/fish-unique-6.png
    Images/fish-unique-7.png
    Images/fish-unique-8.png
    Images/fish-unique-9.png
    Images/fish-unique-10.png
    Images/fish-unique-11.png
    Images/fish-unique-12.png
    Images/fish-unique-13.png
    Images/fish-unique-14.png
    Images/fish-unique-15.png
    Images/fish-unique-16.png
    Images/fish-unique-17.png
    Images/fish-unique-18.png
    Images/fish-unique-19.png
    Images/fish-unique-20.png
    Images/fish-unique-21.png
    Images/fish-unique-22.png
    Images/fish-unique-23.png
    Images/fish-unique-24.png
    Images/fish-unique-25.png
    Images/fish-unique-26.png
    Images/fish-unique-27.png
    Images/fish-unique-28.png
    Images/fish-unique-29.png
    Images/fish-unique-30.png
    Images/fish-unique-31.png
    Images/fish-unique-32.png
    Images/fish-unique-33.png
    Images/fish-unique-34.png
)

# Required for Linux happiness:
# See https://forum.juce.com/t/loading-pytorch-model-using-binarydata/39997/2
set_target_properties(Assets PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

# Clean up folder organization on Xcode.
# It tucks the Plugin varieties into a "Targets" folder and generate an Xcode Scheme manually
# Xcode scheme generation is turned off globally to limit noise from other targets
# The non-hacky way of doing this is via the global PREDEFINED_TARGETS_FOLDER property
# However that doesn't seem to be working in Xcode
# Not all plugin types (au, vst) available on each build type (win, macos, linux)
foreach(target ${FORMATS} "All")
    if(TARGET ${PROJECT_NAME}_${target})
        set_target_properties(${PROJECT_NAME}_${target} PROPERTIES
            # Tuck the actual plugin targets into a folder where they won't bother us
            FOLDER "Targets"

            # MacOS only: Sets the default executable that Xcode will open on build
            # For this exact path to to work, manually build the AudioPluginHost.xcodeproj in the JUCE subdir
            XCODE_SCHEME_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/extras/AudioPluginHost/Builds/MacOSX/build/Debug/AudioPluginHost.app"

            # Let us build the target in Xcode
            XCODE_GENERATE_SCHEME ON)
    endif()
endforeach()
set_target_properties(Assets PROPERTIES FOLDER "Targets")


target_compile_definitions("${PROJECT_NAME}"
    PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0)


# link_directories(./Source/lib/lame/libmp3lame/.libs/)
target_link_libraries("${PROJECT_NAME}"
    PRIVATE
    Assets
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/lib/lame/libmp3lame/.libs/libmp3lame.a
    PUBLIC
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

# When present, use Intel IPP for performance on Windows
if(WIN32) # Can't use MSVC here, as it won't catch Clang on Windows
    find_package(IPP)
    if(IPP_FOUND)
        target_link_libraries("${PROJECT_NAME}" PUBLIC IPP::ipps IPP::ippcore IPP::ippi IPP::ippcv)
        message("IPP LIBRARIES FOUND")
        target_compile_definitions("${PROJECT_NAME}" PUBLIC PAMPLEJUCE_IPP=1)
    else()
        message("IPP LIBRARIES *NOT* FOUND")
    endif()
endif()

# Required for ctest (which is just easier for cross-platform CI)
# include(CTest) does this too, but adds tons of targets we don't want
# See: https://github.com/catchorg/Catch2/issues/2026
# You also could forgo ctest and call ./Tests directly from the build dir
enable_testing()

# "GLOBS ARE BAD" is brittle and silly dev UX, sorry CMake!
# CONFIGURE_DEPENDS / Clion's CMake integration makes globbing absolutely fine
file(GLOB_RECURSE TestFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Tests PREFIX "" FILES ${TestFiles})

# Use Catch2 v3 on the devel branch
Include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    GIT_TAG v3.3.2)
FetchContent_MakeAvailable(Catch2) # find_package equivalent

# Setup the test executable, again C++ 20 please
add_executable(Tests ${TestFiles})
target_compile_features(Tests PRIVATE cxx_std_20)

# Our test executable also wants to know about our plugin code...
target_include_directories(Tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
        Source/lib/lame/include)

target_link_libraries(Tests
        PRIVATE
        Catch2::Catch2WithMain "${PROJECT_NAME}"
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/lib/lame/libmp3lame/.libs/libmp3lame.a
        )

# We can't link again to the shared juce target without ODL violations
# However, we can steal its include dirs and compile definitions to use in tests!
target_compile_definitions(Tests INTERFACE $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_COMPILE_DEFINITIONS>)
target_include_directories(Tests INTERFACE $<TARGET_PROPERTY:${PROJECT_NAME},INCLUDE_DIRECTORIES>)

# Make an Xcode Scheme for the test executable so we can run tests in the IDE
set_target_properties(Tests PROPERTIES XCODE_GENERATE_SCHEME ON)

# Organize the test source in the Tests/ folder in the IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Tests PREFIX "" FILES ${TestFiles})

# Load and use the .cmake file provided by Catch2
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
# We have to manually provide the source directory here for now
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
catch_discover_tests(Tests)

# Color our warnings and errors
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options (-fcolor-diagnostics)
endif ()

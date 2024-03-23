cmake_minimum_required(VERSION 3.14.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "C:/Program Files/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(NSimpleSaturator
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "NSimpleSaturator VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(NSimpleSaturator
    source/version.h
    source/NSimpleSaturatorcids.h
    source/NSimpleSaturatorprocessor.h
    source/NSimpleSaturatorprocessor.cpp
    source/NSimpleSaturatorcontroller.h
    source/NSimpleSaturatorcontroller.cpp
    source/NSimpleSaturatorentry.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(NSimpleSaturator
        PRIVATE
            resource/NSimpleSaturatoreditor.uidesc
    )
    target_link_libraries(NSimpleSaturator
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(NSimpleSaturator
        RESOURCES
            "resource/NSimpleSaturatoreditor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (NSimpleSaturator
    RESOURCES
        resource/9E66FA67818C5E25AD9CFCA085E38588_snapshot.png
        resource/9E66FA67818C5E25AD9CFCA085E38588_snapshot_2.0x.png
)

target_link_libraries(NSimpleSaturator
    PRIVATE
        sdk
)

smtg_target_configure_version_file(NSimpleSaturator)

if(SMTG_MAC)
    smtg_target_set_bundle(NSimpleSaturator
        BUNDLE_IDENTIFIER com.noahdominic.NSimpleSaturator
        COMPANY_NAME "Noah Dominic Miranda Silvio"
    )
    smtg_target_set_debug_executable(NSimpleSaturator
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(NSimpleSaturator PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT NSimpleSaturator)

        smtg_target_set_debug_executable(NSimpleSaturator
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)

cmake_minimum_required(VERSION 3.5)

project(lab3 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###############################################################################

set(USE_QT TRUE)

set(PROJECT_SOURCES
        main.cpp
        config.h
        config.yml
        application.h application.cpp
        guis/gui_bridge.h
        guis/gui_impl.h
)

if(USE_QT)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

    list(APPEND PROJECT_SOURCES
        guis/qt6_impl.h
        guis/qt6_ui_impl/mainwindow.cpp
        guis/qt6_ui_impl/mainwindow.h
        guis/qt6_ui_impl/mainwindow.ui
        )

###############################################################################

else()
    set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/guis/imgui_opengl3_glfw_impl)
    set(DEPS_DIR ${SOURCE_DIR}/deps)

    set(LIBS_DIR /nix/var/nix/profiles/per-user/human/profile/lib)
    set(INCS_DIR /nix/var/nix/profiles/per-user/human/profile/include)

    list(APPEND PROJECT_SOURCES
        guis/imgui_opengl3_glfw_impl.h
        ${SOURCE_DIR}/includes.h
        )

    include_directories(
        ${INCS_DIR}
        ${DEPS_DIR}/GLFW
        ${DEPS_DIR}/OpenGL/glad
        ${DEPS_DIR}/ImGui
        )

    link_directories(
        ${LIBS_DIR}
        ${DEPS_DIR}/GLFW/lib
        )

    list(APPEND OPENGL_LIBRARIES "-ldl -lGL -lX11 -lpthread -lXrandr -lXi -lglfw3")

    # ADD OpenGL LIBRARY: glad
    add_library(glad STATIC
        ${DEPS_DIR}/OpenGL/glad/glad.c
        )

    # ADD LIBRARY: imgui
    add_library(imgui STATIC
        ${DEPS_DIR}/ImGui/imgui.cpp
        ${DEPS_DIR}/ImGui/imgui_demo.cpp
        ${DEPS_DIR}/ImGui/imgui_draw.cpp
        ${DEPS_DIR}/ImGui/imgui_tables.cpp
        ${DEPS_DIR}/ImGui/imgui_widgets.cpp
        ${DEPS_DIR}/ImGui/imgui_impl_glfw.cpp
        ${DEPS_DIR}/ImGui/imgui_impl_opengl3.cpp
        )
endif()

###############################################################################

if(USE_QT)
    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(lab3
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
        )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET lab3 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    else()
        if(ANDROID)
            add_library(lab3 SHARED
                ${PROJECT_SOURCES}
            )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
        else()
            add_executable(lab3
                ${PROJECT_SOURCES}
            )
        endif()
    endif()

    target_link_libraries(lab3 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

    set_target_properties(lab3 PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )

    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(lab3)
    endif()
else()
    add_executable(lab3
        ${PROJECT_SOURCES}
    )

    target_link_libraries(lab3 PRIVATE ${OPENGL_LIBRARIES} glad imgui)
endif()

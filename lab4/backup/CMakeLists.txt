cmake_minimum_required(VERSION 3.5)

project(lab4 VERSION 0.1 LANGUAGES CXX)

set(EXE main)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

set(COMMON_SRC
    writer.h
    doors.h
    cabin.h
    controller.h
    )

set(CLIENT_SRC
    client.cpp
    )

set(SERVER_SRC
    server.cpp
    )

set(INCLUDE_DIRS
    /nix/var/nix/profiles/per-user/human/profile/include
    /home/human/Code/boost_1_82_0
    )

set(LINK_DIRS
    /nix/var/nix/profiles/per-user/human/profile/lib
    )

include_directories(${INCLUDE_DIRS})
link_directories(${LINK_DIRS})

add_executable(server ${SERVER_SRC} ${COMMON_SRC})
target_link_libraries(server boost_system boost_thread)

add_executable(client ${CLIENT_SRC} ${COMMON_SRC})
target_link_libraries(client boost_system boost_thread)

# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt_add_executable(lab3
#         MANUAL_FINALIZATION
#         ${PROJECT_SOURCES}
#     )
# target_link_libraries(lab3 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(lab3)
# endif()

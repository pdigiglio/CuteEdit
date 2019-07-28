# Version 3.1.0 is the minimum required to use Qt5
cmake_minimum_required(VERSION 3.1.0)

project ("CuteEdit" LANGUAGES CXX)

#set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_AUTOUIC_SEARCH_PATHS}
    ${CMAKE_CURRENT_SOURCE_DIR}/forms)

message("CMake autouic search paths: ${CMAKE_AUTOUIC_SEARCH_PATHS}")

set(INCLUDE_DIRECTORIES
    ${INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}/forms)

#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

#if (CMAKE_VERSION VERSION_LESS "3.7.0")
#    set(CMAKE_INCLUDE_CURRENT_DIR ON)
#endif()

find_package(Qt5
    COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED)

#set(UIC_SOURCES "")
#add_subdirectory(forms)
#message("uic sources: ${UIC_SOURCES}")

add_subdirectory(forms)
add_subdirectory(include)
add_subdirectory(src)

set(SOURCES
    ${SOURCES}
    main.cpp)

message("Sources:     ${SOURCES}")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} moc)

message("Include directories: ${INCLUDE_DIRECTORIES}")
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

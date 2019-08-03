cmake_minimum_required(VERSION 3.10.0)

project ("CuteEdit" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

#set(CMAKE_AUTORCC ON)

find_package(Qt5
    COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED)

add_subdirectory(forms)
message("CMake autouic search paths: ${CMAKE_AUTOUIC_SEARCH_PATHS}")

add_subdirectory(include)
add_subdirectory(src)

set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

message("Sources:")
foreach(SOURCE ${SOURCES})
    message("  ${SOURCE}")
endforeach()

target_link_libraries(${PROJECT_NAME} moc)

message("Include directories:")
foreach(DIRECTORY ${INCLUDE_DIRECTORIES})
    message("  ${DIRECTORY}")
endforeach()
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

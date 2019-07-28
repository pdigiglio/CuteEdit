set(INCLUDE_DIRECTORIES
    ${INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    PARENT_SCOPE)

qt5_wrap_cpp(MOC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow.h)

add_library(moc SHARED ${MOC_SOURCES})
target_link_libraries(moc Qt5::Widgets)

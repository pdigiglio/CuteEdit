set(INCLUDE_DIRECTORIES
    ${INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    PARENT_SCOPE)

qt5_wrap_cpp(MOC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow.h)
qt5_wrap_cpp(MOC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/TemplateHandler.h)

message("moc sources:")
foreach(SOURCE ${MOC_SOURCES})
    message("  ${SOURCE}")
endforeach()

add_library(moc SHARED ${MOC_SOURCES})
target_link_libraries(moc Qt5::Widgets)

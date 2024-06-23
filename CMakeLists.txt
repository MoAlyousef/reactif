cmake_minimum_required(VERSION 3.20)
set(REACTIF_PROJECT_VERSION 0.1.0)
project(reactif
    VERSION ${REACTIF_PROJECT_VERSION}
    DESCRIPTION "A reactive layer on top of FLTK"
    LANGUAGES CXX
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(REACTIF_TOPLEVEL_PROJECT TRUE)
else()
    set(REACTIF_TOPLEVEL_PROJECT FALSE)
endif()

find_package(FLTK CONFIG REQUIRED)

set(REACTIF_HEADER_FILES
    include/reactif/box.hpp
    include/reactif/browser.hpp
    include/reactif/button.hpp
    include/reactif/enums.hpp
    include/reactif/group.hpp
    include/reactif/input.hpp
    include/reactif/menu.hpp
    include/reactif/output.hpp
    include/reactif/reactif.hpp
    include/reactif/tree.hpp
    include/reactif/valuator.hpp
    include/reactif/widget.hpp
    include/reactif/widgets.hpp 
)
add_library(reactif INTERFACE)
target_include_directories(reactif INTERFACE 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)
target_compile_features(reactif INTERFACE cxx_std_20)
target_link_libraries(reactif INTERFACE fltk::fltk)
set_target_properties(reactif PROPERTIES VERSION ${REACTIF_PROJECT_VERSION} PUBLIC_HEADER "${REACTIF_HEADER_FILES}")
add_library(reactif::reactif ALIAS reactif)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/reactifConfigVersion.cmake
  VERSION ${REACTIF_PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
    TARGETS reactif
    EXPORT reactifConfig
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/reactif
)

export(
    TARGETS reactif
    NAMESPACE reactif::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/reactifConfig.cmake"
)

install(
  EXPORT reactifConfig
  DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/reactif"
  NAMESPACE reactif::
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/reactifConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cmake/reactif
)

option(REACTIF_BUILD_EXAMPLES "Build examples" ${REACTIF_TOPLEVEL_PROJECT})

if (REACTIF_BUILD_EXAMPLES)
    # set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    add_executable(counter examples/counter.cpp)
    target_link_libraries(counter PRIVATE reactif::reactif)
    target_compile_options(counter PRIVATE -g)
    add_executable(flutter-like examples/flutter-like.cpp)
    target_link_libraries(flutter-like PRIVATE reactif::reactif)
    target_compile_options(flutter-like PRIVATE -g)
    add_executable(greeter examples/greeter.cpp)
    target_link_libraries(greeter PRIVATE reactif::reactif)
    target_compile_options(greeter PRIVATE -g)
    add_executable(todo examples/todo.cpp)
    target_link_libraries(todo PRIVATE reactif::reactif)
    target_compile_options(todo PRIVATE -g)
    add_executable(custom_widgets examples/custom_widgets.cpp)
    target_link_libraries(custom_widgets PRIVATE reactif::reactif)
    target_compile_options(custom_widgets PRIVATE -g)
endif()
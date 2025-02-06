cmake_minimum_required(VERSION 3.5.0)
project(numerical-algorithms VERSION 0.1.0 LANGUAGES CXX)

add_library(numerical "src/numerical.cpp")

set_target_properties(numerical PROPERTIES
    CMAKE_CXX_STANDARD 20
    CMAKE_CXX_STANDARD_REQUIRED ON
    CMAKE_CXX_EXTENSIONS OFF
)

include(GNUInstallDirs)

target_include_directories(numerical
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

install(TARGETS numerical
    EXPORT numerical-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT numerical-targets
    FILE numerical-targets.cmake
    NAMESPACE numerical::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/numerical
)

install(FILES "src/numerical.hpp" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/numerical)

install(EXPORT numerical-targets
    FILE numerical-targets.cmake
    NAMESPACE numerical::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/numerical
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/numerical-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/numerical
)

install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/numerical-config.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/numerical
)

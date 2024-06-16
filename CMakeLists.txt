cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(floppy
  VERSION 1.1.3
  DESCRIPTION "Library that augments and extends C++ standard library"
  HOMEPAGE_URL "whs31.github.io/floppy"
  LANGUAGES C CXX
)
set(PROJECT_NAMESPACE ${PROJECT_NAME}::)

include(CMakePrintHelpers)

option(TESTS "Enable integration tests" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(NO_SUBMODULES "Build without submodules, relying on system or conan packages" OFF)

if("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
  set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# dependencies
if(NO_SUBMODULES)
  message(STATUS "[${PROJECT_NAME}] building without submodules, relying on system or conan packages")
  find_package(fmt REQUIRED)
  find_package(spdlog REQUIRED)
  if(WIN32)
    find_package(winapi20 REQUIRED)
  endif()
else()
  message(STATUS "[${PROJECT_NAME}] building with git submodules")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/3rd-party/fmt)
  if(TARGET spdlog::spdlog)
    message(STATUS "[${PROJECT_NAME}] linked with spdlog")
  else()
    message(FATAL_ERROR "[${PROJECT_NAME}] failed to link with spdlog. don't forget to link with spdlog::spdlog in parent project!")
  endif()
  if(WIN32)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/winapi20)
  endif()
endif()

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAMESPACE}${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
)

cmake_print_properties(TARGETS ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD
  CXX_STANDARD_REQUIRED
  CXX_EXTENSIONS
  POSITION_INDEPENDENT_CODE
)

target_sources(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/floppy.h> $<INSTALL_INTERFACE:include/floppy/floppy.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/traits.h> $<INSTALL_INTERFACE:include/floppy/traits.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/directories.h> $<INSTALL_INTERFACE:include/floppy/directories.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/serialization.h> $<INSTALL_INTERFACE:include/floppy/serialization.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/configuration.h> $<INSTALL_INTERFACE:include/floppy/configuration.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/logging.h> $<INSTALL_INTERFACE:include/floppy/logging.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/inl/configuration.inl> $<INSTALL_INTERFACE:include/floppy/inl/configuration.inl>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/detail/export.h> $<INSTALL_INTERFACE:include/floppy/detail/export.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/detail/predefs.h> $<INSTALL_INTERFACE:include/floppy/detail/predefs.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/detail/types.h> $<INSTALL_INTERFACE:include/floppy/detail/types.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/detail/type_traits.h> $<INSTALL_INTERFACE:include/floppy/detail/type_traits.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/detail/formatters.h> $<INSTALL_INTERFACE:include/floppy/detail/formatters.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/detail/concepts.h> $<INSTALL_INTERFACE:include/floppy/detail/concepts.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/detail/platform.h> $<INSTALL_INTERFACE:include/floppy/detail/platform.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/detail/backports.h> $<INSTALL_INTERFACE:include/floppy/detail/backports.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/detail/print_helpers.h> $<INSTALL_INTERFACE:include/floppy/detail/print_helpers.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/detail/corefn.h> $<INSTALL_INTERFACE:include/floppy/detail/corefn.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy/detail/math.h> $<INSTALL_INTERFACE:include/floppy/detail/math.h>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/detail/rtti.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/directories.cc
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    fmt::fmt
    spdlog::spdlog
  PRIVATE
    $<$<BOOL:${WIN32}>:winapi::winapi20>
)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>$<INSTALL_INTERFACE:include> PRIVATE src/c++)

if(BUILD_SHARED_LIBS)
  message(STATUS "[${PROJECT_NAME}] adding shared export symbols: FLOPPY_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DFLOPPY_LIBRARY=1)
else()
  message(STATUS "[${PROJECT_NAME}] adding static export symbols: FLOPPY_STATIC_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DFLOPPY_STATIC_LIBRARY=1)
endif()

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    -DCMAKE_TARGET_NAME=${PROJECT_NAME}
    -DCMAKE_PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DCMAKE_PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DCMAKE_PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  message(STATUS "[${PROJECT_NAME}] adding compiler flags: -Wall -Wextra -Wpedantic -Werror")
  target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
  )
endif()

if(TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

message(STATUS "[${PROJECT_NAME}] compile integration tests status: ${INTEGRATION_TESTS}")
message(STATUS "[${PROJECT_NAME}] installing ${PROJECT_NAME} in namespace ${PROJECT_NAMESPACE}")
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
message(STATUS "[${PROJECT_NAME}] installing in prefix: ${CMAKE_INSTALL_PREFIX}")

set(PROJECT_CONFIG_FILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in")
set(PROJECT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
set(CONFIG_TARGETS_FILE "${PROJECT_NAME}-config-targets.cmake")
set(CONFIG_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")
set(INSTALL_EXPORT_DESTINATION_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/floppy> $<INSTALL_INTERFACE:include/floppy> ; DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
export(TARGETS ${PROJECT_NAME} NAMESPACE ${PROJECT_NAMESPACE} FILE "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TARGETS_FILE}")
install(EXPORT ${PROJECT_NAME}
  DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR}
  NAMESPACE ${PROJECT_NAMESPACE}
  FILE ${CONFIG_TARGETS_FILE}
)
configure_package_config_file(${PROJECT_CONFIG_FILE_IN} ${PROJECT_CONFIG_FILE} INSTALL_DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR})
write_basic_package_version_file(${CONFIG_VERSION_FILE} COMPATIBILITY AnyNewerVersion)
install(FILES ${PROJECT_CONFIG_FILE} ${CONFIG_VERSION_FILE} DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR})

message(STATUS "[${PROJECT_NAME}] configuring ${PROJECT_NAME} done!")
unset(TESTS CACHE)

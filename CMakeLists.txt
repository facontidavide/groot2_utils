cmake_minimum_required(VERSION 3.15)

project(groot2 VERSION 1.5.2)

enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} )
list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} )

#---- library ----

include_directories(include 3rdparty)

add_library(tinyxml_static STATIC 3rdparty/tinyxml2/tinyxml2.cpp)

add_executable(groot2_formatter
    src/groot2_formatter.cpp
    src/xml_formatter.cpp)

target_link_libraries(groot2_formatter tinyxml_static)

#---- install ----

install(TARGETS groot2_formatter EXPORT groot2_formatterTargets)

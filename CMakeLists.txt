cmake_minimum_required(VERSION 3.10)
project(ADIF2XML
    VERSION     1.0.1
    DESCRIPTION "COnvert ADIF logfiles to XML")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (WIN32)
    set(CMAKE_INSTALL_PREFIX "C:/local/${CMAKE_BUILD_TYPE}/")
else()
    set(CMAKE_INSTALL_PREFIX "/usr/local/${CMAKE_BUILD_TYPE}/")
endif()

include_directories(BEFORE "${CMAKE_INSTALL_PREFIX}/include/")
link_directories(BEFORE "${CMAKE_INSTALL_PREFIX}/lib/")

set(ADIF2XML_SOURCES
    adifio.cpp
    adif2xml.cpp
    attributeimpl.cpp
    attributelistimpl.cpp
    ediio.cpp
    getopt.c
    SAXPrintHandlers.cpp
)

set(ADIF2XML_HEADERS
    adifio.hpp
    adif2xml.hpp
    attributeimpl.hpp
    attributelistimpl.hpp
    date/date.h
    ediio.hpp
    getopt.h
    SAXPrint.hpp
    SAXPrintHandlers.hpp
)

add_executable(ADIF2XML ${ADIF2XML_SOURCES} ${ADIF2XML_HEADERS})

configure_file(adif2xml_version.hpp.in adif2xml_version.hpp)

target_compile_features(ADIF2XML
    PUBLIC cxx_std_20)
target_include_directories(ADIF2XML PUBLIC
    BEFORE "${CMAKE_INSTALL_PREFIX}/include/"
    BEFORE "${PROJECT_BINARY_DIR}")
target_link_directories(ADIF2XML PUBLIC
    BEFORE "${CMAKE_INSTALL_PREFIX}/lib/")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DFLAG "D")
else()
    set(DFLAG "")
endif()

if (WIN32)
    target_link_libraries(ADIF2XML PRIVATE
        xerces-c_3${DFLAG}
    )
    add_custom_command(TARGET ADIF2XML POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_INSTALL_PREFIX}/bin/xerces-c_3_2${DFLAG}.dll"
            $<TARGET_FILE_DIR:TS-WordCount>)
else()
    target_link_libraries(ADIF2XML PRIVATE
        xerces-c
        m
        stdc++)
endif()

install(TARGETS ADIF2XML
    RUNTIME DESTINATION "bin")

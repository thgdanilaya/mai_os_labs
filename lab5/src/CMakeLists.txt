cmake_minimum_required(VERSION 3.16.3)

project(laba5os)
include_directories(include)
set(LIB_PRINT_TYPE EXTENDED)

if (NOT (DEFINED LIB_PRINT_TYPE))
    set(LIB_PRINT_TYPE DEFAULT)
endif ()

if (LIB_PRINT_TYPE STREQUAL EXTENDED)
    add_compile_definitions(SYSTEM="${CMAKE_SYSTEM}")
elseif (NOT (LIB_PRINT_TYPE STREQUAL DEFAULT))
    message(FATAL_ERROR "Unexpected LIB_PRINT_TYPE: ${LIB_PRINT_TYPE}")
endif ()


add_library(dynamic1 SHARED src/realisation1.cpp)
add_library(dynamic2 SHARED src/realisation2.cpp)

add_executable(main11.out src/main1.cpp include/lib.h)
add_executable(main12.out src/main1.cpp include/lib.h)
target_link_libraries(main11.out dynamic1)
target_link_libraries(main12.out dynamic2)

add_executable(main2.out src/main2.cpp)
target_link_libraries(main2.out ${CMAKE_DL_LIBS})

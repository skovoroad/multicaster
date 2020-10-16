cmake_minimum_required (VERSION 3.0)

project (multicaster_test)

set (CMAKE_CXX_STANDARD 17)
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../include ${PROJECT_SOURCE_DIR})

add_executable(${PROJECT_NAME}
  main.cpp
  test_multicaster.cpp
)

add_dependencies(${PROJECT_NAME} gtest)

if (WIN32)
  set(CMAKE_CXX_FLAGS "/D_HAS_TR1_NAMESPACE /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING" CACHE INTERNAL "")
  target_link_libraries(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/Debug/gtest.lib multicaster)
else()
  target_link_libraries(${PROJECT_NAME} gtest multicaster -pthread)
endif()

add_test(NAME    ${PROJECT_NAME}
         COMMAND ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
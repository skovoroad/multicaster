cmake_minimum_required(VERSION 3.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/include/multicaster.h")
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
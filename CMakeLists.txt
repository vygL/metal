cmake_minimum_required(VERSION 3.5)
project(metal)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(metal src/main.cpp)
target_compile_features(metal PRIVATE cxx_std_20)

target_include_directories(metal PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_include_directories(metal SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/package/metal-cpp)

find_library(APPLE_FWK_FOUNDATION Foundation REQUIRED)
find_library(APPLE_FWK_QUARTZ_CORE QuartzCore REQUIRED)
find_library(APPLE_FWK_METAL Metal REQUIRED)

target_link_libraries(metal
  ${APPLE_FWK_FOUNDATION}
  ${APPLE_FWK_QUARTZ_CORE}
  ${APPLE_FWK_METAL}
)
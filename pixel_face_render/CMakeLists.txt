cmake_minimum_required(VERSION 3.8)
project(pixel_face_render)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

add_executable(hello_sfml test/test_sfml.cpp)
target_link_libraries(hello_sfml
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

install(
    TARGETS
        hello_sfml
    RUNTIME DESTINATION bin/${PROJECT_NAME} 
)
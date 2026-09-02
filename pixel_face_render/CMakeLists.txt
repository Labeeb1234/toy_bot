cmake_minimum_required(VERSION 3.8)
project(pixel_face_render)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

# change the instructions structure below later using INTERFACE lib linking if required
add_executable(hello_sfml test/test_sfml.cpp)
target_link_libraries(hello_sfml
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

add_executable(test_face test/test_face.cpp)
target_link_libraries(test_face
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

add_executable(test_face_anim test/test_face_anim.cpp)
target_link_libraries(test_face_anim
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)


install(
    TARGETS
        hello_sfml
        test_face
        test_face_anim
    RUNTIME DESTINATION bin/${PROJECT_NAME} 
)
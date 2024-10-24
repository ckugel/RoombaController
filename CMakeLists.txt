cmake_minimum_required(VERSION 3.28)
project(RoombaControllers)

set(CMAKE_CXX_STANDARD 20)

# Add ImGui files
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/include/imgui)
file(GLOB IMGUI_SOURCES
        ${IMGUI_DIR}/*.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Include GLFW and OpenGL (assuming you're using these)
find_package(OpenGL REQUIRED)
# find_package(glfw3 REQUIRED)

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(RoombaControllers src/main.cpp ${IMGUI_SOURCES})

# add_executable(RoombaControllers main.cpp ${IMGUI_SOURCES})
target_include_directories(RoombaControllers PRIVATE ${IMGUI_DIR} ${IMGUI_DIR}/backends)

target_link_libraries(RoombaControllers ${GLFW_LIBRARIES}
        ${OPENGL_gl_LIBRARY} glfw OpenGL::GL)


set(CMAKE_CXX_STANDARD_REQUIRED ON)

# pkg_check_modules(GLFW REQUIRED glfw3)


#target_link_libraries(RoombaControllers PRIVATE ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} GLFW glfw  # Make sure it's installed or included in your devShell)

# Add any required compile definitions
target_compile_definitions(RoombaControllers PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLEW)

# add_executable(RoombaControllers main.cpp)



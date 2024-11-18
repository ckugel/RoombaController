cmake_minimum_required(VERSION 3.28)
project(RoombaControllers)

set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CROSSCOMPILING true)
# set(CMAKE_SYSTEM_NAME Windows)
# set(CMAKE_SYSTEM_VERSION 10.0)
# set(CMAKE_SYSTEM_PROCESSOR x86)

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

add_executable(RoombaControllers ${IMGUI_SOURCES}
	src/Pose2D.cpp src/Pillar.cpp src/Graph.cpp src/main.cpp src/Field.cpp src/HoleManager.cpp src/Hole.cpp)

# add_executable(RoombaControllers main.cpp ${IMGUI_SOURCES})
target_include_directories(RoombaControllers PRIVATE ${IMGUI_DIR} ${IMGUI_DIR}/backends)

target_link_libraries(RoombaControllers ${GLFW_LIBRARIES}
        ${OPENGL_gl_LIBRARY} glfw OpenGL::GL)


set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# pkg_check_modules(GLFW REQUIRED glfw3)


#target_link_libraries(RoombaControllers PRIVATE ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} GLFW glfw  # Make sure it's installed or included in your devShell)

# Add any required compile definitions
target_compile_definitions(RoombaControllers PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLEW)

# add_executable(RoombaControllers main.cpp)



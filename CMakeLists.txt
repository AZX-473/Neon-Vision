cmake_minimum_required(VERSION 3.10)
project(NeonVision)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options("/utf-8")
    # Disable MSVC secure CRT warnings like deprecated localtime
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    # Reduce Windows.h macro pollution and avoid min/max macro conflicts
    add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)

# 查找依赖（不需要 COMPONENTS）
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# 打印路径用于调试
message(STATUS "imgui found at: ${imgui_DIR}")
get_target_property(IMGUI_INCLUDE_DIR imgui::imgui INTERFACE_INCLUDE_DIRECTORIES)

if(WIN32)
    set(PLATFORM_LIBS opengl32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
elseif(UNIX)
    set(PLATFORM_LIBS GL)
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IGM
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UI/Component
    ${IMGUI_INCLUDE_DIR}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    GLFW_EXPOSE_NATIVE_WIN32
)

# 【关键修复】只链接 imgui::imgui，不链接不存在的组件
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    imgui::imgui
    glfw
    ${OPENGL_LIBRARIES}
    ${PLATFORM_LIBS}
)

# Windows 隐藏控制台
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
endif()

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
cmake_minimum_required(VERSION 3.16)
project(NeonVision VERSION 1.0.0 LANGUAGES CXX)

# ============================================================
# 1. C++ 标准
# ============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# 2. 全局编译选项
# ============================================================
if(MSVC)
    add_compile_options("/utf-8")
    add_compile_definitions(
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
elseif(UNIX)
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ============================================================
# 3. 查找依赖（通过 vcpkg 工具链提供路径）
# ============================================================
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# WinSparkle 没有提供 CMake 配置文件，需要用 FindWinSparkle.cmake
# 或者使用 find_path/find_library 配合 vcpkg 的变量
find_path(WINSPARKLE_INCLUDE_DIR
    NAMES winsparkle.h
    PATH_SUFFIXES winsparkle
)

find_library(WINSPARKLE_LIBRARY
    NAMES WinSparkle
)

find_file(WINSPARKLE_DLL
    NAMES WinSparkle.dll
    PATH_SUFFIXES bin
)

# ============================================================
# 4. 收集源文件
# ============================================================
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# ============================================================
# 5. 创建可执行文件
# ============================================================
if(WIN32)
    list(APPEND SOURCES "resources/app.rc")
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

# ============================================================
# 6. 包含目录
# ============================================================
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    $<TARGET_PROPERTY:imgui::imgui,INTERFACE_INCLUDE_DIRECTORIES>
)

if(WINSPARKLE_INCLUDE_DIR)
    target_include_directories(${PROJECT_NAME} PRIVATE ${WINSPARKLE_INCLUDE_DIR})
endif()

# ============================================================
# 7. 编译定义
# ============================================================
target_compile_definitions(${PROJECT_NAME} PRIVATE
    GLFW_EXPOSE_NATIVE_WIN32
)

# ============================================================
# 8. 链接库
# ============================================================
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    imgui::imgui
    glfw
    OpenGL::GL
)

if(WINSPARKLE_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${WINSPARKLE_LIBRARY})
endif()

# ============================================================
# 9. Windows 子系统
# ============================================================
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
        LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
endif()

# ============================================================
# 10. 复制 WinSparkle.dll
# ============================================================
if(WIN32 AND WINSPARKLE_DLL)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${WINSPARKLE_DLL}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying WinSparkle.dll to output directory"
    )
endif()

# ============================================================
# 11. 最终信息
# ============================================================
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
if(WINSPARKLE_INCLUDE_DIR)
    message(STATUS "WinSparkle: ${WINSPARKLE_INCLUDE_DIR}")
endif()
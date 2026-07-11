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
# 3. 查找依赖（通过 vcpkg 工具链提供）
# ============================================================
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# ============================================================
# 4. 收集源文件
# ============================================================
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# ============================================================
# 5. 创建可执行文件
# ============================================================
if(WIN32)
    # 添加资源文件到源列表
    list(APPEND SOURCES "resources/app.rc")
    # 或者显式声明，让 VS 知道这是一个资源文件
    set(RESOURCE_FILES "resources/app.rc")
else()
    set(RESOURCE_FILES "")
endif()

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

# ============================================================
# 6. 包含目录
# ============================================================
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 从 imgui target 自动获取包含路径
target_include_directories(${PROJECT_NAME} PRIVATE
    $<TARGET_PROPERTY:imgui::imgui,INTERFACE_INCLUDE_DIRECTORIES>
)

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
# 10. 最终信息
# ============================================================
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
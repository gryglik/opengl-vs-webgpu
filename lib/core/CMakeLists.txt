add_library(core STATIC)

# -- OPTIONS -- #
option(USE_WAYLAND "Use Wayland backed" OFF)
option(USE_X11 "Use X11 backend" ON)

# -- LINKING LIBRARIES -- #
target_sources(core PRIVATE
  src/Window.cpp
  opengl/src/OpenGLRenderer.cpp
  webgpu/src/utils_metal.mm
  webgpu/src/WebGPURenderer.cpp
)

target_include_directories(core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/opengl/include
  ${CMAKE_CURRENT_SOURCE_DIR}/webgpu/include
)

if (APPLE)
  target_compile_definitions(core PRIVATE PLATFORM_MACOS DAWN_ENABLE_BACKEND_METAL)
  target_link_libraries(core PRIVATE
    "-framework IOSurface"
    "-framework CoreVideo"
    "-framework CoreFoundation"
    "-framework Metal"
    "-framework QuartzCore"
  )

elseif(WIN32)
  target_compile_definitions(core PRIVATE PLATFORM_WINDOWS DAWN_EXPOSE_NATIVE_WIN32)

elseif(UNIX)
  target_compile_definitions(core PRIVATE PLATFORM_UNIX)

  if(USE_WAYLAND)
    target_compile_definitions(core PRIVATE PLATFORM_WAYLAND DAWN_EXPOSE_NATIVE_WAYLAND)

    target_link_libraries(core PRIVATE wayland-client)

  elseif(USE_X11)
    target_compile_definitions(core PRIVATE PLATFORM_X11 DAWN_EXPOSE_NATIVE_X11)

    find_package(X11 REQUIRED)
    target_link_libraries(core PRIVATE X11)
  endif()
endif()

target_link_libraries(core PUBLIC
  glm::glm
  glad_core
  glfw
  dawn
)

# -- FETCHING CONTENT -- #
# -- GLFW -- #
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# -- GLM -- #
FetchContent_Declare(
 glm
 GIT_REPOSITORY	https://github.com/g-truc/glm.git
 GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

# -- GLAD -- #
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad
    GIT_TAG        v2.0.8
    GIT_PROGRESS   TRUE
    SOURCE_SUBDIR cmake
)
FetchContent_MakeAvailable(glad)
glad_add_library(glad_core STATIC REPRODUCIBLE LOADER API gl:core=4.6)

# -- webgpu-dawn (pre-built) -- #
if (APPLE)
  set (DAWN_URL https://github.com/google/dawn/releases/download/v20260423.175430/Dawn-31e25af254ab572c77054edec4946d2244e184dd-macos-latest-Release.tar.gz)
elseif (WIN32)
  set (DAWN_URL https://github.com/google/dawn/releases/download/v20260423.175430/Dawn-31e25af254ab572c77054edec4946d2244e184dd-windows-latest-Release.tar.gz)
elseif (UNIX)
  set (DAWN_URL https://github.com/google/dawn/releases/download/v20260423.175430/Dawn-31e25af254ab572c77054edec4946d2244e184dd-ubuntu-latest-Release.tar.gz)
else ()
  message(FATAL_ERROR "OS not supported.")
endif()

FetchContent_Declare(
  dawn_prebuilt
  URL ${DAWN_URL}
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(dawn_prebuilt)

set(DAWN_DIR ${dawn_prebuilt_SOURCE_DIR})
add_library(dawn STATIC IMPORTED)
set_target_properties(dawn PROPERTIES
  IMPORTED_LOCATION ${DAWN_DIR}/lib/libwebgpu_dawn.a
  INTERFACE_INCLUDE_DIRECTORIES ${DAWN_DIR}/include
)


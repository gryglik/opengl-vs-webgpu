add_library(core STATIC)

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

target_sources(core PRIVATE src/Window.cpp opengl/src/OpenGLRenderer.cpp)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/opengl/include)
target_link_libraries(core PUBLIC glm::glm glad_core glfw)

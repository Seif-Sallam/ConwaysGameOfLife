cmake_minimum_required(VERSION 3.16)

FILE(GLOB SOURCES ./src/*.cpp)

FILE(GLOB HEADERS ./headers/*.h)

add_executable(
    ConwayGameOfLife
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(
    ConwayGameOfLife
    PRIVATE
    ./headers/
)

set(OS_LIBRARIES
    opengl32
)

if (UNIX)
    set(OS_LIBRARIES
        GL
    )
endif()

target_link_libraries(ConwayGameOfLife
    PRIVATE
    ${OS_LIBRARIES}
    imgui
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

target_compile_definitions(ConwayGameOfLife
    PRIVATE
    RSC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/rsc/"
)
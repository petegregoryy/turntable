cmake_minimum_required(VERSION 3.16)
project(Turntable LANGUAGES CXX)

# ---- Build options ---------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Find raylib -----------------------------------------------------------
# Works with both system‑installed and local builds. Accept any version.
find_package(raylib CONFIG REQUIRED)

# Determine the exported target name (some builds export "raylib::raylib", others just "raylib")
if(TARGET raylib::raylib)
    set(RAYLIB_TARGET raylib::raylib)
elseif(TARGET raylib)
    set(RAYLIB_TARGET raylib)
else()
    message(FATAL_ERROR "raylib target not found; ensure raylibConfig.cmake is discoverable")
endif()

# ---- Executable ------------------------------------------------------------
set(SRC
    src/main.cpp
    src/world/world.cpp
    src/render/models.cpp
    src/render/draw.cpp
    src/camera/camera.cpp
)

add_executable(turntable ${SRC})
target_include_directories(turntable PRIVATE
    src
    src/world
    src/render
    src/camera
)


# ---- Link libs -------------------------------------------------------------
# On Linux raylib::raylib already pulls dl, pthread, m, GL … but add them
# explicitly for portability.
# ---- Link libs -------------------------------------------------------------
if (UNIX AND NOT APPLE)
    target_link_libraries(turntable PRIVATE ${RAYLIB_TARGET} dl m pthread)
elseif (APPLE)
    target_link_libraries(turntable PRIVATE ${RAYLIB_TARGET})
elseif (WIN32)
    target_link_libraries(turntable PRIVATE ${RAYLIB_TARGET} winmm shell32 gdi32)
endif()

# ---- Include dirs ----------------------------------------------------------
# (raylib::raylib already sets include path)

# ---- Install ---------------------------------------------------------------
install(TARGETS turntable RUNTIME DESTINATION bin)

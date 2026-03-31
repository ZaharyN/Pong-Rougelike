cmake_minimum_required(VERSION 3.20)
project(PongGame VERSION 1.0.0 LANGUAGES CXX)

# ─────────────────────────────────────────────
# 1. C++ Standard
# ─────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─────────────────────────────────────────────
# 2. Fetch SFML
# ─────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(SFML)

# ─────────────────────────────────────────────
# 3. Source Files
# ─────────────────────────────────────────────
set(MY_SOURCES
    "src/main.cpp"
    "src/Paddle.cpp"
    "src/Ball.cpp"
    "src/Player.cpp"
    "src/Enemy.cpp"
    "src/Buddy.cpp"
    "src/GameManager.cpp"
    "src/AudioManager.cpp"
    "src/UIManager.cpp"
    "src/Collectible.cpp"
    "src/CollectibleManager.cpp"
    "src/UpgradeManager.cpp"
    "src/UpgradeCard.cpp"
)

# ─────────────────────────────────────────────
# 4. Header Files (for IDE visibility only)
# ─────────────────────────────────────────────
set(MY_HEADERS
    "include/Paddle.h"
    "include/Ball.h"
    "include/Player.h"
    "include/Enemy.h"
    "include/Buddy.h"
    "include/GameManager.h"
    "include/AudioManager.h"
    "include/UIManager.h"
    "include/Types.h"
    "include/Collectible.h"
    "include/CollectibleManager.h"
    "include/UpgradeManager.h"
    "include/UpgradeCard.h"
)

# ─────────────────────────────────────────────
# 5. Windows Resources (.ico embed)
# ─────────────────────────────────────────────
set(MY_RESOURCES "")
if(WIN32)
    set(MY_RESOURCES "resources/icon.rc")
endif()

# ─────────────────────────────────────────────
# 6. Create Executable
# ─────────────────────────────────────────────
add_executable(PongGame
    ${MY_SOURCES}
    ${MY_HEADERS}
    ${MY_RESOURCES}
)

# ─────────────────────────────────────────────
# 7. Suppress Console Window in Release (Windows)
# ─────────────────────────────────────────────
if(WIN32)
    set_target_properties(PongGame PROPERTIES
        WIN32_EXECUTABLE $<CONFIG:Release>
    )
endif()

# ─────────────────────────────────────────────
# 8. Include Directories
# ─────────────────────────────────────────────
target_include_directories(PongGame PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

# ─────────────────────────────────────────────
# 9. Link SFML
# ─────────────────────────────────────────────
target_link_libraries(PongGame PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::Audio
    SFML::System
)

# ─────────────────────────────────────────────
# 10. Compiler Warnings
# ─────────────────────────────────────────────
if(MSVC)
    target_compile_options(PongGame PRIVATE /W4)
else()
    target_compile_options(PongGame PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ─────────────────────────────────────────────
# 11. Copy Assets to Build Directory
# ─────────────────────────────────────────────
add_custom_command(TARGET PongGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Assets"
        "$<TARGET_FILE_DIR:PongGame>/Assets"
    COMMENT "Copying Assets to build directory..."
)

# ─────────────────────────────────────────────
# 12. Copy Resources to Build Directory
# ─────────────────────────────────────────────
add_custom_command(TARGET PongGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:PongGame>/resources"
    COMMENT "Copying resources to build directory..."
)
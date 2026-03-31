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
set(SOURCES_CORE
    "src/core/GameManager.cpp"
)

set(SOURCES_ENTITIES
    "src/entities/Ball.cpp"
    "src/entities/Paddle.cpp"
    "src/entities/Player.cpp"
    "src/entities/Enemy.cpp"
    "src/entities/Buddy.cpp"
)

set(SOURCES_SYSTEMS
    "src/systems/AudioManager.cpp"
    "src/systems/CollectibleManager.cpp"
    "src/systems/Collectible.cpp"
    "src/systems/UIManager.cpp"
    "src/systems/UpgradeCard.cpp"
    "src/systems/UpgradeManager.cpp"
)

set(SOURCES_MAIN
    "src/main.cpp"
)

# ─────────────────────────────────────────────
# 4. Header Files (for IDE visibility only)
# ─────────────────────────────────────────────
set(HEADERS_CORE
    "include/core/Types.h"
    "include/core/GameManager.h"
)

set(HEADERS_ENTITIES
    "include/entities/Ball.h"
    "include/entities/Paddle.h"
    "include/entities/PaddleStats.h"
    "include/entities/Player.h"
    "include/entities/Enemy.h"
    "include/entities/Buddy.h"
)

set(HEADERS_SYSTEMS
    "include/systems/AudioManager.h"
    "include/systems/CollectibleManager.h"
    "include/systems/Collectible.h"
    "include/systems/UIManager.h"
    "include/systems/UpgradeCard.h"
    "include/systems/UpgradeManager.h"
)

# ─────────────────────────────────────────────
# 5. Windows Resources
# ─────────────────────────────────────────────
set(MY_RESOURCES "")
if(WIN32)
    set(MY_RESOURCES "resources/icon.rc")
endif()

# ─────────────────────────────────────────────
# 6. Create Executable
# ─────────────────────────────────────────────
add_executable(PongGame
    ${SOURCES_MAIN}
    ${SOURCES_CORE}
    ${SOURCES_ENTITIES}
    ${SOURCES_SYSTEMS}
    ${HEADERS_CORE}
    ${HEADERS_ENTITIES}
    ${HEADERS_SYSTEMS}
    ${MY_RESOURCES}
)

# ─────────────────────────────────────────────
# 7. Include Directories
# ─────────────────────────────────────────────
target_include_directories(PongGame PRIVATE
    "${PROJECT_SOURCE_DIR}/src/core"
    "${PROJECT_SOURCE_DIR}/src/entities"
    "${PROJECT_SOURCE_DIR}/src/systems"
    "${PROJECT_SOURCE_DIR}/include/core"
    "${PROJECT_SOURCE_DIR}/include/entities"
    "${PROJECT_SOURCE_DIR}/include/systems"
)

# ─────────────────────────────────────────────
# 8. Suppress Console Window in Release
# ─────────────────────────────────────────────
if(WIN32)
    set_target_properties(PongGame PROPERTIES
        WIN32_EXECUTABLE $<CONFIG:Release>
    )
endif()

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
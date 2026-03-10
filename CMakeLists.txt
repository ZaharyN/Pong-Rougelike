cmake_minimum_required(VERSION 3.20)
project(PongGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(SFML)

add_executable(PongGame src/main.cpp "include/Paddle.h" "src/Paddle.cpp" "include/Ball.h" "src/Ball.cpp" "include/Player.h" "include/Enemy.h"  "src/Player.cpp" "src/Enemy.cpp"  "include/GameManager.h" "include/AudioManager.h" "include/UIManager.h" "src/GameManager.cpp" "src/AudioManager.cpp" "src/UIManager.cpp" "include/Types.h" "include/Collectible.h" "include/CollectibleManager.h" "src/Collectible.cpp" "src/CollectibleManager.cpp")
target_include_directories(PongGame PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(PongGame sfml-graphics sfml-window sfml-audio sfml-system)

# 4. Handle Assets (Copies them to where the EXE is so the game can find them)
if(WIN32)
    add_custom_command(TARGET PongGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Assets" "$<TARGET_FILE_DIR:PongGame>/Assets")
endif()
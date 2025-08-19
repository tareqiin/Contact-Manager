cmake_minimum_required(VERSION 3.28)

set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(Rocket VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Find the packages
find_package(fmt CONFIG REQUIRED)
find_package(ftxui CONFIG REQUIRED)

# Add the executable
add_executable(${PROJECT_NAME} main.cpp)

# Add your C++20 module
target_sources(${PROJECT_NAME}
    PUBLIC
    FILE_SET CXX_MODULES FILES
    utilities.ixx
)

# Include FTXUI headers for module compilation
target_include_directories(${PROJECT_NAME} PRIVATE ${ftxui_INCLUDE_DIRS})

# Link both fmt and ftxui
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt ftxui::dom ftxui::screen ftxui::component)

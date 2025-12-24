cmake_minimum_required(VERSION 3.18)

add_subdirectory(third_party/glaze)
add_subdirectory(third_party/nanobind)

# ---------------------------------------------------------------------------
# json test
# ---------------------------------------------------------------------------

project(test LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}_json src/json.cpp)
target_include_directories(${PROJECT_NAME}_json PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME}_json PRIVATE glaze::glaze)

# ---------------------------------------------------------------------------
# nanobind test
# ---------------------------------------------------------------------------

if (CMAKE_VERSION VERSION_LESS 3.18)
  set(DEV_MODULE Development)
else()
  set(DEV_MODULE Development.Module)
endif()

find_package(Python 3.9 COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)

nanobind_add_module(${PROJECT_NAME}_py src/python.cpp)
target_include_directories(${PROJECT_NAME}_py PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME}_py PRIVATE glaze::glaze)

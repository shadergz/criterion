cmake_minimum_required(VERSION 3.21)
project(Criterion)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE -O3)

add_executable(criterion)
include_directories(src)

add_subdirectory(src)
set(CMAKE_DISABLE_FIND_PACKAGE_MNG ON)
set(CMAKE_DISABLE_FIND_PACKAGE_libSquish ON)
set(CMAKE_DISABLE_FIND_PACKAGE_LCMS2 ON)

add_subdirectory(external/boost)
add_subdirectory(external/DevIL/DevIL)
add_subdirectory(external/glm)
target_compile_options(IL PRIVATE -w)
target_include_directories(criterion PRIVATE external/DevIL/DevIL/include)
target_link_libraries(criterion PRIVATE Boost::program_options Boost::algorithm IL glm::glm)


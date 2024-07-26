cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# Find Boost
set(BOOST_ROOT "C:/Program Files/Boost/boost_1_85_0")
find_package(Boost 1.85 REQUIRED)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(MyProject main.cpp)
    target_link_libraries(MyProject ${Boost_LIBRARIES})
endif()

cmake_minimum_required(VERSION 3.12)

project(SofaMeshCollision_test)

set(SOURCE_FILES
    BaryMapper_test.cpp
	MeshNewProximityIntersection_test.cpp)

sofa_find_package(SofaMeshCollision REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Sofa.Testing SofaTest SofaMeshCollision)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

cmake_minimum_required(VERSION 3.12)
project(SofaFramework)

# Version
set(SOFAFRAMEWORK_VERSION "${Sofa_VERSION}")

# Package install include directory
set(SOFAFRAMEWORK_TARGETS Sofa.Type SofaHelper SofaDefaultType SofaCore SofaSimulationCore)

foreach(TARGET ${SOFAFRAMEWORK_TARGETS})
    add_subdirectory(../modules/${TARGET} ${CMAKE_CURRENT_BINARY_DIR}/${TARGET})
    if(SOFA_BUILD_TESTS)
        target_compile_options(${TARGET} PUBLIC "-DFRAMEWORK_TEST_RESOURCES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/resources/tests\"")
    endif()
endforeach()

if(SOFA_WITH_OPENGL)
    add_subdirectory(../modules/Sofa.GL ${CMAKE_CURRENT_BINARY_DIR}/Sofa.GL)
    list(APPEND SOFAFRAMEWORK_TARGETS Sofa.GL)
endif()

set(SOFAFRAMEWORK_SRC src/SofaFramework)
set(HEADER_FILES
    ${SOFAFRAMEWORK_SRC}/config.h.in
    ${SOFAFRAMEWORK_SRC}/initSofaFramework.h
    )
set(SOURCE_FILES
    ${SOFAFRAMEWORK_SRC}/initSofaFramework.cpp
    )

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SOFAFRAMEWORK_TARGETS})

sofa_create_package_with_targets(
    PACKAGE_NAME ${PROJECT_NAME}
    PACKAGE_VERSION ${Sofa_VERSION}
    TARGETS ${PROJECT_NAME} AUTO_SET_TARGET_PROPERTIES
    INCLUDE_SOURCE_DIR "src"
    INCLUDE_INSTALL_DIR "${PROJECT_NAME}"
    )

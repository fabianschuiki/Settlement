cmake_minimum_required(VERSION 2.6)
project(Settlement)

# Default to debug build.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions("-DBUILD_DEBUG")
endif()


# Executable name.
set(EXECUTABLE_NAME Settlement)


# Find the required libraries
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules" ${CMAKE_MODULE_PATH})

find_package(BoehmGC REQUIRED)
find_package(SFML REQUIRED system window audio network graphics)
find_package(OpenGL REQUIRED)
find_package(Cairomm REQUIRED)

include_directories(${BOEHMGC_INCLUDE_DIRS})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${Cairomm_INCLUDE_DIRS})
include_directories(/opt/X11/include)


# Generate the executable
file(GLOB_RECURSE sources RELATIVE ${PROJECT_SOURCE_DIR} source/*.cpp)


add_executable(
	${EXECUTABLE_NAME}
	${sources}
)


# Link the libraries.
target_link_libraries(${EXECUTABLE_NAME} ${BOEHMGC_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
target_link_libraries(${EXECUTABLE_NAME} ${Cairomm_LIBRARIES})


# Copy the SFML frameworks into the application bundle on Mac OS X.
if (APPLE)
	install(DIRECTORY ${SFML_LIBRARIES} DESTINATION ${CMAKE_BINARY_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks)
endif ()
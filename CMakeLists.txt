# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.0)

# Set the project name
project(assignment)

# Tell IDE to use folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# define some variables to prevent from defining full paths later on
set(ASSIGNMENT_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(ASSIGNMENT_SOURCE_ROOT "${ASSIGNMENT_SOURCE_DIR}/source")
set(ASSIGNMENT_INCLUDE_ROOT "${ASSIGNMENT_SOURCE_DIR}/include")
set(ASSIGNMENT_SHADER_ROOT "${ASSIGNMENT_SOURCE_DIR}/shaders")

# You'll see why we need this in a second (?)
set(ASSIGNMENT_GENERATED_ROOT "${ASSIGNMENT_SOURCE_DIR}/generated")

# Set this one to wherever atlas is installed
set(ASSIGNMENT_ATLAS_ROOT "${ASSIGNMENT_SOURCE_DIR}/lib/atlas")


# For windows
add_definitions(
	-wd4201
	-D_CRT_SECURE_NO_WARNINGS
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

# Add atlas subdirectory
add_subdirectory(${ASSIGNMENT_ATLAS_ROOT})

# Add source folders
add_subdirectory(${ASSIGNMENT_INCLUDE_ROOT})
add_subdirectory(${ASSIGNMENT_SOURCE_ROOT})
add_subdirectory(${ASSIGNMENT_SHADER_ROOT})

# Tell the compiler where to find include files
include_directories(
	${ATLAS_INCLUDE_DIRS}
	${ASSIGNMENT_GENERATED_ROOT}
	${ASSIGNMENT_INCLUDE_ROOT}
)

# Tell the compiler which folders to place files in
source_group("include" FILES ${INCLUDE_LIST})
source_group("source" FILES ${SOURCE_LIST})
source_group("shaders" FILES ${SHADER_LIST})

# Now let's make our executable and link the libraries
message(STATUS "Source list: " ${ASSIGNMENT_SOURCE_ROOT} ${SOURCE_LIST})
message(STATUS "Include list: " ${INCLUDE_LIST})
message(STATUS "Shader list: " ${SHADER_LIST})
add_executable(assignment ${SOURCE_LIST} ${INCLUDE_LIST} ${SHADER_LIST})
target_link_libraries(assignment ${ATLAS_LIBRARIES})

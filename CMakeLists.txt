#
# build everything with cmake
#

# min version for cmake
cmake_minimum_required(VERSION 2.8.0)

#	project name
project (Simulations)

message (SOURCE DIRECTORY = ${PROJECT_SOURCE_DIR})
message (BUILD DIRECTORY = ${PROJECT_BINARY_DIR})

#
#	Compiler flags and some definitions
#
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-DSTANDALONE=1)

#
#	project's versions and some other env vars to pass
#
set (Simulations_VERSION_MAJOR 1)
set (Simulations_VERSION_MINOR 0)
configure_file(
	"${PROJECT_SOURCE_DIR}/config/config.h.in"
	"${PROJECT_BINARY_DIR}/config/config.h"
)

#
#	add the config.h to the search include path and enable
#
include_directories("${PROJECT_BINARY_DIR}/config")

#
# Set the Path for Geant4 cmake lookup
#
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    /Users/vk/software/geant/geant4.10.02.p01-install/lib/Geant4-10.2.1/Modules/
    /afs/cern.ch/sw/lcg/external/geant4/10.2.p01/x86_64-slc6-gcc49-opt-MT/lib64/Geant4-10.2.1/Modules
)
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
set (EXTRA_LIBS ${EXTRA_LIBS} ${Geant4_LIBRARIES})

#
#	Find BOOST
#
find_package(Boost COMPONENTS program_options system REQUIRED )
include_directories("${Boost_INCLUDE_DIRS}")
set (EXTRA_LIBS ${Boost_LIBRARIES} ${EXTRA_LIBS})
#add_definitions(${})

#	
#	find and add ROOT
#
set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/config)
find_package(ROOT REQUIRED COMPONENTS RHTTP)
include_directories("${ROOT_INCLUDE_DIR}")
add_definitions(${ROOT_CXX_FLAGS})
set (EXTRA_LIBS ${EXTRA_LIBS} ${ROOT_LIBRARIES})

#
# search for include files starts with Project folder itself
#
include_directories ("${PROJECT_SOURCE_DIR}")

#
# add various dirs to be built as libs
#
add_subdirectory (hgc)

#
#	add all the executables and link against ROOT
#
file (GLOB MAIN_FILES ${PROJECT_SOURCE_DIR}/drivers/src/*.cc)
foreach (MAIN_FILE ${MAIN_FILES})
	string(REPLACE ".cc" "" EXECUTABLE_NAME ${MAIN_FILE})
	string(REPLACE "${PROJECT_SOURCE_DIR}/drivers/src/" "" 
		EXECUTABLE_NAME ${EXECUTABLE_NAME}
	)
	message("Adding Executable: ${EXECUTABLE_NAME}")
	#	add_executable(${EXECUTABLE_NAME} ${MAIN_FILE} ${SOURCES} ${HEADERS})
	add_executable(${EXECUTABLE_NAME} ${MAIN_FILE})
	target_link_libraries (${EXECUTABLE_NAME} ${EXTRA_LIBS} hgc)
endforeach(MAIN_FILE)

file (COPY ${PROJECT_SOURCE_DIR}/hgc/config DESTINATION ${PROJECT_BINARY_DIR}/hgc)
file (COPY ${PROJECT_SOURCE_DIR}/scripts DESTINATION ${PROJECT_BINARY_DIR})

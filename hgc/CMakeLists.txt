#
# Geant4 lookup
#
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    /Users/vk/software/geant/geant4.10.02.p01-install/lib/Geant4-10.2.1/Modules/
    /afs/cern.ch/sw/lcg/external/geant4/10.2.p01/x86_64-slc6-gcc49-opt-MT/lib64/Geant4-10.2.1/Modules
)
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

#
# ROOT lookup
#
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/config)
find_package(ROOT REQUIRED COMPONENTS RHTTP)
include_directories("${ROOT_INCLUDE_DIR}")
add_definitions(${ROOT_CXX_FLAGS})
set (EXTRA_LIBS ${EXTRA_LIBS} ${ROOT_LIBRARIES})

#
# hgc and also add common stuff in here
#
file (GLOB sources ${PROJECT_SOURCE_DIR}/hgc/src/*.cc)
file (GLOB materials_sources ${PROJECT_SOURCE_DIR}/common/src/* ${PROJECT_SOURCE_DIR}/common/include/*)

#
# lib definition and linking
#
add_library(hgc SHARED ${sources} ${materials_sources})
target_link_libraries (hgc ${Geant4_LIBRARIES} ${EXTRA_LIBS})

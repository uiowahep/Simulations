SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    /Users/vk/software/geant/geant4.10.02.p01-install/lib/Geant4-10.2.1/Modules/
    /afs/cern.ch/sw/lcg/external/geant4/10.2.p01/x86_64-slc6-gcc49-opt-MT/lib64/Geant4-10.2.1/Modules
)
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

file (GLOB sources ${PROJECT_SOURCE_DIR}/common/src/*.cc)
add_library(common SHARED ${sources})
target_link_libraries (common ${Geant4_LIBRARIES})

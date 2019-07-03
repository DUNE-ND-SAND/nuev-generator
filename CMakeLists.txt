# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(generate VERSION 1.0
                  DESCRIPTION "generate neutrino event with GENIE"
                  LANGUAGES CXX)

#---Check "ROOT" environment variable is set
#message("THING:" $ENV{THING})
if(NOT DEFINED ENV{ROOTSYS})
    message(FATAL_ERROR "ROOTSYS environment variable not defined")
endif()

#---Check "GENIE" environment variable is set
#message("THING:" $ENV{THING})
if(NOT DEFINED ENV{GENIE})
    message(FATAL_ERROR "GENIE environment variable not defined")
endif()

#---Check "LOG4CPP" environment variable is set
#message("THING:" $ENV{THING})
if(NOT DEFINED ENV{LOG4CPP})
    message(FATAL_ERROR "LOG4CPP environment variable not defined")
endif()

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net Geom EGPythia6 XMLParser XMLIO)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#---Create a shared library with geneated dictionary
#add_library(Event SHARED Event.cxx G__Event.cxx)
#target_link_libraries(Event ${ROOT_LIBRARIES})


#---Define GENIE environment variables
set(GENIE_DIR "$ENV{GENIE}")
set(GENIE_INC_DIR "${GENIE_DIR}/include/GENIE")
set(GENIE_LIB_DIR "${GENIE_DIR}/lib")
set(GENIE_LIBRARIES)
foreach(_cpt GFwMsg GFwReg GFwAlg GFwInt GFwGHEP GFwNum GFwUtl GFwParDat GFwEG GFwNtp GPhXSIg GPhPDF GPhNuclSt GPhCmn GPhDcy GPhHadTransp GPhHadnz GPhDeEx GPhAMNGXS GPhAMNGEG GPhChmXS GPhCohXS GPhCohEG GPhDISXS GPhDISEG GPhDfrcXS GPhDfrcEG GPhGlwResXS GPhGlwResEG GPhIBDXS GPhIBDEG GPhMNucXS GPhMNucEG GPhMEL GPhNuElXS GPhNuElEG GPhQELXS GPhQELEG GPhResXS GPhResEG GPhStrXS GPhStrEG GPhNDcy GPhNNBarOsc GPhBDMXS GPhBDMEG GTlGeo GTlFlx)
    find_library(GENIE_${_cpt}_LIBRARY ${_cpt} HINTS ${GENIE_LIB_DIR})
    if(GENIE_${_cpt}_LIBRARY)
        mark_as_advanced(GENIE_${_cpt}_LIBRARY)
        list(APPEND GENIE_LIBRARIES ${GENIE_${_cpt}_LIBRARY})
    endif()
endforeach()
if(GENIE_LIBRARIES)
    list(REMOVE_DUPLICATES GENIE_LIBRARIES)
endif()

include_directories("${GENIE_INC_DIR}")

#---Add LOG4CPP library
set(LOG4CPP_DIR "$ENV{LOG4CPP}")
set(LOG4CPP_LIB_DIR "${LOG4CPP_DIR}/lib")
set(LOG4CPP_LIBRARIES)
find_library(LOG4CPP_LIBRARY log4cpp HINTS ${LOG4CPP_LIB_DIR})
    if(LOG4CPP_LIBRARY)
        mark_as_advanced(LOG4CPP_LIBRARY)
        list(APPEND LOG4CPP_LIBRARIES ${LOG4CPP_LIBRARY})
    endif()
if(LOG4CPP_LIBRARIES)
    list(REMOVE_DUPLICATES LOG4CPP_LIBRARIES)
endif()

#---Create  a main program using the library
add_executable(generate ${CMAKE_CURRENT_SOURCE_DIR}/src/generate.cpp)
#add_executable(Main MainEvent.cxx)
target_link_libraries(generate ${ROOT_LIBRARIES} ${GENIE_LIBRARIES} ${LOG4CPP_LIBRARIES})

install(TARGETS generate
        EXPORT generate
        LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
        RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin
        )
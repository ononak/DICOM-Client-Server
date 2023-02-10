
cmake_minimum_required(VERSION 3.16)

# set the project name
project(DicomCommunication VERSION 0.1.0)
message("Dicom Communication VERSION = ${CMAKE_PROJECT_VERSION}")

include_directories(/usr/local/Cellar/dcmtk/3.6.7/include)
link_directories(/usr/local/Cellar/dcmtk/3.6.7/lib)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions("-Wno-c++11-extensions") 
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(DCMTK_USE_STATIC_LIBS ON)



find_package(Threads )
# find_package(DCMTK REQUIRED)

# if( DCMTK_FOUND )
#     message( ${DCMTK_INCLUDE_DIRS} )
#  endif()
 

include_directories(${DCMTK_INCLUDE_DIRS})
find_package( Boost COMPONENTS system filesystem REQUIRED )
file(GLOB DICOM_COMM_SOURCES "/*.cpp")


file(GLOB D_SRC
     "*.h"
     "*.cpp"
)
add_executable(DicomComm ${D_SRC})
target_link_libraries(DicomComm PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} -ldcmnet -ldcmdata -loflog -lofstd)

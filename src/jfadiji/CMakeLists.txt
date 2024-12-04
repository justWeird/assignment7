CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#############################################
SET(MODULENAME jfadiji)
#############################################

PROJECT(${MODULENAME})

FILE(GLOB folder_source *.cpp *.c )
FILE(GLOB folder_header *.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

ADD_EXECUTABLE(${MODULENAME} ${folder_source} ${folder_header}) 

# Change 2: Set the output path to bin,
# by specifying the executable be built in the `RUNTIME_OUTPUT_DIRECTORY`
#  and remove the install step (and all it's issues)
#
# Note: The $<0:> expands to an empty string, to ensure Visual studio
#     does not create
set_target_properties(${MODULENAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<0:>)


# Change 3: Set the startup project to be the project we define,
# not the default `ALL_BUILD`
# This informs Visual Studio
# see https://cmake.org/cmake/help/latest/prop_dir/VS_STARTUP_PROJECT.html
# Set my_executable as the startup project
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${MODULENAME})


# Change 4: Set the debugging working directory in Visual Studio
# to the bin directory
# This informs Visual Studio to run the executable from the bin directory
# see https://cmake.org/cmake/help/latest/prop_tgt/VS_DEBUGGER_WORKING_DIRECTORY.html
set_property(TARGET ${MODULENAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

## --------------------------------------------------
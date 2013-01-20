cmake_minimum_required(VERSION 2.6)
		
project(platformutes)

# We have some custom .cmake scripts not in the official distribution.
SET(CMAKE_MODULE_PATH "${platformutes_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

file(
	GLOB_RECURSE
	source_files
	src/*
)

file(
	GLOB_RECURSE
	data_files
	data/*
)

Find_Package(SDL REQUIRED)
Find_Package(SDL_image REQUIRED)
Find_Package(OpenAL REQUIRED)
Find_Package(ALUT REQUIRED)
Find_Package(VorbisFile REQUIRED)

link_libraries (
   ${SDL_LIBRARY}
   ${SDLIMAGE_LIBRARY}
   ${SDLMIXER_LIBRARY}
   ${OPENAL_LIBRARY}
   ${ALUT_LIBRARY}
   ${VORBISFILE_LIBRARY}
)

add_executable(
	platformutes
	${source_files}
)

add_definitions(-Wall -Wextra -pedantic -g)

install(PROGRAMS ${CMAKE_BINARY_DIR}/platformutes DESTINATION bin)
install(DIRECTORY data/common data/levels data/tilesets DESTINATION share/${PROJECT_NAME})

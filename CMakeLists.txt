cmake_minimum_required(VERSION 2.6)
		
project(Platformutes)

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

link_libraries (
   ${SDL_LIBRARY}
   ${SDLIMAGE_LIBRARY}
)

add_executable(
	platformutes
	${source_files}
)

add_definitions(-Wall -Wextra -pedantic)

install(PROGRAMS ${CMAKE_BINARY_DIR}/platformutes DESTINATION bin)
install(DIRECTORY data/images DESTINATION share)


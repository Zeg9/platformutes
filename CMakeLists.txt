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

if (WIN32)
	set(CMAKE_RC_COMPILE_OBJECT "${CMAKE_RC_COMPILER} -i <SOURCE> -o <OBJECT> -O coff")
	set(source_files icon.rc ${source_files})
endif()

Find_Package(SDL REQUIRED)
Find_Package(SDL_image REQUIRED)
Find_Package(SDL_ttf REQUIRED)
Find_Package(OpenAL REQUIRED)
Find_Package(ALUT REQUIRED)
Find_Package(VorbisFile REQUIRED)
Find_Package(Boost COMPONENTS system filesystem REQUIRED)

link_libraries (
   ${SDL_LIBRARY}
   ${SDLIMAGE_LIBRARY}
   ${SDLTTF_LIBRARY}
   ${OPENAL_LIBRARY}
   ${ALUT_LIBRARY}
   ${VORBISFILE_LIBRARY}
   ${Boost_LIBRARIES}
)

add_executable(
	platformutes
	${source_files}
)

add_definitions(-Wall -Wextra -pedantic -g)

install(PROGRAMS ${CMAKE_BINARY_DIR}/platformutes DESTINATION bin)
install(DIRECTORY data/common data/levels data/tilesets DESTINATION share/${PROJECT_NAME})
install(FILES platformutes.desktop DESTINATION share/applications)
install(FILES platformutes.png DESTINATION share/icons/hicolor/48x48/apps)

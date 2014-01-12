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


include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

Find_Package(OpenAL REQUIRED)
Find_Package(ALUT REQUIRED)
Find_Package(VorbisFile REQUIRED)
Find_Package(Boost COMPONENTS system filesystem REQUIRED)

add_executable(
	platformutes
	${source_files}
)

include_directories(
	${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
	${SDL2_TTF_INCLUDE_DIRS}
)

target_link_libraries (
   platformutes
   ${SDL2_LIBRARIES}
   ${SDL2_IMAGE_LIBRARIES}
   ${SDL2_TTF_LIBRARIES}
   ${OPENAL_LIBRARY}
   ${ALUT_LIBRARY}
   ${VORBISFILE_LIBRARY}
   ${Boost_LIBRARIES}
)

add_definitions(-Wall -Wextra -pedantic -g)

install(PROGRAMS ${CMAKE_BINARY_DIR}/platformutes DESTINATION bin)
install(DIRECTORY data/common data/levels data/tilesets DESTINATION share/${PROJECT_NAME})
install(FILES platformutes.desktop DESTINATION share/applications)
install(FILES platformutes.png DESTINATION share/icons/hicolor/48x48/apps)

cmake_minimum_required(VERSION 3.5)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")

add_executable(engine ${sources})

# Calar o CMake sobre o OpenGL legacy
set(OpenGL_GL_PREFERENCE "GLVND")

find_package(OpenGL REQUIRED)
target_include_directories(engine PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(engine ${OPENGL_LIBRARIES})
add_definitions(${OpenGL_DEFINITIONS})

target_link_libraries(engine fmt)
target_link_libraries(engine pugixml)

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if(NOT OPENGL_GLU_FOUND)
	message(ERROR " OPENGL not found!")
endif(NOT OPENGL_GLU_FOUND)

# Em NixOS, o GLU está noutro sítio, por isso verificar isso
if (DEFINED ENV{GLU_DIRECTORY})
	target_include_directories(engine PRIVATE $ENV{GLU_DIRECTORY})
endif(DEFINED ENV{GLU_DIRECTORY})

if  (WIN32)

	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
	set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")

	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")


	include_directories(${TOOLKITS_FOLDER}/glut )
	target_link_libraries(
      ${OPENGL_LIBRARIES}
	  ${TOOLKITS_FOLDER}/glut/glut32.lib
      engine
      common
      )

	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )

	# set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

else (WIN32) #Linux and Mac

	# set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")
	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIR})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})

    target_link_libraries(
      engine
      common
      ${OPENGL_LIBRARIES}
      ${GLUT_LIBRARIES}
     )
	if(NOT GLUT_FOUND)
	   message(ERROR ": GLUT not found!")
	endif(NOT GLUT_FOUND)

endif(WIN32)

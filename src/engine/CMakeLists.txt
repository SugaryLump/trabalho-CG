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
target_link_libraries(engine argparse)

if (NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif (NOT OPENGL_FOUND)

if (NOT OPENGL_GLU_FOUND)
    message(ERROR " OPENGL not found!")
endif (NOT OPENGL_GLU_FOUND)

# Em NixOS, o GLU está noutro sítio, por isso verificar isso
if (DEFINED ENV{GLU_DIRECTORY})
    target_include_directories(engine PRIVATE $ENV{GLU_DIRECTORY})
endif (DEFINED ENV{GLU_DIRECTORY})

if (DEFINED ENV{GLUT_DIRECTORY})
    target_include_directories(engine PRIVATE $ENV{GLUT_DIRECTORY})
endif (DEFINED ENV{GLUT_DIRECTORY})

if (WIN32)

    message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
    set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")

    if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
        message(ERROR ": GLUT not found")
    endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")

    if (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")
        message(ERROR ": GLEW not found")
    endif (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")

    if (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/devIL.lib")
        message(ERROR ": DevIL not found")
    endif (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/devIL.lib")	


    target_include_directories(engine PRIVATE ${TOOLKITS_FOLDER}/glut ${TOOLKITS_FOLDER}/glew)
    target_link_libraries(engine ${OPENGL_LIBRARIES}
            ${TOOLKITS_FOLDER}/glut/glut32.lib
            ${TOOLKITS_FOLDER}/glew/glew32.lib
            ${TOOLKITS_FOLDER}/devil/devIL.lib
            common)

    if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll"
                                                    AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${TOOLKITS_FOLDER}/devil/devil.dll DESTINATION ${CMAKE_BINARY_DIR})
        file(COPY ${TOOLKITS_FOLDER}/glew/glew32.dll DESTINATION ${CMAKE_BINARY_DIR})
    endif (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll"
                                                       AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")

    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT engine)

else (WIN32) #Linux and Mac

    # set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")
    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    link_directories(${GLUT_LIBRARY_DIRS})
    find_package(DevIL REQUIRED)
    include_directories(${IL_INCLUDE_DIR})
    add_definitions(${GLUT_DEFINITIONS})

    if (NOT APPLE)
        find_package(GLEW REQUIRED)
        include_directories(${GLEW_INCLUDE_DIRS})
        link_libraries(${GLEW_LIBRARIES})
        target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${IL_LIBRARIES})
    else (NOT APPLE)
        target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${IL_LIBRARIES})
    endif (NOT APPLE)

    target_link_libraries(
            engine
            common
            ${OPENGL_LIBRARIES}
            ${GLUT_LIBRARIES}
            ${IL_LIBRARIES}
    )
    if (NOT GLUT_FOUND)
        message(ERROR ": GLUT not found!")
    endif (NOT GLUT_FOUND)

endif (WIN32)

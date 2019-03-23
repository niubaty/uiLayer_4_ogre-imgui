cmake_minimum_required (VERSION 2.8.12)
project(uiLayer)

# specify which version you need
find_package(OGRE 1.11 REQUIRED)
find_package(Freetype QUIET)

# the search paths
include_directories(${OGRE_INCLUDE_DIRS})
link_directories(${OGRE_LIBRARY_DIRS})

set(IMGUI_SRCS 
    ${CMAKE_SOURCE_DIR}/ogre-imgui/imgui/imgui.cpp 
    ${CMAKE_SOURCE_DIR}/ogre-imgui/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/ogre-imgui/imgui/imgui_widgets.cpp)
set(OGRE_IMGUI_SRCS ${CMAKE_SOURCE_DIR}/ogre-imgui/ImguiManager.cpp ${CMAKE_SOURCE_DIR}/ogre-imgui/ImguiRenderable.cpp)
if(FREETYPE_FOUND)
    include_directories(${CMAKE_SOURCE_DIR}/ogre-imgui/imgui/misc/freetype/)
    list(APPEND IMGUI_SRCS ${CMAKE_SOURCE_DIR}/ogre-imgui/imgui/misc/freetype/imgui_freetype.cpp)
endif()

add_definitions(-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/ogre-imgui/ ${CMAKE_SOURCE_DIR}/ogre-imgui/imgui/)
add_library(OgreImgui ${IMGUI_SRCS} ${OGRE_IMGUI_SRCS} uiLayer.cpp)
set_property(TARGET OgreImgui PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(OgreImgui PUBLIC ${OGRE_LIBRARIES})

if(FREETYPE_FOUND)
    target_compile_definitions(OgreImgui PRIVATE -DUSE_FREETYPE)
    target_include_directories(OgreImgui PRIVATE ${FREETYPE_INCLUDE_DIRS})
    target_link_libraries(OgreImgui PRIVATE ${FREETYPE_LIBRARIES})
endif()


if(NOT FREETYPE_FOUND)
    set(FREETYPE_FOUND "FALSE")
endif()
message(STATUS "using freetype: ${FREETYPE_FOUND}")

# Python Bindings
#option(BUILD_PYTHON "build python bindings" ON)
#if(BUILD_PYTHON)
#    add_subdirectory(ogre-imgui/python)
#endif()

option(BUILD_EXAMPLE "build example app" ON)
if(BUILD_EXAMPLE)
add_executable(uiLayerExample py/example.cpp ${CMAKE_SOURCE_DIR}/ogre-imgui/imgui/imgui_demo.cpp)
target_link_libraries(uiLayerExample OgreImgui)
endif()

# Python Bindings
option(BUILD_PYTHON "build python bindings" ON)
if(BUILD_PYTHON)
    add_subdirectory(py)
endif()


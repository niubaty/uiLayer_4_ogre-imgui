find_package(SWIG 3.0.8)
find_package(PythonInterp)
find_package(PythonLibs)

if(NOT PYTHONLIBS_FOUND OR NOT SWIG_FOUND)
    return()
endif()

include(${SWIG_USE_FILE})
include_directories(${PYTHON_INCLUDE_PATH})

set(CMAKE_SWIG_FLAGS -w401,314 -builtin -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)

set_source_files_properties(${CMAKE_SOURCE_DIR}/uilayer.i PROPERTIES CPLUSPLUS ON)

if(MSVC)
	add_definitions(/wd4101 /wd4102)
else()
    add_definitions(-Wno-cast-qual -Wno-shadow -Wno-maybe-uninitialized -Wno-unused-label -Wno-unused-but-set-variable -Wno-missing-declarations)
endif()

swig_add_module(uilayer python ${CMAKE_SOURCE_DIR}/uilayer.i ${CMAKE_SOURCE_DIR}/ogre-imgui/imgui/imgui_demo.cpp)
swig_link_libraries(uilayer OgreImgui OgreMain OgreBites ${PYTHON_LIBRARIES})
set_target_properties(${SWIG_MODULE_OgreImgui_REAL_NAME} PROPERTIES DEBUG_POSTFIX "")

# install
set(PYTHON_SITE_PACKAGES lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/dist-packages/)
install(TARGETS ${SWIG_MODULE_uilayer_REAL_NAME} LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES})
install(FILES ${PROJECT_BINARY_DIR}/py/uilayer.py DESTINATION ${PYTHON_SITE_PACKAGES})

%module(directors="1") uilayer
%{
#include "imgui.h"
#include "Ogre.h"
#include "Bites/OgreApplicationContext.h"
#include "uilayer.h"
%}

%include std_string.i
%include exception.i
%include stdint.i
%include typemaps.i
%import "Ogre.i"
%import "Bites/OgreBites.i"

%apply bool* INOUT { bool* p_open };
%apply float* INOUT { float* v };
%apply int* INOUT { int* v };
%include "imgui.h"

#ifdef SWIGPYTHON
%pythoncode
%{
    __version__ = "0.1"
%}
#endif

/// Ogre
%include "uilayer.h"
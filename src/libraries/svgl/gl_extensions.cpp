// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "gl_extensions.h"

namespace
{
    typedef void (APIENTRY *T_lockArraysEXT )( GLint first, GLsizei size);
    typedef void (APIENTRY *T_unlockArraysEXT )();

    T_lockArraysEXT   lockArraysEXT = 0;
    T_unlockArraysEXT unlockArraysEXT = 0;


    bool InitializeEXTs()
    {
        lockArraysEXT   = (T_lockArraysEXT)   wglGetProcAddress( "lockArraysEXT" );
        unlockArraysEXT = (T_unlockArraysEXT) wglGetProcAddress( "unlockArraysEXT" );
        return true;
    }

    void InitializeExtensions()
    {
        static bool bDummy = InitializeEXTs();
    }
}

// -----------------------------------------------------------------------------
// Name: gl::lockArrays
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
void gl::lockArrays( GLint first, GLsizei size )
{
    InitializeExtensions();
    if( lockArraysEXT )
        lockArraysEXT( first, size );
}
    
// -----------------------------------------------------------------------------
// Name: gl_extensions::unlockArrays
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
void gl::unlockArrays()
{
    InitializeExtensions();
    if( unlockArraysEXT )
        unlockArraysEXT();

}

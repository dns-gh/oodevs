// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GlContext.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GlContext constructor
// Created: MMC 2013-04-10
// -----------------------------------------------------------------------------
GlContext::GlContext() 
{ 
    Reset(); 
}

// -----------------------------------------------------------------------------
// Name: GlContext destructor
// Created: MMC 2013-04-10
// -----------------------------------------------------------------------------
GlContext::~GlContext() 
{ 
    Purge();
}

// -----------------------------------------------------------------------------
// Name: GlContext::Init
// Created: MMC 2013-04-10
// -----------------------------------------------------------------------------
void GlContext::Init( HWND hWnd )
{
    // remember the window handle (HWND)
    hWnd_ = hWnd;
    // get the device context (DC)
    hDC_ = GetDC( hWnd_ );
    // set the pixel format for the DC
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int format = ChoosePixelFormat( hDC_, &pfd );
    SetPixelFormat( hDC_, format, &pfd );
    // create the render context (RC)
    hRC_ = wglCreateContext( hDC_ );
    // make it the current render context
    wglMakeCurrent( hDC_, hRC_ );
}

// -----------------------------------------------------------------------------
// Name: GlContext::Purge
// Created: MMC 2013-04-10
// -----------------------------------------------------------------------------
void GlContext::Purge()
{
    if ( hRC_ )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( hRC_ );
    }
    if ( hWnd_ && hDC_ )
    {
        ReleaseDC( hWnd_, hDC_ );
    }
    Reset();
}

// -----------------------------------------------------------------------------
// Name: GlContext::Reset
// Created: MMC 2013-04-10
// -----------------------------------------------------------------------------
void GlContext::Reset()
{
    hWnd_ = NULL;
    hDC_ = NULL;
    hRC_ = NULL;
}

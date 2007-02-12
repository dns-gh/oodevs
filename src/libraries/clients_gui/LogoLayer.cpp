// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LogoLayer.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LogoLayer constructor
// Created: AGE 2007-02-12
// -----------------------------------------------------------------------------
LogoLayer::LogoLayer( const kernel::GlTools_ABC& tools, const QImage& image )
    : tools_( tools )
    , image_( image )
{
    image_ = image_.mirror();
    image_.setAlphaBuffer( true );
}

// -----------------------------------------------------------------------------
// Name: LogoLayer destructor
// Created: AGE 2007-02-12
// -----------------------------------------------------------------------------
LogoLayer::~LogoLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogoLayer::Paint
// Created: AGE 2007-02-12
// -----------------------------------------------------------------------------
void LogoLayer::Paint( const geometry::Rectangle2f& /*viewport*/ )
{
    if( ! image_.bits() )
        return;
    
    // Unproject
    double modelViewMatrix[16];
    glGetDoublev( GL_MODELVIEW_MATRIX, modelViewMatrix );
    double projectionMatrix[16];
    glGetDoublev( GL_PROJECTION_MATRIX, projectionMatrix );
    int viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    double rX, rY, rZ;
    gluUnProject( 1, viewport[3] - 1, 0, modelViewMatrix, projectionMatrix, viewport, &rX, &rY, &rZ );

    glRasterPos3f( rX, rY, rZ );
    glBitmap( 0, 0, 0, 0, 0, - image_.height(), 0 );
    glDrawPixels( image_.width(), image_.height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image_.bits() );
}

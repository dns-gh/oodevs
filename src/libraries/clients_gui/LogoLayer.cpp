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
LogoLayer::LogoLayer( const kernel::GlTools_ABC& tools, const QImage& image, float alpha /* = -1*/ )
    : tools_( tools )
    , image_( image )
{
    image_ = image_.mirror();
    image_.setAlphaBuffer( true );
    if( alpha > 0 )
        SetAlpha( alpha );
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
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LogoLayer::Paint( kernel::Viewport_ABC& )
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
    gluUnProject( 1, viewport[3] - 1, 0.5, modelViewMatrix, projectionMatrix, viewport, &rX, &rY, &rZ );

    glRasterPos3d( rX, rY, rZ );
    glBitmap( 0, 0, 0, 0, 0, float( - image_.height() ), 0 );
    glDrawPixels( image_.width(), image_.height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image_.bits() );
}

// -----------------------------------------------------------------------------
// Name: LogoLayer::SetAlpha
// Created: AGE 2007-02-12
// -----------------------------------------------------------------------------
void LogoLayer::SetAlpha( float alpha )
{
    unsigned int a = unsigned char( 255 * alpha );
    a <<= 24;
    for( unsigned i = 0; i < unsigned( image_.width() ); ++i )
        for( unsigned j = 0; j < unsigned( image_.height() ); ++j )
        {
            QRgb rgb = image_.pixel( i, j );
            rgb = ( rgb & 0x00FFFFFF ) | a;
            image_.setPixel( i, j, rgb );
        }
}

// -----------------------------------------------------------------------------
// Name: LogoLayer::RegisterIn
// Created: AGE 2007-02-12
// -----------------------------------------------------------------------------
void LogoLayer::RegisterIn( Gl3dWidget& w )
{
    Layer_ABC::RegisterIn( w );
}

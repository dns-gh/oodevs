// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FrameCounter.h"
#include <gl/glu.h>

using namespace gui;

FrameCounter::FrameCounter( QGLWidget& widget, int x, int y )
    : widget_( widget )
    , x_( x )
    , y_( y )
    , frames_( 0 )
    , fps_( 0 )
{
    // NOTHING
}

void FrameCounter::Update()
{
    Compute();
    Draw();
}

void FrameCounter::Compute()
{
    if( time_.isNull() )
    {
        time_.start();
        return;
    }
    const int span = 100;
    if( ++frames_ == span )
    {
        frames_ = 0;
        fps_ =  static_cast< int >( 1000.0 * span / time_.restart() );
    }
}

void FrameCounter::Draw() const
{
    double modelViewMatrix[16];
    glGetDoublev( GL_MODELVIEW_MATRIX, modelViewMatrix );
    double projectionMatrix[16];
    glGetDoublev( GL_PROJECTION_MATRIX, projectionMatrix );
    int viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    double rX, rY, rZ;
    gluUnProject(
        x_ + ( x_ < 0 ? viewport[2] : 0 ),
        y_ + ( y_ < 0 ? viewport[3] : 0 ),
        0,
        modelViewMatrix, projectionMatrix,
        viewport,
        &rX, &rY, &rZ );
    widget_.renderText( rX, rY, 0, QString( "%1 FPS" ).arg( fps_ ) );
}

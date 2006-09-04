// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerLayer.h"
#include "DrawerShape.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerLayer constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerLayer::DrawerLayer()
    : current_( 0 )
    , show_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer destructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerLayer::~DrawerLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::Show
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void DrawerLayer::Show( bool show )
{
    show_ = show;
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::StartShape
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerLayer::StartShape( const DrawerStyle& style )
{
    delete current_;
    current_ = new DrawerShape( style );
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::Paint
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( show_ )
    {
        for( CIT_Shapes it = shapes_.begin(); it != shapes_.end(); ++it )
            (*it)->Draw();
        if( current_ )
            current_->Draw();
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::HandleKeyPress
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
bool DrawerLayer::HandleKeyPress( QKeyEvent* key )
{
    if( ! current_ )
        return false;

    switch( key->key() )
    {
        case Qt::Key_BackSpace:
        case Qt::Key_Delete:
            current_->PopPoint();
            return true;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            Done();
            return true;
        case Qt::Key_Escape:
            delete current_;
            current_ = 0;
            return true;
    };
    return false;
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::Done
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerLayer::Done()
{
    if( current_ )
    {
        shapes_.push_back( current_ );
        current_ = 0;
    }
}


// -----------------------------------------------------------------------------
// Name: DrawerLayer::HandleMouseMove
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
bool DrawerLayer::HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return false;     // $$$$ AGE 2006-09-01: 
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::HandleMousePress
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
bool DrawerLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( ! current_ )
        return false;

    if( mouse->button() == Qt::LeftButton && mouse->state() == Qt::NoButton )
        current_->AddPoint( point );

    if( mouse->button() == Qt::RightButton && mouse->state() == Qt::NoButton )
        current_->PopPoint();

    return true;
}

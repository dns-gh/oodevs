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
#include "DrawerShapeFactory.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerLayer constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerLayer::DrawerLayer( const kernel::GlTools_ABC& tools )
    : tools_( tools )
    , factory_( *new DrawerShapeFactory() )
    , current_( 0 )
    , show_( true )
    , selected_( 0 )
    , overlined_( 0 )
    , selectedStyle_( 0 )
    , selectedColor_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer destructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerLayer::~DrawerLayer()
{
    delete &factory_;
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::TakeFocus
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
void DrawerLayer::TakeFocus( bool take )
{
    if( ! take )
        overlined_ = selected_ = 0;
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
void DrawerLayer::StartShape( const DrawerStyle& style, const QColor& color )
{
    delete current_;
    current_ = factory_.CreateShape( style, color );
    selectedStyle_ = &style;
    selectedColor_ = color;
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
        {
            if( *it == selected_ )
                (*it)->Draw( viewport, Qt::red, true );
            else
                (*it)->Draw( viewport, *it == overlined_ );
        }
        if( current_ )
            current_->Draw( viewport, true );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::HandleKeyPress
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
bool DrawerLayer::HandleKeyPress( QKeyEvent* key )
{
    switch( key->key() )
    {
        case Qt::Key_BackSpace:
        case Qt::Key_Delete:
            if( current_ ) 
                current_->PopPoint();
            else if( selected_ )
                DeleteSelected();
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
// Name: DrawerLayer::DeleteSelected
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void DrawerLayer::DeleteSelected()
{
    T_Shapes::iterator it = std::find( shapes_.begin(), shapes_.end(), selected_ );
    if( it != shapes_.end() )
        shapes_.erase( it );
    delete selected_;
    selected_ = 0;
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
// Name: DrawerLayer::Precision
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
float DrawerLayer::Precision() const
{
    return 5.f * tools_.Pixels();
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::HandleMouseMove
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
bool DrawerLayer::HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point )
{
    overlined_ = 0;
    const float precision = Precision();
    for( CIT_Shapes it = shapes_.begin(); it != shapes_.end() && ! overlined_; ++it )
        if( (*it)->IsAt( point, precision ) )
            overlined_ = *it;

    if( selected_ && mouse->state() == Qt::LeftButton && ! dragPoint_.IsZero() )
    {
        const geometry::Vector2f translation( dragPoint_, point );
        selected_->Translate( dragPoint_, translation, precision );
        dragPoint_ = point;
    }

    return true;
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::HandleMousePress
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
bool DrawerLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( mouse->button() == Qt::LeftButton && mouse->state() == Qt::NoButton ) 
        dragPoint_ = point;
    else 
        dragPoint_ = geometry::Point2f();

    if( current_ && mouse->button() == Qt::LeftButton && mouse->state() == Qt::LeftButton )
        current_->AddPoint( point );
    else if( current_ && mouse->button() == Qt::RightButton && mouse->state() == Qt::RightButton )
        current_->PopPoint();
    else if( mouse->button() == Qt::LeftButton )
        selected_ = overlined_;

    return true;
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::HandleMouseDoubleClick
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
bool DrawerLayer::HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& )
{
    if( ! current_ )
        return false;
    if( mouse->button() == Qt::RightButton )
        current_->PopPoint();
    if( mouse->button() == Qt::LeftButton )
        Done();
    return true;
}

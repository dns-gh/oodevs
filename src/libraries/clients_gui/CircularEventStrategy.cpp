// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CircularEventStrategy.h"
#include <graphics/MapLayer_ABC.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy constructor
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
CircularEventStrategy::CircularEventStrategy()
    : default_  ( 0 )
    , reverse_  ( true )
    , exclusive_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy destructor
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
CircularEventStrategy::~CircularEventStrategy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::SetReverse
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::SetReverse( bool b )
{
    reverse_ = b;
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::SetExclusive
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::SetExclusive( bool b )
{
    exclusive_ = b;
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::SetDefault
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::SetDefault( MapLayer_ABC& layer )
{
    default_ = &layer;
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::Register
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::Register( MapLayer_ABC& layer )
{
    layers_.push_back( &layer );
    last_ = layers_.begin();
    rlast_ = layers_.rbegin();
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::Remove
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::Remove( MapLayer_ABC& layer )
{
    IT_MapLayers it = std::find( layers_.begin(), layers_.end(), &layer );
    if( it != layers_.end() )
        layers_.erase( it );
    last_ = layers_.begin();
    rlast_ = layers_.rbegin();
}

namespace
{
    struct ExclusiveFunctor
    {
        void SetExclusive( bool b ) { exclusive_ =  b; }
        bool exclusive_;
    };

    struct KeyPressFunctor : public ExclusiveFunctor
    {
        KeyPressFunctor( QKeyEvent* key )
            : key_( key ){};
        bool operator()( MapLayer_ABC& layer )
        {
            return layer.HandleKeyPress( key_ ) && exclusive_;
        }
        QKeyEvent* key_;
    };

    template< typename Event >
    struct EventFunctor : public ExclusiveFunctor
    {
        EventFunctor( Event* button, const geometry::Point2f& point, bool (MapLayer_ABC::*func)( Event* button, const geometry::Point2f& point ) )
            : button_( button )
            , point_( point )
            , func_( func )
        {};
        bool operator()( MapLayer_ABC& layer )
        {
            return (layer.*func_)( button_,point_ ) && exclusive_;
        }
        Event* button_;
        geometry::Point2f point_;
        bool (MapLayer_ABC::*func_)( Event* button, const geometry::Point2f& point );
    };
    typedef EventFunctor< QMouseEvent >     MouseFunctor;
    typedef EventFunctor< QDropEvent >      DropFunctor;
    typedef EventFunctor< QDragEnterEvent > DragFunctor;
    typedef EventFunctor< QDragMoveEvent >  DragMoveFunctor;
    typedef EventFunctor< QWheelEvent >     WheelFunctor;
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::Loop
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
template< typename It, typename Functor >
bool CircularEventStrategy::Loop( It& use, It first, It begin, It end, Functor functor )
{
    use = first;
    while( use != end )
        if( functor( **use ) )
            return true;
        else
            ++use;
    use = begin;
    while( use != first )
        if( functor( **use ) )
            return true;
        else
            ++use;
    return false;
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::Apply
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
template< typename Functor >
bool CircularEventStrategy::Apply( Functor functor )
{
    const T_MapLayers& layers = layers_;
    functor.SetExclusive( exclusive_ );
    if( reverse_ )
    {
        CRIT_MapLayers first = rlast_;
        return Loop( rlast_, first, layers.rbegin(), layers.rend(), functor );
    }
    else
    {
        CIT_MapLayers first = last_;
        return Loop( last_, first, layers.begin(), layers.end(), functor );
    }
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleKeyPress
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleKeyPress( QKeyEvent* key )
{
    Apply( KeyPressFunctor( key ) );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleKeyRelease
// Created: LGY 2011-10-06
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleKeyRelease( QKeyEvent* /*key*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleMousePress
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( ! Apply( MouseFunctor( mouse, point, &MapLayer_ABC::HandleMousePress  ) ) && default_ )
        default_->HandleMousePress( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleMouseDoubleClick
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( ! Apply( MouseFunctor( mouse, point, &MapLayer_ABC::HandleMouseDoubleClick ) ) && default_ )
        default_->HandleMousePress( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleMouseMove
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( ! Apply( MouseFunctor( mouse, point, &MapLayer_ABC::HandleMouseMove ) ) && default_ )
        default_->HandleMouseMove( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleMouseWheel
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleMouseWheel( QWheelEvent* mouse, const geometry::Point2f& point )
{
    if( ! Apply( WheelFunctor( mouse, point, &MapLayer_ABC::HandleMouseWheel ) ) && default_ )
        default_->HandleMouseWheel( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleDropEvent
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( ! Apply( DropFunctor( event, point, &MapLayer_ABC::HandleDropEvent ) ) && default_ )
        default_->HandleDropEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleEnterDragEvent
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    bool accept = false;
    accept = Apply( DragFunctor( event, point, &MapLayer_ABC::HandleEnterDragEvent ) );
    if( ! accept && default_ )
        accept = default_->HandleEnterDragEvent( event, point );
    event->accept( accept );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleMoveDragEvent
// Created: ABR 2011-10-26
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleMoveDragEvent( QDragMoveEvent*  event, const geometry::Point2f& point )
{
    bool accept = false;
    accept = Apply( DragMoveFunctor( event, point, &MapLayer_ABC::HandleMoveDragEvent ) );
    if( !accept && default_ )
        accept = default_->HandleMoveDragEvent( event, point );
    event->accept( accept );
}

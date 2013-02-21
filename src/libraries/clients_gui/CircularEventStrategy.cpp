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
#include "GlTools_ABC.h"
#include "SelectionMenu.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy constructor
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
CircularEventStrategy::CircularEventStrategy( EntitySymbols& entitySymbols, ColorStrategy& colorStrategy,
                                              DrawingTypes& drawingTypes, GlTools_ABC& tools )
    : menu_( new SelectionMenu( entitySymbols, colorStrategy, drawingTypes, tools ) )
    , default_( 0 )
    , exclusive_( true )
    , tools_( tools )
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
void CircularEventStrategy::SetDefault( Layer_ABC& layer )
{
    default_ = &layer;
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::Register
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::Register( Layer_ABC& layer )
{
    layers_.push_back( &layer );
    rlast_ = layers_.rbegin();
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::Remove
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::Remove( Layer_ABC& layer )
{
    auto it = std::find( layers_.begin(), layers_.end(), &layer );
    if( it != layers_.end() )
        layers_.erase( it );
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
        bool operator()( Layer_ABC& layer )
        {
            if( layer.IsReadOnly() )
                return false;
            return layer.HandleKeyPress( key_ ) && exclusive_;
        }
        QKeyEvent* key_;
    };

    template< typename Event >
    struct EventFunctor : public ExclusiveFunctor
    {
        EventFunctor( Event* button, const geometry::Point2f& point, bool (Layer_ABC::*func)( Event* button, const geometry::Point2f& point ), bool testReadOnly = true )
            : button_( button )
            , point_( point )
            , func_( func )
            , testReadOnly_( testReadOnly )
        {};
        bool operator()( Layer_ABC& layer )
        {
            if( testReadOnly_ && layer.IsReadOnly() )
                return false;
            return (layer.*func_)( button_,point_ ) && exclusive_;
        }
        Event* button_;
        geometry::Point2f point_;
        bool (Layer_ABC::*func_)( Event* button, const geometry::Point2f& point );
        bool testReadOnly_;
    };

    struct DragLeaveFunctor : public ExclusiveFunctor
    {
        DragLeaveFunctor( QDragLeaveEvent* button, bool (Layer_ABC::*func)( QDragLeaveEvent* button ), bool testReadOnly = true )
            : button_( button )
            , func_( func )
            , testReadOnly_( testReadOnly )
        {};
        bool operator()( Layer_ABC& layer )
        {
            if( testReadOnly_ && layer.IsReadOnly() )
                return false;
            return (layer.*func_)( button_ ) && exclusive_;
        }
        QDragLeaveEvent* button_;
        bool (Layer_ABC::*func_)( QDragLeaveEvent* button );
        bool testReadOnly_;
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
    const Layer_ABC::T_Layers& layers = layers_;
    functor.SetExclusive( exclusive_ );
    return Loop( rlast_, rlast_, layers.rbegin(), layers.rend(), functor );
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
    if( !mouse ||
        Apply( MouseFunctor( mouse, point, &Layer_ABC::HandleMousePress, false ) ) ||                       // a layer has return true
        ( mouse->buttons() & mouse->button() ) == 0 ||                                                      // mouse release
        ( mouse->globalPos() - QCursor::pos() ).manhattanLength() > 3 ||                                    // the mouse has moved more than 3 pixels since the oldPosition
        ( mouse->button() != Qt::LeftButton && mouse->button() != Qt::RightButton ) )                       // no good button 
        return;


    GlTools_ABC::T_ObjectsPicking selection;
    tools_.FillSelection( point, selection );

    Layer_ABC::T_LayerElements extractedElements;
    for( auto it = layers_.begin(); it != layers_.end(); ++it )                                             // Extract elements
    {
        if( !( mouse->button() == Qt::LeftButton || ( mouse->button() == Qt::RightButton && !( *it )->IsReadOnly() ) ) )
            continue;
        ( *it )->ExtractElements( extractedElements, selection );
    }

    if( default_ && extractedElements.size() == 0 )                                                         // No element extracted
    {
        default_->HandleMousePress( mouse, point );
        return;
    }

    menu_->ExecMenu( extractedElements, point, mouse->globalPos(), mouse->button(), mouse->modifiers() );   // Elements extracted, let the menu handle it
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleMouseDoubleClick
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( ! Apply( MouseFunctor( mouse, point, &Layer_ABC::HandleMouseDoubleClick, false ) ) && default_ )
        default_->HandleMousePress( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleMouseMove
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( ! Apply( MouseFunctor( mouse, point, &Layer_ABC::HandleMouseMove, false ) ) && default_ )
        default_->HandleMouseMove( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleMouseWheel
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleMouseWheel( QWheelEvent* mouse, const geometry::Point2f& point )
{
    if( ! Apply( WheelFunctor( mouse, point, &Layer_ABC::HandleMouseWheel ) ) && default_ )
        default_->HandleMouseWheel( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleDropEvent
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( ! Apply( DropFunctor( event, point, &Layer_ABC::HandleDropEvent ) ) && default_ )
        default_->HandleDropEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleEnterDragEvent
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    bool accept = false;
    accept = Apply( DragFunctor( event, point, &Layer_ABC::HandleEnterDragEvent ) );
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
    rlast_ = layers_.rbegin();
    accept = Apply( DragMoveFunctor( event, point, &Layer_ABC::HandleMoveDragEvent ) );
    if( !accept && default_ )
        accept = default_->HandleMoveDragEvent( event, point );
    event->accept( accept );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleLeaveDragEvent
// Created: JSR 2012-06-26
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleLeaveDragEvent( QDragLeaveEvent* event )
{
    bool accept = false;
    rlast_ = layers_.rbegin();
    accept = Apply( DragLeaveFunctor( event, &Layer_ABC::HandleLeaveDragEvent ) );
    if( !accept && default_ )
        accept = default_->HandleLeaveDragEvent( event );
    if( accept )
        event->accept();
    else
        event->ignore();
}

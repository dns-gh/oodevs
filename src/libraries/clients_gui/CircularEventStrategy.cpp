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
#include "moc_CircularEventStrategy.cpp"
#include "GLView_ABC.h"
#include "SelectionMenu.h"
#include "Selection.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include <boost/optional.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy constructor
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
CircularEventStrategy::CircularEventStrategy( kernel::Controllers& controllers, EntitySymbols& entitySymbols, ColorStrategy& colorStrategy,
                                              DrawingTypes& drawingTypes, GLView_ABC& tools )
    : QObject()
    , menu_( new SelectionMenu( controllers, entitySymbols, colorStrategy, drawingTypes, tools ) )
    , selection_( new Selection( controllers ) )
    , default_( 0 )
    , exclusive_( true )
    , tools_( tools )
    , timer_( new QTimer( this ) )
    , tooltiped_( false )
{
    connect( timer_, SIGNAL( timeout() ), SLOT( OnDisplayToolTip() ) );
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
void CircularEventStrategy::SetDefault( const T_Layer& layer )
{
    default_ = layer;
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::GetSelectionMenu
// Created: ABR 2013-02-21
// -----------------------------------------------------------------------------
const SelectionMenu* CircularEventStrategy::GetSelectionMenu() const
{
    return menu_.get();
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::AddLayers
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void CircularEventStrategy::AddLayers( const T_LayersVector& layers )
{
    layers_.insert( layers_.end(), layers.begin(), layers.end() );
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
    const auto& layers = layers_;
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
    if( !mouse || ( mouse->buttons() & mouse->button() ) == 0 ) // mouse release
        return;

    // Extract elements with opengl picking
    Layer_ABC::T_LayerElements extractedElements;
    RetrieveEntities( mouse, point, extractedElements );

    if( Apply( MouseFunctor( mouse, point, &Layer_ABC::HandleMousePress ) ) ||                              // a layer has return true
        ( mouse->globalPos() - QCursor::pos() ).manhattanLength() > 3 ||                                    // the mouse has moved more than 3 pixels since the oldPosition
        ( mouse->button() != Qt::LeftButton && mouse->button() != Qt::RightButton ) ||                      // no good button
        ( mouse->modifiers() == Qt::ShiftModifier  ) )                                                      // metrics mode
        return;

    if( DisplayDefaultMenu( mouse, point, extractedElements ) )                                             // No element extracted and display default menu
        return;

    if( DisplaySelectedMenu( mouse, point, extractedElements ) )                                           // Show context menu on selected unit
        return;

    menu_->ExecMenu( extractedElements, point, mouse->globalPos(), mouse->button(), mouse->modifiers() );   // Elements extracted, let the menu handle it
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::RetrieveEntities
// Created: LGY 2013-04-04
// -----------------------------------------------------------------------------
void CircularEventStrategy::RetrieveEntities( QMouseEvent* mouse, const geometry::Point2f& point, Layer_ABC::T_LayerElements& extractedElements )
{
    if( mouse->button() != Qt::LeftButton && mouse->button() != Qt::RightButton )
        return;
    GLView_ABC::T_ObjectsPicking selection;
    tools_.FillSelection( point, selection, boost::none );
    for( auto it = layers_.begin(); it != layers_.end(); ++it )
        if( !( *it )->IsReadOnly() )
            ( *it )->ExtractElements( extractedElements, selection );
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::DisplayDefaultMenu
// Created: LGY 2013-04-04
// -----------------------------------------------------------------------------
bool CircularEventStrategy::DisplayDefaultMenu( QMouseEvent* mouse, const geometry::Point2f& point, Layer_ABC::T_LayerElements& extractedElements )
{
    if( !default_ || extractedElements.size() != 0 )
        return false;
    default_->HandleMousePress( mouse, point );
    return true;
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::DisplaySelectedMenu
// Created: LGY 2013-04-04
// -----------------------------------------------------------------------------
bool CircularEventStrategy::DisplaySelectedMenu( QMouseEvent* mouse, const geometry::Point2f& point, Layer_ABC::T_LayerElements& extractedElements )
{
    if( mouse->button() == Qt::RightButton )
    {
        // is selected
        for( auto it = extractedElements.begin(); it != extractedElements.end(); ++it )
        {
            const std::vector< const kernel::GraphicalEntity_ABC* >& entities = (*it).second;
            if( selection_->IsSelected( entities ) )
                if( (*it).first->ContextMenu( entities, point, mouse->globalPos() ) )
                    return true;
        }
        // is not selected
        if( extractedElements.size() == 1 )
        {
            auto it = extractedElements.begin();
            const std::vector< const kernel::GraphicalEntity_ABC* >& entities = (*it).second;
            if( entities.size() == 1 )
            {
                (*it).first->ContextMenu( **(*it).second.begin(), point, mouse->globalPos() );
                return true;
            }
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HandleMouseRelease
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
void CircularEventStrategy::HandleMouseRelease( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( ! Apply( MouseFunctor( mouse, point, &MapLayer_ABC::HandleMouseRelease ) ) && default_ )
        default_->HandleMouseRelease( mouse, point );
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
    if( tooltiped_ )
        HideTooltip();
    timer_->start( 500 );
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

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::OnDisplayToolTip
// Created: LGY 2013-03-01
// -----------------------------------------------------------------------------
void CircularEventStrategy::OnDisplayToolTip()
{
    if( QApplication::activeWindow() && !QApplication::activePopupWidget() )
    {
        if( !tooltiped_ && tools_.HasFocus() )
        {
            GLView_ABC::T_ObjectsPicking selection;
            geometry::Point2f point = tools_.MapToterrainCoordinates( QCursor::pos().x(), QCursor::pos().y() );
            tools_.FillSelection( point, selection, boost::none );
            if( !selection.empty() )
                for( auto it = layers_.begin(); it != layers_.end(); ++it )
                     if( ( *it )->ShowTooltip( selection.back() ) )
                     {
                         tooltiped_ = true;
                         timer_->start( 10000 );
                         return;
                     }
        }
        else
            HideTooltip();
    }
}

// -----------------------------------------------------------------------------
// Name: CircularEventStrategy::HideTooltip
// Created: LGY 2013-03-11
// -----------------------------------------------------------------------------
void CircularEventStrategy::HideTooltip()
{
    tooltiped_ = false;
    for( auto it = layers_.begin(); it != layers_.end(); ++it )
        ( *it )->HideTooltip();
    timer_->stop();
}

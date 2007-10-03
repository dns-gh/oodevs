// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterActionCanvas.h"
#include "moc_AfterActionCanvas.cpp"
#include "AfterActionCanvasItem.h"
#include "AfterActionCanvasConnection.h"
#include "gaming/AfterActionItem_ABC.h"
#include "gaming/AfterActionFactory.h"
#include "gaming/AfterActionFunction.h"
#include <boost/bind.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionCanvas::AfterActionCanvas( QWidget* parent )
    : QCanvasView( parent )
    , function_          ( 0 )
    , selected_          ( 0 )
    , selectedConnection_( 0 )
    , currentConnection_ ( 0 )
    , connect_           ( false )
{
    QPalette palette( palette() );
    palette.setColor( QPalette::Active  , QColorGroup::Foreground, QColor(  50, 105, 200 ) );
    setPalette( palette );

    QCanvas* canvas = new QCanvas();
    canvas->resize( 610, 480 );
    canvas->retune( 600, 1 );
    canvas->setDoubleBuffering( true );
    canvas->setBackgroundColor( Qt::white );
    setCanvas( canvas );
    setMinimumSize( 610, 480 );
    setAcceptDrops( true );
    viewport()->setMouseTracking( true );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionCanvas::~AfterActionCanvas()
{
    setCanvas( 0 );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::dragEnterEvent
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionCanvas::dragEnterEvent( QDragEnterEvent* event )
{
    event->accept( event->provides( "AfterActionFactory" ) && function_ );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::dropEvent
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionCanvas::dropEvent( QDropEvent* event )
{
    if( event->provides( "AfterActionFactory" ) && function_ )
    {
        QByteArray bytes = event->encodedData( "AfterActionFactory" );
        const AfterActionFactory* factory = *reinterpret_cast< const AfterActionFactory** >( bytes.data() );
        if( factory )
        {
            Add( *factory->Create( *function_), event->pos() );
            canvas()->update();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::ClearSelection
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvas::ClearSelection()
{
    QCanvasItemList list = canvas()->allItems();
    for( QCanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
        (*it)->setSelected( false );
    selected_ = 0;
    selectedConnection_ = 0;
    canvas()->update();
}

namespace
{
    template< typename T, typename TO >
    void ChangeSelection( T* newSelection, T*& selection, TO*& other )
    {
        if( selection ) selection->setSelected( false );
        if( other )     other->setSelected( false );
        other = 0;
        selection = newSelection;
        if( selection )
            selection->setSelected( true );
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::contentsMousePressEvent
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvas::contentsMousePressEvent( QMouseEvent* event )
{
    ClearSelection();
    setFocus();
    grabPoint_ = event->pos();
    QCanvasItemList list = canvas()->collisions( event->pos() );
    for( QCanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
    {
        if( AfterActionCanvasItem* item = dynamic_cast< AfterActionCanvasItem* >( *it ) )
            ChangeSelection( item, selected_, selectedConnection_ );
        else if( AfterActionCanvasConnection* item = dynamic_cast< AfterActionCanvasConnection* >( *it ) )
            ChangeSelection( item, selectedConnection_, selected_ );
    }
    if( selected_ && connect_ )
        currentConnection_ = selected_->StartConnection( grabPoint_ );
    canvas()->update();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::contentsMouseReleaseEvent
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvas::contentsMouseReleaseEvent( QMouseEvent* event )
{
    setCursor( QCursor::arrowCursor );
    IT_Items it = items_.begin();
    while( connect_ && currentConnection_ && it != items_.end() )
    {
        if( (*it)->EndConnection( *currentConnection_, event->pos() ) )
            currentConnection_ = 0;
        ++it;
    }
    delete currentConnection_;
    currentConnection_ = 0;
    canvas()->update();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::contentsMouseMoveEvent
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvas::contentsMouseMoveEvent( QMouseEvent* event )
{
    if( !selected_ || ( event->state() & Qt::LeftButton ) == 0 )
        return;
    if( connect_ )
        Connect( event );
    else
        Move( event );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::keyPressEvent
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvas::keyPressEvent( QKeyEvent* event )
{
    if( event->key() == Qt::Key_Delete || event->key() == Qt::Key_BackSpace )
    {
        DeleteSelected();
        canvas()->setAllChanged();
        canvas()->update();
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::resizeEvent
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
void AfterActionCanvas::resizeEvent( QResizeEvent* event )
{
    canvas()->resize( std::max( event->size().width(), canvas()->width() ),
                      std::max( event->size().height(), canvas()->height() ) );
    QCanvasView::resizeEvent( event );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::DeleteSelected
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvas::DeleteSelected()
{
    IT_Items it = std::find( items_.begin(), items_.end(), selected_ );
    if( it != items_.end() )
    {
        std::swap( *it, items_.back() );
        items_.pop_back();
    }
    delete selected_;
    selected_ = 0;
    delete selectedConnection_;
    selectedConnection_ = 0;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Connect
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvas::Connect( QMouseEvent* event )
{
    if( currentConnection_ )
        currentConnection_->Move( event->pos() );
    canvas()->update();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Move
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvas::Move( QMouseEvent* event )
{
    setCursor( QCursor::pointingHandCursor );
    selected_->moveBy( event->pos().x() - grabPoint_.x(), event->pos().y() - grabPoint_.y() );
    ensureVisible( selected_->x(), selected_->y() );
    grabPoint_ = event->pos();
    canvas()->update();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Connect
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvas::Connect()
{
    connect_ = true;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Select
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvas::Select()
{
    connect_ = false;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Clear
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
void AfterActionCanvas::Clear()
{
    function_ = 0;
    ClearSelection();
    for( IT_Items it = items_.begin(); it != items_.end(); ++it )
        delete *it;
    items_.clear();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Edit
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
void AfterActionCanvas::Edit( const AfterActionFunction* function )
{
    Clear();
    function_ = const_cast< AfterActionFunction* >( function );
    if( function_ )
    {
        Iterator< const AfterActionItem_ABC& > it = function_->CreateItemIterator();
        while( it.HasMoreElements() )
            Add( const_cast< AfterActionItem_ABC& >( it.NextElement() ) );
        std::for_each( items_.begin(), items_.end(), boost::bind( &AfterActionCanvasItem::Reconnect, _1 ) );
    }
    canvas()->update();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Delete
// Created: AGE 2007-10-01
// -----------------------------------------------------------------------------
bool AfterActionCanvas::Delete( const AfterActionFunction* function )
{
    if( function == function_ )
    {
        Clear();
        return true;
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Add
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void AfterActionCanvas::Add( AfterActionItem_ABC& item, const QPoint& p /*= QPoint()*/ )
{
    QPoint position = p.isNull() ? QPoint( 100, 100 ) : p;
    AfterActionCanvasItem* pItem = new AfterActionCanvasItem( *this, item, position );
    items_.push_back( pItem );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Resolve
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
AfterActionCanvasItem* AfterActionCanvas::Resolve( AfterActionItem_ABC* item ) const
{
    T_Items::const_iterator it = std::find_if( items_.begin(), items_.end(), boost::bind( &AfterActionCanvasItem::Holds, _1, item ) );
    return it != items_.end() ? *it : 0;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Remove
// Created: AGE 2007-10-01
// -----------------------------------------------------------------------------
void AfterActionCanvas::Remove( AfterActionItem_ABC& item )
{
    if( function_ )
        function_->Remove( item );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Disconnect
// Created: AGE 2007-10-01
// -----------------------------------------------------------------------------
void AfterActionCanvas::Disconnect( AfterActionItem_ABC& from, AfterActionItem_ABC& to )
{
    if( function_ )
        from.Disconnect( &to );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::Disconnect
// Created: AGE 2007-10-01
// -----------------------------------------------------------------------------
void AfterActionCanvas::Disconnect( AfterActionItem_ABC& from, AfterActionItem_ABC& to, int index )
{
    if( function_ )
        from.Disconnect( &to, index );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::hideEvent
// Created: AGE 2007-10-03
// -----------------------------------------------------------------------------
void AfterActionCanvas::hideEvent( QHideEvent* )
{
    emit Closed();
}

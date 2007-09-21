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

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionCanvas::AfterActionCanvas( QWidget* parent )
    : QCanvasView( parent )
    , selected_  ( 0 )
    , selectedConnection_( 0 )
    , currentConnection_( 0 )
    , connect_   ( false )
    , currentId_ ( 0 )
{
    QPalette palette;
    palette.setColor( QPalette::Disabled, QColorGroup::Foreground, QColor( 180, 180, 180 ) );
    palette.setColor( QPalette::Inactive, QColorGroup::Foreground, QColor(   0,   0,   0 ) );
    palette.setColor( QPalette::Active  , QColorGroup::Foreground, QColor(  50, 105, 200 ) );
    setPalette( palette );

    QCanvas* canvas = new QCanvas();
    canvas->resize( 600, 480 );
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
    event->accept( event->provides( "AfterActionFactory" ) );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvas::dropEvent
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionCanvas::dropEvent( QDropEvent* event )
{
    if( event->provides( "AfterActionFactory" ) )
    {
        QByteArray bytes = event->encodedData( "AfterActionFactory" );
        const AfterActionFactory* factory = *reinterpret_cast< const AfterActionFactory** >( bytes.data() );
        if( factory )
        {
            AfterActionCanvasItem* item = new AfterActionCanvasItem( canvas(), palette(), factory->Create(), event->pos(), ++currentId_ );
            items_.push_back( item );
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

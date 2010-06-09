// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineListView.h"
#include "moc_TimelineListView.cpp"
#include "actions/Action_ABC.h"
#include "actions/ActionTasker.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_gui/ValuedListItem.h"
#include <qpainter.h>

namespace
{
    class TimeLineEntityListItem : public gui::ValuedListItem
    {
    public:
        TimeLineEntityListItem( QListView* parent, QListViewItem* after ) : gui::ValuedListItem( parent, after ) {}

        virtual void paintCell( QPainter* p, const QColorGroup& cg, int column, int width, int align )
        {
            gui::ValuedListItem::paintCell( p, cg, column, width, align );
            const QPen oldPen = p->pen();
            p->setPen( QColor( 225, 225, 225 ) );
            p->drawLine( 0, height() - 1, width, height() - 1 );
            p->setPen( oldPen );
        }

    protected:
        virtual void setHeight( int )
        {
            gui::ValuedListItem::setHeight( 25 );
        }
    };
}

using namespace actions;

// -----------------------------------------------------------------------------
// Name: TimelineListView constructor
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
TimelineListView::TimelineListView( QWidget* parent, kernel::Controllers& controllers )
    : QListView( parent, "TimelineListView" )
    , controllers_( controllers )
{
    setMinimumWidth( 200 );
    addColumn( "action count", 0 );
    addColumn( tr( "Units" ) );
    setResizeMode( LastColumn );
    setHScrollBarMode( QScrollView::AlwaysOn ); //--> to have the same height as canvasview
    setSortColumn( -1 ); // $$$$ SBO 2008-04-25: TODO

    connect( this, SIGNAL( selectionChanged( QListViewItem* ) ), SLOT( OnSelectionChange( QListViewItem* ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineListView destructor
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
TimelineListView::~TimelineListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::NotifyCreated
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineListView::NotifyCreated( const Action_ABC& action )
{
    const kernel::Entity_ABC* entity = 0;
    QString name = "Magic";
    if( const ActionTasker* tasker = action.Retrieve< ActionTasker >() )
    {
        entity = tasker->GetTasker();
        name = entity->GetName();
    }

    gui::ValuedListItem* item = gui::FindItem( entity, firstChild() );
    if( !item )
        item = new TimeLineEntityListItem( this, lastItem() );
    item->Set( entity, QString::number( item->text( 0 ).toInt() + 1 ), name );
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::NotifyDeleted
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineListView::NotifyDeleted( const Action_ABC& action )
{
    const kernel::Entity_ABC* entity = 0;
    if( const ActionTasker* tasker = action.Retrieve< ActionTasker >() )
        entity = tasker->GetTasker();

    if( gui::ValuedListItem* item = gui::FindItem( entity, firstChild() ) )
    {
        const int actions = item->text( 0 ).toInt();
        if( actions == 1 )
            delete item;
        else
            item->setText( 0, QString::number( actions - 1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::NotifyDeleted
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineListView::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( gui::ValuedListItem* item = gui::FindItem( &entity, firstChild() ) )
        delete item;    
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::setContentsPos
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineListView::setContentsPos( int x, int y )
{
    blockSignals( true );
    QListView::setContentsPos( x, y );
    blockSignals( false );
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::OnSelectionChange
// Created: SBO 2008-04-29
// -----------------------------------------------------------------------------
void TimelineListView::OnSelectionChange( QListViewItem* item )
{
    if( gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( item ) )
        valuedItem->Select( controllers_.actions_ );
}

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
#include "actions/ActionsFilter_ABC.h"
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
    , filter_( 0 )
{
    setMinimumWidth( 200 );
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
        if( entity )
            name = entity->GetName();
    }
    gui::ValuedListItem* item = gui::FindItem( entity, firstChild() );
    if( !item )
        item = new TimeLineEntityListItem( this, lastItem() );
    item->Set( entity, name );
    item->setVisible( !filter_ || filter_->Allows( action ) );
    actions_[ entity ].push_back( &action );
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
        T_Actions& actions = actions_[ entity ];
        T_Actions::iterator it = std::find( actions.begin(), actions.end(), &action );
        if( it != actions.end() )
            actions.erase( it );
        if( actions.empty() )
        {
            delete item;
            Update();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::NotifyDeleted
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineListView::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( gui::ValuedListItem* item = gui::FindItem( &entity, firstChild() ) )
    {
        actions_.erase( &entity );
        delete item;
    }
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

// -----------------------------------------------------------------------------
// Name: TimelineListView::SetFilter
// Created: SBO 2010-07-19
// -----------------------------------------------------------------------------
void TimelineListView::SetFilter( const actions::ActionsFilter_ABC& filter )
{
    if( filter_ != &filter )
    {
        filter_ = &filter;
        Update();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::Update
// Created: SBO 2010-07-19
// -----------------------------------------------------------------------------
void TimelineListView::Update()
{
    QListViewItemIterator it( this );
    while( it.current() )
    {
        gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( it.current() );
        item->setVisible( ShouldDisplay( *item->GetValue< const kernel::Entity_ABC >() ) );
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::ShouldDisplay
// Created: SBO 2010-07-19
// -----------------------------------------------------------------------------
bool TimelineListView::ShouldDisplay( const kernel::Entity_ABC& entity ) const
{
    if( !filter_ )
        return true;
    T_EntityActions::const_iterator actions = actions_.find( &entity );
    for( T_Actions::const_iterator it = actions->second.begin(); it != actions->second.end(); ++it )
        if( filter_->Allows( **it ) )
            return true;
    return false;
}

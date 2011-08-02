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
#include "clients_kernel/OrderType.h"
#include "clients_gui/ValuedListItem.h"

namespace
{
    class TimeLineEntityListItem : public gui::ValuedListItem
    {
    public:
        TimeLineEntityListItem( Q3ListView* parent, Q3ListViewItem* after ) : gui::ValuedListItem( parent, after )
        {
            setVisible( false );
        }

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
    : Q3ListView( parent, "TimelineListView" )
    , controllers_( controllers )
    , filter_     ( 0 )
{
    setMinimumWidth( 200 );
    addColumn( tr( "Units" ) );
    setBackgroundColor( Qt::white );
    setResizeMode( LastColumn );
    setHScrollBarMode( Q3ScrollView::AlwaysOn ); //--> to have the same height as canvasview
    setSortColumn( -1 ); // $$$$ SBO 2008-04-25: TODO
    gui::ValuedListItem* item = new TimeLineEntityListItem( this, lastItem() );
    item->Set( &Action_ABC::actionTypeMagic_, tr( "Magic" ) );
    item = new TimeLineEntityListItem( this, lastItem() );
    item->Set( &Action_ABC::actionTypeWeather_, tr( "Weather" ) );
    item = new TimeLineEntityListItem( this, lastItem() );
    item->Set( &Action_ABC::actionTypeObjects_, tr( "Objects" ) );
    connect( this, SIGNAL( selectionChanged( Q3ListViewItem* ) ), SLOT( OnSelectionChange( Q3ListViewItem* ) ) );
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
// Name: TimelineListView::FindListItem
// Created: JSR 2011-03-03
// -----------------------------------------------------------------------------
gui::ValuedListItem* TimelineListView::FindListItem( const actions::Action_ABC& action, EActionType& actionType ) const
{
    const ActionTasker* tasker = action.Retrieve< ActionTasker >();
    if( tasker && tasker->GetTasker() )
    {
        actionType = eTypeEntity;
        return gui::FindItem( tasker->GetTasker(), firstChild() );
    }
    const std::string& actionTypeName = action.GetType().GetName();
    if( actionTypeName == "global_weather" || actionTypeName == "local_weather" )
    {
        actionType = eTypeWeather;
        return gui::FindItem( &Action_ABC::actionTypeWeather_, firstChild() );;
    }
    if( actionTypeName == "create_object" || actionTypeName == "update_object" ||
        actionTypeName == "destroy_object" || actionTypeName == "request_object" )
    {
        actionType = eTypeObjects;
        return gui::FindItem( &Action_ABC::actionTypeObjects_, firstChild() );;
    }
    actionType = eTypeMagic;
    return gui::FindItem( &Action_ABC::actionTypeMagic_, firstChild() );;
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::NotifyCreated
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineListView::NotifyCreated( const Action_ABC& action )
{
    EActionType actionType;
    gui::ValuedListItem* item = FindListItem( action, actionType );
    switch( actionType )
    {
    case eTypeEntity :
        {
            const kernel::Entity_ABC* entity = action.Retrieve< ActionTasker >()->GetTasker(); // cannot be null
            if( !item )
            {
                item = new TimeLineEntityListItem( this, lastItem() );
                item->Set( entity, entity->GetName() );
            }
            // $$$$ _RC_ JSR 2011-03-03: on cache toute la ligne si une action n'est pas autorisée?? a vérifier
            item->setVisible( !filter_ || filter_->Allows( action ) );
            entityActions_[ entity ].push_back( &action );
        }
        break;
    case eTypeWeather :
        {
            item->setVisible( !filter_ || filter_->Allows( action ) );
            weatherActions_.push_back( &action );
        }
        break;
    case eTypeObjects :
        {
            item->setVisible( !filter_ || filter_->Allows( action ) );
            objectsActions_.push_back( &action );
        }
        break;
    case eTypeMagic:
    default:
        {
            item->setVisible( !filter_ || filter_->Allows( action ) );
            magicActions_.push_back( &action );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::NotifyDeleted
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineListView::NotifyDeleted( const Action_ABC& action )
{
    EActionType actionType;
    gui::ValuedListItem* item = FindListItem( action, actionType );
    if( !item )
        return;
    T_Actions* actions = 0;
    switch( actionType )
    {
    case eTypeEntity :
        actions = &entityActions_[ action.Retrieve< ActionTasker >()->GetTasker() ]; // cannot be null
        break;
    case eTypeWeather :
        actions = &weatherActions_;
        break;
    case eTypeObjects :
        actions = &objectsActions_;
        break;
    case eTypeMagic:
    default:
        actions = &magicActions_;
    }
    T_Actions::iterator it = std::find( actions->begin(), actions->end(), &action );
    if( it != actions->end() )
        actions->erase( it );
    if( actions->empty() )
    {
        if( actionType == eTypeEntity )
            delete item;
        else
            item->setVisible( false );
        Update();
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
        entityActions_.erase( &entity );
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
    Q3ListView::setContentsPos( x, y );
    blockSignals( false );
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::OnSelectionChange
// Created: SBO 2008-04-29
// -----------------------------------------------------------------------------
void TimelineListView::OnSelectionChange( Q3ListViewItem* item )
{
    gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( item ) ;
    if( valuedItem && valuedItem->IsA< const kernel::Entity_ABC >() )
        valuedItem->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::SetFilter
// Created: SBO 2010-07-19
// -----------------------------------------------------------------------------
void TimelineListView::SetFilter( const ActionsFilter_ABC& filter )
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
    Q3ListViewItemIterator it( this );
    while( it.current() )
    {
        gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( it.current() );
        if( item->IsA< const kernel::Entity_ABC >() )
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
    T_EntityActions::const_iterator actions = entityActions_.find( &entity );
    for( T_Actions::const_iterator it = actions->second.begin(); it != actions->second.end(); ++it )
        if( filter_->Allows( **it ) )
            return true;
    return false;
}

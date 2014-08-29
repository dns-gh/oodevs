// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineView.h"
#include "TimelineMarker.h"
#include "TimelineActionItem.h"
#include "TimelineRuler.h"
#include "icons.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsFilter_ABC.h"
#include "actions/ActionTasker.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Tools.h"
#include "gaming/Model.h"

using namespace kernel;
using namespace actions;

const unsigned int TimelineView::rowHeight_ = 25;

// -----------------------------------------------------------------------------
// Name: TimelineView constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineView::TimelineView( QWidget* parent, Q3Canvas* canvas, Controllers& controllers, Model& model, ActionsScheduler& scheduler, TimelineRuler& ruler )
    : Q3CanvasView( canvas, parent, "TimelineView" )
    , controllers_   ( controllers )
    , model_         ( model )
    , magicVisible_  ( false )
    , weatherVisible_( false )
    , objectsVisible_( false )
    , ruler_         ( ruler )
    , marker_        ( new TimelineMarker( canvas, scheduler, controllers_, ruler_ ) )
    , selectedItem_  ( 0 )
    , filter_        ( 0 )
{
    // initialize some elements needed in action tooltips
    Q3MimeSourceFactory::defaultFactory()->setPixmap( "mission", MAKE_PIXMAP( mission ) );

    viewport()->setMouseTracking( true );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineView destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineView::~TimelineView()
{
    Q3MimeSourceFactory::defaultFactory()->setData( "mission", 0 );
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineView::FindActions
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
TimelineView::T_Actions* TimelineView::FindActions( const actions::Action_ABC& action, actions::EActionType& actionType )
{
    const ActionTasker* tasker = action.Retrieve< ActionTasker >();
    if( tasker )
    {
        if( const kernel::Entity_ABC* entity = model_.FindEntity( tasker->GetId() ) )
        {
            actionType = eTypeEntity;
            std::map< const kernel::Entity_ABC*, T_Actions >::iterator it = entityActions_.find( entity );
            if( it == entityActions_.end() )
                return 0;
            return &it->second;
        }
    }
    if( action.GetType() )
    {
        const std::string& actionTypeName = action.GetType()->GetName();
        if( actionTypeName == "global_weather" || actionTypeName == "local_weather" || actionTypeName == "local_weather_destruction" )
        {
            actionType = eTypeWeather;
            return &weatherActions_;
        }
        if( actionTypeName == "create_object" || actionTypeName == "update_object" ||
                    actionTypeName == "destroy_object" || actionTypeName == "request_object" )
        {
            actionType = eTypeObjects;
            return &objectsActions_;
        }
    }
    actionType = eTypeMagic;
    return &magicActions_;
}

// -----------------------------------------------------------------------------
// Name: TimelineView::NotifyCreated
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::NotifyCreated( const Action_ABC& action )
{
    actions::EActionType actionType;
    T_Actions* actions = FindActions( action, actionType );
    switch( actionType )
    {
    case eTypeEntity :
        if( actions == 0 )
        {
            const kernel::Entity_ABC* entity = model_.FindEntity( action.Get< ActionTasker >().GetId() );
            actions = &entityActions_[ entity ];
            orderedActions_.push_back( entity );
        }
        break;
    case eTypeWeather:
        weatherVisible_ = true;
        break;
    case eTypeObjects:
        objectsVisible_ = true;
        break;
    case eTypeMagic:
    default:
        magicVisible_ = true;
        break;
    }
    ( *actions )[ &action ] = new TimelineActionItem( canvas(), ruler_, controllers_, model_.actions_, action );
    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::NotifyDeleted
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::NotifyDeleted( const Action_ABC& action )
{
    actions::EActionType actionType;
    T_Actions* actions = FindActions( action, actionType );
    if( actions )
    {
        T_Actions::iterator itAction = actions->find( &action );
        if( itAction != actions->end() )
        {
            if( itAction->second == selectedItem_ )
                ClearSelection();
            delete itAction->second;
            actions->erase( itAction );
            if( actions->empty() )
            {
                if( actionType == eTypeEntity )
                    NotifyDeleted( *model_.FindEntity( action.Get< ActionTasker >().GetId() ) );
                else if( actionType == eTypeWeather )
                    weatherVisible_ = false;
                else if( actionType == eTypeObjects )
                    objectsVisible_ = false;
                else if( actionType == eTypeMagic )
                    magicVisible_ = false;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineView::NotifyDeleted
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    T_EntityActions::iterator it = entityActions_.find( &entity );
    if( it != entityActions_.end() )
    {
        for( T_Actions::iterator itAction = it->second.begin(); itAction != it->second.end(); ++itAction )
        {
            if( itAction->second == selectedItem_ )
                ClearSelection();
            delete itAction->second;
        }
        entityActions_.erase( it );
        for( T_OrderedEntities::iterator orderedIt = orderedActions_.begin(); orderedIt != orderedActions_.end(); ++orderedIt )
        {
            if( *orderedIt == &entity )
            {
                orderedActions_.erase( orderedIt );
                break;
            }
        }
        Update();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineView::Update
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::Update()
{
    if( !isVisible() )
        return;
    int row = 0;
    if( magicVisible_ )
        DrawActions( magicActions_, row );
    if( weatherVisible_ )
        DrawActions( weatherActions_, row );
    if( objectsVisible_ )
        DrawActions( objectsActions_, row );
    for( T_OrderedEntities::const_iterator it = orderedActions_.begin(); it != orderedActions_.end(); ++it )
        DrawActions( entityActions_[ *it ], row );
    canvas()->resize( canvas()->width(), rowHeight_ * row );
    marker_->Update();
    canvas()->setAllChanged();
    canvas()->update();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::DrawActions
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
void TimelineView::DrawActions( T_Actions& actions, int& row )
{
    bool actionVisible = false;
    for( T_Actions::iterator itAction = actions.begin(); itAction != actions.end(); ++itAction )
    {
        TimelineActionItem& item = *itAction->second;
        item.setY( row * rowHeight_ );
        item.setSize( item.width(), rowHeight_ );
        const bool visible = !filter_ || filter_->Allows( *itAction->first );
        item.setVisible( visible );
        item.Update();
        actionVisible |= visible;
    }
    if( actionVisible )
        ++row;
}

// -----------------------------------------------------------------------------
// Name: TimelineView::Select
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineView::Select( const QPoint& point )
{
    grabPoint_ = point;
    Q3CanvasItemList list = canvas()->collisions( grabPoint_ );
    if( list.empty() )
        ClearSelection();
    else
        for( Q3CanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
            if( *it != selectedItem_ )
            {
                TimelineItem_ABC* item = dynamic_cast< TimelineItem_ABC* >( *it );
                SetSelected( *item );
                break;
            }
    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::contentsMousePressEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::contentsMousePressEvent( QMouseEvent* evt )
{
    setFocus();
    if( ( evt->button() & Qt::LeftButton ) == Qt::LeftButton )
        Select( evt->pos() );
    if( selectedItem_ )
        ensureVisible( int( selectedItem_->x() ), int( selectedItem_->y() ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineView::contentsMouseMoveEvent
// Created: SBO 2007-07-19
// -----------------------------------------------------------------------------
void TimelineView::contentsMouseMoveEvent( QMouseEvent* evt )
{
    if( evt->buttons() & Qt::LeftButton )
    {
        if( !selectedItem_ )
            Select( grabPoint_ );
        if( selectedItem_ )
        {
            selectedItem_->Move( evt->pos().x() - grabPoint_.x() );
            ensureVisible( int( selectedItem_->x() ), int( selectedItem_->y() ) );
            grabPoint_ = evt->pos();
            setCursor( Qt::sizeHorCursor );
        }
    }
    Update();
    Q3CanvasItemList list = canvas()->collisions( evt->pos() );
    if( list.empty() )
        QToolTip::remove( this );
    else if( const TimelineItem_ABC* item = dynamic_cast< const TimelineItem_ABC* >( *list.begin() ) )
        item->DisplayToolTip( this );
}

// -----------------------------------------------------------------------------
// Name: TimelineView::contentsMouseReleaseEvent
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void TimelineView::contentsMouseReleaseEvent( QMouseEvent* )
{
    if( selectedItem_ == marker_ )
        marker_->CommitMove();
    setCursor( Qt::arrowCursor );
}

// -----------------------------------------------------------------------------
// Name: TimelineView::contentsContextMenuEvent
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineView::contentsContextMenuEvent( QContextMenuEvent* evt )
{
    if( !selectedItem_ )
        Select( evt->pos() );
    kernel::ContextMenu* menu = new kernel::ContextMenu( this );
    connect( menu, SIGNAL( aboutToHide() ), menu, SLOT( deleteLater() ) );
    if( selectedItem_ )
    {
        selectedItem_->DisplayContextMenu( menu );
        menu->insertSeparator();
    }
    menu->insertItem( tools::translate( "TimelineView", "Zoom In" ), &ruler_, SLOT( ZoomIn() ), Qt::Key_Plus );
    menu->insertItem( tools::translate( "TimelineView", "Zoom Out" ), &ruler_, SLOT( ZoomOut() ), Qt::Key_Minus );
    menu->popup( evt->globalPos() );
    evt->accept();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::keyPressEvent
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::keyPressEvent( QKeyEvent* evt )
{
    if( evt->key() == Qt::Key_Plus )
        ruler_.ZoomIn();
    else if( evt->key() == Qt::Key_Minus )
        ruler_.ZoomOut();
    if( selectedItem_ && selectedItem_->isActive() )
    {
        if( evt->key() == Qt::Key_Delete )
            selectedItem_->Delete();
        else if( evt->key() == Qt::Key_Left || evt->key() == Qt::Key_Right )
        {
            const short sign = evt->key() == Qt::Key_Left ? -1 : 1;
            const long seconds = ( evt->modifiers() & Qt::ShiftModifier ) ? 3600 * 24 : 3600;
            selectedItem_->Move( ruler_.ConvertToPixels( sign * seconds ) );
        }
    }
    Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::ClearSelection
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::ClearSelection()
{
    if( selectedItem_ )
    {
        selectedItem_->setSelected( false );
        controllers_.actions_.Select( (const actions::Action_ABC*)0 );
    }
    selectedItem_ = 0;
}

// -----------------------------------------------------------------------------
// Name: TimelineView::SetSelected
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineView::SetSelected( TimelineItem_ABC& item )
{
    if( selectedItem_ )
        selectedItem_->setSelected( false );
    item.setSelected( true );
    selectedItem_ = &item;
}

// -----------------------------------------------------------------------------
// Name: TimelineView::NotifySelected
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
void TimelineView::NotifySelected( const actions::Action_ABC* action )
{
    if( action )
    {
        actions::EActionType actionType;
        T_Actions* actions = FindActions( *action, actionType );
        if( actions )
        {
            T_Actions::iterator itAction = actions->find( action );
            if( itAction != actions->end() )
            {
                if( selectedItem_ != itAction->second )
                    SetSelected( *itAction->second );
                return;
            }
        }
    }
    ClearSelection();
}

// -----------------------------------------------------------------------------
// Name: TimelineView::setContentsPos
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineView::setContentsPos( int x, int y )
{
    bool wasBlocked = blockSignals( true );
    Q3CanvasView::setContentsPos( x, y );
    blockSignals( wasBlocked );
}

// -----------------------------------------------------------------------------
// Name: TimelineView::SetFilter
// Created: SBO 2010-05-06
// -----------------------------------------------------------------------------
void TimelineView::SetFilter( const actions::ActionsFilter_ABC& filter )
{
    if( filter_ != &filter )
    {
        filter_ = &filter;
        Update();
    }
}

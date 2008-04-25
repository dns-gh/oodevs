// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineEntityItem.h"
#include "TimelineView.h"
#include "TimelineActionItem.h"
#include "gaming/Action_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controllers.h"
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineEntityItem::TimelineEntityItem( TimelineView& view, kernel::Controllers& controllers, const kernel::Entity_ABC& entity )
    : TimelineItem_ABC( view.canvas(), QRect( 0, 0, view.canvas()->width(), 25 ) ) // $$$$ SBO 2008-04-23: 
    , view_( view )
    , controllers_( controllers )
    , entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineEntityItem::~TimelineEntityItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::MoveAfter
// Created: SBO 2008-04-23
// -----------------------------------------------------------------------------
void TimelineEntityItem::MoveAfter( const TimelineItem_ABC* previous )
{
    if( previous )
        setY( previous->y() + previous->height() );
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::Update
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void TimelineEntityItem::Update()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::AddAction
// Created: SBO 2008-04-23
// -----------------------------------------------------------------------------
void TimelineEntityItem::AddAction( const Action_ABC& action )
{
    if( &action.GetEntity() != &entity_ )
        return;
    TimelineItem_ABC*& item = childItems_[ &action ];
    if( !item )
        item = new TimelineActionItem( view_, *this, controllers_, action );
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::RemoveAction
// Created: SBO 2008-04-23
// -----------------------------------------------------------------------------
void TimelineEntityItem::RemoveAction( const Action_ABC& action )
{
    if( &action.GetEntity() != &entity_ )
        return;
    T_Items::iterator it = childItems_.find( &action );
    if( it != childItems_.end() )
    {
        delete it->second;
        childItems_.erase( it );
    }
}

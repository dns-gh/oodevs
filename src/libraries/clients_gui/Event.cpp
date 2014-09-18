// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Event.h"

#include "clients_kernel/ActionController.h"
#include "clients_kernel/TimelineHelpers.h"

#include "ENT/ENT_Tr.h"

#include <timeline/api.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Event constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
Event::Event( E_EventTypes type, const timeline::Event& event )
    : type_( type )
    , event_( new timeline::Event( event ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Event destructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
Event::~Event()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Event::GetType
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
E_EventTypes Event::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Event::GetEvent
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
timeline::Event& Event::GetEvent() const
{
    return *event_;
}

// -----------------------------------------------------------------------------
// Name: Event::Clone
// Created: ABR 2013-06-03
// -----------------------------------------------------------------------------
Event* Event::Clone() const
{
    return new Event( type_, *event_ );
}

// -----------------------------------------------------------------------------
// Name: Event::Update
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
void Event::Update()
{
    if( !event_ )
        throw MASA_EXCEPTION( "Event not set");
    Update( *event_ );
}

// -----------------------------------------------------------------------------
// Name: Event::Update
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void Event::Update( const timeline::Event& event )
{
    *event_ = event;
}

// -----------------------------------------------------------------------------
// Name: Event::GetName
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
QString Event::GetName() const
{
    if( event_ )
        return QString::fromStdString( event_->name );
    return "";
}

// -----------------------------------------------------------------------------
// Name: Event::GetTooltip
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
QString Event::GetTooltip() const
{
    if( event_ )
        return QString::fromStdString( event_->name );
    return "";
}

// -----------------------------------------------------------------------------
// Name: Event::Select
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void Event::Select( kernel::ActionController& controller ) const
{
    controller.Select( *this );
}

// -----------------------------------------------------------------------------
// Name: Event::MultipleSelect
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void Event::MultipleSelect( kernel::ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    controller.MultipleSelect( elements );
}

// -----------------------------------------------------------------------------
// Name: Event::ContextMenu
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void Event::ContextMenu( kernel::ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, where );
}

// -----------------------------------------------------------------------------
// Name: Event::Activate
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void Event::Activate( kernel::ActionController& controller ) const
{
    controller.Activate( *this );
}

// -----------------------------------------------------------------------------
// Name: Event::OverFly
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void Event::OverFly( kernel::ActionController& controller ) const
{
    controller.OverFly( *this );
}

// -----------------------------------------------------------------------------
// Name: Event::GetAction
// Created: ABR 2014-01-14
// -----------------------------------------------------------------------------
const actions::Action_ABC* Event::GetAction() const
{
    return 0;
}

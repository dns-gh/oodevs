// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionPublisher.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "protocol/Protocol.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionPublisher constructor
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
ActionPublisher::ActionPublisher( Publisher_ABC& publisher, kernel::Controllers& controllers )
    : controllers_( controllers )
    , publisher_( publisher )
    , design_( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher destructor
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
ActionPublisher::~ActionPublisher()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToSim& msg )
{
    if( !design_ )
        publisher_.Send( msg );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Register
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void ActionPublisher::Register( T_Handler /*handler*/ )
{
    throw MASA_EXCEPTION( "not to be called." );;
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToAuthentication& /*message*/ )
{
    throw MASA_EXCEPTION( "not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToReplay& /*message*/ )
{
    throw MASA_EXCEPTION( "not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToAar& /*message*/ )
{
    throw MASA_EXCEPTION( "not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToMessenger& /*message*/ )
{
    throw MASA_EXCEPTION( "not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::NotifyModeChanged
// Created: LGY 2012-06-22
// -----------------------------------------------------------------------------
void ActionPublisher::NotifyModeChanged( E_Modes newMode )
{
    design_ = newMode == eModes_Planning;
}


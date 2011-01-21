// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionPublisher.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "protocol/Protocol.h"

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
    if( !design_ || force_ )
        publisher_.Send( msg );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToAuthentication& /*message*/ )
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToReplay& /*message*/ )
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToAar& /*message*/ )
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Send
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::Send( const sword::ClientToMessenger& /*message*/ )
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::OptionChanged
// Created: SBO 2010-03-17
// -----------------------------------------------------------------------------
void ActionPublisher::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "DesignMode" )
        design_ = value.To< bool >();
}

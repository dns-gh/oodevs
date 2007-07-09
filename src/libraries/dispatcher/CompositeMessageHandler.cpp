// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "CompositeMessageHandler.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: CompositeMessageHandler constructor
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
CompositeMessageHandler::CompositeMessageHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositeMessageHandler destructor
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
CompositeMessageHandler::~CompositeMessageHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositeMessageHandler::Add
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void CompositeMessageHandler::Add( boost::shared_ptr< MessageHandler_ABC > handler )
{
    handlers_.push_back( handler );
}

// -----------------------------------------------------------------------------
// Name: CompositeMessageHandler::Receive
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void CompositeMessageHandler::Receive( const ASN1T_MsgsSimToClient& message )
{
    for( CIT_Handlers it = handlers_.begin(); it != handlers_.end(); ++it )
        (*it)->Receive( message );
}

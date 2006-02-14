// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Agent.h"
#include "AutomatType.h"

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const ASN1T_MsgAutomateCreation& message, const Resolver_ABC< AutomatType >& resolver )
    : automatType_( & resolver.Get( message.type_automate ) )
    , type_( automatType_->GetTypePC() )
    , name_( message.nom )
    , id_( message.oid_automate )
{
    // $$$$ AGE 2006-02-14: 
//    message.oid_camp;
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const ASN1T_MsgPionCreation& message, const Resolver_ABC< AgentType >& resolver )
    : type_( & resolver.Get( message.type_pion ) )
    , name_( message.nom )
    , id_( message.oid_pion )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
std::string Agent::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long Agent::GetId() const
{
    return id_;
}

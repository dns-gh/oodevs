// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanPerceptions.h"

#include "clients_kernel/Automat_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanPerceptions constructor
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
UrbanPerceptions::UrbanPerceptions( Controller& controller, const tools::Resolver_ABC< Automat_ABC >& agentResolver )
    : controller_( controller )
    , agentResolver_( agentResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPerceptions destructor
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
UrbanPerceptions::~UrbanPerceptions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPerceptions::DoUpdate
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanPerceptions::DoUpdate( const ASN1T_MsgUrbanKnowledgeUpdate& message )
{
    if( message.m.automat_perceptionPresent )
    {
        detectingAutomats_.clear();
        detectingAutomats_.reserve( message.automat_perception.n );
        for( uint i = 0; i < message.automat_perception.n; ++i )
            detectingAutomats_.push_back( & agentResolver_.Get( message.automat_perception.elem[i] ) );
    }
}

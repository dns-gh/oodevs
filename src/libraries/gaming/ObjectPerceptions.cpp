// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectPerceptions.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectPerceptions constructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
ObjectPerceptions::ObjectPerceptions( Controller& controller, const Resolver_ABC< Automat_ABC >& agentResolver )
    : controller_( controller )
    , agentResolver_( agentResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPerceptions destructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
ObjectPerceptions::~ObjectPerceptions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPerceptions::DoUpdate
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void ObjectPerceptions::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    if( message.m.automat_perceptionPresent )
    {
        detectingAutomats_.clear();
        detectingAutomats_.reserve( message.automat_perception.n );
        for( uint i = 0; i < message.automat_perception.n; ++i )
            detectingAutomats_.push_back( & agentResolver_.Get( message.automat_perception.elem[i] ) );
    }
}

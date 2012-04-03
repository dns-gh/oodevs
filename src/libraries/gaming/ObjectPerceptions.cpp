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
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectPerceptions constructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
ObjectPerceptions::ObjectPerceptions( Controller& controller, const tools::Resolver_ABC< Automat_ABC >& agentResolver )
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
void ObjectPerceptions::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    if( message.has_perceiving_automats()  )
    {
        detectingAutomats_.clear();
        detectingAutomats_.reserve( message.perceiving_automats().elem_size() );
        for( int i = 0; i < message.perceiving_automats().elem_size(); ++i )
            detectingAutomats_.push_back( & agentResolver_.Get( message.perceiving_automats().elem( i ).id() ) );
    }
}

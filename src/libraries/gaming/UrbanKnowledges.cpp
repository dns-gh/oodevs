// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanKnowledges.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/UrbanKnowledge_ABC.h"
#include "UrbanKnowledgeFactory.h"
#include "protocol/Simulation.h"

#pragma warning( disable : 4355 )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledges constructor
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
UrbanKnowledges::UrbanKnowledges( const Team_ABC& team, Controller& controller, UrbanKnowledgeFactory& factory )
    : Creatable< UrbanKnowledges >( controller, this )
    , team_      ( team )
    , controller_( controller )
    , factory_   ( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledges destructor
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
UrbanKnowledges::~UrbanKnowledges()
{
    controller_.Delete( *this );
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledges::DoUpdate
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanKnowledges::DoUpdate( const sword::UrbanKnowledgeCreation& message )
{
    if( ! Find( message.knowledge().id() ) )
        Register( message.knowledge().id(), *factory_.Create( team_, message ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledges::DoUpdate
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanKnowledges::DoUpdate( const sword::UrbanKnowledgeUpdate& message )
{
    Get( message.knowledge().id() ).Update( message );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledges::DoUpdate
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanKnowledges::DoUpdate( const sword::UrbanKnowledgeDestruction& message )
{
    delete Find( message.knowledge().id() );
    Remove( message.knowledge().id() );
    controller_.Update( *this );
}

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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledges constructor
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
UrbanKnowledges::UrbanKnowledges( const Team_ABC& team, Controller& controller, UrbanKnowledgeFactory& factory )
    : Creatable< UrbanKnowledges >( controller, this )
    , team_( team )
    , controller_( controller )
    , factory_( factory )
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
void UrbanKnowledges::DoUpdate( const ASN1T_MsgUrbanKnowledgeCreation& message )
{
    if( ! Find( message.oid ) )
        Register( message.oid, * factory_.Create( team_, message ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledges::DoUpdate
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanKnowledges::DoUpdate( const ASN1T_MsgUrbanKnowledgeUpdate& message )
{
    Get( message.oid ).Update( message );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledges::DoUpdate
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanKnowledges::DoUpdate( const ASN1T_MsgUrbanKnowledgeDestruction& message )
{
    delete Find( message.oid );
    Remove( message.oid );
    controller_.Update( *this );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TeamsModel.h"
#include "DIN/DIN_Input.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "TeamFactory_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
TeamsModel::TeamsModel( TeamFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
TeamsModel::~TeamsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void TeamsModel::Purge()
{
    Resolver< Formation_ABC >::DeleteAll();
    Resolver< Team_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateTeam
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void TeamsModel::CreateTeam( const ASN1T_MsgTeamCreation& asnMsg )
{
    if( ! Resolver< Team_ABC >::Find( asnMsg.oid ) )
    {
        Team_ABC* team = factory_.CreateTeam( asnMsg );
        Resolver< Team_ABC >::Register( asnMsg.oid, *team );
    }
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateFormation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void TeamsModel::CreateFormation( const ASN1T_MsgFormationCreation& asnMsg )
{
    if( ! Resolver< Formation_ABC >::Find( asnMsg.oid ) )
    {
        Formation_ABC* formation = factory_.CreateFormation( asnMsg );
        Resolver< Formation_ABC >::Register( asnMsg.oid, *formation );
    }
}
 
// -----------------------------------------------------------------------------
// Name: TeamsModel::GetTeam
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Team_ABC& TeamsModel::GetTeam( unsigned long id )
{
    return Resolver< Team_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::FindTeam
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Team_ABC* TeamsModel::FindTeam( const QString& team )
{
    for( Resolver< Team_ABC >::CIT_Elements it = Resolver< Team_ABC >::elements_.begin(); it != Resolver< Team_ABC >::elements_.end(); ++it )
        if( it->second->GetName() == team )
            return it->second;
    return 0;
}

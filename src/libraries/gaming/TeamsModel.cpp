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
    : factory_   ( factory )
    , noSideTeam_( factory.CreateNoSideTeam() )
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
    tools::Resolver< Formation_ABC >::DeleteAll();
    tools::Resolver< Team_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateTeam
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void TeamsModel::CreateTeam( const sword::PartyCreation& message )
{
    if( ! tools::Resolver< Team_ABC >::Find( message.party().id() ) )
    {
        Team_ABC* team = factory_.CreateTeam( message );
        tools::Resolver< Team_ABC >::Register( message.party().id(), *team );
    }
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateFormation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void TeamsModel::CreateFormation( const sword::FormationCreation& message )
{
    if( ! tools::Resolver< Formation_ABC >::Find( message.formation().id() ) )
    {
        Formation_ABC* formation = factory_.CreateFormation( message );
        tools::Resolver< Formation_ABC >::Register( message.formation().id(), *formation );
    }
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::DestroyFormation
// Created: LDC 2010-11-02
// -----------------------------------------------------------------------------
void TeamsModel::DestroyFormation( const sword::FormationDestruction& message )
{
    delete tools::Resolver< Formation_ABC >::Find( message.formation().id() );
    tools::Resolver< Formation_ABC >::Remove( message.formation().id() );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::GetTeam
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Team_ABC& TeamsModel::GetTeam( unsigned long id )
{
    if( id != 0 )
        return tools::Resolver< Team_ABC >::Get( id );
    return *noSideTeam_;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::FindTeam
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Team_ABC* TeamsModel::FindTeam( const QString& team )
{
    for( tools::Resolver< Team_ABC >::CIT_Elements it = tools::Resolver< Team_ABC >::elements_.begin(); it != tools::Resolver< Team_ABC >::elements_.end(); ++it )
        if( it->second->GetName() == team )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::GetNoSideTeam
// Created: JSR 2011-11-10
// -----------------------------------------------------------------------------
const kernel::Team_ABC& TeamsModel::GetNoSideTeam() const
{
    return *noSideTeam_;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::GetFormation
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
kernel::Formation_ABC& TeamsModel::GetFormation( unsigned long id )
{
    return tools::Resolver< Formation_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::FindFormation
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
kernel::Formation_ABC* TeamsModel::FindFormation( unsigned long id )
{
    return tools::Resolver< Formation_ABC >::Find( id );
}

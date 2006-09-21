// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamsModel.h"
#include "Team.h"
#include "TeamFactory_ABC.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: TeamsModel constructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
TeamsModel::TeamsModel( Controllers& controllers, TeamFactory_ABC& factory )
    : controllers_( controllers )
    , factory_( factory )
{
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: TeamsModel destructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
TeamsModel::~TeamsModel()
{
    controllers_.Remove( *this );
}
 
// -----------------------------------------------------------------------------
// Name: TeamsModel::Purge
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void TeamsModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateTeam
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void TeamsModel::CreateTeam()
{
    Team_ABC* team = factory_.CreateTeam();
    Register( team->GetId(), *team );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateKnowledgeGroup
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void TeamsModel::CreateKnowledgeGroup( const kernel::Team_ABC& team )
{
    static_cast< Team& >( Get( team.GetId() ) ).CreateKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::FindTeam
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Team_ABC* TeamsModel::FindTeam( const QString& name ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->GetName() == name )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::FindKnowledgeGroup
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC* TeamsModel::FindKnowledgeGroup( const unsigned long& id ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        Resolver_ABC< KnowledgeGroup_ABC >& team = *it->second;
        KnowledgeGroup_ABC* group = team.Find( id );
        if( group )
            return group;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::NotifyDeleted
// Created: SBO 2006-09-05
// -----------------------------------------------------------------------------
void TeamsModel::NotifyDeleted( const Team_ABC& team )
{
    Remove( team.GetId() );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::Serialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void TeamsModel::Serialize( xml::xostream& xos ) const
{
    xos << start( "sides" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "side" );
        it->second->Serialize( xos );
        xos << end();
    }
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: Iterator< const kernel::Entity_ABC& > TeamsModel::CreateEntityIterator
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
Iterator< const Entity_ABC& > TeamsModel::CreateEntityIterator() const
{
    return new AssociativeIterator< const Entity_ABC&,Resolver< Team_ABC >::T_Elements >( elements_ );
}

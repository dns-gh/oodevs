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
#include "Model.h"
#include "FormationModel.h"
#include "AgentsModel.h"
#include "Diplomacies.h"
#include "Exceptions.h"
#include "Tools.h"
#include "IntelligencesModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
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
    controllers_.Unregister( *this );
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
    CreateKnowledgeGroup( *team );
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
// Name: TeamsModel::CreateObject
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
Object_ABC* TeamsModel::CreateObject( const kernel::Team_ABC& team, kernel::ObjectType& type, const QString& name, const Enum_ObstacleType& obstacleType, bool reservedObstacleActivated, const kernel::Location_ABC& location )
{
    return static_cast< Team& >( Get( team.GetId() ) ).CreateObject( type, name, obstacleType, reservedObstacleActivated, location );
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
    for( Resolver< Team_ABC >::CIT_Elements it = Resolver< Team_ABC >::elements_.begin(); it != Resolver< Team_ABC >::elements_.end(); ++it )
    {
        Team_ABC& team = *it->second;
        const CommunicationHierarchies& hierarchies = team.Get< CommunicationHierarchies >();
        Iterator< const Entity_ABC& > subIt = hierarchies.CreateSubordinateIterator();
        while( subIt.HasMoreElements() )
        {
            const Entity_ABC& kg = subIt.NextElement();
            if( kg.GetId() == id )
                // $$$$ AGE 2006-10-09: 
                return const_cast< KnowledgeGroup_ABC* >( static_cast< const KnowledgeGroup_ABC* >( &kg ) );
        };
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
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        it->second->Interface().Apply( & Serializable_ABC::SerializeLogistics, xos );
        xos << end();
    }
    xos << end();

    xos << start( "diplomacies" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "side" );
        it->second->Get< Diplomacies >().Serialize( xos );
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

// -----------------------------------------------------------------------------
// Name: TeamsModel::Load
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void TeamsModel::Load( const std::string& filename, Model& model )
{
    Purge();

    xifstream xis( filename );
    xis >> start( "orbat" )
            >> start( "sides" )
                >> list( "side", *this, &TeamsModel::ReadTeam, model )
            >> end()
            >> start( "diplomacies" )
                >> list( "side", *this, &TeamsModel::ReadDiplomacy )
            >> end()
        >> end();

    CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::ReadTeam
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void TeamsModel::ReadTeam( xml::xistream& xis, Model& model )
{
    Team_ABC* team = factory_.CreateTeam( xis );
    Register( team->GetId(), *team );

    // $$$$ SBO 2006-10-05: forward to communications extension?
    xis >> start( "communication" )
            >> list( "knowledge-group", static_cast< Team& >( *team ), &Team::CreateKnowledgeGroup )
        >> end();
    xis >> start( "tactical" )
            >> list( "formation", model.formations_, &FormationModel::Create, *team, model )
        >> end();
    xis >> start( "logistic" )
            >> list( "automat", model.agents_, &AgentsModel::ReadLogistic )
        >> end();
    xis >> start( "objects" )
            >> list( "object", static_cast< Team& >( *team ), &Team::CreateObject )
        >> end();
    xis >> start( "populations" )
            >> list( "population", model.agents_, &AgentsModel::CreatePopulation, *team )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::ReadDiplomacy
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void TeamsModel::ReadDiplomacy( xml::xistream& xis )
{
    int id;
    xis >> attribute( "id", id );
    Get( id ).Get< Diplomacies >().Load( xis );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CheckValidity
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void TeamsModel::CheckValidity() const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const CommunicationHierarchies& hierarchies = it->second->Get< CommunicationHierarchies >();
        Iterator< const Entity_ABC& > itSub = hierarchies.CreateSubordinateIterator();
        if( !itSub.HasMoreElements() )
            throw InvalidModelException( tools::translate( "Preparation", "Communication model" )
                                       , tools::translate( "Preparation", "Team '%1' has no knowledge group" ).arg( it->second->GetName() ) );
        while( itSub.HasMoreElements() )
        {
            const Entity_ABC* entity = &itSub.NextElement();
            if( !dynamic_cast< const KnowledgeGroup_ABC* >( entity ) )
                throw InvalidModelException( tools::translate( "Preparation", "Communication model" )
                                           , tools::translate( "Preparation", "Unit '%1' in team '%2' has no knowledge group" ).arg( entity->GetName() ).arg( it->second->GetName() ) );
        }
    }
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamsModel.h"
#include "Team.h"
#include "KnowledgeGroupsModel.h"
#include "TeamFactory_ABC.h"
#include "Model.h"
#include "FormationModel.h"
#include "AgentsModel.h"
#include "Diplomacies.h"
#include "Tools.h"
#include "IntelligencesModel.h"
#include "ModelChecker_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamsModel constructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
TeamsModel::TeamsModel( Controllers& controllers, TeamFactory_ABC& factory )
    : controllers_      ( controllers )
    , factory_          ( factory )
    , infiniteDotations_( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel destructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
TeamsModel::~TeamsModel()
{
    Purge();
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
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateObject
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
Object_ABC* TeamsModel::CreateObject( const kernel::Team_ABC& team, const kernel::ObjectType& type, const QString& name, const kernel::Location_ABC& location )
{
    return static_cast< Team& >( Get( team.GetId() ) ).CreateObject( type, name, location );
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
    xos << xml::start( "dotations" )
            << xml::attribute( "infinite", infiniteDotations_ )
        << xml::end;

    xos << xml::start( "sides" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "side" );
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        it->second->Interface().Apply( & Serializable_ABC::SerializeLogistics, xos );
        xos << xml::end;
    }
    xos << xml::end;

    xos << xml::start( "diplomacies" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "side" );
        static_cast< const Diplomacies& >( it->second->Get< kernel::Diplomacies_ABC >() ).Serialize( xos ); // $$$$ SBO 2008-12-09: !
        xos << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateEntityIterator
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
tools::Iterator< const Entity_ABC& > TeamsModel::CreateEntityIterator() const
{
    return new tools::AssociativeIterator< const Entity_ABC&, tools::Resolver< Team_ABC >::T_Elements >( elements_ );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::Load
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void TeamsModel::Load( xml::xistream& xis, Model& model, std::string& loadingErrors )
{
    xis >> xml::start( "orbat" )
            >> xml::optional >> xml::start( "dotations" )
                >> xml::attribute( "infinite", infiniteDotations_ )
            >> xml::end
            >> xml::start( "sides" )
                >> xml::list( "side", *this, &TeamsModel::ReadTeam, model, loadingErrors )
            >> xml::end
            >> xml::start( "diplomacies" )
                >> xml::list( "side", *this, &TeamsModel::ReadDiplomacy )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::ReadTeam
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void TeamsModel::ReadTeam( xml::xistream& xis, Model& model, std::string& loadingErrors )
{
    Team_ABC* team = factory_.CreateTeam( xis );
    Register( team->GetId(), *team );

    // $$$$ SBO 2006-10-05: forward to communications extension?
    xis >> xml::start( "communication" )
            >> xml::list( "knowledge-group", model.knowledgeGroups_, &KnowledgeGroupsModel::Create, *team, model )
        >> xml::end;
    xis >> xml::start( "tactical" )
            >> xml::list( "formation", model.formations_, &FormationModel::Create, *team, model, loadingErrors )
        >> xml::end;
    xis >> xml::start( "logistic" )
            >> xml::list( "automat", model.agents_, &AgentsModel::ReadLogistic )
        >> xml::end;
    xis >> xml::start( "objects" )
            >> xml::list( "object", static_cast< Team& >( *team ), &Team::CreateObject, loadingErrors  )
        >> xml::end;
    xis >> xml::start( "populations" )
            >> xml::list( "population", model.agents_, &AgentsModel::CreatePopulation, *team )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::ReadDiplomacy
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void TeamsModel::ReadDiplomacy( xml::xistream& xis )
{
    int id;
    xis >> xml::attribute( "id", id );
    static_cast< Diplomacies& >( Get( id ).Get< Diplomacies_ABC >() ).Load( xis ); // $$$$ SBO 2008-12-09: !
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CheckValidity
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
bool TeamsModel::CheckValidity( ModelChecker_ABC& checker ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const CommunicationHierarchies& hierarchies = it->second->Get< CommunicationHierarchies >();
        tools::Iterator< const Entity_ABC& > itSub = hierarchies.CreateSubordinateIterator();
        while( itSub.HasMoreElements() )
        {
            const Entity_ABC* entity = &itSub.NextElement();
            if( !dynamic_cast< const KnowledgeGroup_ABC* >( entity ) )
                return checker.Reject( QString( "%1:\n%2" )
                                      .arg( tools::translate( "Preparation", "Communication model" ) )
                                      .arg( tools::translate( "Preparation", "Unit '%1' in team '%2' has no knowledge group" ).arg( entity->GetName() ).arg( it->second->GetName() ) ) );
        }
    }
    return checker.Validate();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::InfiniteDotations
// Created: JSR 2010-05-04
// -----------------------------------------------------------------------------
bool& TeamsModel::InfiniteDotations()
{
    return infiniteDotations_;
}

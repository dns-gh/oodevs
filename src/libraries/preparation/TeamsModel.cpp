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
#include "KnowledgeGroupsModel.h"
#include "TeamFactory_ABC.h"
#include "Model.h"
#include "FormationModel.h"
#include "ObjectsModel.h"
#include "AgentsModel.h"
#include "Diplomacies.h"
#include "Tools.h"
#include "ModelChecker_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/LogisticHierarchies.h"
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
// Name: TeamsModel::FindTeam
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
Team_ABC* TeamsModel::FindTeam( unsigned int id ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->GetId() == id )
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
    if( infiniteDotations_ )
        xos << xml::start( "resources" )
                << xml::attribute( "infinite", infiniteDotations_ )
            << xml::end;
    xos << xml::start( "parties" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "party" );
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        it->second->Interface().Apply( & Serializable_ABC::SerializeLogistics, xos );
        xos << xml::end;
    }
    xos << xml::end;
    xos << xml::start( "diplomacy" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "party" );
        static_cast< const Diplomacies& >( it->second->Get< Diplomacies_ABC >() ).Serialize( xos ); // $$$$ SBO 2008-12-09: !
        xos << xml::end;
    }
    /*if( !undergroundNetworks_.empty() )
    {
        xos << xml::start( "underground-networks" );
        for( std::map< std::string, unsigned int >::const_iterator it = undergroundNetworks_.begin(); it != undergroundNetworks_.end(); ++it )
            xos << xml::start( "underground-network" )
                << xml::attribute( "name", it->first )
                << xml::end
        xos << xml::end;
    }*/
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
            >> xml::optional >> xml::start( "resources" )
                >> xml::attribute( "infinite", infiniteDotations_ )
            >> xml::end
            >> xml::start( "parties" )
                >> xml::list( "party", *this, &TeamsModel::ReadTeam, model, loadingErrors )
            >> xml::end
            >> xml::start( "diplomacy" )
                >> xml::list( "party", *this, &TeamsModel::ReadDiplomacy )
            >> xml::end
            /*>> xml::optional >> xml::start( "underground-networks" )
            << xml::list( "underground-network", *this, &TeampsModel::ReadUndergroundNetwork )
                << xml::attribute( "name", *it )
                << xml::end
            xos << xml::end;*/
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
            >> xml::list( "knowledge-group", model.knowledgeGroups_, &KnowledgeGroupsModel::Create, *team, model, loadingErrors )
        >> xml::end;
    xis >> xml::start( "tactical" )
            >> xml::list( "formation", model.formations_, &FormationModel::Create, *team, model, loadingErrors )
        >> xml::end;
    xis >> xml::start( "logistics" )
            >> xml::list( "logistic-base", *this, &TeamsModel::ReadLogistic, model )
        >> xml::end;
    xis >> xml::start( "objects" )
            >> xml::list( "object", model.objects_, &ObjectsModel::CreateObject, *team, loadingErrors  )
        >> xml::end;
    xis >> xml::start( "populations" )
            >> xml::list( "population", model.agents_, &AgentsModel::CreatePopulation, *team, loadingErrors )
        >> xml::end;
    xis >> xml::optional >> xml::start( "inhabitants" )
        >> xml::list( "inhabitant", model.agents_, &AgentsModel::CreateInhabitant, *team, loadingErrors )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::ReadDiplomacy
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void TeamsModel::ReadDiplomacy( xml::xistream& xis )
{
    static_cast< Diplomacies& >( Get( xis.attribute< int >( "id" ) ).Get< Diplomacies_ABC >() ).Load( xis ); // $$$$ SBO 2008-12-09: !
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::ReadLogistic
// Created: VGS 2011-05-31
// -----------------------------------------------------------------------------
void TeamsModel::ReadLogistic( xml::xistream& xis, Model& model )
{
    int id;
    xis >> xml::attribute( "id", id );
    Entity_ABC* entity = model.formations_.Find( id );
    if( !entity )
        entity = model.agents_.FindAutomat( id );
    if( entity )
        xis >> xml::list( "subordinate", *this, &TeamsModel::ReadLogisticLink, model, *entity );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::ReadLogisticLink
// Created: VGS 2011-05-31
// -----------------------------------------------------------------------------
void TeamsModel::ReadLogisticLink( xml::xistream& xis, Model& model, kernel::Entity_ABC& superior )
{
    int id;
    xis >> xml::attribute( "id", id );
    Entity_ABC* entity = model.formations_.Find( id );
    if( !entity )
        entity = model.agents_.FindAutomat( id );
    if( entity )
    {
        LogisticHierarchiesBase* hierarchies = entity->Retrieve< LogisticHierarchiesBase >();
        if( hierarchies )
            hierarchies->Load( xis, &superior );
    }
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

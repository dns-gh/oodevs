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
#include "LogisticHierarchiesBase.h"
#include "ObjectsModel.h"
#include "AgentsModel.h"
#include "Diplomacies.h"
#include "GhostModel.h"
#include "Objects.h"
#include "Object.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
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
    , noSideTeam_       ( factory.CreateNoSideTeam() )
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
    noSideTeam_->Retrieve< Objects >()->DeleteAll();
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
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
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
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->GetId() == id )
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
    xos << xml::start( "parties" );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "party" );
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        it->second->Interface().Apply( & Serializable_ABC::SerializeLogistics, xos );
        xos << xml::end;
    }
    if( noSideTeam_->Get< Objects >().Count() )
    {
        xos << xml::start( "no-party" );
        noSideTeam_->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        xos << xml::end;
    }
    xos << xml::end;
    xos << xml::start( "diplomacy" );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "party" );
        static_cast< const Diplomacies& >( it->second->Get< Diplomacies_ABC >() ).Serialize( xos ); // $$$$ SBO 2008-12-09: !
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
void TeamsModel::Load( xml::xistream& xis, Model& model )
{
    xis >> xml::start( "orbat" )
            >> xml::start( "parties" )
                >> xml::list( *this, &TeamsModel::ReadTeam, model )
            >> xml::end
            >> xml::start( "diplomacy" )
                >> xml::list( "party", *this, &TeamsModel::ReadDiplomacy )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::ReadTeam
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void TeamsModel::ReadTeam( const std::string& tag, xml::xistream& xis, Model& model )
{
    Team_ABC* team = 0;
    if( tag == "party" )
    {
        team = factory_.CreateTeam( xis );
        Register( team->GetId(), *team );
    }
    else if( tag == "no-party" )
        team = noSideTeam_.get();
    else
        xis.error( "Unknown tag in \"parties\"" );

    // $$$$ SBO 2006-10-05: forward to communications extension?
    xis >> xml::optional >> xml::start( "communication" )
            >> xml::list( "knowledge-group", model.knowledgeGroups_, &KnowledgeGroupsModel::Create, *team, model )
        >> xml::end;
    xis >> xml::optional >> xml::start( "tactical" )
            >> xml::list( "formation", model.formations_, &FormationModel::Create, *team, model )
        >> xml::end;
    xis >> xml::optional >> xml::start( "logistics" )
            >> xml::list( "logistic-base", *this, &TeamsModel::ReadLogistic, model )
        >> xml::end;
    xis >> xml::optional >> xml::start( "objects" )
            >> xml::list( "object", model.objects_, &ObjectsModel::CreateObject, *team, model  )
        >> xml::end;
    xis >> xml::optional >> xml::start( "populations" )
            >> xml::list( "population", model.agents_, &AgentsModel::CreatePopulation, *team, model )
        >> xml::end;
    xis >> xml::optional >> xml::start( "inhabitants" )
        >> xml::list( "inhabitant", model.agents_, &AgentsModel::CreateInhabitant, *team, model )
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
    int id = xis.attribute< int >( "id" );
    Entity_ABC* entity = model.formations_.Find( id );
    if( !entity )
        entity = model.agents_.FindAutomat( id );
    if( !entity )
        entity = model.ghosts_.Find( id );
    if( entity )
        xis >> xml::list( "subordinate", *this, &TeamsModel::ReadLogisticLink, model, *entity );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::ReadLogisticLink
// Created: VGS 2011-05-31
// -----------------------------------------------------------------------------
void TeamsModel::ReadLogisticLink( xml::xistream& xis, Model& model, kernel::Entity_ABC& superior )
{
    try
    {
        int id = xis.attribute< int >( "id" );
        Entity_ABC* entity = model.formations_.Find( id );
        if( !entity )
            entity = model.agents_.FindAutomat( id );
        if( !entity )
            entity = model.ghosts_.Find( id );
        if( entity )
        {
            LogisticHierarchiesBase* hierarchies = entity->Retrieve< LogisticHierarchiesBase >();
            if( hierarchies )
                hierarchies->Load( xis, &superior );
        }
        else
            model.AppendLoadingError( eBadLogisticSubordinate, superior.GetName().toStdString(), &superior );
    }
    catch( const std::exception& e )
    {
        model.AppendLoadingError( eOthers, tools::GetExceptionMsg( e ) );
    }
}

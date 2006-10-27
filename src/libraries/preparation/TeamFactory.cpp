// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamFactory.h"
#include "Model.h"
#include "StaticModel.h"
#include "TeamsModel.h"
#include "AgentsModel.h"
#include "Team.h"
#include "KnowledgeGroup.h"
#include "Diplomacies.h"
#include "TeamHierarchies.h"
#include "TeamCommunications.h"
#include "Object.h"
#include "ObjectPositions.h"
#include "LogisticRouteAttributes.h"
#include "NBCAttributes.h"
#include "RotaAttributes.h"
#include "CrossingSiteAttributes.h"
#include "CampAttributes.h"
#include "ObjectHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/InstanciationComplete.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( Controllers& controllers, Model& model, const StaticModel& staticModel, IdManager& idManager )
    : controllers_( controllers )
    , model_( model )
    , staticModel_( staticModel )
    , idManager_( idManager )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamFactory destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::~TeamFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateTeam
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Team_ABC* TeamFactory::CreateTeam()
{
    Team* result = new Team( controllers_.controller_, *this, *this, idManager_ );
    result->Attach( *new Diplomacies( controllers_.controller_, model_.teams_, *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateTeam
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Team_ABC* TeamFactory::CreateTeam( xml::xistream& xis )
{
    Team* result = new Team( xis, controllers_.controller_, *this, *this, idManager_ );
    result->Attach( *new Diplomacies( controllers_.controller_, model_.teams_, *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateKnowledgeGroup
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* TeamFactory::CreateKnowledgeGroup( kernel::Team_ABC& team )
{
    KnowledgeGroup* result = new KnowledgeGroup( controllers_.controller_, idManager_, staticModel_.knowledgeGroupTypes_ );
    result->Attach< kernel::CommunicationHierarchies >( *new EntityCommunications( controllers_.controller_, *result, &team ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateKnowledgeGroup
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* TeamFactory::CreateKnowledgeGroup( xml::xistream& xis, kernel::Team_ABC& team )
{
    KnowledgeGroup* result = new KnowledgeGroup( xis, controllers_.controller_, idManager_, staticModel_.knowledgeGroupTypes_ );
    result->Attach< kernel::CommunicationHierarchies >( *new EntityCommunications( controllers_.controller_, *result, &team ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateObject
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
kernel::Object_ABC* TeamFactory::CreateObject( const kernel::ObjectType& type, kernel::Team_ABC& team, const kernel::Location_ABC& location )
{
    Object* result = new Object( controllers_.controller_, staticModel_.coordinateConverter_, type, idManager_ );
    result->Attach< Positions >( *new ObjectPositions( staticModel_.coordinateConverter_, location ) );
    result->Attach< kernel::TacticalHierarchies >( *new ObjectHierarchies( controllers_.controller_, *result, &team ) );
    
    switch( type.id_ )
    {
    case eObjectType_CampPrisonniers:
    case eObjectType_CampRefugies:
        result->Attach< CampAttributes_ABC >( *new CampAttributes( controllers_ ) );
        break;
    case eObjectType_ItineraireLogistique:
        result->Attach< LogisticRouteAttributes_ABC >( *new LogisticRouteAttributes() );
        break;
    case eObjectType_NuageNbc:
    case eObjectType_ZoneNbc:
        result->Attach< NBCAttributes_ABC >( *new NBCAttributes() );
        break;
    case eObjectType_Rota:
        result->Attach< RotaAttributes_ABC >( *new RotaAttributes() );
        break;
    case eObjectType_SiteFranchissement:
        result->Attach< CrossingSiteAttributes_ABC >( *new CrossingSiteAttributes() );
        break;
    default:
        break;
    };
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateObject
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
kernel::Object_ABC* TeamFactory::CreateObject( xml::xistream& xis, kernel::Team_ABC& team )
{
    Object* result = new Object( xis, controllers_.controller_, staticModel_.coordinateConverter_, staticModel_.objectTypes_, idManager_ );
    result->Attach< Positions >( *new ObjectPositions( xis, staticModel_.coordinateConverter_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new ObjectHierarchies( controllers_.controller_, *result, &team ) );
    switch( result->GetType().id_ )
    {
    case eObjectType_CampPrisonniers:
    case eObjectType_CampRefugies:
        result->Attach< CampAttributes_ABC >( *new CampAttributes( xis, controllers_, model_.agents_ ) );
        break;
    case eObjectType_ItineraireLogistique:
        result->Attach< LogisticRouteAttributes_ABC >( *new LogisticRouteAttributes( xis ) );
        break;
    case eObjectType_NuageNbc:
    case eObjectType_ZoneNbc:
        result->Attach< NBCAttributes_ABC >( *new NBCAttributes( xis, staticModel_.objectTypes_ ) );
        break;
    case eObjectType_Rota:
        result->Attach< RotaAttributes_ABC >( *new RotaAttributes( xis, staticModel_.objectTypes_ ) );
        break;
    case eObjectType_SiteFranchissement:
        result->Attach< CrossingSiteAttributes_ABC >( *new CrossingSiteAttributes( xis ) );
        break;
    default:
        break;
    };
    result->Polish();
    return result;
}

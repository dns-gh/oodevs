// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentFactory.h"

#include "Model.h"
#include "clients_kernel/AgentTypes.h"
#include "Agent.h"
#include "Automat.h"

#include "AgentsModel.h"
#include "KnowledgeGroupsModel.h"
#include "AgentFactory.h"
#include "AgentPositions.h"
#include "AgentCommunications.h"
#include "AgentHierarchies.h"
#include "AutomatDecisions.h"
#include "AutomatPositions.h"
#include "AutomatHierarchies.h"
#include "AutomatCommunications.h"
#include "Dotations.h"
#include "MaintenanceStates.h"
#include "MedicalStates.h"
#include "SupplyStates.h"
#include "Tc2States.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/ObjectTypes.h"
#include "StaticModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( Controllers& controllers, Model& model, const StaticModel& staticModel, IdManager& idManager )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
    , idManager_( idManager )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AgentFactory destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::~AgentFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC* AgentFactory::Create( Automat_ABC& parent, const AgentType& type, const geometry::Point2f& position, bool commandPost )
{
    Agent* result = new Agent( type, controllers_.controller_, idManager_, commandPost );
    result->Attach< Positions >( *new AgentPositions( *result, static_.coordinateConverter_, position ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach< CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
    result->Attach( *new Dotations( controllers_.controller_, static_.objectTypes_ ) );

    AttachExtensions( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( Formation_ABC& parent, const AutomatType& type, const geometry::Point2f& position )
{
    Automat* result = new Automat( type, controllers_.controller_, idManager_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AutomatPositions( *result ) );
    result->Attach( *new AutomatDecisions( controllers_.controller_, *result ) );
    // $$$$ AGE 2006-10-10: AutomatHierarchies ?
    result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, &parent ) );

    Entity_ABC* kg = FindKnowledgeGroup( parent );
    result->Attach< CommunicationHierarchies >( *new AutomatCommunications( controllers_.controller_, *result, kg ) );
    result->Attach( *new Tc2States( controllers_.controller_, *result, static_.objectTypes_, dico ) );
    result->Attach( *new MaintenanceStates( controllers_.controller_, *result, static_.objectTypes_ , dico ) );
    result->Attach( *new MedicalStates( controllers_.controller_, *result, static_.objectTypes_ , dico ) );
    result->Attach( *new SupplyStates( controllers_.controller_, *result, static_.objectTypes_ , dico ) );

    AttachExtensions( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::FindKnowledgeGroup
// Created: AGE 2006-10-10
// -----------------------------------------------------------------------------
Entity_ABC* AgentFactory::FindKnowledgeGroup( const kernel::Entity_ABC& parent )
{
    const Entity_ABC& team = parent.Get< kernel::TacticalHierarchies >().GetTop();
    const CommunicationHierarchies& teamHierarchy = team.Get< CommunicationHierarchies >();
    Iterator< const Entity_ABC& > it = teamHierarchy.CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const Entity_ABC* entity = &it.NextElement();
        if( dynamic_cast< const KnowledgeGroup_ABC* >( entity ) )
            return const_cast< Entity_ABC* >( entity );
    }
    return const_cast< Entity_ABC* >( &team );
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Agent_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Automat_ABC& parent )
{
    Agent* result = new Agent( xis, controllers_.controller_, idManager_, static_.types_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AgentPositions( xis, *result, static_.coordinateConverter_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach< CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
    result->Attach( *new Dotations( xis, controllers_.controller_, static_.objectTypes_ ) );

    AttachExtensions( *result );
    result->Polish();
    return result;
}
    
// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Formation_ABC& parent )
{
    Automat* result = new Automat( xis, controllers_.controller_, idManager_, static_.types_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AutomatPositions( *result ) );
    result->Attach( *new AutomatDecisions( xis, controllers_.controller_, *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach< CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, model_.knowledgeGroups_ ) );
    result->Attach( *new Tc2States( controllers_.controller_, *result, static_.objectTypes_, dico ) );
    result->Attach( *new MaintenanceStates( controllers_.controller_, *result, static_.objectTypes_ , dico ) );
    result->Attach( *new MedicalStates( controllers_.controller_, *result, static_.objectTypes_ , dico ) );
    result->Attach( *new SupplyStates( controllers_.controller_, *result, static_.objectTypes_ , dico ) );

    AttachExtensions( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC* AgentFactory::Create( Team_ABC& parent, const PopulationType& type, const geometry::Point2f& position )
{
     // $$$$ AGE 2006-10-04: 
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::AttachExtensions
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentFactory::AttachExtensions( Entity_ABC& agent )
{

}


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
#include "Agent.h"
#include "AgentCommunications.h"
#include "AgentFactory.h"
#include "AgentHierarchies.h"
#include "AgentPositions.h"
#include "AgentsModel.h"
#include "Automat.h"
#include "AutomatCommunications.h"
#include "AutomatDecisions.h"
#include "AutomatHierarchies.h"
#include "AutomatPositions.h"
#include "CommandPostAttributes.h"
#include "InitialState.h"
#include "KnowledgeGroupsModel.h"
#include "LogisticBaseStates.h"
#include "Model.h"
#include "LogisticLevelAttritube.h"
#include "Inhabitant.h"
#include "Affinities.h"
#include "PeopleAffinities.h"
#include "InhabitantHierarchies.h"
#include "InhabitantPositions.h"
#include "Inhabitants.h"
#include "Population.h"
#include "PopulationHierarchies.h"
#include "PopulationPositions.h"
#include "Populations.h"
#include "StaticModel.h"
#include "Stocks.h"
#include "TacticalLines.h"
#include "Color.h"
#include "Symbol.h"
#include "AgentAffinities.h"
#include "ProfileHierarchies.h"
#include "ProfileHierarchies_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"
#include "clients_kernel/Color_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( Controllers& controllers, Model& model, const StaticModel& staticModel, IdManager& idManager, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
    , idManager_( idManager )
    , knowledgeGroupFactory_( knowledgeGroupFactory )
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

namespace
{
    void SetNationality( Entity_ABC& entity, const Entity_ABC& parent )
    {
        const DictionaryExtensions* parentExt = parent.Retrieve< DictionaryExtensions >();
        DictionaryExtensions* childExt = entity.Retrieve< DictionaryExtensions >();
        if( parentExt && childExt )
        {
            const std::string& country = parentExt->GetValue( "Nationalite" );
            if( !country.empty() )
            {
                childExt->SetEnabled( true );
                childExt->SetValue( "Nationalite", country );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC* AgentFactory::Create( Automat_ABC& parent, const AgentType& type, const geometry::Point2f& position, bool commandPost, const QString& name )
{
    Agent* result = new Agent( type, controllers_.controller_, idManager_ );
    if( !name.isEmpty() )
        result->Rename( name );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AgentPositions( *result, static_.coordinateConverter_, controllers_.controller_, position, dico ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, result->GetType().GetLevelSymbol(), result->GetType().GetSymbol(), &parent ) );
    result->Attach< CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
    result->Attach( *new InitialState( static_, result->GetType().GetId() ) );
    result->Attach< Affinities >( *new AgentAffinities( *result, controllers_, model_, dico, tools::translate( "Affinities", "Affinities" ) ) );
    if( result->GetType().IsLogisticSupply() )
        result->Attach( *new Stocks( controllers_.controller_, *result, dico ) );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Attach( *new CommandPostAttributes( *result, type, dico, commandPost ) );
    result->Attach< kernel::Color_ABC >( *new Color( parent ) );
    SetNationality( *result, parent );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
Automat_ABC* AgentFactory::Create( Entity_ABC& parent, const AutomatType& type, const QString& name )
{
    Automat* result = new Automat( type, controllers_.controller_, idManager_, name );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AutomatPositions( *result ) );
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol() );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach( *new AutomatDecisions( controllers_.controller_, *result ) );
    Entity_ABC* kg = FindorCreateKnowledgeGroup( parent, knowledgeGroupFactory_ );
    result->Attach< CommunicationHierarchies >( *new AutomatCommunications( controllers_.controller_, *result, kg ) );

    bool isTC2 = result->GetType().IsTC2(); //$$ NAZE
    result->Attach( *new LogisticLevelAttritube( controllers_, *result, isTC2, dico ) );
    result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *result, static_.objectTypes_, dico, isTC2 ) );

    result->Attach( *new TacticalLines() );
    result->Attach< kernel::Color_ABC >( *new Color( parent ) );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Attach< ProfileHierarchies_ABC >( *new ProfileHierarchies( controllers_.controller_, *result, &parent ) );
    SetNationality( *result, parent );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population_ABC* AgentFactory::Create( Entity_ABC& parent, const PopulationType& type, int number, const geometry::Point2f& position )
{
    Entity_ABC* top = 0;
    if( const kernel::TacticalHierarchies* hierarchies = parent.Retrieve< kernel::TacticalHierarchies >() )
        top = const_cast< Entity_ABC* >( &hierarchies->GetTop() );
    else
        top = const_cast< Entity_ABC* >( &parent.Get< CommunicationHierarchies >().GetTop() );
    Population* result = new Population( type, number, controllers_.controller_, idManager_ );
    result->Attach< Positions >( *new PopulationPositions( *result, controllers_.controller_, static_.coordinateConverter_, position ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, top ) );
    kernel::PropertiesDictionary& dictionary = result->Get< kernel::PropertiesDictionary >();
    result->Attach< Affinities >( *new PeopleAffinities( controllers_, model_, dictionary ) );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Attach< ProfileHierarchies_ABC >( *new ProfileHierarchies( controllers_.controller_, *result, &parent ) );
    if( Populations* popus = top->Retrieve< Populations >() )
        popus->AddPopulation( *result );
    SetNationality( *result, *top );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant_ABC* AgentFactory::Create( Entity_ABC& parent, const InhabitantType& type, int number, const QString& name, const Location_ABC& location )
{
    Entity_ABC* top = 0;
    if( const kernel::TacticalHierarchies* hierarchies = parent.Retrieve< kernel::TacticalHierarchies >() )
        top = const_cast< Entity_ABC* >( &hierarchies->GetTop() );
    else
        top = const_cast< Entity_ABC* >( &parent.Get< CommunicationHierarchies >().GetTop() );

    Inhabitant* result = new Inhabitant( type, number, name, controllers_.controller_, idManager_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();

    Positions& positions = *new InhabitantPositions( controllers_.controller_, static_.coordinateConverter_, location, model_.urban_, *result, dico );
    if( positions.GetPosition() == geometry::Point2f( 0, 0 ) )
    {
        delete &positions;
        delete result;
        return 0;
    }
    result->Attach< Positions >( positions );
    result->Attach< kernel::TacticalHierarchies >( *new InhabitantHierarchies( *result, top ) );
    result->Attach< Affinities >( *new PeopleAffinities( controllers_, model_, dico ) );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    if( Inhabitants* inhabs = top->Retrieve< Inhabitants >() )
        inhabs->AddInhabitant( *result );
    SetNationality( *result, *top );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::FindKnowledgeGroup
// Created: AGE 2006-10-10
// -----------------------------------------------------------------------------
Entity_ABC* AgentFactory::FindorCreateKnowledgeGroup( const Entity_ABC& parent, kernel::KnowledgeGroupFactory_ABC& knowledgeFactory )
{
    const Entity_ABC& team = parent.Get< kernel::TacticalHierarchies >().GetTop();
    const CommunicationHierarchies& teamHierarchy = team.Get< CommunicationHierarchies >();
    tools::Iterator< const Entity_ABC& > it = teamHierarchy.CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const Entity_ABC* entity = &it.NextElement();
        if( dynamic_cast< const KnowledgeGroup_ABC* >( entity ) )
            return const_cast< Entity_ABC* >( entity );
    }
    // LTO begin
    Team_ABC* teamtop = dynamic_cast< Team_ABC* >( const_cast< Entity_ABC* >(&team) );
    if( teamtop )
        return knowledgeFactory.Create( *teamtop );
    // LTO end
    return const_cast< Entity_ABC* >( &team );
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Agent_ABC* AgentFactory::Create( xml::xistream& xis, Automat_ABC& parent )
{
    const AgentType* type = static_.types_.Resolver< AgentType, std::string >::Find( xis.attribute< std::string >( "type" ) );
    if( !type )
        return 0;
    Agent* result = new Agent( xis, controllers_.controller_, idManager_, *type );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AgentPositions( xis, *result, static_.coordinateConverter_, controllers_.controller_, dico ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, result->GetType().GetLevelSymbol(), result->GetType().GetSymbol(), &parent ) );
    result->Attach< CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
    result->Attach( *new InitialState( xis, static_, result->GetType().GetId() ) );
    result->Attach< Affinities >( *new AgentAffinities( xis, *result, controllers_, model_, dico, tools::translate( "Affinities", "Affinities" ) ) );
    result->Attach( *new CommandPostAttributes( xis, *result, *type, dico ) );
    if( result->GetType().IsLogisticSupply() )
        result->Attach( *new Stocks( xis, controllers_.controller_, *result, static_.objectTypes_, dico ) );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Automat_ABC* AgentFactory::Create( xml::xistream& xis, Entity_ABC& parent )
{
    const AutomatType* type = static_.types_.Resolver< AutomatType, std::string >::Find( xis.attribute< std::string >( "type" ) );
    if( !type )
        return 0;
    Automat* result = new Automat( xis, controllers_.controller_, idManager_, *type );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AutomatPositions( *result ) );
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( xis ) );
    result->Attach( *new AutomatDecisions( xis, controllers_.controller_, *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach< CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, model_.knowledgeGroups_ ) );

    bool isTC2 = result->GetType().IsTC2(); //$$ NAZE
    result->Attach( *new LogisticLevelAttritube( controllers_, xis, *result, isTC2, dico ) );
    result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *result, static_.objectTypes_, dico, isTC2 ) );

    result->Attach( *new TacticalLines() );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Attach< ProfileHierarchies_ABC >( *new ProfileHierarchies( controllers_.controller_, *result, &parent ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
Population_ABC* AgentFactory::Create( xml::xistream& xis, Team_ABC& parent, const kernel::PopulationType& type )
{
    Population* result = new Population( xis, type, controllers_.controller_, idManager_ );
    result->Attach< Positions >( *new PopulationPositions( xis, *result, controllers_.controller_, static_.coordinateConverter_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, &parent ) );
    kernel::PropertiesDictionary& dictionary = result->Get< kernel::PropertiesDictionary >();
    result->Attach< Affinities >( *new PeopleAffinities( xis, controllers_, model_, dictionary ) );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    result->Attach< ProfileHierarchies_ABC >( *new ProfileHierarchies( controllers_.controller_, *result, &parent ) );
    if( Populations* popus = parent.Retrieve< Populations >() )
        popus->AddPopulation( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant_ABC* AgentFactory::Create( xml::xistream& xis, Team_ABC& parent, const kernel::InhabitantType& type )
{
    Inhabitant* result = new Inhabitant( xis, type, controllers_.controller_, idManager_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new InhabitantPositions( xis, controllers_.controller_, static_.coordinateConverter_, model_.urban_, *result, dico ) );
    result->Attach< kernel::TacticalHierarchies >( *new InhabitantHierarchies( *result, &parent ) );
    result->Attach< Affinities >( *new PeopleAffinities( xis, controllers_, model_, dico ) );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    if( Inhabitants* popus = parent.Retrieve< Inhabitants >() )
        popus->AddInhabitant( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: ABR 2011-10-25
// -----------------------------------------------------------------------------
kernel::Agent_ABC* AgentFactory::Create( kernel::Ghost_ABC& ghost, const kernel::AgentType& type, const geometry::Point2f position )
{
    assert( ghost.GetGhostType() == eGhostType_Agent );
    Agent* result = new Agent( type, controllers_.controller_, idManager_ );
    result->Rename( ghost.GetName() );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AgentPositions( *result, static_.coordinateConverter_, controllers_.controller_, position, dico ) );
    // Hierarchies
    {
        const kernel::TacticalHierarchies* ghostHierarchy = ghost.Retrieve< kernel::TacticalHierarchies >();
        assert( ghostHierarchy );
        Entity_ABC* tactSuperior = const_cast< Entity_ABC* >( ghostHierarchy->GetSuperior() );
        assert( tactSuperior );

        result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, result->GetType().GetLevelSymbol(), result->GetType().GetSymbol(), tactSuperior ) );
        result->Attach< CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, tactSuperior ) );
    }
    result->Attach( *new InitialState( static_, result->GetType().GetId() ) );
    result->Attach< Affinities >( *new AgentAffinities( *result, controllers_, model_, dico, tools::translate( "Affinities", "Affinities" ) ) );
    if( result->GetType().IsLogisticSupply() )
        result->Attach( *new Stocks( controllers_.controller_, *result, dico ) );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Attach( *new CommandPostAttributes( *result, type, dico ) );
    result->Attach< kernel::Color_ABC >( *new Color( ghost ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: ABR 2011-10-25
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( kernel::Ghost_ABC& ghost, const kernel::AutomatType& type )
{
    assert( ghost.GetGhostType() == eGhostType_Automat );

    Automat* result = new Automat( type, controllers_.controller_, idManager_, ghost.GetName() );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new AutomatPositions( *result ) );
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol() );
    result->Attach( *new AutomatDecisions( controllers_.controller_, *result ) );
    // Tactical Hierarchies
    {
        const kernel::TacticalHierarchies* ghostHierarchy = ghost.Retrieve< kernel::TacticalHierarchies >();
        assert( ghostHierarchy );
        Entity_ABC* tactSuperior = const_cast< Entity_ABC* >( ghostHierarchy->GetSuperior() );
        assert( tactSuperior );
        result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, tactSuperior ) );
        result->Attach< ProfileHierarchies_ABC >( *new ProfileHierarchies( controllers_.controller_, *result, tactSuperior ) );
    }
    // Communication Hierarchies
    {
        const CommunicationHierarchies* ghostHierarchy = ghost.Retrieve< CommunicationHierarchies >();
        assert( ghostHierarchy );
        Entity_ABC* comSuperior = const_cast< Entity_ABC* >( ghostHierarchy->GetSuperior() );
        assert( comSuperior );
        result->Attach< CommunicationHierarchies >( *new AutomatCommunications( controllers_.controller_, *result, comSuperior ) );
    }

    bool isTC2 = result->GetType().IsTC2(); //$$ NAZE
    result->Attach( *new LogisticLevelAttritube( controllers_, *result, isTC2, dico ) );
    result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *result, static_.objectTypes_, dico, isTC2 ) );

    result->Attach( *new TacticalLines() );
    result->Attach< kernel::Color_ABC >( *new Color( ghost ) );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Polish();
    return result;
}

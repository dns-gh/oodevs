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
#include "AutomatHierarchies.h"
#include "AutomatPositions.h"
#include "CommandPostAttributes.h"
#include "Dotation.h"
#include "InitialState.h"
#include "KnowledgeGroupsModel.h"
#include "LogisticBaseStates.h"
#include "Model.h"
#include "Inhabitant.h"
#include "Affinities.h"
#include "PeopleAffinities.h"
#include "InhabitantHierarchies.h"
#include "InhabitantPositions.h"
#include "Inhabitants.h"
#include "ProfilesModel.h"
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
#include "UserProfile.h"

#include "clients_gui/LogisticBase.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_gui/AutomatDecisions.h"
#include "clients_gui/CriticalIntelligence.h"
#include "clients_gui/EntityType.h"
#include "clients_gui/PropertiesDictionary.h"

#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( kernel::Controllers& controllers, Model& model, const ::StaticModel& staticModel, tools::IdManager& idManager, kernel::SymbolFactory& symbolsFactory )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
    , idManager_( idManager )
    , symbolsFactory_( symbolsFactory )
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
kernel::Agent_ABC* AgentFactory::Create( kernel::Automat_ABC& parent, const kernel::AgentType& type, const geometry::Point2f& position, bool commandPost, const QString& name )
{
    Agent* result = new Agent( type, controllers_.controller_, idManager_ );
    if( !name.isEmpty() )
        result->Rename( name );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new AgentPositions( *result, static_.coordinateConverter_, controllers_.controller_, position, dictionary ) );
    result->Attach< gui::CriticalIntelligence >( *new gui::CriticalIntelligence( *result, controllers_.controller_, dictionary ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, &parent, symbolsFactory_ ) );
    result->Attach( *new InitialState( static_, result->GetType().GetId() ) );
    result->Attach< Affinities >( *new AgentAffinities( *result, controllers_, model_, dictionary, tools::translate( "Affinities", "Affinities" ) ) );
    if( result->GetType().IsLogisticSupply() )
        result->Attach( *new Stocks( controllers_.controller_, *result, dictionary ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Attach< kernel::CommandPostAttributes_ABC >( *new CommandPostAttributes( *result, type, dictionary, commandPost ) );
    result->Attach< kernel::Color_ABC >( *new Color( parent ) );
    result->Attach( *new TacticalLines() );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( kernel::Entity_ABC& parent, const kernel::AutomatType& type, const QString& name )
{
    Automat* result = new Automat( type, controllers_.controller_, idManager_, name );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach( *new gui::EntityType< kernel::AutomatType >( *result, type, dictionary ) );
    result->Attach< kernel::Positions >( *new AutomatPositions( *result ) );
    const kernel::Karma& karma = parent.Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( symbolsFactory_.GetSymbolBase( karma ), symbolsFactory_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach< gui::Decisions_ABC >( *new gui::AutomatDecisions( controllers_.controller_, *result, static_.types_.automatModels_ ) );
    kernel::Entity_ABC* kg = FindOrCreateKnowledgeGroup( parent );
    result->Attach< kernel::CommunicationHierarchies >( *new AutomatCommunications( controllers_.controller_, *result, kg ) );

    bool isTC2 = type.IsTC2(); //$$ NAZE
    result->Attach( *new gui::LogisticBase( controllers_, *result, dictionary, isTC2, isTC2 ) );
    result->Attach< gui::LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *result, static_.objectTypes_, dictionary, isTC2 ) );

    result->Attach( *new TacticalLines() );
    result->Attach< kernel::Color_ABC >( *new Color( parent ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
kernel::Population_ABC* AgentFactory::Create( kernel::Entity_ABC& parent, const kernel::PopulationType& type, int number, const geometry::Point2f& position )
{
    kernel::Entity_ABC* top = 0;
    if( const kernel::TacticalHierarchies* hierarchies = parent.Retrieve< kernel::TacticalHierarchies >() )
        top = const_cast< kernel::Entity_ABC* >( &hierarchies->GetTop() );
    else
        top = const_cast< kernel::Entity_ABC* >( &parent.Get< kernel::CommunicationHierarchies >().GetTop() );
    Population* result = new Population( type, number, controllers_.controller_, idManager_ );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach( *new gui::EntityType< kernel::PopulationType >( *result, type, dictionary ) );
    result->Attach< gui::CriticalIntelligence >( *new gui::CriticalIntelligence( *result, controllers_.controller_, dictionary ) );
    result->Attach< kernel::Positions >( *new PopulationPositions( *result, controllers_.controller_, static_.coordinateConverter_, position, dictionary ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, top ) );
    result->Attach< Affinities >( *new PeopleAffinities( controllers_, model_, dictionary, *result ) );
    result->Attach< kernel::Color_ABC >( *new Color( parent ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    if( Populations* popus = top->Retrieve< Populations >() )
        popus->AddPopulation( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2010-11-23
// -----------------------------------------------------------------------------
kernel::Inhabitant_ABC* AgentFactory::Create( kernel::Entity_ABC& parent, const kernel::InhabitantType& type, int number, const QString& name, const kernel::Location_ABC& location )
{
    kernel::Entity_ABC* top = 0;
    if( const kernel::TacticalHierarchies* hierarchies = parent.Retrieve< kernel::TacticalHierarchies >() )
        top = const_cast< kernel::Entity_ABC* >( &hierarchies->GetTop() );
    else
        top = const_cast< kernel::Entity_ABC* >( &parent.Get< kernel::CommunicationHierarchies >().GetTop() );

    Inhabitant* result = new Inhabitant( type, number, name, controllers_.controller_, idManager_ );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();

    kernel::Positions& positions = *new InhabitantPositions( controllers_.controller_, static_.coordinateConverter_, location, *model_.urban_, *result, dictionary );
    if( positions.GetPosition() == geometry::Point2f( 0, 0 ) )
    {
        delete &positions;
        delete result;
        return 0;
    }
    result->Attach( *new gui::EntityType< kernel::InhabitantType >( *result, type, dictionary ) );
    result->Attach< kernel::Positions >( positions );
    result->Attach< kernel::TacticalHierarchies >( *new InhabitantHierarchies( *result, top ) );
    result->Attach< Affinities >( *new PeopleAffinities( controllers_, model_, dictionary, *result ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    if( Inhabitants* inhabs = top->Retrieve< Inhabitants >() )
        inhabs->AddInhabitant( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::FindOrCreateKnowledgeGroup
// Created: AGE 2006-10-10
// -----------------------------------------------------------------------------
kernel::Entity_ABC* AgentFactory::FindOrCreateKnowledgeGroup( const kernel::Entity_ABC& parent )
{
    const kernel::Entity_ABC& team = parent.Get< kernel::TacticalHierarchies >().GetTop();
    const kernel::CommunicationHierarchies& teamHierarchy = team.Get< kernel::CommunicationHierarchies >();
    tools::Iterator< const kernel::Entity_ABC& > it = teamHierarchy.CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC* entity = &it.NextElement();
        if( dynamic_cast< const kernel::KnowledgeGroup_ABC* >( entity ) )
            return const_cast< kernel::Entity_ABC* >( entity );
    }
    return const_cast< kernel::Entity_ABC* >( &team );
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Agent_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Automat_ABC& parent )
{
    const kernel::AgentType* type = static_.types_.Resolver< kernel::AgentType, std::string >::Find( xis.attribute< std::string >( "type" ) );
    if( !type )
        return 0;
    Agent* result = new Agent( xis, controllers_.controller_, idManager_, *type, symbolsFactory_ );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach< gui::CriticalIntelligence >( *new gui::CriticalIntelligence( xis, controllers_.controller_, *result, dictionary ) );
    const geometry::Point2f position = model_.ReadPosition( xis, result );
    result->Attach< kernel::Positions >( *new AgentPositions( *result, static_.coordinateConverter_, controllers_.controller_, position, dictionary ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, &parent, symbolsFactory_ ) );
    result->Attach( *new InitialState( xis, static_, result->GetType().GetId() ) );
    result->Attach< Affinities >( *new AgentAffinities( xis, *result, controllers_, model_, dictionary, tools::translate( "Affinities", "Affinities" ) ) );
    result->Attach< kernel::CommandPostAttributes_ABC >( *new CommandPostAttributes( xis, *result, *type, dictionary ) );
    if( result->GetType().IsLogisticSupply() )
        result->Attach( *new Stocks( xis, controllers_.controller_, *result, static_.objectTypes_, dictionary ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Attach( *new TacticalLines() );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Entity_ABC& parent )
{
    const kernel::AutomatType* type = static_.types_.Resolver< kernel::AutomatType, std::string >::Find( xis.attribute< std::string >( "type" ) );
    if( !type )
        return 0;
    Automat* result = new Automat( xis, controllers_.controller_, idManager_, *type );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach( *new gui::EntityType< kernel::AutomatType >( *result, *type, dictionary ) );
    result->Attach< kernel::Positions >( *new AutomatPositions( *result ) );
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( xis, std::string(), symbolsFactory_ ) );
    result->Attach< gui::Decisions_ABC >( *new gui::AutomatDecisions( xis, controllers_.controller_, *result, static_.types_.automatModels_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, &parent ) );
    result->Attach< kernel::CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, *model_.knowledgeGroups_ ) );

    bool isTC2 = type->IsTC2(); //$$ NAZE
    result->Attach( *new gui::LogisticBase( controllers_, *result, dictionary, isTC2, xis ) );
    result->Attach< gui::LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *result, static_.objectTypes_, dictionary, isTC2 ) );

    result->Attach( *new TacticalLines() );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
kernel::Population_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Team_ABC& parent, const kernel::PopulationType& type )
{
    Population* result = new Population( xis, type, controllers_.controller_, idManager_ );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach( *new gui::EntityType< kernel::PopulationType >( *result, type, dictionary ) );
    result->Attach< gui::CriticalIntelligence >( *new gui::CriticalIntelligence( xis, controllers_.controller_, *result, dictionary ) );
    const geometry::Point2f position = model_.ReadPosition( xis, result );
    result->Attach< kernel::Positions >( *new PopulationPositions( *result, controllers_.controller_, static_.coordinateConverter_, position, dictionary ) );
    result->Attach< kernel::TacticalHierarchies >( *new PopulationHierarchies( *result, &parent ) );
    result->Attach< Affinities >( *new PeopleAffinities( xis, controllers_, model_, dictionary, *result ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    if( Populations* popus = parent.Retrieve< Populations >() )
        popus->AddPopulation( *result );
    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
kernel::Inhabitant_ABC* AgentFactory::Create( xml::xistream& xis, kernel::Team_ABC& parent, const kernel::InhabitantType& type )
{
    Inhabitant* result = new Inhabitant( xis, controllers_.controller_, idManager_ );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach( *new gui::EntityType< kernel::InhabitantType >( *result, type, dictionary ) );
    result->Attach< kernel::Positions >( *new InhabitantPositions( xis, controllers_.controller_, static_.coordinateConverter_, *model_.urban_, *result, dictionary ) );
    result->Attach< kernel::TacticalHierarchies >( *new InhabitantHierarchies( *result, &parent ) );
    result->Attach< Affinities >( *new PeopleAffinities( xis, controllers_, model_, dictionary, *result ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", xis, static_.extensions_ ) );
    if( Inhabitants* popus = parent.Retrieve< Inhabitants >() )
        popus->AddInhabitant( *result );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create from ghost
// Created: ABR 2011-10-25
// -----------------------------------------------------------------------------
kernel::Agent_ABC* AgentFactory::Create( kernel::Ghost_ABC& ghost, const kernel::AgentType& type, const geometry::Point2f position )
{
    assert( ghost.GetGhostType() == eGhostType_Agent );
    Agent* result = new Agent( type, controllers_.controller_, idManager_ );
    result->Rename( ghost.GetName() );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new AgentPositions( *result, static_.coordinateConverter_, controllers_.controller_, position, dictionary ) );
    result->Attach< gui::CriticalIntelligence >( *new gui::CriticalIntelligence( *result, controllers_.controller_, dictionary ) );
    // Hierarchies
    {
        const kernel::TacticalHierarchies* ghostHierarchy = ghost.Retrieve< kernel::TacticalHierarchies >();
        assert( ghostHierarchy );
        kernel::Entity_ABC* tactSuperior = const_cast< kernel::Entity_ABC* >( ghostHierarchy->GetSuperior() );
        assert( tactSuperior );

        result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, tactSuperior, symbolsFactory_ ) );
    }
    result->Attach( *new InitialState( static_, result->GetType().GetId() ) );
    result->Attach< Affinities >( *new AgentAffinities( *result, controllers_, model_, dictionary, tools::translate( "Affinities", "Affinities" ) ) );
    if( result->GetType().IsLogisticSupply() )
        result->Attach( *new Stocks( controllers_.controller_, *result, dictionary ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Attach< kernel::CommandPostAttributes_ABC >( *new CommandPostAttributes( *result, type, dictionary, ghost.Get< kernel::CommandPostAttributes_ABC >().IsCommandPost() ) );
    result->Attach< kernel::Color_ABC >( *new Color( ghost ) );
    result->Attach( *new TacticalLines() );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create from ghost
// Created: ABR 2011-10-25
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentFactory::Create( kernel::Ghost_ABC& ghost, const kernel::AutomatType& type )
{
    assert( ghost.GetGhostType() == eGhostType_Automat );

    Automat* result = new Automat( type, controllers_.controller_, idManager_, ghost.GetName() );
    gui::PropertiesDictionary& dictionary = result->Get< gui::PropertiesDictionary >();
    result->Attach( *new gui::EntityType< kernel::AutomatType >( *result, type, dictionary ) );
    result->Attach< kernel::Positions >( *new AutomatPositions( *result ) );
    const kernel::Karma& karma = ghost.Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
    result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( symbolsFactory_.GetSymbolBase( karma ), symbolsFactory_ ) );
    result->Attach< gui::Decisions_ABC >( *new gui::AutomatDecisions( controllers_.controller_, *result, static_.types_.automatModels_ ) );
    // Tactical hierarchy
    {
        const kernel::TacticalHierarchies& ghostHierarchy = ghost.Get< kernel::TacticalHierarchies >();
        kernel::Entity_ABC* tactSuperior = const_cast< kernel::Entity_ABC* >( ghostHierarchy.GetSuperior() );
        assert( tactSuperior );
        result->Attach< kernel::TacticalHierarchies >( *new AutomatHierarchies( controllers_.controller_, *result, tactSuperior ) );
        // Profile hierarchy
        //result->Attach< ProfileHierarchies_ABC >( *new ProfileHierarchies( controllers_.controller_, *result, tactSuperior ) );
        std::vector< std::string > readingProfiles = model_.profiles_->GetProfilesWhoCanRead( ghost );
        for( std::vector< std::string >::const_iterator it = readingProfiles.begin(); it != readingProfiles.end(); ++it )
            if( UserProfile* profile = model_.profiles_->Find( *it ) )
                profile->SetReadable( *result, true );
        std::vector< std::string > writingProfiles = model_.profiles_->GetProfilesWhoCanWrite( ghost );
        for( std::vector< std::string >::const_iterator it = writingProfiles.begin(); it != writingProfiles.end(); ++it )
            if( UserProfile* profile = model_.profiles_->Find( *it ) )
                profile->SetWriteable( *result, true );
    }
    // Communication hierarchy
    {
        const kernel::CommunicationHierarchies& ghostHierarchy = ghost.Get< kernel::CommunicationHierarchies >();
        kernel::Entity_ABC* comSuperior = const_cast< kernel::Entity_ABC* >( ghostHierarchy.GetSuperior() );
        assert( comSuperior );
        result->Attach< kernel::CommunicationHierarchies >( *new AutomatCommunications( controllers_.controller_, *result, comSuperior ) );
    }
    // Logistic hierarchy
    {
        const gui::LogisticHierarchiesBase& ghostHierarchy = ghost.Get< gui::LogisticHierarchiesBase >();
        // $$$$ ABR 2012-06-27: TODO: Warn if dropping a non log base to a log base ghost.
        bool isTC2 = type.IsTC2(); //$$ NAZE
        result->Attach( *new gui::LogisticBase( controllers_, *result, dictionary, isTC2, isTC2 ) );
        LogisticBaseStates* logBaseStates = new LogisticBaseStates( controllers_.controller_, *result, static_.objectTypes_, dictionary, isTC2 );
        result->Attach< gui::LogisticHierarchiesBase >( *logBaseStates );
        logBaseStates->SetLogisticSuperior( ghostHierarchy.GetSuperior() );
        tools::Iterator< const kernel::Entity_ABC& > it = ghostHierarchy.CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::Entity_ABC& child = it.NextElement();
            gui::LogisticHierarchiesBase& childHierarchy = const_cast< kernel::Entity_ABC& >( child ).Get< gui::LogisticHierarchiesBase >();
            childHierarchy.SetLogisticSuperior( result );
        }
        tools::Iterator< const Dotation& > itDot = static_cast< const LogisticBaseStates& >( ghostHierarchy ).tools::Resolver< Dotation >::CreateIterator();
        while( itDot.HasMoreElements() )
        {
            const Dotation& dotation = itDot.NextElement();
            if( dotation.quantity_ > 0 )
                logBaseStates->SetDotation( dotation.type_, dotation.quantity_ );
        }
    }
    result->Attach( *new TacticalLines() );
    result->Attach< kernel::Color_ABC >( *new Color( ghost ) );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", static_.extensions_ ) );
    result->Polish();
    return result;
}

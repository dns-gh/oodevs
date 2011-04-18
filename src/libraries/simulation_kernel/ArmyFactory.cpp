// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/AgentFactory.h"
#include "simulation_kernel/ArmyFactory.h"
#include "simulation_kernel/AutomateFactory.h"
#include "simulation_kernel/Entities/MIL_Army.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectManager.h"
#include "simulation_kernel/FormationFactory_ABC.h"
#include "simulation_kernel/PopulationFactory_ABC.h"
#include "simulation_kernel/InhabitantFactory_ABC.h"
#include "simulation_kernel/knowledge/KnowledgeGroupFactory_ABC.h" // LTO
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/map.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( ArmyFactory )

// -----------------------------------------------------------------------------
// Name: ArmyFactory::save_construct_data
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void save_construct_data( Archive& archive, const ArmyFactory* factory, const unsigned int /*version*/ )
{
    const AutomateFactory_ABC* const automateFactory = &factory->automateFactory_;
    const AgentFactory_ABC* const agentFactory = &factory->agentFactory_;
    const FormationFactory_ABC* const formationFactory = &factory->formationFactory_;
    const MIL_ObjectManager* const objectFactory = &factory->objectFactory_;
    const PopulationFactory_ABC* const populationFactory = &factory->populationFactory_;
    const InhabitantFactory_ABC* const inhabitantFactory = &factory->inhabitantFactory_;
    const KnowledgeGroupFactory_ABC* const knowledgeGroupFactory = &factory->knowledgeGroupFactory_; // LTO
    archive << automateFactory
            << agentFactory
            << formationFactory
            << objectFactory
            << populationFactory
            << inhabitantFactory
            << knowledgeGroupFactory; // LTO
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::save_construct_data
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void load_construct_data( Archive& archive, ArmyFactory* factory, const unsigned int /*version*/ )
{
    AutomateFactory_ABC* automateFactory;
    AgentFactory_ABC* agentFactory;
    FormationFactory_ABC* formationFactory;
    MIL_ObjectManager* objectFactory;
    PopulationFactory_ABC* populationFactory;
    InhabitantFactory_ABC* inhabitantFactory;
    KnowledgeGroupFactory_ABC* knowledgeGroupFactory; // LTO
    archive >> automateFactory
            >> agentFactory
            >> formationFactory
            >> objectFactory
            >> populationFactory
            >> inhabitantFactory
            >> knowledgeGroupFactory; // LTO
    ::new( factory )ArmyFactory( *automateFactory, *agentFactory, *formationFactory, *objectFactory, *populationFactory, *inhabitantFactory, *knowledgeGroupFactory ); // LTO
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory constructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
ArmyFactory::ArmyFactory( AutomateFactory_ABC& automateFactory, AgentFactory_ABC& agentFactory, FormationFactory_ABC& formationFactory, MIL_ObjectManager& objectFactory, PopulationFactory_ABC& populationFactory, InhabitantFactory_ABC& inhabitantFactory, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
    : automateFactory_      ( automateFactory )
    , agentFactory_         ( agentFactory )
    , formationFactory_     ( formationFactory )
    , objectFactory_        ( objectFactory )
    , populationFactory_    ( populationFactory )
    , inhabitantFactory_    ( inhabitantFactory )
    , knowledgeGroupFactory_( knowledgeGroupFactory ) // LTO
    , diplomacyConverter_   ( new MT_Converter< std::string, MIL_Army_ABC::E_Diplomacy, sCaseInsensitiveLess >( MIL_Army_ABC::eUnknown ) ) // LTO
{
    InitializeDiplomacies();
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory destructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
ArmyFactory::~ArmyFactory()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::Create
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
MIL_Army_ABC* ArmyFactory::Create( xml::xistream& xis )
{
    MIL_Army_ABC* army = new MIL_Army( xis, *this, formationFactory_, automateFactory_, objectFactory_, populationFactory_, inhabitantFactory_, knowledgeGroupFactory_, *diplomacyConverter_ );
    Register( army->GetID(), *army );
    return army;
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::InitializeDiplomacies
// Created: HBD 2009-12-22
// -----------------------------------------------------------------------------
void ArmyFactory::InitializeDiplomacies()
{
    MT_LOG_INFO_MSG( "Initializing armies diplomacies" );
    diplomacyConverter_->Register( "enemy"  , MIL_Army_ABC::eEnemy   );
    diplomacyConverter_->Register( "friend" , MIL_Army_ABC::eFriend  );
    diplomacyConverter_->Register( "neutral", MIL_Army_ABC::eNeutral );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void ArmyFactory::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object < ArmyFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void ArmyFactory::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object < ArmyFactory_ABC >( *this );
}

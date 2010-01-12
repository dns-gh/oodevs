// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/ArmyFactory.h"
#include "simulation_kernel/AutomateFactory.h"
#include "simulation_kernel/Entities/MIL_Army.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectManager.h"
#include "simulation_kernel/FormationFactory_ABC.h"
#include "simulation_kernel/PopulationFactory_ABC.h"
#include "simulation_kernel/knowledge/KnowledgeGroupFactory_ABC.h"


#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( ArmyFactory, "ArmyFactory" )

// -----------------------------------------------------------------------------
// Name: ArmyFactory::save_construct_data
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void save_construct_data( Archive& archive, const ArmyFactory* factory, const unsigned int /*version*/ )
{
    const AutomateFactory_ABC* const automateFactory = &factory->automateFactory_;
    const FormationFactory_ABC* const formationFactory = &factory->formationFactory_;
    const MIL_ObjectManager* const objectFactory = &factory->objectFactory_;
    const PopulationFactory_ABC* const populationFactory = &factory->populationFactory_;
    const KnowledgeGroupFactory_ABC* const knowledgeGroupFactory = &factory->knowledgeGroupFactory_;
    archive << automateFactory
            << formationFactory
            << objectFactory
            << populationFactory
            << knowledgeGroupFactory;
}
// -----------------------------------------------------------------------------
// Name: ArmyFactory::save_construct_data
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void load_construct_data( Archive& archive, ArmyFactory* factory, const unsigned int /*version*/ )
{
    AutomateFactory_ABC* automateFactory;
    FormationFactory_ABC* formationFactory;
    MIL_ObjectManager* objectFactory;
    PopulationFactory_ABC* populationFactory;
    KnowledgeGroupFactory_ABC* knowledgeGroupFactory;
    archive >> automateFactory
            >> formationFactory
            >> objectFactory
            >> populationFactory
            >> knowledgeGroupFactory;
    ::new( factory )ArmyFactory( *automateFactory, *formationFactory, *objectFactory, *populationFactory, *knowledgeGroupFactory );
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory constructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
ArmyFactory::ArmyFactory( AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory, MIL_ObjectManager& objectFactory, PopulationFactory_ABC& populationFactory, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
    : automateFactory_( automateFactory )
    , formationFactory_( formationFactory )
    , objectFactory_( objectFactory )
    , populationFactory_( populationFactory )
    , knowledgeGroupFactory_( knowledgeGroupFactory )
    , diplomacyConverter_( new MT_Converter< std::string, MIL_Army_ABC::E_Diplomacy >( MIL_Army_ABC::eUnknown ) )
{
    InitializeDiplomacies();
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory destructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
ArmyFactory::~ArmyFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::Create
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
MIL_Army_ABC* ArmyFactory::Create( xml::xistream& xis )
{
    MIL_Army_ABC* army = new MIL_Army( xis, *this, formationFactory_, automateFactory_, objectFactory_, populationFactory_, knowledgeGroupFactory_, *diplomacyConverter_ );
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

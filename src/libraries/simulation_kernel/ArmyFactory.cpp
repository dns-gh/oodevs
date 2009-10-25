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
    archive << automateFactory
            << formationFactory
            << objectFactory
            << populationFactory;
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
    archive >> automateFactory
            >> formationFactory
            >> objectFactory
            >> populationFactory;
    ::new( factory )ArmyFactory( *automateFactory, *formationFactory, *objectFactory, *populationFactory );
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory constructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
ArmyFactory::ArmyFactory( AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory, MIL_ObjectManager& objectFactory, PopulationFactory_ABC& populationFactory )
    : automateFactory_( automateFactory )
    , formationFactory_( formationFactory )
    , objectFactory_( objectFactory )
    , populationFactory_( populationFactory )
{

}

// -----------------------------------------------------------------------------
// Name: ArmyFactory destructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
ArmyFactory::~ArmyFactory()
{

}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::Create
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
MIL_Army* ArmyFactory::Create( xml::xistream& xis )
{
    MIL_Army* Army = new MIL_Army( xis, *this, formationFactory_, automateFactory_, objectFactory_, populationFactory_ );
    Register( Army->GetID(), *Army );
    return Army;
}
// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MockAgentFactory.h"
#include "MockAutomateFactory.h"
#include "MockFormationFactory.h"
#include "MockKnowledgeGroupFactory.h" // LTO
#include "MockObjectManager.h"
#include "MockPopulationFactory.h"
#include "simulation_kernel/ArmyFactory.h"
#include "simulation_kernel/UrbanModel.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: FactoriesTest_ArmyFactory
// Created: MGD 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( FactoriesTest_ArmyFactory )
{
     MockAutomateFactory automateFactory;
     MockAgentFactory agentFactory;
     MockFormationFactory formationFactory;
     MockPopulationFactory populationFactory;
     MockObjectManager objetFactory;
     MockKnowledgeGroupFactory knowledgeGroupFactory; // LTO
     std::auto_ptr< UrbanModel > urbanModel( new UrbanModel() );
     ArmyFactory armyFactory( automateFactory, agentFactory, formationFactory, objetFactory, populationFactory, knowledgeGroupFactory /*LTO*/ );
     xml::xistringstream xis(
         "<side id='1' name='Local militia' type='enemy'>"
         "    <objects/>"
         "    <tactical/>"
         "    <communication/>"
         "    <populations/>"
         "    <logistics/>"
         "</side>" );
     xis >> xml::start( "side" );
     armyFactory.Create( xis );
     xis >> xml::end;
     BOOST_TODO;
}

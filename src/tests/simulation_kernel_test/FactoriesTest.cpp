// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"

#include "simulation_kernel/ArmyFactory.h"


#include "MockFormationFactory.h"
#include "MockAutomateFactory.h"
#include "MockPopulationFactory.h"
#include "MockObjectManager.h"

#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: FactoriesTest_ArmyFactory
// Created: MGD 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( FactoriesTest_ArmyFactory )
{
    MockAutomateFactory automateFactory;
    MockFormationFactory formationFactory;
    MockPopulationFactory populationFactory;
    MockObjectManager objetFactory;

    ArmyFactory armyFactory( automateFactory, formationFactory, objetFactory, populationFactory );


    MOCKPP_CHAINER_FOR( MockFormationFactory, CreateFormationShadow ) ( &formationFactory ).expects( mockpp::once() );


    xml::xistringstream xis( "<side id='1' name='Local militia' type='enemy'>"
                                "<objects>"
                                "</objects>"
                                "<tactical>"
                                    "<formation></formation>"
                                "</tactical>"
                                "<communication/>"
                                "<populations>"
                                "</populations>"
                                "<logistic/>"
                             "</side>");

    xis >> xml::start( "side" );
    armyFactory.Create( xis );
    xis >> xml::end();

    formationFactory.verify();
}
// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MockMIL_Time_ABC.h"
#include "SingletonTerminator.h"
#include "simulation_kernel/Entities/Agents/Units/Categories/PHY_Protection.h"
#include "simulation_kernel/Entities/Agents/Units/Composantes/PHY_ComposanteState.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_DotationLogisticType.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "simulation_kernel/Entities/Objects/BuildableCapacity.h"
#include "simulation_kernel/Entities/Objects/ImprovableCapacity.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectFactory.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectType_ABC.h"
#include "simulation_kernel/Entities/Objects/SpawnCapacity.h"
#include "simulation_kernel/Entities/Objects/PropagationCapacity.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyEmptyObjectDefinition )
{
    MIL_ObjectFactory factory;
    xml::xistringstream xis( "<objects/>" );
    factory.Initialize( xis );
    BOOST_CHECK_THROW( factory.FindType( "fake" ), std::exception );
}

BOOST_AUTO_TEST_CASE( VerifyDangerousObjects )
{
    // Need time to generate probability tables for protections...
    MockMIL_Time_ABC time;
    SingletonTerminator timeGuard;
    MOCK_EXPECT( time.GetTickDuration ).returns( 10 );

    xml::xistringstream xdotationLogisticType(
        "<logistic-supply-classes>"
        "  <logistic-supply-class id='8' type='Class V - Ammunition, explosives, and chemical agents of all types'/>"
        "</logistic-supply-classes>" );
    PHY_DotationLogisticType::Initialize( xdotationLogisticType );

    // Segfaults without this
    PHY_ComposanteState::Initialize();

    xml::xistringstream xdotationNatures(
        "<natures>"
        "  <nature id='3' type='Solid'/>"
        "</natures>" );
    PHY_DotationNature::Initialize( xdotationNatures );

    xml::xistringstream xprotections( 
        "<protections>"
        "  <protection name='HeavyTank' type='materiel'>"
        "    <neutralization average-time='300s' variance='60s'/>"
        "    <random-breakdown-probability eva='8' neva='0'/>"
        "    <attrition-effects>"
        "      <attrition-effect dead-percentage='15' equipment-state='ReparableAvecEvacuation' injured-percentage='60'/>"
        "    </attrition-effects>"
        "  </protection>"
        "</protections>" );
    PHY_Protection::Initialize( xprotections );

    xml::xistringstream xdotation(
        "<resources>"
        "  <resource category='mine' codeEMAT6='' codeEMAT8='' codeLFRIL='' codeNNO='' id='64' id-nature='3' "
        "      logistic-supply-class='Class V - Ammunition, explosives, and chemical agents of all types' "
        "      name='Standard Landmine' nature='Solid' network-usable='false' package-mass='0.005' "
        "      package-size='1' package-volume='0.005'>"
        "    <attritions>"
        "      <attrition destruction='0' protection='HeavyTank' repairable-with-evacuation='0.1' repairable-without-evacuation='0.3'/>"
        "    </attritions>"
        "  </resource>"
        "</resources>" );
    PHY_DotationType::Initialize( xdotation );

    MIL_ObjectFactory factory;
    xml::xistringstream xobject(
        "<objects>"
        "  <object name='Mine Cluster' point-size='50' type='mines'>"
        "    <geometries>"
        "      <geometry symbol='G*MPOMC---****X' type='point'/>"
        "    </geometries>"
        "    <activable/>"
        "    <attrition attrition-surface='0' category='Standard Landmine' ph='1'/>"
        "      <avoidable distance='50'/>"
        "      <bypassable bypass-speed='10'/>"
        "      <constructor default-consumption-mode='EnTravaux' unit-type='raw'>"
        "        <buildable>"
        "          <resources>"
        "            <resource count='10' name='Standard Landmine'/>"
        "          </resources>"
        "        </buildable>"
        "      </constructor>"
        "    <heuristic>"
        "      <terrain type='highway' value='1'/>"
        "    </heuristic>"
        "    <interaction-height height='1'/>"
        "    <mobility default-speed='0' max-unit-percentage-speed='0' unit-speed-impact-mode='AuPlusLent'/>"
        "    <population-filter density='0.1'/>"
        "    <time-limited/>"
        "  </object>"
        "</objects>");
    BOOST_CHECK_NO_THROW( factory.Initialize( xobject ) );

    std::vector< unsigned int > ids = factory.GetDangerousObjects();
    BOOST_CHECK_EQUAL( size_t( 1 ), ids.size() );
    BOOST_CHECK_EQUAL( 1UL, ids[0] );

    BOOST_CHECK_EQUAL( 50.0, factory.GetMaxAvoidanceDistance() );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Constructor )
{
    MIL_ObjectFactory factory;
    xml::xistringstream xis( "<objects>"
                             "    <object type='object'>"
                             "        <constructor unit-type='raw' default-consumption-mode='EnTravaux'/>"
                             "    </object>"
                             "</objects>" );
    BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );

    const MIL_ObjectType_ABC& type = factory.FindType( "object" );
    BOOST_CHECK( type.GetCapacity< BuildableCapacity >() == 0 );
    BOOST_CHECK( type.GetCapacity< ImprovableCapacity >() == 0 );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_ConstructorBuildable )
{
    MIL_ObjectFactory factory;
    xml::xistringstream xis( "<objects>"
                             "    <object type='object'>"
                             "        <constructor unit-type='raw' default-consumption-mode='EnTravaux'>"
                             "            <buildable/>"
                             "        </constructor>"
                             "    </object>"
                             "</objects>" );
    BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );

    const MIL_ObjectType_ABC& type = factory.FindType( "object" );
    BOOST_CHECK( type.GetCapacity< BuildableCapacity >() != 0 );
    BOOST_CHECK( type.GetCapacity< ImprovableCapacity >() == 0 );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_ConstructorImprovable )
{
    MIL_ObjectFactory factory;
    xml::xistringstream xis( "<objects>"
                             "    <object type='object'>"
                             "        <constructor unit-type='raw' default-consumption-mode='EnTravaux'>"
                             "            <buildable/>"
                             "            <improvable/>"
                             "        </constructor>"
                             "    </object>"
                             "</objects>" );
    BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );

    const MIL_ObjectType_ABC& type = factory.FindType( "object" );
    BOOST_CHECK( type.GetCapacity< BuildableCapacity >() != 0 );
    BOOST_CHECK( type.GetCapacity< ImprovableCapacity >() != 0 );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: SLG 2010-02-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Spawn )
{
    MIL_ObjectFactory factory;
    xml::xistringstream xis( "<objects>"
                             "    <object type='object'>"
                             "        <spawn object='toto' action-range='10' nbc='false'/>"
                             "    </object>"
                             "</objects>" );
    BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );

    const MIL_ObjectType_ABC& type = factory.FindType( "object" );
    BOOST_CHECK( type.GetCapacity< SpawnCapacity >() != 0 );
}

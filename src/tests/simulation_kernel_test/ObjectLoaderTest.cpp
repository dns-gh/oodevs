// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "simulation_kernel/Entities/Objects/BuildableCapacity.h"
#include "simulation_kernel/Entities/Objects/ImprovableCapacity.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectLoader.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectType_ABC.h"
#include "simulation_kernel/Entities/Objects/SpawnCapacity.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyEmptyObjectDefinition )
{
    MIL_ObjectLoader loader;
    xml::xistringstream xis( "<objects/>" );
    loader.Initialize( xis );
    BOOST_CHECK_THROW( loader.GetType( "fake" ), std::runtime_error );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Constructor )
{
    MIL_ObjectLoader loader;
    xml::xistringstream xis( "<objects>"
                             "    <object type='object'>"
                             "        <constructor unit-type='raw' default-consumption-mode='EnTravaux'/>"
                             "    </object>"
                             "</objects>" );
    BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );

    const MIL_ObjectType_ABC& type = loader.GetType( "object" );
    BOOST_CHECK( type.GetCapacity< BuildableCapacity >() == 0 );
    BOOST_CHECK( type.GetCapacity< ImprovableCapacity >() == 0 );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_ConstructorBuildable )
{
    MIL_ObjectLoader loader;
    xml::xistringstream xis( "<objects>"
                             "    <object type='object'>"
                             "        <constructor unit-type='raw' default-consumption-mode='EnTravaux'>"
                             "            <buildable/>"
                             "        </constructor>"
                             "    </object>"
                             "</objects>" );
    BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );

    const MIL_ObjectType_ABC& type = loader.GetType( "object" );
    BOOST_CHECK( type.GetCapacity< BuildableCapacity >() != 0 );
    BOOST_CHECK( type.GetCapacity< ImprovableCapacity >() == 0 );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_ConstructorImprovable )
{
    MIL_ObjectLoader loader;
    xml::xistringstream xis( "<objects>"
                             "    <object type='object'>"
                             "        <constructor unit-type='raw' default-consumption-mode='EnTravaux'>"
                             "            <buildable/>"
                             "            <improvable/>"
                             "        </constructor>"
                             "    </object>"
                             "</objects>" );
    BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );

    const MIL_ObjectType_ABC& type = loader.GetType( "object" );
    BOOST_CHECK( type.GetCapacity< BuildableCapacity >() != 0 );
    BOOST_CHECK( type.GetCapacity< ImprovableCapacity >() != 0 );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: SLG 2010-02-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Spawn )
{
    MIL_ObjectLoader loader;
    xml::xistringstream xis( "<objects>"
                             "    <object type='object'>"
                             "        <spawn object='toto' action-range='10' nbc='false'/>"
                             "    </object>"
                             "</objects>" );
    BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );

    const MIL_ObjectType_ABC& type = loader.GetType( "object" );
    BOOST_CHECK( type.GetCapacity< SpawnCapacity >() != 0 );
}

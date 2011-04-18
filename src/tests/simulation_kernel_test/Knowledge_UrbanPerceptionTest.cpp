// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "simulation_kernel/Knowledge/DEC_Knowledge_UrbanPerception.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectLoader.h"
#include "simulation_kernel/Entities/Objects/UrbanObjectWrapper.h"
#include "Fixture.h"
#include "MockMIL_Time_ABC.h"
#include "StubTER_World.h"
#include <urban/CoordinateConverter.h>
#include <urban/UrbanObject.h>
#include <memory>

namespace
{
    xml::xistringstream flux( "<urban-object id='9' name='Blokosolo'>"
                              "    <footprint>"
                              "        <point location='31TCM1272691661'/>"
                              "        <point location='31TCM1285291745'/>"
                              "        <point location='31TCM1291891650'/>"
                              "    <point location='31TCM1278691560'/>"
                              "    </footprint>"
                              "    <urban-objects/>"
                              "    <physical>"
                              "        <architecture trafficability='2' floor-number='10' height='20' occupation='0.25' material='concrete' roof-shape='flat'/>"
                              "    </physical>"
                              "</urban-object>" );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_UrbanTest_Update
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Knowledge_UrbanPerceptionTest_Update )
{
    WorldInitialize( "Paris" );
    MIL_ObjectLoader loader;
    {
        xml::xistringstream xis( "<objects>"
            "    <object type='urban block'/>"
            "</objects>" );
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    MIL_EffectManager effectManager;
    FixturePion pion( effectManager );
    urban::CoordinateConverter_ABC* converter = new urban::CoordinateConverter();
    flux >> xml::start( "urban-object" );
    const urban::TerrainObject_ABC* object = new urban::UrbanObject( flux, 0, *converter );
    UrbanObjectWrapper* pObject = static_cast< UrbanObjectWrapper* >( loader.CreateUrbanObject( *object ) );
    flux >> xml::end;

    {
        MockMIL_Time_ABC time;
        MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
        DEC_Knowledge_UrbanPerception kn( *pion.pPion_, pObject->GetID() );
    }
    delete object;
    delete pObject;
    delete converter;
    TER_World::DestroyWorld();
}

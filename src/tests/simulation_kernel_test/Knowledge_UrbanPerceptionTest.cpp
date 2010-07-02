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
#include "Fixture.h"
#include "MockMIL_Time_ABC.h"
#include <urban/CoordinateConverter.h>
#include "urban/UrbanObject.h"
#include <memory>

namespace
{
    xml::xistringstream flux( "<urbanObject id='9' name='Blokosolo'>"
                              "    <footprint>"
                              "        <point location='31TCM1272691661'/>"
                              "        <point location='31TCM1285291745'/>"
                              "        <point location='31TCM1291891650'/>"
                              "    <point location='31TCM1278691560'/>"
                              "    </footprint>"
                              "    <urbanObjects/>"
                              "    <physical>"
                              "        <architecture trafficability='2' floor-number='10' height='20' occupation='0.25' material='concrete' roof-shape='flat'/>"
                              "    </physical>"
                              "    <roles/>"
                              "</urbanObject>" );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_UrbanTest_Update
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Knowledge_UrbanPerceptionTest_Update )
{
    MIL_EffectManager effectManager;
    FixturePion pion( effectManager );
    urban::CoordinateConverter_ABC* converter = new urban::CoordinateConverter();
    flux >> xml::start( "urbanObject" );
    const urban::TerrainObject_ABC* object = new urban::UrbanObject( flux, 0, *converter );
    flux >> xml::end();

    {
        MockMIL_Time_ABC time;
        MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
        DEC_Knowledge_UrbanPerception kn( *pion.pPion_, *object );
    }
    delete object;
    delete converter;
}
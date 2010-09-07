// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MockArmy.h"
#include "simulation_kernel/Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "simulation_kernel/Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectLoader.h"
#include "simulation_kernel/Entities/Objects/MIL_Object_ABC.h"
#include "simulation_kernel/Knowledge/DEC_Knowledge_Urban.h"
#include "simulation_kernel/Knowledge/DEC_Knowledge_UrbanPerception.h"
#include "simulation_terrain/TER_World.h"
#include "Fixture.h"
#include "MockMIL_Time_ABC.h"
#include <urban/CoordinateConverter.h>
#include <urban/UrbanObject.h>
#include <memory>

namespace
{
    xml::xistringstream flux( "<urban-object id='9' name='Blokosolo'>"
                              "   <footprint>"
                              "       <point location='31TCM1272691661'/>"
                              "       <point location='31TCM1285291745'/>"
                              "       <point location='31TCM1291891650'/>"
                              "       <point location='31TCM1278691560'/>"
                              "   </footprint>"
                              "   <urban-objects/>"
                              "   <physical>"
                              "       <architecture trafficability='2' floor-number='10' height='20' occupation='0.25' material='concrete' roof-shape='flat'/>"
                              "   </physical>"
                              "</urban-object>" );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_UrbanTest_Update
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Knowledge_UrbanTest_Update )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
    MIL_ObjectLoader loader;
    {
        xml::xistringstream xis( "<objects>"
            "    <object type='urban block'/>"
            "</objects>" );
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    MockArmy army;
    MIL_EffectManager effectManager;
    FixturePion pion( effectManager );
    urban::CoordinateConverter_ABC* converter = new urban::CoordinateConverter();
    flux >> xml::start( "urban-object" );
    const urban::TerrainObject_ABC* pBlock = new urban::UrbanObject( flux, 0, *converter );
    flux >> xml::end;
    MIL_Object_ABC* pObject = loader.CreateUrbanObject( *pBlock );
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *pion.pPion_ );
    urbanRole->NotifyMovingInsideObject( *pObject);
    pion.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
    {
        MockMIL_Time_ABC time;
        MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
        DEC_Knowledge_Urban kn( army, *pBlock );
        DEC_Knowledge_UrbanPerception perception( *pion.pPion_, *pBlock );
        perception.SetPerceptionLevel( PHY_PerceptionLevel::detected_ );

        kn.Update( perception );
        BOOST_CHECK( kn.GetCurrentRecceProgress() > 0. );
    }
    delete pBlock;
    delete pObject;
    delete converter;
    TER_World::DestroyWorld();
}

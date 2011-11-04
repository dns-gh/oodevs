// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MockAgent.h"
#include "TestPK.h"
#include "Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectLoader.h"
#include "StubTER_World.h"
#include <urban/Model.h>
#include <urban/UrbanObject.h>
#include <urban/CoordinateConverter_ABC.h>
#include <urban/CoordinateConverter.h>
#include <urban/Drawer_ABC.h>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PkComputerTest
// Created: SLG 2010-04-14
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( PkComputerUrbanProtectionTest, TestPK )
{
    WorldInitialize( "france/BMDrosoville" );

    const PHY_DotationCategory* pCategory = PHY_DotationType::FindDotationCategory( "ammo" );

    xml::xistringstream xisModel(
        "<urban-object id='3' name='School'>"
        "   <footprint>"
        "       <point location='31TCM1058982147'/>"
        "       <point location='31TCM1055881170'/>"
        "       <point location='31TCM1157881118'/>"
        "       <point location='31TCM1158882115'/>"
        "   </footprint>"
        "   <physical>"
        "       <architecture floor-number='30' height='40' material='Brique' roof-shape='flat' occupation ='0.8' trafficability='60'/>"
        "   </physical>"
        "</urban-object>" );

    MockAgent firer;
    urban::CoordinateConverter coord;
    xisModel >> xml::start( "urban-object" );
    std::auto_ptr< urban::UrbanObject > urbanBlock;
    urbanBlock.reset( new urban::UrbanObject ( xisModel, 0, coord ) );
    xisModel >> xml::end;
    MIL_ObjectLoader loader;
    {
        xml::xistringstream xis(
            "<objects>"
            "    <object type='urban block'/>"
            "</objects>" );
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    std::auto_ptr< MIL_Object_ABC > pObject( loader.CreateUrbanObject( *urbanBlock ) );
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( firer );

    urbanRole->NotifyMovingInsideObject( *pObject );
    firer.RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );

    BOOST_CHECK_CLOSE( 0.48, urbanRole->ComputeUrbanProtection( *pCategory ), 1. );

    pObject.reset();
    TER_World::DestroyWorld();
}

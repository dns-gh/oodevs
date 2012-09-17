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
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Urban/MIL_UrbanObject.h"
#include "StubTER_World.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PkComputerTest
// Created: SLG 2010-04-14
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( PkComputerUrbanProtectionTest, TestPK )
{
    WorldInitialize( "worldwide/Paris" );

    const PHY_DotationCategory* pCategory = PHY_DotationType::FindDotationCategory( "ammo" );

    xml::xistringstream xisCity( "<urban-object name='city' id='1'/>" );
    xml::xistringstream xisDistrict( "<urban-object name='district' id='2'/>" );
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

    MIL_ObjectFactory factory;
    {
        xml::xistringstream xis(
            "<objects>"
            "    <object type='urban block'/>"
            "</objects>" );
        BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );
    }
    MockAgent firer;
    xisCity >> xml::start( "urban-object" );
    std::auto_ptr< MIL_UrbanObject_ABC > city;
    city.reset( factory.CreateUrbanObject( xisCity, 0 ) );
    xisCity >> xml::end;
    xisDistrict >> xml::start( "urban-object" );
    std::auto_ptr< MIL_UrbanObject_ABC > district;
    district.reset( factory.CreateUrbanObject( xisDistrict, city.get() ) );
    xisDistrict >> xml::end;
    xisModel >> xml::start( "urban-object" );
    std::auto_ptr< MIL_UrbanObject_ABC > urbanBlock;
    urbanBlock.reset( factory.CreateUrbanObject( xisModel, district.get() ) );
    xisModel >> xml::end;
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( firer );

    urbanRole->NotifyMovingInsideObject( *urbanBlock );
    firer.RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );

    BOOST_CHECK_CLOSE( 0.48, urbanRole->ComputeUrbanProtection( *pCategory ), 1. );

    urbanBlock.reset();
    TER_World::DestroyWorld();
}

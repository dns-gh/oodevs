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
#include <Urban/Model.h>
#include <Urban/UrbanObject.h>
#include <Urban/CoordinateConverter_ABC.h>
#include <Urban/CoordinateConverter.h>
#include <Urban/Drawer_ABC.h>
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: PkComputerTest
// Created: SLG 2010-04-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( PkComputerUrbanProtectionTest )
{
    TestPK* test = new TestPK();

    const PHY_DotationCategory* pCategory = PHY_DotationType::FindDotationCategory( "ammo" );

    xml::xistringstream xisModel( "<urbanObject id='3' name='School'>"
        "<footprint><point location='31TCM1058982147'/><point location='31TCM1055881170'/><point location='31TCM1157881118'/><point location='31TCM1158882115'/></footprint>"
        "<physical><architecture floor-number='30' height='40' material='Brique' roof-shape='flat' occupation ='0.8' trafficability='60'/>"
        "</physical>"
        "</urbanObject>");

    MockAgent firer;
    urban::Drawer_ABC* drawer = 0;
    urban::CoordinateConverter_ABC* coord = new urban::CoordinateConverter();
    xisModel >> xml::start( "urbanObject" );
    urban::UrbanObject urbanBlock( xisModel, 0, *coord );
    xisModel >> xml::end();
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( firer );
    urbanRole->NotifyMovingInsideUrbanBlock( urbanBlock );
    firer.RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );

    BOOST_CHECK_CLOSE( 0.48, urbanRole->ComputeUrbanProtection( *pCategory ), 1. );
    delete coord;
    delete test;
}
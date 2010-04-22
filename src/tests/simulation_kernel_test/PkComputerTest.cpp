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
#include <Urban/Block.h>
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

    xml::xistringstream xisModel( "<block id='3' name='School'>"
        "<footprint><point location='31TCM1058982147'/><point location='31TCM1055881170'/><point location='31TCM1157881118'/><point location='31TCM1158882115'/></footprint>"
        "<physical><architecture basement-level-number='2' facade-opacity='0.6' floor-number='30' height='40' inner-cluttering='0.25' material='Brique' roof-shape='flat'/>"
        "<soil enclosure='open' is-multiple='true' occupation='0.8' trafficability='0.1'/></physical>"
        "</block>");

    MockAgent firer;
    urban::Drawer_ABC* drawer = 0;
    urban::CoordinateConverter_ABC* coord = new urban::CoordinateConverter();
    xisModel >> xml::start( "block" );
    urban::Block urbanBlock( xisModel, 0, drawer, *coord );
    xisModel >> xml::end();
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( firer );
    urbanRole->NotifyMovingInsideUrbanBlock( urbanBlock );
    firer.RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );

    BOOST_CHECK_CLOSE( 0.64, urbanRole->ComputeUrbanProtection( *pCategory ), 1. );
    delete coord;
    delete test;
}
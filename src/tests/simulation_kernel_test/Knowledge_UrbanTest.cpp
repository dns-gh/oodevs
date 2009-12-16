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
#include "simulation_kernel/Entities/MIL_Army.h"
#include "simulation_kernel/Knowledge/DEC_Knowledge_Urban.h"
#include "simulation_kernel/Knowledge/DEC_Knowledge_UrbanPerception.h"
#include "Fixture.h"
#include "urban/BlockModel.h"
#include "urban/Block.h"
#include "urban/model.h"
#include "urban/StaticModel.h"
#include "xeumeuleu/xml.h"
#include <memory>

namespace
{
    xml::xistringstream flux( "<?xml version='1.0' encoding='UTF-8' standalone='no' ?>"
        "<urban>"
        "<model version='1.0'/>"
        "<cities>"
        "<city id='7' name='default'>"
        "<footprint/>"
        "<districts>"
        "<district id='8' name='default'>"
        "<footprint/>"
        "<blocks>"
        "<block id='9' name='Blokosolo'>"
        "<footprint>"
        "<point location='31TCM1272691661'/>"
        "<point location='31TCM1285291745'/>"
        "<point location='31TCM1291891650'/>"
        "<point location='31TCM1278691560'/>"
        "</footprint>"
        "<physical>"
        "<architecture basement-level-number='2' facade-opacity='0.6' floor-number='10' height='20' inner-cluttering='0.25' material='concrete' roof-shape='flat'/>"
        "<soil enclosure='open' is-multiple='true' occupation='0.8' trafficability='0.1'/>"
        "<vegetation density='0.25' height='10' type='trees'/>"
        "</physical>"
        "<roles/>"
        "</block>"
        "</blocks>"
        "</district>"
        "</districts>"
        "</city>"
        "</cities>"
        "<resource-networks/>"
        "<underground-networks/>"
        "</urban>" );

    std::auto_ptr< urban::StaticModel > staticModel( new urban::StaticModel() );
    std::auto_ptr< urban::Model > model( new urban::Model( *staticModel ) );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_UrbanTest_Update
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Knowledge_UrbanTest_Update )
{
    model->Load( flux );

    MockArmy army;
    FixturePion pion;

    const urban::Block& object = model->blocks_.Get( 9 );

    {
        DEC_Knowledge_Urban kn( army, object );
        DEC_Knowledge_UrbanPerception perception( *pion.pPion_, object );

        kn.Update( perception );
        BOOST_CHECK_EQUAL( &kn.GetCurrentPerceptionLevel(), &PHY_PerceptionLevel::detected_ );
        //IN WORK
        //BOOST_CHECK( kn.GetProgress() > 0. );

    }
}
// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"

#include "SingletonTerminator.h"
//#include "MockPHY_RawVisionData_ABC.h"
#include "simulation_kernel/Meteo/PHY_LocalMeteo.h"
#include "meteo/PHY_Lighting.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: SLG 2010-03-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( UpdateMeteoPatch )
{
   /*
   SingletonTerminator terminate;
   xml::xistringstream xis( "<local bottom-right='31TCM3740970227' end-time='20091126T180000' start-time='20091126T170000' top-left='31TCM1890188682'>"
        "<wind direction='1' speed='5'/>"
        "<cloud-cover ceiling='9900' density='1' floor='900'/>"
        "<precipitation value='Crachin'/>"
        "</local>" );
    PHY_Lighting::Initialize();
    const PHY_Lighting* light = PHY_Lighting::FindLighting( "JourTresNuageux" );
    MockPHY_RawVisionData_ABC mockRawVisionData();
    PHY_LocalMeteo localMeteo( xis, *light, 1 );

    MOCKPP_CHAINER_FOR( MockPHY_RawVisionData_ABC, RegisterMeteoPatch ) ( &mockRawVisionData ).expects( mockpp::once() );
    localMeteo.UpdateMeteoPatch( 10, mockRawVisionData );

    MOCKPP_CHAINER_FOR( MockPHY_RawVisionData_ABC, UnregisterMeteoPatch ) ( &mockRawVisionData ).expects( mockpp::once() );
    localMeteo.UpdateMeteoPatch( 20, mockRawVisionData );
    */

}
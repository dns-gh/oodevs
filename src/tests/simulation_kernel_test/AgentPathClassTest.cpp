// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Decision/DEC_Agent_PathClass.h"
#include "simulation_kernel/Entities/Populations/MIL_PopulationAttitude.h"
#include "protocol/Protocol.h"
#include "spatialcontainer/TerrainData.h"
#include <tools/BoostTest.h>
#include <xeumeuleu/xml.hpp>

BOOST_AUTO_TEST_CASE( path_class_reads_xml )
{
    MIL_PopulationAttitude::Initialize();

    xml::xistringstream  xis( "<rule autonomous='false' flying='false' type='base'>"
      "<optimisation shortest='false'/>"
      "<preferred-altitude value='1'/>"
      "<preferred-terrains strength='2'>"
        "<preferred-terrain type='urban area'/>"
        "<preferred-terrain type='forest edge'/>"
      "</preferred-terrains>"
      "<avoided-terrains strength='3'>"
        "<avoided-terrain type='cliff'/>"
        "<preferred-terrain type='forest'/>"
      "</avoided-terrains>"
      "<zone>"
        "<outter-tolerance cost-per-meter='4' with-automat='5' without-automat='6'/>"
        "<inner-comfort cost-per-meter='7' distance='8'/>"
      "</zone>"
      "<automat-zone>"
        "<outter-tolerance cost-per-meter='9' distance='10'/>"
      "</automat-zone>"
      "<danger-direction base-cost-beyond='11' cost-per-meter='12'/>"
      "<enemies cost-at-security-range='13' cost-on-contact='14' maximum-cost='15'/>"
      "<population-costs maximum='16' security-range='17' outside-of-population='18'>"
        "<population-cost attitude='agitee' contact-cost='19'/>"
      "</population-costs>"
      "<object-costs avoid='true' threshold='20'>"
// Should be tested but requires MIL_AgentServer::GetWorkspace().GetObjectFactory() to be defined...
//        "<object-cost type='anti tank obstacle' value='22'/>"
      "</object-costs>"
    "</rule>" );
    
    const std::vector< unsigned int > dangerousObjects;
    xis >> xml::start( "rule" );
    DEC_Agent_PathClass pathClass( xis, dangerousObjects );

    BOOST_CHECK_EQUAL( 2, pathClass.GetPreferedTerrainCost() );
    const TerrainData& preferredTerrainData = pathClass.GetPreferedTerrain();
    BOOST_CHECK( preferredTerrainData.GetCode() == ( TerrainData::Urban().GetCode() | TerrainData::ForestBorder().GetCode() ) );
    BOOST_CHECK_EQUAL( 3, pathClass.GetAvoidedTerrainCost() );
    const TerrainData& avoidedTerrainData = pathClass.GetAvoidedTerrain();
    BOOST_CHECK( avoidedTerrainData.GetCode() == ( TerrainData::Cliff().GetCode() | TerrainData::Forest().GetCode() ) );
    BOOST_CHECK_EQUAL( 1, pathClass.GetAltitudePreference() );
    BOOST_CHECK_EQUAL( 8, pathClass.GetComfortFuseauDistance() );
    BOOST_CHECK_EQUAL( 4, pathClass.GetFuseauCostPerMeterOut() );
    BOOST_CHECK_EQUAL( 7, pathClass.GetFuseauCostPerMeterIn() );
    BOOST_CHECK_EQUAL( 6, pathClass.GetMaximumFuseauDistance() );
    BOOST_CHECK_EQUAL( 5, pathClass.GetMaximumFuseauDistanceWithAutomata() );
    BOOST_CHECK_EQUAL( 9, pathClass.GetAutomateFuseauCostPerMeterOut() );
    BOOST_CHECK_EQUAL( 10, pathClass.GetMaximumAutomataFuseauDistance() );
    BOOST_CHECK_EQUAL( 11, pathClass.GetDangerDirectionBaseCost() );
    BOOST_CHECK_EQUAL( 12, pathClass.GetDangerDirectionLinearCost() );
    BOOST_CHECK_EQUAL( true, pathClass.AvoidObjects() );
    BOOST_CHECK_EQUAL( true, pathClass.AvoidEnemies() );
    BOOST_CHECK_EQUAL( true, pathClass.HandlePopulations() );
    BOOST_CHECK_EQUAL( 14, pathClass.GetEnemyCostOnContact() );
    BOOST_CHECK_EQUAL( 13, pathClass.GetEnemyCostAtSecurityRange() );
    BOOST_CHECK_EQUAL( 15, pathClass.GetEnemyMaximumCost() );
    // Should be tested but requires MIL_AgentServer::GetWorkspace().GetObjectFactory() to be defined...
    //      double       GetObjectCost                       ( const MIL_ObjectType_ABC& objectType ) const;
    BOOST_CHECK_EQUAL( 16, pathClass.GetPopulationMaximumCost() );
    BOOST_CHECK_EQUAL( 20, pathClass.GetThreshold() );
    BOOST_CHECK_EQUAL( 19, pathClass.GetPopulationAttitudeCost( sword::agitated ) );
    BOOST_CHECK_EQUAL( 17, pathClass.GetPopulationSecurityRange() );
    BOOST_CHECK_EQUAL( 18, pathClass.GetCostOutsideOfPopulation() );
    BOOST_CHECK_EQUAL( false, pathClass.IsShort() );
    BOOST_CHECK_EQUAL( false, pathClass.IsFlying() );
}
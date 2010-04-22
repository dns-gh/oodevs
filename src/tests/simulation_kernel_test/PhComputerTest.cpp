// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"

#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/MT_Vector3D.h"
#include "MT_Tools/MT_Tools_Types.h"
#include <xeumeuleu/xml.h>
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponDataType_DirectFire.h"
#include "Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Fixture.h"
#include "UrbanModel.h"
#include <Urban/Model.h>
#include <Urban/Block.h>
#include <Urban/Drawer_ABC.h>

#include "MockMIL_Time_ABC.h"
#include "MockAgent.h"
#include "MockRoleInterface_Posture.h"
#include "MockPHY_Composante_ABC.h"
#include "MockPHY_FireResults_ABC.h"
#include "MockPHY_RoleInterface_ActiveProtection.h"
#include "MockPHY_RoleInterface_HumanFactors.h"
#include "MockRoleLocation.h"
#include "TestPhDirectFire.h"

#include "simulation_terrain/TER_World.h"


// -----------------------------------------------------------------------------
// Name: PhComputerTest
// Created: SLG 2010-01-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( PhComputerFirerPositionTest )
{
    TER_World::Initialize( "../../data/data/terrains/Drosoville/Terrain.xml" );

    MIL_EffectManager effectManager;
    FixturePion firerFixture( effectManager );
    FixturePion targetFixture( effectManager );
    std::vector< geometry::Point2f > vertices;
    vertices.push_back( geometry::Point2f( -1, -1 ) );
    vertices.push_back( geometry::Point2f( -1, 1 ) );
    vertices.push_back( geometry::Point2f( 1, 1 ) );
    vertices.push_back( geometry::Point2f( 1, -1 ) );
    
    urban::Drawer_ABC* drawer = 0;
    urban::Block urbanBlock( 0, "test", geometry::Polygon2f( vertices ), 0, drawer );

    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    urbanRole->NotifyMovingInsideUrbanBlock( urbanBlock );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );

    PHY_RolePion_Location* firerlocationRole = new PHY_RolePion_Location( *firerFixture.pPion_ );
    firerlocationRole->MagicMove( MT_Vector2D( 0, 0 ) );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *firerlocationRole );

    PHY_RolePion_Location* targetLocationRole = new PHY_RolePion_Location( *targetFixture.pPion_ );
    targetLocationRole->MagicMove( MT_Vector2D( 2, 1 ) );
    targetFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *targetLocationRole );

    geometry::Point2f result( 1, 0.5 );
    BOOST_CHECK_EQUAL( result, urbanRole->GetFirerPosition( *targetFixture.pPion_ ) );
    TER_World::DestroyWorld();  
}

BOOST_AUTO_TEST_CASE( PhComputerTargetPositionTest )
{
    TER_World::Initialize( "../../data/data/terrains/Drosoville/Terrain.xml" );
    MIL_EffectManager effectManager;
    FixturePion firerFixture( effectManager );
    FixturePion targetFixture( effectManager );
    std::vector< geometry::Point2f > vertices;
    vertices.push_back( geometry::Point2f( -1, -1 ) );
    vertices.push_back( geometry::Point2f( -1, 1 ) );
    vertices.push_back( geometry::Point2f( 1, 1 ) );
    vertices.push_back( geometry::Point2f( 1, -1 ) );

    urban::Drawer_ABC* drawer = 0;
    urban::Block urbanBlock( 0, "test", geometry::Polygon2f( vertices ), 0, drawer );
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    urbanRole->NotifyMovingInsideUrbanBlock( urbanBlock );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );

    PHY_RolePion_Location* targetLocationRole = new PHY_RolePion_Location( *targetFixture.pPion_ );
    targetLocationRole->MagicMove( MT_Vector2D( 0, 0 ) );
    targetFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *targetLocationRole );

    PHY_RolePion_Location* firerLocationRole = new PHY_RolePion_Location( *firerFixture.pPion_ );
    firerLocationRole->MagicMove( MT_Vector2D( 2, 1 ) );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *firerLocationRole );

    const geometry::Point2f firerPosition( 2, 1 );
    const geometry::Point2f targetPosition( 0, 0 );
    geometry::Point2f result = urbanRole->GetTargetPosition( *targetFixture.pPion_ );
    BOOST_CHECK_CLOSE( std::sqrt( 5.0 ), firerPosition.Distance( result ), 50 );
    BOOST_CHECK_EQUAL( true, geometry::Vector2f( firerPosition, targetPosition ).IsParallel( geometry::Vector2f( firerPosition, result ) ) );
    TER_World::DestroyWorld();
}

BOOST_AUTO_TEST_CASE( PhComputerDistanceInSameBUTest )
{
    TER_World::Initialize( "../../data/data/terrains/Drosoville/Terrain.xml" );
    MIL_EffectManager effectManager;
    FixturePion firerFixture( effectManager );
    FixturePion targetFixture( effectManager );
    std::vector< geometry::Point2f > vertices;
    vertices.push_back( geometry::Point2f( -1, -1 ) );
    vertices.push_back( geometry::Point2f( -1, 1 ) );
    vertices.push_back( geometry::Point2f( 1, 1 ) );
    vertices.push_back( geometry::Point2f( 1, -1 ) );

    urban::Drawer_ABC* drawer = 0;
    urban::Block urbanBlock( 0, "test", geometry::Polygon2f( vertices ), 0, drawer );
    PHY_RolePion_UrbanLocation* firerUrbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    firerUrbanRole->NotifyMovingInsideUrbanBlock( urbanBlock );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *firerUrbanRole );

    PHY_RolePion_Location* firerLocationRole = new PHY_RolePion_Location( *firerFixture.pPion_ );
    firerLocationRole->MagicMove( MT_Vector2D( -0.5, -0.5 ) );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *firerLocationRole );

    PHY_RolePion_UrbanLocation* targetUrbanRole = new PHY_RolePion_UrbanLocation( *targetFixture.pPion_ );
    targetUrbanRole->NotifyMovingInsideUrbanBlock( urbanBlock );
    targetFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *targetUrbanRole );

    PHY_RolePion_Location* targetLocationRole = new PHY_RolePion_Location( *targetFixture.pPion_ );
    targetLocationRole->MagicMove( MT_Vector2D( 0.5, 0.5 ) );
    targetFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *targetLocationRole );

    float result = firerUrbanRole->ComputeDistanceInsideSameUrbanBlock( *targetFixture.pPion_ );
    bool isUnderMaxValue = 2 >= result;
    bool isUpMinValue = 0 <= result;
    BOOST_CHECK_EQUAL( true, isUnderMaxValue );
    BOOST_CHECK_EQUAL( true, isUpMinValue );

    TER_World::DestroyWorld();
}



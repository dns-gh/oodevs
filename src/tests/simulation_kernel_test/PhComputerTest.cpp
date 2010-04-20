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

// -----------------------------------------------------------------------------
// Name: PhComputerTest
// Created: SLG 2010-01-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( PhComputerFirerPositionTest )
{
    MockAgent firer;
    std::vector< geometry::Point2f > vertices;
    vertices.push_back( geometry::Point2f( -1, -1 ) );
    vertices.push_back( geometry::Point2f( -1, 1 ) );
    vertices.push_back( geometry::Point2f( 1, 1 ) );
    vertices.push_back( geometry::Point2f( 1, -1 ) );
    
    urban::Drawer_ABC* drawer = 0;
    urban::Block urbanBlock( 0, "test", geometry::Polygon2f( vertices ), 0, drawer );
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( firer );
    urbanRole->NotifyMovingInsideUrbanBlock( urbanBlock );
    firer.RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
    const geometry::Point2f firerPosition( 0, 0 );
    const geometry::Point2f targetPosition( 2, 1 );

    geometry::Point2f result( 1, 0.5 );
    BOOST_CHECK_EQUAL( result, urbanRole->GetFirerPosition( firerPosition, targetPosition ) );
}

BOOST_AUTO_TEST_CASE( PhComputerTargetPositionTest )
{
    MockAgent target;
    std::vector< geometry::Point2f > vertices;
    vertices.push_back( geometry::Point2f( -1, -1 ) );
    vertices.push_back( geometry::Point2f( -1, 1 ) );
    vertices.push_back( geometry::Point2f( 1, 1 ) );
    vertices.push_back( geometry::Point2f( 1, -1 ) );

    urban::Drawer_ABC* drawer = 0;
    urban::Block urbanBlock( 0, "test", geometry::Polygon2f( vertices ), 0, drawer );
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( target );
    urbanRole->NotifyMovingInsideUrbanBlock( urbanBlock );
    target.RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
    const geometry::Point2f firerPosition( 2, 1 );
    const geometry::Point2f targetPosition( 0, 0 );
    geometry::Point2f result = urbanRole->GetTargetPosition( firerPosition, targetPosition );
    BOOST_CHECK_CLOSE( std::sqrt( 5.0 ), firerPosition.Distance( result ), 50 );
    BOOST_CHECK_EQUAL( true, geometry::Vector2f( firerPosition, targetPosition ).IsParallel( geometry::Vector2f( firerPosition, result ) ) );
}



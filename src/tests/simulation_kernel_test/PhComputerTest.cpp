// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MT_Tools/MT_Droite.h"
#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/MT_Vector3D.h"
#include "MT_Tools/MT_Ellipse.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponDataType_DirectFire.h"
#include "Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Objects/MIL_ObjectLoader.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Fixture.h"
#include "MockMIL_Time_ABC.h"
#include "MockAgent.h"
#include "MockRoleInterface_Posture.h"
#include "MockPHY_Composante_ABC.h"
#include "MockPHY_FireResults_ABC.h"
#include "MockPHY_RoleInterface_ActiveProtection.h"
#include "MockPHY_RoleInterface_HumanFactors.h"
#include "MockRoleLocation.h"
#include "TestPhDirectFire.h"
#include "StubTER_World.h"
#include <boost/assign/list_of.hpp>
#include <xeumeuleu/xml.hpp>
#include <urban/CoordinateConverter.h>
#include <urban/Model.h>
#include <urban/UrbanObject.h>

namespace
{
    struct WorldFixture
    {
        WorldFixture()
        {
            WorldInitialize( "Paris" );
        }
        ~WorldFixture()
        {
            TER_World::DestroyWorld();
        }
    };

    struct Fixture : WorldFixture
    {
        Fixture()
            : firerFixture ( effectManager )
            , targetFixture( effectManager )
            , vertices     ( boost::assign::list_of( geometry::Point2f( 0, 0 ) )( geometry::Point2f( 0, 2 ) )
                                                   ( geometry::Point2f( 2, 2 ) )( geometry::Point2f( 2, 0 ) ) )
            , poly         ( vertices )
            , urbanBlock   ( 0, "test", &poly, 0, coord )
            , xis          ( "<objects>"
                             "    <object type='urban block'/>"
                             "</objects>" )
        {
            loader.Initialize( xis );
        }
        MIL_EffectManager effectManager;
        FixturePion firerFixture;
        FixturePion targetFixture;
        std::vector< geometry::Point2f > vertices;
        geometry::Polygon2f poly;
        urban::CoordinateConverter coord;
        urban::UrbanObject urbanBlock;
        xml::xistringstream xis;
        MIL_ObjectLoader loader;
    };
}

// -----------------------------------------------------------------------------
// Name: PhComputerTest
// Created: SLG 2010-01-19
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( PhComputerFirerPositionTest, Fixture )
{
    std::auto_ptr< MIL_Object_ABC > pObject( loader.CreateUrbanObject( urbanBlock ) );
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
    PHY_RolePion_Location* firerlocationRole = new PHY_RolePion_Location( *firerFixture.pPion_ );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *firerlocationRole );
    PHY_RolePion_Location* targetLocationRole = new PHY_RolePion_Location( *targetFixture.pPion_ );
    targetFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *targetLocationRole );
    urbanRole->NotifyMovingInsideObject( *pObject);
    firerlocationRole->MagicMove( MT_Vector2D( 1, 1 ) );
    targetLocationRole->MagicMove( MT_Vector2D( 3, 2 ) );
    MT_Vector2D result( 2, 1.5 );
    BOOST_CHECK_EQUAL( result, urbanRole->GetFirerPosition( *targetFixture.pPion_ ) );
}

BOOST_FIXTURE_TEST_CASE( PhComputerTargetPositionTest, Fixture )
{
    std::auto_ptr< MIL_Object_ABC > pObject( loader.CreateUrbanObject( urbanBlock ) );
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
    PHY_RolePion_Location* targetLocationRole = new PHY_RolePion_Location( *targetFixture.pPion_ );
    targetFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *targetLocationRole );
    PHY_RolePion_Location* firerLocationRole = new PHY_RolePion_Location( *firerFixture.pPion_ );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *firerLocationRole );
    urbanRole->NotifyMovingInsideObject( *pObject );
    targetLocationRole->MagicMove( MT_Vector2D( 1, 1 ) );
    firerLocationRole->MagicMove( MT_Vector2D( 3, 2 ) );
    const MT_Vector2D firerPosition( 3, 2 );
    const MT_Vector2D targetPosition( 1, 1 );
    MT_Vector2D result = urbanRole->GetTargetPosition( *targetFixture.pPion_ );
    BOOST_CHECK_CLOSE( std::sqrt( 1.25 ), firerPosition.Distance( result ), 1. );
    MT_Vector2D tmp;
    BOOST_CHECK_EQUAL( eCollinear, MT_Droite( firerPosition, targetPosition ).Intersect2D( MT_Droite( firerPosition, result ), tmp ) );
}

BOOST_FIXTURE_TEST_CASE( PhComputerIndirectPhModifier, Fixture )
{
    std::auto_ptr< MIL_Object_ABC > pObject( loader.CreateUrbanObject( urbanBlock ) );
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
    PHY_RolePion_Location* locationRole = new PHY_RolePion_Location( *firerFixture.pPion_ );
    urbanRole->NotifyMovingInsideObject( *pObject );
    locationRole->MagicMove( MT_Vector2D( 1, 1 ) );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *locationRole );
    const MT_Ellipse attritionSurface( MT_Vector2D( 3, 2 ), MT_Vector2D( 5, 2 ),  MT_Vector2D( 3, 3 ) );
    BOOST_CHECK_CLOSE( 0.0, urbanRole->ComputeRatioPionInside( attritionSurface ), 1. ); // $$$$ _RC_ SLG 2010-04-26: trouver un moyen de changer la posture du pion pour tester le résultat
}

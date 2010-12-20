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
    class Configuration
    {
    public:
        Configuration()
            : firerFixture ( effectManager )
            , targetFixture( effectManager )
        {
            vertices = boost::assign::list_of( geometry::Point2f( -1, -1 ) )( geometry::Point2f( -1, 1 ) )
                                             ( geometry::Point2f( 1, 1 ) )( geometry::Point2f( 1, -1 ) );
        }
        MIL_EffectManager effectManager;
        FixturePion firerFixture;
        FixturePion targetFixture;
        std::vector< geometry::Point2f > vertices;
    };
    class Fixture : public Configuration
    {
    public:
        Fixture()
            : poly( vertices )
            , coord( new urban::CoordinateConverter() )
            , urbanBlock( new urban::UrbanObject ( 0, "test", &poly, 0, *coord ) )
        {
            WorldInitialize( "Paris" );
        }
        ~Fixture()
        {
            TER_World::DestroyWorld();
            delete coord;
        }
        geometry::Polygon2f poly;
        std::auto_ptr< urban::UrbanObject > urbanBlock;
        urban::CoordinateConverter_ABC* coord;
    };
}

// -----------------------------------------------------------------------------
// Name: PhComputerTest
// Created: SLG 2010-01-19
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( PhComputerFirerPositionTest, Fixture )
{
    MIL_ObjectLoader loader;
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object type='urban block'/>"
                                "</objects>" );
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    std::auto_ptr< MIL_Object_ABC > pObject;
    {
        BOOST_CHECK_NO_THROW( pObject.reset( loader.CreateUrbanObject( *urbanBlock ) ) );
    }
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    urbanRole->NotifyMovingInsideObject( *pObject);
    firerFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
    PHY_RolePion_Location* firerlocationRole = new PHY_RolePion_Location( *firerFixture.pPion_ );
    firerlocationRole->MagicMove( MT_Vector2D( -0.1, -0.1 ) );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *firerlocationRole );
    PHY_RolePion_Location* targetLocationRole = new PHY_RolePion_Location( *targetFixture.pPion_ );
    targetLocationRole->MagicMove( MT_Vector2D( 2, 1 ) );
    targetFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *targetLocationRole );
    geometry::Point2f result( 1, 0.5 );
    BOOST_CHECK_EQUAL( result, urbanRole->GetFirerPosition( *targetFixture.pPion_ ) );
    pObject.reset();
}

BOOST_FIXTURE_TEST_CASE( PhComputerTargetPositionTest, Fixture )
{
    MIL_ObjectLoader loader;
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object type='urban block'/>"
                                 "</objects>" );
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    MIL_Object_ABC* pObject = loader.CreateUrbanObject( *urbanBlock );
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    urbanRole->NotifyMovingInsideObject( *pObject );
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
}

BOOST_AUTO_TEST_CASE( PhComputerDistanceInSameBUTest )
{
    //TODO
}

BOOST_FIXTURE_TEST_CASE( PhComputerIndirectPhModifier, Fixture )
{
    MIL_ObjectLoader loader;
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object type='urban block'/>"
                                 "</objects>" );
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    MIL_Object_ABC* pObject = loader.CreateUrbanObject( *urbanBlock );
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    urbanRole->NotifyMovingInsideObject( *pObject );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
    PHY_RolePion_Location* locationRole = new PHY_RolePion_Location( *firerFixture.pPion_ );
    locationRole->MagicMove( MT_Vector2D( 0, 0 ) );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *locationRole );
    const MT_Ellipse attritionSurface( MT_Vector2D( 2, 1 ), MT_Vector2D( 4, 1 ),  MT_Vector2D( 2, 2 ) );
    BOOST_CHECK_CLOSE( 0.0, urbanRole->ComputeRatioPionInside( attritionSurface ), 1. ); // $$$$ _RC_ SLG 2010-04-26: trouver un moyen de changer la posture du pion pour tester le résultat
}

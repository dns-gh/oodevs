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
#include "MissionController.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponDataType_DirectFire.h"
#include "Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Urban/MIL_UrbanObject.h"
#include "Fixture.h"
#include "MockMIL_Time_ABC.h"
#include "MockAgent.h"
#include "MockRoleInterface_Posture.h"
#include "MockPHY_Composante_ABC.h"
#include "MockPHY_FireResults_ABC.h"
#include "MockPHY_RoleInterface_ActiveProtection.h"
#include "MockPHY_RoleInterface_HumanFactors.h"
#include "MockRoleLocation.h"
#include "StubTER_World.h"
#include "ActionManager.h"
#include <boost/assign/list_of.hpp>
#include <xeumeuleu/xml.hpp>

namespace
{
    struct Fixture
    {
        Fixture()
            : world( "worldwide/tests/EmptyParis-ML" )
            , controller   ( actions )
            , firerFixture ( controller, effectManager )
            , targetFixture( controller, effectManager )
            , vertices     ( boost::assign::list_of( MT_Vector2D( 0, 0 ) )( MT_Vector2D( 0, 2 ) )
                                                   ( MT_Vector2D( 2, 2 ) )( MT_Vector2D( 2, 0 ) ) )
            , xis          ( "<objects>"
                             "    <object type='urban block'/>"
                             "</objects>" )
        {
            factory.Initialize( xis );
            xml::xistringstream xisCity( "<urban-object name='city' id='2'/>" );
            xml::xistringstream xisDistrict( "<urban-object name='district' id='1'/>" );
            xml::xistringstream xisBlock( "<urban-object name='test' id='0'/>" );
            xisCity >> xml::start( "urban-object" );
            city.reset( factory.CreateUrbanObject( xisCity, 0 ) );
            xisCity >> xml::end;
            xisDistrict >> xml::start( "urban-object" );
            district.reset( factory.CreateUrbanObject( xisDistrict, city.get() ) );
            xisDistrict >> xml::end;
            xisBlock >> xml::start( "urban-object" );
            urbanBlock.reset( factory.CreateUrbanObject( xisBlock, district.get() ) );
            xisBlock >> xml::end;
            TER_Localisation localisation( TER_Localisation::ePolygon, vertices );
            urbanBlock->UpdateLocalisation( localisation );
        }
        FakeWorld world;
        ActionManager actions;
        MissionController controller;
        MIL_EffectManager effectManager;
        FixturePion firerFixture;
        FixturePion targetFixture;
        std::vector< MT_Vector2D > vertices;
        std::auto_ptr< MIL_UrbanObject_ABC > city;
        std::auto_ptr< MIL_UrbanObject_ABC > district;
        std::auto_ptr< MIL_UrbanObject_ABC > urbanBlock;
        xml::xistringstream xis;
        MIL_ObjectFactory factory;
    };
}

// -----------------------------------------------------------------------------
// Name: PhComputerTest
// Created: SLG 2010-01-19
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( PhComputerFirerPositionTest, Fixture )
{
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
    PHY_RolePion_Location* firerlocationRole = new PHY_RolePion_Location( *firerFixture.pPion_ );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *firerlocationRole );
    urbanRole->NotifyMovingInsideObject( *urbanBlock);
    firerlocationRole->MagicMove( MT_Vector2D( 1, 1 ) );
    const MT_Vector2D result( 2, 1.5 );
    BOOST_CHECK_EQUAL( result, urbanRole->GetFirerPosition( MT_Vector2D( 3, 2 ) ) );
}

BOOST_FIXTURE_TEST_CASE( PhComputerTargetPositionTest, Fixture )
{
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
    PHY_RolePion_Location* targetLocationRole = new PHY_RolePion_Location( *targetFixture.pPion_ );
    targetFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *targetLocationRole );
    PHY_RolePion_Location* firerLocationRole = new PHY_RolePion_Location( *firerFixture.pPion_ );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *firerLocationRole );
    urbanRole->NotifyMovingInsideObject( *urbanBlock );
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
    PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *firerFixture.pPion_ );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
    PHY_RolePion_Location* locationRole = new PHY_RolePion_Location( *firerFixture.pPion_ );
    urbanRole->NotifyMovingInsideObject( *urbanBlock );
    locationRole->MagicMove( MT_Vector2D( 1, 1 ) );
    firerFixture.pPion_->RegisterRole< PHY_RolePion_Location >( *locationRole );
    const MT_Ellipse attritionSurface( MT_Vector2D( 3, 2 ), MT_Vector2D( 5, 2 ),  MT_Vector2D( 3, 3 ) );
    BOOST_CHECK_CLOSE( 0.0, urbanRole->ComputeRatioPionInside( attritionSurface ), 1. ); // $$$$ _RC_ SLG 2010-04-26: trouver un moyen de changer la posture du pion pour tester le résultat
}

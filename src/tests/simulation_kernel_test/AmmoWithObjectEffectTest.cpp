#include "simulation_kernel_test_pch.h"
#include "SingletonTerminator.h"
#include "MockMIL_Time_ABC.h"
#include "MockAgent.h"
#include "MockArmy.h"
#include "MockMIL_EntityManager_ABC.h"
#include "MockMIL_Object_ABC.h"
#include "MockMIL_Time_ABC.h"
#include "MockNET_Publisher_ABC.h"
#include "MockRoleDotations.h"
#include "MockRoleLocation.h"
#include "StubDEC_Decision.h"
#include "TestIndirectFireModifier.h"
#include "AlgorithmsFactories.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Position.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCapacity.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Orders/MIL_PionOrderManager.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/Role_ABC.h"
#include "StubTER_World.h"
#include "DotationComputer_ABC.h"
#include "WeaponAvailabilityComputer_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: TestScramblingAmmo
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
namespace
{
    PHY_Action_ABC* pExpectedAction;
    int callbackValue = -1;
    void InitCallback( unsigned int /*action*/, int callback )
    {
        BOOST_CHECK_EQUAL( firing::PHY_RoleAction_IndirectFiring::eRunning, callback );
    }
    void CheckCallback( unsigned int action, int callback )
    {
        BOOST_CHECK_EQUAL( action, pExpectedAction->GetId() );
        callbackValue = callback;
    }
}

BOOST_AUTO_TEST_CASE( TestScramblingAmmo )
{
    SingletonTerminator terminator;
    MockMIL_Time_ABC time;
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );
    MockNET_Publisher_ABC mockPublisher;

    {
        // Ordre de conduite Appliquer les feux avec munition idoine à un pion de modele SAM / Pion ASS -> "DEC_StartTirIndirectSurPosition"
        const std::string dotationName( "ammo" );
        PHY_IndirectFireDotationClass::Initialize();
        MockMIL_EntityManager_ABC entityManager;
        MIL_EffectManager effectManager;

        const std::string dotations( "<resources><resource name='ammo' category='munition' id='1' logistic-supply-class='whatever' nature='Solide' id-nature='1' package-size='1' package-mass='1' package-volume='1'><indirect-fires intervention-type='1' x-dispersion='1' y-dispersion='1'><indirect-fire type='effect' life-time='300s' object-type='Zone brouillage'/></indirect-fires></resource></resources>" );

        MockAgent pion;
        MOCK_EXPECT( pion.GetID ).once().returns( 1u );
        StubDEC_Decision< MockAgent >* decision = new StubDEC_Decision< MockAgent >( pion );
        pion.RegisterRole( *decision );
        firing::PHY_RoleAction_IndirectFiring* roleIndirectFiring = new firing::PHY_RoleAction_IndirectFiring( pion );
        pion.RegisterRole( *roleIndirectFiring );
        PHY_RoleAction_MovingUnderground* roleMovingUnderground = new PHY_RoleAction_MovingUnderground( pion );
        pion.RegisterRole( *roleMovingUnderground );

        MOCK_EXPECT( time.GetTickDuration ).returns( 10u );
        MOCK_EXPECT( time.GetCurrentTimeStep ).returns( 10u );
        TestIndirectFireModifier* testRole = new TestIndirectFireModifier( time, effectManager, pion, dotations );
        pion.RegisterRole< TestIndirectFireModifier >( *testRole );
        MockRoleLocation* locationRole = new MockRoleLocation();
        pion.RegisterRole( *locationRole );
        const MT_Vector2D sourcePosition;
        MOCK_EXPECT( locationRole->GetPosition ).once().returns( sourcePosition );

        MockRoleDotations* dotationRole = new MockRoleDotations();
        pion.RegisterRole( *dotationRole );
        MOCK_EXPECT( dotationRole->AddFireReservation ).once().returns( 1. );

        const PHY_DotationCategory* pCategory = PHY_DotationType::FindDotationCategory( "ammo" );

        decision->RegisterFunction( "CallbackAction", &InitCallback );

        const MT_Vector2D targetPosition;
        PHY_ActionIndirectFire_Position* pAction = new PHY_ActionIndirectFire_Position( pion, pCategory, 1, &targetPosition );
        pExpectedAction = pAction;
        // $$$$ LDC: New needed because it's deleted by PHY_Actor (inherited by MockAgent)

        // Expect a Callback
        decision->RegisterFunction( "CallbackAction", &CheckCallback );

        MOCK_EXPECT( mockPublisher.Send ).at_least( 1 );

        pAction->Execute();

        BOOST_CHECK_EQUAL( firing::PHY_RoleAction_IndirectFiring::eFinished, callbackValue );
        MockMIL_Object_ABC object;
        MOCK_EXPECT( entityManager.CreateObjectFromType ).once().returns( &object ); // $$$$ with type Zone brouillage
        MOCK_EXPECT( object.RegisterAttribute ).once();
        MockArmy mockArmy;
        MOCK_EXPECT( pion.GetArmy ).once().returns( boost::ref( mockArmy ) );
        effectManager.Update();
        mock::verify( entityManager );
    }
}


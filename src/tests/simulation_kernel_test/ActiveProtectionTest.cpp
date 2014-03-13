// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "SingletonTerminator.h"
#include "MockAgent.h"
#include "MockArmy.h"
#include "MockMIL_EntityManager_ABC.h"
#include "MockMIL_Time_ABC.h"
#include "MockNET_Publisher_ABC.h"
#include "MockPHY_RoleInterface_ActiveProtection.h"
#include "MockPHY_RoleInterface_Composantes.h"
#include "MockPHY_RoleInterface_HumanFactors.h"
#include "MockPHY_RoleInterface_Perceiver.h"
#include "MockPHY_RoleInterface_UrbanLocation.h"
#include "MockRoleDotations.h"
#include "MockRoleLocation.h"
#include "StubDEC_Decision.h"
#include "TestIndirectFireModifier.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Position.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
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
// Name: ActiveProtectionTest
// Created: LDC 2010-01-07
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

BOOST_AUTO_TEST_CASE( ActiveProtectionTest )
{
    SingletonTerminator terminator;
    MockMIL_Time_ABC time;
    FakeWorld world( "worldwide/tests/EmptyParis-ML" ); // $$$$ because used in asn and in destructor of action!!!
    MockNET_Publisher_ABC mockPublisher;
    {
        // Ordre de conduite Appliquer les feux avec munition idoine à un pion de modele SAM / Pion ASS -> "DEC_StartTirIndirectSurPosition"
        const std::string dotationName( "ammo" );
        PHY_IndirectFireDotationClass::Initialize();
        MockMIL_EntityManager_ABC entityManager;
        MIL_EffectManager effectManager;

        MockAgent pion;
        MOCK_EXPECT( pion.GetID ).once().returns( 1u );
        StubDEC_Decision< MockAgent >* decision = new StubDEC_Decision< MockAgent >( pion );
        pion.RegisterRole( *decision );
        firing::PHY_RoleAction_IndirectFiring* roleIndirectFiring = new firing::PHY_RoleAction_IndirectFiring( pion );
        pion.RegisterRole( *roleIndirectFiring );
        PHY_RoleAction_MovingUnderground* roleMovingUnderground = new PHY_RoleAction_MovingUnderground( pion );
        pion.RegisterRole( *roleMovingUnderground );
        MockPHY_RoleInterface_Perceiver* rolePerceiver = new MockPHY_RoleInterface_Perceiver();
        pion.RegisterRole( *rolePerceiver );

        MOCK_EXPECT( time.GetTickDuration ).returns( 10u );
        MOCK_EXPECT( time.GetCurrentTimeStep ).returns( 10u );

        const std::string dotations( "<resources><resource name='ammo' category='munition' id='1' logistic-supply-class='whatever' nature='Solide' id-nature='1' package-size='1' package-mass='1' package-volume='1'>"
            "<attritions><attrition destruction='0' protection='protection1' repairable-with-evacuation='0.2' repairable-without-evacuation='0.8'/></attritions>"
            "<indirect-fires intervention-type='1' x-dispersion='100' y-dispersion='100'><indirect-fire type='explosif' dispersion-factor='0' neutralization-ratio='1.5' /></indirect-fires></resource></resources>" );
        TestIndirectFireModifier* testRole = new TestIndirectFireModifier( time, effectManager, pion, dotations );

        const PHY_DotationCategory* pCategory = PHY_DotationType::FindDotationCategory( "ammo" );

        pion.RegisterRole< TestIndirectFireModifier >( *testRole );
        MockRoleLocation* locationRole = new MockRoleLocation();
        pion.RegisterRole( *locationRole );
        const MT_Vector2D sourcePosition;
        locationRole->UpdatePatch();
        MOCK_EXPECT( locationRole->GetPosition ).at_least( 1 ).returns( sourcePosition );
        MOCK_EXPECT( locationRole->GetAgent ).at_least( 1 ).returns( boost::ref( pion ) );
        MOCK_EXPECT( locationRole->GetHeight ).at_least( 1 ).returns( 0. );

        MockPHY_RoleInterface_UrbanLocation* urbanRole = new MockPHY_RoleInterface_UrbanLocation();
        pion.RegisterRole( *urbanRole );


        MockRoleDotations* dotationRole = new MockRoleDotations();
        pion.RegisterRole( *dotationRole );
        MOCK_EXPECT( dotationRole->AddFireReservation ).once().returns( 1. );

        decision->RegisterFunction( "CallbackAction", &InitCallback );

        const MT_Vector2D targetPosition;
        PHY_ActionIndirectFire_Position* pAction = new PHY_ActionIndirectFire_Position( pion, pCategory, 1, &targetPosition );
        pExpectedAction = pAction;
        // $$$$ LDC: New needed because it's deleted by PHY_Actor (inherited by MockAgent)

        // Expect a Callback
        decision->RegisterFunction( "CallbackAction", &CheckCallback );

        MOCK_EXPECT( mockPublisher.Send ).at_least( 1 );

        pAction->Execute();

        MockPHY_RoleInterface_HumanFactors* humanFactorRole = new MockPHY_RoleInterface_HumanFactors();
        pion.RegisterRole< PHY_RoleInterface_HumanFactors >( *humanFactorRole );
        MOCK_EXPECT( humanFactorRole->NotifyAttacked ).once();

        BOOST_CHECK_EQUAL( firing::PHY_RoleAction_IndirectFiring::eFinished, callbackValue );
        MockPHY_RoleInterface_ActiveProtection* protectionRole = new MockPHY_RoleInterface_ActiveProtection();
        pion.RegisterRole< PHY_RoleInterface_ActiveProtection >( *protectionRole );
        MOCK_EXPECT( protectionRole->UseAmmunition ).once();
        MOCK_EXPECT( protectionRole->DestroyIndirectFire ).once().returns( false );
        MOCK_EXPECT( protectionRole->CounterIndirectFire ).once().returns( false );
        MockArmy mockArmy;
        MOCK_EXPECT( pion.GetArmy ).at_least( 1 ).returns( boost::ref( mockArmy ) );
        MockPHY_RoleInterface_Composantes* composanteRole = new MockPHY_RoleInterface_Composantes();
        pion.RegisterRole( *composanteRole );
        MOCK_EXPECT( composanteRole->Neutralize ).once();
        MOCK_EXPECT( urbanRole->ComputeRatioPionInsideEllipse ).once().returns( 1.f );
        MOCK_EXPECT( composanteRole->ApplyIndirectFire ).once();
        MOCK_EXPECT( pion.CanEmitReports ).returns( true );
        MOCK_EXPECT( mockArmy.IsAFriend ).at_least( 1 ).returns( eTristate_False );
        MOCK_EXPECT( mockArmy.GetID ).at_least( 1 ).returns( 42 );
        MOCK_EXPECT( rolePerceiver->IsFireObserver ).once().returns ( false );
        effectManager.Update();
        mock::verify( entityManager );
    }
}

template<>
void MIL_Report::Send< MockAgent >( const MockAgent& /*sender*/, E_Type /*nType*/, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& /*diaParameters*/ ) const
{
    // NOTHING
}

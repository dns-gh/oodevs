#include "simulation_kernel_test_pch.h"

#include "MockAgent.h"
#include "MockArmy.h"
#include "MockMIL_EntityManager_ABC.h"
#include "MockMIL_Time_ABC.h"
#include "MockNET_Publisher_ABC.h"
#include "MockPHY_RoleInterface_ActiveProtection.h"
#include "MockRoleDotations.h"
#include "MockRoleLocation.h"
#include "StubDEC_Database.h"
#include "StubDEC_Decision.h"
#include "StubMIL_Object_ABC.h"
#include "TestIndirectFireModifier.h"

#include "AlgorithmsFactories.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Position.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCapacity.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/Role_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "DotationComputer_ABC.h"
#include "WeaponAvailabilityComputer_ABC.h"
#include "UrbanModel.h"
#include <xeumeuleu/xml.h>

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: ActiveProtectionTest
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
namespace
{
    PHY_Action_ABC* pExpectedAction;
    int callbackValue = -1;
    void CheckCallback( unsigned int action, int callback )
    {
        BOOST_CHECK_EQUAL( action, pExpectedAction->GetId() );
        callbackValue = callback;
    }
}

BOOST_AUTO_TEST_CASE( ActiveProtectionTest )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" ); // $$$$ because used in asn and in destructor of action!!! 
    MockNET_Publisher_ABC mockPublisher;
    
    {
        // Ordre de conduite Appliquer les feux avec munition idoine à un pion de modele SAM / Pion ASS -> "DEC_StartTirIndirectSurPosition"
        const std::string dotationName( "ammo" );
        PHY_IndirectFireDotationClass::Initialize();
        MockMIL_EntityManager_ABC entityManager;
        MIL_EffectManager effectManager;

        MockAgent pion;
        MOCK_EXPECT( pion, GetID ).once().returns( 1u );
        StubDEC_Database database;
        StubDEC_Decision< MockAgent >* decision = new StubDEC_Decision< MockAgent >( pion, database );
        pion.RegisterRole( *decision );
        firing::PHY_RoleAction_IndirectFiring* roleIndirectFiring = new firing::PHY_RoleAction_IndirectFiring( pion );
        pion.RegisterRole( *roleIndirectFiring );

        MockMIL_Time_ABC time;
        MOCK_EXPECT( time, GetTickDuration ).returns( 10u );
        MOCK_EXPECT( time, GetCurrentTick ).returns( 10u );

        const std::string dotations( "<dotations><dotation category='ammo' name='munition' id='1' nature='Solide' package-size='1' package-mass='1' package-volume='1'>"
            "<attritions><attrition destruction='0' protection='protection1' repairable-with-evacuation='0.2' repairable-without-evacuation='0.8'/></attritions>"
            "<indirect-fire type='explosif' intervention-type='1' neutralization-ratio='1.5' x-dispersion='100' y-dispersion='100'/></dotation></dotations>" );
        TestIndirectFireModifier* testRole = new TestIndirectFireModifier( time, effectManager, pion, dotations );

        const PHY_DotationCategory* pCategory = PHY_DotationType::FindDotationCategory( "ammo" );

        pion.RegisterRole< TestIndirectFireModifier >( *testRole );
        MockRoleLocation* locationRole = new MockRoleLocation();
        pion.RegisterRole( *locationRole );
        const MT_Vector2D sourcePosition;
        MOCK_EXPECT( locationRole, GetPosition ).once().returns( sourcePosition );

        MockRoleDotations* dotationRole = new MockRoleDotations();
        pion.RegisterRole( *dotationRole );
        MOCK_EXPECT( dotationRole, AddFireReservation ).once().returns( 1. );

        const MT_Vector2D targetPosition;
        PHY_ActionIndirectFire_Position* pAction = new PHY_ActionIndirectFire_Position( pion, pCategory, 1, &targetPosition );
        pExpectedAction = pAction;
        // $$$$ LDC: New needed because it's deleted by PHY_Actor (inherited by MockAgent) 

        // Expect a Callback
        decision->RegisterFunction( "CallbackAction", &CheckCallback );
        
        MOCK_EXPECT( mockPublisher, Send ).at_least( 1 );

        AlgorithmsFactories algorithms;
        MOCK_EXPECT( pion, GetAlgorithms ).at_least( 1 ).returns( boost::cref( algorithms ) );
        pAction->Execute();

        BOOST_CHECK_EQUAL( firing::PHY_RoleAction_IndirectFiring::eRunning, callbackValue );
        MockPHY_RoleInterface_ActiveProtection* protectionRole = new MockPHY_RoleInterface_ActiveProtection();
        pion.RegisterRole< PHY_RoleInterface_ActiveProtection >( *protectionRole );
        MOCK_EXPECT( protectionRole, UseAmmunition ).once();
        MOCK_EXPECT( protectionRole, DestroyIndirectFire ).once().returns( false );
        MOCK_EXPECT( protectionRole, CounterIndirectFire ).once().returns( false );
        MockArmy mockArmy;
        MOCK_EXPECT( pion, GetArmy ).once().returns( boost::ref( mockArmy ) );
        UrbanModel urbanModel;
        effectManager.Update();
        entityManager.verify();

        PHY_IndirectFireDotationClass::Terminate();
    }
    
    TER_World::DestroyWorld();    
}
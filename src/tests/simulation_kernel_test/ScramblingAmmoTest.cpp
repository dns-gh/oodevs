#include "simulation_kernel_test_pch.h"

#include "MockAgent.h"
#include "MockArmy.h"
#include "MockMIL_EntityManager_ABC.h"
#include "MockMIL_Time_ABC.h"
#include "MockNET_Publisher_ABC.h"
#include "MockRoleDotations.h"
#include "MockRoleLocation.h"
#include "StubDEC_Database.h"
#include "StubDEC_Decision.h"
#include "StubMIL_Object_ABC.h"

#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Position.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCapacity.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Agents/Units/Weapons/PHY_LauncherType.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/Role_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "DotationComputer_ABC.h"
#include "WeaponAvailabilityComputer_ABC.h"
#include <xeumeuleu/xml.h>

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: TestScramblingAmmo
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
namespace
{
    PHY_Action_ABC* pExpectedAction;
    int callbackValue = -1;
    void CheckCallback( PHY_Action_ABC* pAction, int callback )
    {
        BOOST_CHECK_EQUAL( pAction, pExpectedAction );
        callbackValue = callback;
    }
}

class Stub_PHY_RoleInterface_Dotation : public dotation::PHY_RoleInterface_Dotations
{
public:
    static Stub_PHY_RoleInterface_Dotation roleInterfaceDotation;

    virtual const PHY_ConsumptionType& GetConsumptionMode() const { throw; }
    virtual void ReadOverloading( xml::xistream& xis ) {}
    virtual void Update( bool bIsDead ) {}
    virtual void Clean() {}
    virtual bool HasChanged() const { return false; }
    virtual MT_Float SupplyDotation( const PHY_DotationCategory& category, MT_Float rNbr ) { return 1.; }
    virtual void ResupplyDotations() {}
    virtual void ResupplyDotations( const PHY_AmmoDotationClass& ammoDotationClass, MT_Float rFactor ) {}
    virtual void ResupplyDotations( const PHY_DotationType&      type             , MT_Float rFactor ) {}
    virtual bool SetConsumptionMode( const PHY_ConsumptionType& mode ) { return false;}
    virtual void RollbackConsumptionMode() {}
    virtual MT_Float GetMaxTimeForConsumption( const PHY_ConsumptionType& mode ) const { return 1.; }
    virtual MT_Float AddFireReservation( const PHY_DotationCategory& category, MT_Float rNbr ) { return 1.; }
    virtual void NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const {}
    virtual void FillSupplyRequest( PHY_SupplyDotationRequestContainer& supplyRequest ) const {}
    virtual void ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, MT_Float rCapacityFactor ) const {}
};

Stub_PHY_RoleInterface_Dotation Stub_PHY_RoleInterface_Dotation::roleInterfaceDotation;

class TestIndirectFireModifier : public tools::Role_ABC
    , public tools::AlgorithmModifier_ABC< firing::WeaponAvailabilityComputer_ABC >
    , public tools::AlgorithmModifier_ABC< dotation::DotationComputer_ABC >
    , private boost::noncopyable
{
public:
    typedef TestIndirectFireModifier RoleInterface;
    TestIndirectFireModifier( MIL_Time_ABC& time, MIL_EffectManager& effectManager, MIL_Agent_ABC& pion )
        : time_         ( time )
        , effectManager_( effectManager )
        , pion_         ( pion )
        , dotationGroupContainer_( Stub_PHY_RoleInterface_Dotation::roleInterfaceDotation )
    {
        xml::xistringstream xisLauncher( "<launchers><launcher name='gun' indirect-fire='true'/></launchers>" );
        PHY_LauncherType::Initialize( xisLauncher );
        xml::xistringstream xisWeapon( "<weapons><weapon-system launcher='gun' munition='ammo'><burst munition='1' duration='1s'/><reloading munition='1' duration='1s'/><indirect-fire average-speed='100' min-range='0' max-range='1000'/></weapon-system></weapons>" );
        PHY_WeaponType::Initialize( effectManager_, time_, xisWeapon, 1 );
        xml::xistringstream xisProtection( "<protections><protection name='protection1' type='humain'><neutralization average-time='10s' variance='1s'/></protection></protections>" );
        PHY_Protection::Initialize( xisProtection );
        xml::xistringstream xisVolumes( "<volumes><volume name='Big'/></volumes>");
        PHY_Volume::Initialize( xisVolumes );
        xml::xistringstream xisCapacity( "<category capacity='1' logistic-threshold='50'/>" );
        xisCapacity >> xml::start( "category" );
        pCapacity_.reset( new PHY_DotationCapacity( *PHY_DotationType::FindDotationCategory( "ammo" ), xisCapacity ) );
        dotationGroupContainer_.AddCapacity( *pCapacity_ );
    }
    virtual void Execute( firing::WeaponAvailabilityComputer_ABC& algo ) const
    {
        xml::xistringstream xisComposanteTypePion( "<bla name='composanteTypePion' id='1' protection='protection1' weight='1' size='Big'><speeds max='8'></speeds><weapon-systems/><sensors/><transports/><objects/><consumptions/></bla>" );
        xisComposanteTypePion.start( "bla" );
        PHY_ComposanteTypePion type( time_, "composanteTypePion", xisComposanteTypePion );
        role_.reset( new PHY_RolePion_Composantes( pion_, false ) );
        firer_.reset( new PHY_ComposantePion( time_, type, *role_, 1, true, true, true ) );
        weapon_.reset( new PHY_Weapon( time_, *PHY_WeaponType::FindWeaponType( "gun", "ammo" ), true ) );
        algo.ApplyOnWeapon( *firer_, *weapon_ );
    }
    virtual void Execute( dotation::DotationComputer_ABC& algo ) const
    {
        algo.SetDotationContainer( dotationGroupContainer_ );
    }
private:
    MIL_Time_ABC& time_;
    MIL_EffectManager& effectManager_;
    MIL_Agent_ABC& pion_;
    mutable PHY_DotationGroupContainer dotationGroupContainer_;
    std::auto_ptr< PHY_DotationCapacity > pCapacity_;
    mutable std::auto_ptr< PHY_RolePion_Composantes > role_;
    mutable std::auto_ptr< PHY_ComposantePion > firer_;
    mutable std::auto_ptr< PHY_Weapon > weapon_;
};

BOOST_AUTO_TEST_CASE( TestScramblingAmmo )
{
    TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" ); // $$$$ because used in asn and in destructor of action!!! 
    MockNET_Publisher_ABC mockPublisher;
    
    {
        // Ordre de conduite Appliquer les feux avec munition idoine à un pion de modele SAM / Pion ASS -> "DEC_StartTirIndirectSurPosition"
        const std::string dotationName( "ammo" );
        PHY_IndirectFireDotationClass::Initialize();
        MockMIL_EntityManager_ABC entityManager;
        MIL_EffectManager effectManager;

        xml::xistringstream xisDotationNature( "<natures><nature type='Solide'/></natures>" );
        PHY_DotationNature::Initialize( xisDotationNature );
        xml::xistringstream xisDotations( "<dotations><dotation category='ammo' name='munition' id='1' nature='Solide' package-size='1' package-mass='1' package-volume='1'><indirect-fire type='scrambling' intervention-type='1' x-dispersion='1' y-dispersion='1' object-type='Zone brouillage'/></dotation></dotations>" );
        PHY_DotationType::Initialize( xisDotations );
        const PHY_DotationCategory* pCategory = PHY_DotationType::FindDotationCategory( "ammo" );

        MockAgent pion;
        MOCKPP_CHAINER_FOR( MockAgent, GetID )( &pion ).expects( once() ).will( returnValue( 1u ) );
        StubDEC_Database database;
        StubDEC_Decision< MockAgent >* decision = new StubDEC_Decision< MockAgent >( pion, database );
        pion.RegisterRole( *decision );
        firing::PHY_RoleAction_IndirectFiring* roleIndirectFiring = new firing::PHY_RoleAction_IndirectFiring( pion );
        pion.RegisterRole( *roleIndirectFiring );

        MockMIL_Time_ABC time;
        time.GetTickDuration_mocker.stubs().will( returnValue( 10u ) );
        time.GetCurrentTick_mocker.stubs().will( returnValue( 10u ) );
        TestIndirectFireModifier* testRole = new TestIndirectFireModifier( time, effectManager, pion );
        pion.RegisterRole< TestIndirectFireModifier >( *testRole );
        MockRoleLocation* locationRole = new MockRoleLocation();
        pion.RegisterRole( *locationRole );
        const MT_Vector2D sourcePosition;
        MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )( locationRole ).expects( once() ).will( returnValue( &sourcePosition ) );

        MockRoleDotations* dotationRole = new MockRoleDotations();
        pion.RegisterRole( *dotationRole );
        MOCKPP_CHAINER_FOR( MockRoleDotations, AddFireReservationShadow )( dotationRole ).expects( once() ).will( returnValue( 1. ) );

        const MT_Vector2D targetPosition;
        PHY_ActionIndirectFire_Position* pAction = new PHY_ActionIndirectFire_Position( pion, pCategory, 1, &targetPosition );
        pExpectedAction = pAction;
        // $$$$ LDC: New needed because it's deleted by PHY_Actor (inherited by MockAgent) 

        // Expect a Callback
        decision->RegisterFunction( "CallbackAction", &CheckCallback );
        
        mockPublisher.Send_mocker.expects( atLeastOnce() ); // NET_ASN_MsgStartUnitFire, StopUnitFire

        pAction->Execute();

        BOOST_CHECK_EQUAL( firing::PHY_RoleAction_IndirectFiring::eFinished, callbackValue );
        StubMIL_Object_ABC object;
        entityManager.CreateObject_mocker.expects( once() ).will( returnValue( (MIL_Object_ABC*)&object ) ); // $$$$ with type Zone brouillage
        MockArmy mockArmy;
        MOCKPP_CHAINER_FOR( MockAgent, GetArmyShadow )( &pion ).expects( once() ).will( returnValue( (MIL_Army_ABC*)&mockArmy ) );
        effectManager.Update();
        entityManager.verify();

        PHY_IndirectFireDotationClass::Terminate();
        PHY_DotationNature::Terminate();
    }
    
    TER_World::DestroyWorld();    
}
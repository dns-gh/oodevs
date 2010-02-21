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
#include "UrbanModel.h"
#include <Urban/Model.h>

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
BOOST_AUTO_TEST_CASE( PhComputerTest )
{

    UrbanModel* urbanModel = new UrbanModel();

    xml::xistringstream xisModel( "<urban><model version='1.0'/><cities><city id='1' name='Drosoville'><footprint/><districts><district id='2' name='Centre-ville'><footprint/><blocks><block id='3' name='School'>"
        "<footprint><point location='31TCM1058982147'/><point location='31TCM1055881170'/><point location='31TCM1157881118'/><point location='31TCM1158882115'/></footprint>"
        "<physical><architecture basement-level-number='2' facade-opacity='0.6' floor-number='30' height='40' inner-cluttering='0.25' material='concrete' roof-shape='flat'/></physical>"
        "</block></blocks></district></districts></city></cities><resource-networks/><underground-networks/></urban>" ) ;
    urbanModel->GetModel().Load( xisModel );
   
    MockMIL_Time_ABC time;

    TestPhDirectFire fixture( time );
    
    const PHY_WeaponType& type = *PHY_WeaponType::FindWeaponType( "gun", "ammo" );
    xml::xistringstream xis( "<hit-probabilities target='Heavy'><hit-probability distance='0' percentage='1'/>"
        "<hit-probability distance='1000' percentage='0.9'/><hit-probability distance='1500' percentage='0.8'/></hit-probabilities>" ) ;
    PHY_WeaponDataType_DirectFire data( type, xis );

    // Ajout des roles necessaires
    MockAgent firer;
    MockRoleLocation* firerLocationRole = new MockRoleLocation();
    firer.RegisterRole< PHY_RoleInterface_Location >( *firerLocationRole );

    MockRoleInterface_Posture* firerPosture = new MockRoleInterface_Posture();
    firer.RegisterRole< PHY_RoleInterface_Posture >( *firerPosture );

    MockPHY_RoleInterface_HumanFactors* firerHumanFactors = new MockPHY_RoleInterface_HumanFactors();
    firer.RegisterRole< PHY_RoleInterface_HumanFactors >( *firerHumanFactors );

    MockAgent target;
    MockRoleLocation* targetLocationRole = new MockRoleLocation();
    target.RegisterRole< PHY_RoleInterface_Location >( *targetLocationRole );

    MockRoleInterface_Posture* targetPosture = new MockRoleInterface_Posture();
    target.RegisterRole< PHY_RoleInterface_Posture >( *targetPosture );

    MockPHY_RoleInterface_ActiveProtection* targetActiveProtection = new MockPHY_RoleInterface_ActiveProtection();
    target.RegisterRole< PHY_RoleInterface_ActiveProtection >( *targetActiveProtection );

    MockPHY_Composante_ABC compTarget;
    MockPHY_FireResults_ABC fireResult;

    xml::xistringstream xisComposanteTypePion( "<bla name='composanteTypePion' id='1' protection='protection1' weight='1' size='Heavy'><speeds max='8'></speeds><weapon-systems/><sensors/><transports/><objects/><consumptions/></bla>" );
    xisComposanteTypePion.start( "bla" );

    unsigned int tickDuration;
    time.GetTickDuration_mocker.expects( mockpp::once() ).will( mockpp::returnValue( tickDuration ) );
  
    PHY_ComposanteTypePion composanteType( time, "composanteTypePion", xisComposanteTypePion );

    // Test de la fonction GetDangerosity
    MOCKPP_CHAINER_FOR( MockRoleLocation, GetAltitude )( firerLocationRole ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( 0. ) );   

    MOCKPP_CHAINER_FOR( MockRoleLocation, GetAltitude )( targetLocationRole ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( 0. ) );  

    const MT_Vector2D firerPosition( 39700., 4646300. );
    MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )( firerLocationRole ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( &firerPosition ) );  

    const MT_Vector2D targetPosition( 40000., 4646700. );
    MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )( targetLocationRole ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( &targetPosition ) );  

    MOCKPP_CHAINER_FOR( MockRoleInterface_Posture, GetElongationFactor )( firerPosture ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( 1. ) );

    const PHY_Posture& firerCurrentPosture = *PHY_Posture::FindPosture( 0 );
    MOCKPP_CHAINER_FOR( MockRoleInterface_Posture, GetCurrentPostureShadow )( firerPosture ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( &firerCurrentPosture ) );  
        
    const PHY_Posture& firerLastPosture = *PHY_Posture::FindPosture( 0 );
    MOCKPP_CHAINER_FOR( MockRoleInterface_Posture, GetLastPostureShadow )( firerPosture ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( &firerLastPosture ) );  
    
    MOCKPP_CHAINER_FOR( MockRoleInterface_Posture, GetPostureCompletionPercentage )( firerPosture ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( 0.1 ) );  

    const PHY_Posture& targetCurrentPosture = *PHY_Posture::FindPosture( 0 );
    MOCKPP_CHAINER_FOR( MockRoleInterface_Posture, GetCurrentPostureShadow )( targetPosture ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( &targetCurrentPosture ) );  
    
    const PHY_Posture& targetLastPosture = *PHY_Posture::FindPosture( 0 );
    MOCKPP_CHAINER_FOR( MockRoleInterface_Posture, GetLastPostureShadow )( targetPosture ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( &firerCurrentPosture ) );

    MOCKPP_CHAINER_FOR( MockRoleInterface_Posture, GetPostureCompletionPercentage )( targetPosture ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( 0.1 ) );  

    MOCKPP_CHAINER_FOR( MockPHY_RoleInterface_ActiveProtection, GetPHModifier )( targetActiveProtection ).expects( mockpp::atLeastOnce() ).will( mockpp::returnValue( 0.1 ) );  

    MOCKPP_CHAINER_FOR( MockPHY_RoleInterface_HumanFactors, ModifyPH )( firerHumanFactors ).expects( mockpp::atLeastOnce() )/*.with( mockpp::eq( 0.089166666201005385 ) )*/.will( mockpp::returnValue( 0.5 ) );  
    
    BOOST_CHECK_CLOSE( 0.03125, data.GetDangerosity( firer, target, composanteType, true ), 1. );


    // Test du modificateur de Ph provoqué par un bloc urbain
    const MT_Vector3D firerUrbanPosition( 39700., 4646300., 0 );
    const MT_Vector3D targetUrbanPosition( 40000., 4646700., 0 );
    BOOST_CHECK_CLOSE( 0.96, urbanModel->ComputeUrbanPhModifier( firerUrbanPosition, targetUrbanPosition ), 1. );

    delete urbanModel;
}





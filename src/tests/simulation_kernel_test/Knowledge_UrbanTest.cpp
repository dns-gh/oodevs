// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MockArmy.h"
#include "simulation_kernel/ActionManager.h"
#include "simulation_kernel/MissionController.h"
#include "simulation_kernel/Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "simulation_kernel/Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectFactory.h"
#include "simulation_kernel/Entities/Objects/MIL_Object_ABC.h"
#include "simulation_kernel/Knowledge/DEC_Knowledge_Urban.h"
#include "simulation_kernel/Knowledge/DEC_Knowledge_UrbanPerception.h"
#include "simulation_kernel/Urban/MIL_UrbanObject.h"
#include "StubTER_World.h"
#include "Fixture.h"
#include "MockMIL_Time_ABC.h"
#include "MockNET_Publisher_ABC.h"
#include "MockPHY_RoleInterface_Perceiver.h"

#include <memory>

namespace
{
    xml::xistringstream flux( "<urban-object id='9' name='Blokosolo'>"
                              "   <footprint>"
                              "       <point location='31TCM1272691661'/>"
                              "       <point location='31TCM1285291745'/>"
                              "       <point location='31TCM1291891650'/>"
                              "       <point location='31TCM1278691560'/>"
                              "   </footprint>"
                              "   <urban-objects/>"
                              "   <physical>"
                              "       <architecture trafficability='2' floor-number='10' height='20' occupation='0.25' material='concrete' roof-shape='flat'/>"
                              "   </physical>"
                              "</urban-object>" );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_UrbanTest_Update
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Knowledge_UrbanTest_Update )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );
    {
        MIL_ObjectFactory factory;
        {
            xml::xistringstream xis( "<objects>"
                "    <object type='urban block'/>"
                "</objects>" );
            BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );
        }
        MockArmy army;
        ActionManager actions;
        MissionController controller( actions );
        MIL_EffectManager effectManager;
        MockNET_Publisher_ABC publisher;
        FixturePion pion( controller, effectManager );
        flux >> xml::start( "urban-object" );
        std::auto_ptr< MIL_UrbanObject_ABC > pObject( factory.CreateUrbanObject( flux, 0 ) );
        flux >> xml::end;
        PHY_RolePion_UrbanLocation* urbanRole = new PHY_RolePion_UrbanLocation( *pion.pPion_ );
        urbanRole->NotifyMovingInsideObject( *pObject);
        pion.pPion_->RegisterRole< PHY_RolePion_UrbanLocation >( *urbanRole );
        MockPHY_RoleInterface_Perceiver* perceiver = new MockPHY_RoleInterface_Perceiver();
        pion.pPion_->RegisterRole< PHY_RoleInterface_Perceiver >( *perceiver );
        {
            MockMIL_Time_ABC time;
            MOCK_EXPECT( time.GetCurrentTimeStep ).returns( 1u );
            MOCK_EXPECT( army.GetID ).returns( 1u );
            MOCK_EXPECT( publisher.Send ).at_least( 1 );
            MOCK_EXPECT( perceiver->IsReconnoitering ).returns( false );
            DEC_Knowledge_Urban kn( army, *pObject );
            DEC_Knowledge_UrbanPerception perception( *pion.pPion_, pObject->GetID() );
            perception.SetPerceptionLevel( PHY_PerceptionLevel::detected_ );

            kn.Update( perception );
            BOOST_CHECK( kn.GetCurrentRecceProgress() > 0. );
        }
    }
}

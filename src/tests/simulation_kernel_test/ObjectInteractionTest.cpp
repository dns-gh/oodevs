// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/MissionController.h"
#include "simulation_kernel/Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "simulation_kernel/Decision/DEC_Model.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectFactory.h"
#include "simulation_kernel/Entities/Objects/MIL_Object_ABC.h"
#include "simulation_kernel/Entities/Objects/Object.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectType_ABC.h"
#include "simulation_kernel/Entities/Objects/AnimatorAttribute.h"
#include "simulation_kernel/Entities/Objects/ConstructionAttribute.h"
#include "simulation_kernel/Entities/Objects/DetectorAttribute.h"
#include "simulation_kernel/Entities/Objects/NBCAttribute.h"
#include "simulation_kernel/Entities/Objects/MIL_ToxicEffectManipulator.h"
#include "simulation_kernel/Entities/Objects/ContaminationCapacity.h"
#include "simulation_kernel/Entities/Objects/DetectionCapacity.h"
#include "simulation_kernel/Entities/Objects/InteractIfEquippedCapacity.h"
#include "simulation_kernel/Entities/Objects/InteractWithSideCapacity.h"
#include "simulation_kernel/Entities/Objects/ProtectionCapacity.h"
#include "simulation_kernel/Entities/Objects/SupplyRouteAttribute.h"
#include "simulation_kernel/Entities/Objects/SupplyCapacity.h"
#include "simulation_kernel/Entities/Objects/ObstacleAttribute.h"
#include "simulation_kernel/Entities/Objects/TimeLimitedAttribute.h"
#include "simulation_kernel/Entities/Objects/MineAttribute.h"
#include "simulation_kernel/Entities/Orders/MIL_Mission_ABC.h"
#include "simulation_kernel/Entities/Populations/MIL_PopulationConcentration.h"
#include "simulation_kernel/Entities/Populations/MIL_PopulationAttitude.h"
#include "simulation_kernel/Tools/MIL_Color.h"
#include "simulation_kernel/ActionManager.h"
#include "StubMIL_Population.h"
#include "StubMIL_PopulationType.h"
#include "MockNET_Publisher_ABC.h"
#include "MockBuilder.h"
#include "MockArmy.h"
#include "MockAgent.h"
#include "MockToxicAttribute.h"
#include "MockMIL_Time_ABC.h"
#include "MockRoleLocation.h"
#include "MockRoleNBC.h"
#include "MockRoleInterface_Posture.h"
#include "MockRolePerceiver.h"
#include "MockRoleUrbanLocation.h"
#include "simulation_terrain/TER_World.h"
#include <xeumeuleu/xml.hpp>

using namespace sword;

namespace
{
    MIL_Object_ABC* CreateObject( const MIL_ObjectType_ABC& type, MockArmy& army, MIL_ObjectFactory& factory )
    {
        MIL_Object_ABC* pObject = 0;
        MockBuilder builder;
        MOCK_EXPECT( builder.GetType ).once().returns( boost::cref( type ) );
        MOCK_EXPECT( builder.Build ).once();
        BOOST_CHECK_NO_THROW( pObject = factory.CreateObject( builder, &army ); );
        mock::verify( builder );
        BOOST_REQUIRE( pObject != 0 );
        return pObject;
    }

    template< typename T >
    void CheckCapacity( MIL_Object_ABC& object )
    {
        BOOST_REQUIRE( object.Retrieve< T >() != 0 );
    }

    struct ObjectCapacityFixture
    {
        ObjectCapacityFixture()
            : world_( "worldwide/tests/EmptyParis-ML" )
            , controller( actions )
        {
            MOCK_EXPECT( army.RegisterObject ).once();
            MOCK_EXPECT( army.GetColor ).once().returns( boost::cref( color ) );
            MOCK_EXPECT( army.UnregisterObject ).once();
        }
        ~ObjectCapacityFixture()
        {
        }
        FakeWorld world_;
        MIL_ObjectFactory factory;
        ActionManager actions;
        MissionController controller;
        MockArmy army;
        MIL_Color color;

    };
}

// -----------------------------------------------------------------------------
/* Name: Test VerifyObjectCapacity_Interaction_Contamination_NoNBC
   Create an object prototype able to contaminate.
   Instanciate the object and test if the interaction mechanism is triggered
   without nbc/toxic attribute
*/
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectCapacity_Interaction_Contamination_NoNBC, ObjectCapacityFixture )
{
    {
        xml::xistringstream xis( "<objects>"
                                 "   <object type='object'>"
                                 "       <contamination type='nbc' max-toxic='1'/>"
                                 "   </object>"
                                 "</objects>" );
        BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = factory.FindType( "object" );

    std::unique_ptr< MIL_Object_ABC > pObject( CreateObject( type, army, factory ) );
    CheckCapacity< ContaminationCapacity >( *pObject );

    MT_Vector2D position;
    MockAgent agent;
    agent.RegisterRole( *new MockRoleLocation() );
    MOCK_EXPECT( agent.GetRole< MockRoleLocation >().NotifyTerrainObjectCollision ).once();
    MOCK_EXPECT( agent.GetRole< MockRoleLocation >().GetPosition ).once().returns( position );

    agent.RegisterRole( *new MockRoleNBC() );
    MOCK_EXPECT( agent.GetRole< MockRoleNBC >().Contaminate ).never();

    BOOST_CHECK_NO_THROW( pObject->ProcessAgentInside( agent ) );
}

// -----------------------------------------------------------------------------
/* Name: Test VerifyObjectCapacity_Interaction_Contamination_NBC
   Create an object prototype able to contaminate.
   Instanciate the object and test if the interaction mechanism is triggered
   with nbc/toxic attributes
*/
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectCapacity_Interaction_Contamination_NBC, ObjectCapacityFixture )
{
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object type='object'>"
                                 "        <contamination type='nbc' max-toxic='1'/>"
                                 "    </object>"
                                 "</objects>" );
        BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = factory.FindType( "object" );

    std::unique_ptr< MIL_Object_ABC > pObject( CreateObject( type, army, factory ) );
    CheckCapacity< ContaminationCapacity >( *pObject );

    // The following lines are used to force the Instanciation of attributes
    BOOST_CHECK_NO_THROW( ( static_cast< Object& >( *pObject ).GetAttribute< MockToxicAttribute, ToxicAttribute_ABC >() ) );
    BOOST_CHECK_NO_THROW( ( static_cast< Object& >( *pObject ).GetAttribute< NBCAttribute >() ) );

    MT_Vector2D position;
    MockAgent agent;
    agent.RegisterRole( *new MockRoleLocation() );
    MOCK_EXPECT( agent.GetRole< MockRoleLocation >().NotifyTerrainObjectCollision ).once();
    MOCK_EXPECT( agent.GetRole< MockRoleLocation >().GetPosition ).once().returns( position );

    agent.RegisterRole( *new MockRoleNBC() );
    MOCK_EXPECT( agent.GetRole< MockRoleNBC >().Contaminate ).never();

    BOOST_CHECK_NO_THROW( pObject->ProcessAgentInside( agent ) );
}

// -----------------------------------------------------------------------------
/* Name: Test VerifyObjectCapacity_Interaction_Protection
Create an object prototype able to be an implantation area.
Instanciate the object and test if the interaction mechanism is triggered
*/
// Created: MGD 2009-03-04
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectCapacity_Interaction_Protection, ObjectCapacityFixture )
{
    //@TODO test à renforcer après le merge
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object type='implantationArea'>"
                                 "        <protection max-size='1' geniePrepared='false'/>"
                                 "    </object>"
                                 "</objects>" );
        BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = factory.FindType( "implantationArea" );

    std::unique_ptr< MIL_Object_ABC > pObject( CreateObject( type, army, factory ) );
    CheckCapacity< ProtectionCapacity >( *pObject );

    //First add
    MT_Vector2D position;
    MockAgent agent;
    agent.RegisterRole( *new MockRoleLocation() );
    {
        // $$$$ _RC_ SBO 2010-04-27: was not verify'ed
        MOCK_EXPECT( agent.GetRole< MockRoleLocation >().NotifyTerrainObjectCollision ).once();
//        MOCK_EXPECT( agent.GetRole< MockRoleLocation >().GetPosition ).exactly( 2 ).returns( position );
    }
    agent.RegisterRole( *new MockRoleInterface_Posture() );
    {
        MOCK_EXPECT( agent.GetRole< MockRoleInterface_Posture >().SetTimingFactor ).once();
    }

    pObject->ProcessAgentEntering( agent );
    BOOST_CHECK_NO_THROW( pObject->ProcessAgentInside( agent ) );
}

// -----------------------------------------------------------------------------
/* Name: Test VerifyObjectCapacity_Interaction_InteractIfEquipped
    Create an object prototype with supply-route capacity and checks it "can interacti with"
    only if it is equipeed.
*/
// Created: LDC 2009-03-05
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectCapacity_Interaction_InteractIfEquipped, ObjectCapacityFixture )
{
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object geometry='line' type='itineraire logistique'>"
                                 "        <supply-route/>"
                                 "    </object>"
                                 "</objects>" );
        BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = factory.FindType( "itineraire logistique" );

    std::unique_ptr< MIL_Object_ABC > pObject( CreateObject( type, army, factory ) );
    CheckCapacity< InteractIfEquippedCapacity >( *pObject );

    MockAgent agent;
    // Force creation of attribute SupplyRouteAttribute...
    BOOST_CHECK_NO_THROW( ( static_cast< Object& >( *pObject ).GetAttribute< SupplyRouteAttribute >() ) );
    BOOST_CHECK_EQUAL( pObject->GetAttribute< SupplyRouteAttribute >().IsEquipped(), pObject->CanInteractWith( agent ) );
}

// -----------------------------------------------------------------------------
/* Name: Test VerifyObjectCapacity_Interaction_Supply
    Create an object prototype able to be a supply area.
    Instanciate the object and test if the interaction mechanism is triggered
*/
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectCapacity_Interaction_Supply, ObjectCapacityFixture )
{
    //@TODO test à renforcer après le merge
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object type='plot ravitaillement'>"
                                 "        <supply/>"
                                 "    </object>"
                                 "</objects>" );
        BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = factory.FindType( "plot ravitaillement" );

    std::unique_ptr< MIL_Object_ABC > pObject( CreateObject( type, army, factory ) );
    CheckCapacity< SupplyCapacity >( *pObject );

    //First add
    MT_Vector2D position;
    MockAgent agent;
    MOCK_EXPECT( agent.GetArmy ).once().returns( boost::ref( army ) );
    agent.RegisterRole( *new MockRoleLocation() );
    {
        // $$$$ _RC_ SBO 2010-04-27: was not verify'ed
//        MOCK_EXPECT( agent.GetRole< MockRoleLocation >().NotifyTerrainObjectCollision ).once();
//        MOCK_EXPECT( agent.GetRole< MockRoleLocation >().GetPosition ).once().returns( position );
    }
    BOOST_CHECK_NO_THROW( pObject->ProcessAgentEntering( agent ) );
}

// -----------------------------------------------------------------------------
// Name: VerifyObjectCapacity_Interaction_Detection
// Created: SBO 2009-06-08
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectCapacity_Interaction_Detection, ObjectCapacityFixture )
{
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object geometry='point' type='checkpoint'>"
                                 "        <detection/>"
                                 "    </object>"
                                 "</objects>" );
        factory.Initialize( xis );
    }
    const MIL_ObjectType_ABC& type = factory.FindType( "checkpoint" );
    std::unique_ptr< MIL_Object_ABC > object( CreateObject( type, army, factory ) );
    CheckCapacity< DetectionCapacity >( *object );
    BOOST_CHECK_NO_THROW( static_cast< Object& >( *object ).GetAttribute< AnimatorAttribute >() );

    MockAgent animator;
    animator.RegisterRole( *new MockRoleUrbanLocation( animator ) );
    MOCK_EXPECT( animator.GetRole< MockRoleUrbanLocation >().IsInCity ).once().returns( false );
    animator.RegisterRole( *new MockRolePerceiver( animator ) );
    static_cast< Object& >( *object ).GetAttribute< AnimatorAttribute >().AddAnimator( animator );

    MockAgent intruder;
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time.GetCurrentTimeStep ).once().returns( 1u );
    BOOST_CHECK_NO_THROW( object->ProcessAgentEntering( intruder ) );

    MOCK_EXPECT( animator.GetRole< MockRolePerceiver >().NotifyExternalPerception )
                .with( mock::same( intruder ), mock::same( PHY_PerceptionLevel::identified_ ) );
}

// -----------------------------------------------------------------------------
// Name: VerifyObjectCapacity_Interaction_Detection2
// Created: SLG 2010-02-19
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectCapacity_Interaction_Detection2, ObjectCapacityFixture )
{
    {
        xml::xistringstream xis(
            "<objects>"
                "<object geometry='point' type='sensors'>"
                    "<detection>"
                        "<acquisition-times>"
                            /*"<acquisition-time level='detection' time='2s'/>"
                            "<acquisition-time level='recognition' time='4s'/>"
                            "<acquisition-time level='identification' time='6s'/>"*/
                        "</acquisition-times>"
                    "</detection>"
                "</object>"
            "</objects>"
            );
        BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = factory.FindType( "sensors" );
    std::unique_ptr< MIL_Object_ABC > object( CreateObject( type, army, factory ) );
    CheckCapacity< DetectionCapacity >( *object );

    MockAgent intruder;
    intruder.RegisterRole( *new MockRoleLocation() );
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time.GetCurrentTimeStep ).once().returns( 1u );
    BOOST_CHECK_NO_THROW( object->ProcessAgentEntering( intruder ) );
    mock::verify( intruder );

    BOOST_CHECK_NO_THROW( static_cast< Object& >( *object ).GetAttribute< DetectorAttribute >() );
    MockAgent detector;
    MOCK_EXPECT( detector.IsDead ).once().returns( false );
    detector.RegisterRole( *new MockRoleUrbanLocation( detector ) );
    MOCK_EXPECT( detector.GetRole< MockRoleUrbanLocation >().IsInCity ).once().returns( false );
    detector.RegisterRole( *new MockRolePerceiver( detector ) );
    static_cast< Object& >( *object ).GetAttribute< DetectorAttribute >().AddDetector( detector );

    MOCK_EXPECT( time.GetCurrentTimeStep ).once().returns( 3u );

    MOCK_EXPECT( intruder.GetRole< MockRoleLocation >().NotifyTerrainObjectCollision ).once();
    MOCK_EXPECT( detector.GetRole< MockRolePerceiver >().NotifyExternalPerception ).with( mock::same( intruder ), mock::same( PHY_PerceptionLevel::identified_ ) );

    BOOST_CHECK_NO_THROW( object->ProcessAgentInside( intruder ) );
}

// -----------------------------------------------------------------------------
// Name: VerifyObjectCapacity_Interaction_Mine
// Created: SBO 2009-06-08
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectCapacity_Interaction_Mine, ObjectCapacityFixture )
{
    {
        xml::xistringstream xis( "<objects>"
                "<object id='288' name='mines [288]' type='mines'>"
                    "<attributes>"
                        "<obstacle activated='true' type='preliminary'/>"
                        "<activity-time value='0'/>"
                        "<mine><density>0</density></mine>"
                    "</attributes>"
                    "<shape type='point'>"
                        "<points>"
                            "<point>30TYT0388241408</point>"
                        "</points>"
                    "</shape>"
                "</object>"
            "</objects>"
        );
        factory.Initialize( xis );
    }
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time.GetRealTime ).once().returns( 3u );
    const MIL_ObjectType_ABC& type = factory.FindType( "mines" );
    std::unique_ptr< MIL_Object_ABC > object( CreateObject( type, army, factory ) );

    BOOST_CHECK_NO_THROW( static_cast< Object& >( *object ).GetAttribute< ObstacleAttribute >() );
    BOOST_CHECK_NO_THROW( static_cast< Object& >( *object ).GetAttribute< TimeLimitedAttribute >() );
    BOOST_CHECK_NO_THROW( static_cast< Object& >( *object ).GetAttribute< MineAttribute >() );
}

// -----------------------------------------------------------------------------
// Name: VerifyObjectCapacity_InteractionAttitudeModifier
// Created: MGD 2010-03-16
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectCapacity_InteractionAttitudeModifier, ObjectCapacityFixture )
{
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher.Send ).at_least( 1 );
    MIL_PopulationAttitude::Initialize();
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object id='1' name='zone paralysante' type='paralyzing area'>"
                                 "        <attitude-modifier attitude='agressive'/>"
                                 "    </object>"
                                "</objects>" );
        factory.Initialize( xis );
    }

    const MIL_ObjectType_ABC& type = factory.FindType( "paralyzing area" );
    std::unique_ptr< MIL_Object_ABC > object( CreateObject( type, army, factory ) );

    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
    xis.start( "main" );
    std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
    DEC_Model model( "test", xis, testOptions.GetDataPath( "." ), missionTypes, false, testOptions.GetDataPath( "resources" ) );
    StubMIL_PopulationType popuType( model );
    StubMIL_Population population( popuType, controller, army );

    xml::xistringstream xisConcentration( "<population attitude='calme' id='37' name='Population standard [37]' position='35RPQ9407811091' type='Population standard'><composition healthy='10001' wounded='0' contaminated='0' dead='0'/></population>");
    xisConcentration.start( "population" );
    MIL_PopulationConcentration concentration( population, xisConcentration );

    BOOST_CHECK( &concentration.GetAttitude() == MIL_PopulationAttitude::Find("calme") );
    object->ProcessPopulationInside( concentration );
    BOOST_CHECK( &concentration.GetAttitude() == MIL_PopulationAttitude::Find("agressive") );
}

// -----------------------------------------------------------------------------
// Name: VerifyObjectCapacity_InteractionPerception
// Created: MGD 2010-03-16
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectCapacity_InteractionPerception, ObjectCapacityFixture )
{
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher.Send ).at_least( 1 );
    MIL_PopulationAttitude::Initialize();
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object id='1' name='zone aveuglante' type='blinding area'>"
                                 "        <perception blinded='true'/>"
                                 "    </object>"
                                 "</objects>" );
        factory.Initialize( xis );
    }

    const MIL_ObjectType_ABC& type = factory.FindType( "blinding area" );
    std::unique_ptr< MIL_Object_ABC > object( CreateObject( type, army, factory ) );

    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
    xis.start( "main" );
    std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
    DEC_Model model( "test", xis, testOptions.GetDataPath( "." ), missionTypes, false, testOptions.GetDataPath( "resources" ) );
    StubMIL_PopulationType popuType( model );
    StubMIL_Population population( popuType, controller, army );

    xml::xistringstream xisConcentration( "<population attitude='agressive' id='37' name='Population standard [37]' position='35RPQ9407811091' type='Population standard'><composition healthy='10001' wounded='0' contaminated='0' dead='0'/></population>");
    xisConcentration.start( "population" );
    MIL_PopulationConcentration concentration( population, xisConcentration );

    BOOST_CHECK( !population.IsBlinded() );
    object->ProcessPopulationInside( concentration );
    BOOST_CHECK( population.IsBlinded() );
}

// -----------------------------------------------------------------------------
// Name: VerifyObjectCapacity_InteractionPerception
// Created: MGD 2010-03-16
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectCapacity_InteractionScattering, ObjectCapacityFixture )
{
    MockNET_Publisher_ABC mockPublisher;
    MOCK_EXPECT( mockPublisher.Send ).at_least( 1 );
    MIL_PopulationAttitude::Initialize();
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object id='1' name='zone dispercante' type='scattering area'>"
                                 "        <scattering human-by-time-step='30'/>"
                                 "    </object>"
                                 "</objects>" );
        factory.Initialize( xis );
    }

    const MIL_ObjectType_ABC& type = factory.FindType( "scattering area" );
    std::unique_ptr< MIL_Object_ABC > object( CreateObject( type, army, factory ) );

    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
    xis.start( "main" );
    std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
    DEC_Model model( "test", xis, testOptions.GetDataPath( "." ), missionTypes, false, testOptions.GetDataPath( "resources" ) );
    StubMIL_PopulationType popuType( model );
    StubMIL_Population population( popuType, controller, army );

    xml::xistringstream xisConcentration( "<population attitude='agressive' id='37' name='Population standard [37]' position='35RPQ9407811091' type='Population standard'><composition healthy='10001' wounded='0' contaminated='0' dead='0'/></population>");
    xisConcentration.start( "population" );
    MIL_PopulationConcentration concentration( population, xisConcentration );

    unsigned int before = concentration.GetTotalLivingHumans();
    object->ProcessPopulationInside( concentration );
    BOOST_CHECK( before > concentration.GetTotalLivingHumans() );
}

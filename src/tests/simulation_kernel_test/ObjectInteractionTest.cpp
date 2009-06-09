// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"

// ASN
struct ASN1T_MagicActionCreateObject;
struct ASN1T_ObjectAttributes;
struct ASN1T_Location;

#include "simulation_kernel/Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectLoader.h"
#include "simulation_kernel/Entities/Objects/MIL_Object_ABC.h"
#include "simulation_kernel/Entities/Objects/Object.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectType_ABC.h"
#include "simulation_kernel/Entities/Objects/AnimatorAttribute.h"
#include "simulation_kernel/Entities/Objects/ConstructionAttribute.h"
#include "simulation_kernel/Entities/Objects/NBCAttribute.h"
#include "simulation_kernel/Entities/Objects/ToxicAttribute_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_ToxicEffectManipulator.h"
#include "simulation_kernel/Entities/Objects/ContaminationCapacity.h"
#include "simulation_kernel/Entities/Objects/DetectionCapacity.h"
#include "simulation_kernel/Entities/Objects/InteractIfEquippedCapacity.h"
#include "simulation_kernel/Entities/Objects/InteractWithEnemyCapacity.h"
#include "simulation_kernel/Entities/Objects/ProtectionCapacity.h"
#include "simulation_kernel/Entities/Objects/SupplyRouteAttribute.h"
#include "simulation_kernel/Entities/Objects/SupplyCapacity.h"
#include "simulation_kernel/Entities/Objects/ObstacleAttribute.h"
#include "simulation_kernel/Entities/Objects/TimeLimitedAttribute.h"
#include "simulation_kernel/Entities/Objects/MineAttribute.h"

#include "MockBuilder.h"
#include "MockArmy.h"
#include "MockAgent.h"
#include "MockRoleLocation.h"
#include "MockRoleNBC.h"
#include "MockRoleInterface_Posture.h"
#include "MockRolePerceiver.h"
#include <xeumeuleu/xml.h>

using namespace mockpp;

namespace
{
    MIL_Object_ABC* CreateObject( const MIL_ObjectType_ABC& type, MockArmy& army, MIL_ObjectLoader& loader )
    {
        MIL_Object_ABC* pObject =0;
        MockBuilder builder;
        builder.GetType_mocker.expects( mockpp::once() ) .will( returnValue( &type ) );
        MOCKPP_CHAINER_FOR( MockBuilder, Build )         ( &builder ).expects( mockpp::once() );
        BOOST_CHECK_NO_THROW(
            pObject = loader.CreateObject( builder, army );
        );
        builder.verify();
        
        BOOST_REQUIRE( pObject != 0 );
        return pObject;
    }

    template< typename T >
    void CheckCapacity( MIL_Object_ABC& object )
    {
        BOOST_REQUIRE( object.Retrieve< T >() != 0 );
    }
}

// -----------------------------------------------------------------------------
/* Name: Test VerifyObjectCapacity_Interaction_Contamination_NoNBC
   Create an object prototype able to contaminate.
   Instanciate the object and test if the interaction mechanism is triggered
   without nbc/toxic attribute
*/
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Interaction_Contamination_NoNBC )
{
    MIL_ObjectLoader loader;
    
    {
        xml::xistringstream xis( "<objects>" 
                "<object type='object'>"
                    "<contamination type='nbc' max-toxic='1'/>"                    
                "</object>" 
            "</objects>"
            ); 
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = loader.GetType( "object" );
    
    MockArmy army;
    MOCKPP_CHAINER_FOR( MockArmy, RegisterObject ) ( &army ).expects( mockpp::once() );

    MIL_Object_ABC* pObject = CreateObject( type, army, loader );
    CheckCapacity< ContaminationCapacity >( *pObject );

    MT_Vector2D position;
    MockAgent agent;
    agent.RegisterRole< MockRoleLocation >();
    {
        MOCKPP_CHAINER_FOR( MockRoleLocation, NotifyObjectCollision ) ( &agent.GetRole< MockRoleLocation >() ).expects( mockpp::once() );
        MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )     ( &agent.GetRole< MockRoleLocation >() )
            .expects( mockpp::once() ).will( returnValue( static_cast< const MT_Vector2D* >( &position ) ) );
    }
    agent.RegisterRole< MockRoleNBC >();
    {
        MOCKPP_CHAINER_FOR( MockRoleNBC, Contaminate ) ( &agent.GetRole< MockRoleNBC >() ).expects( mockpp::never() );
    }
    BOOST_CHECK_NO_THROW( pObject->ProcessAgentInside( agent ) );

    agent.GetRole< MockRoleLocation >().verify();
    agent.GetRole< MockRoleNBC >().verify();
    agent.verify();
    army.verify();
}

namespace
{        
    class MockToxicAttribute : public ChainableMockObject, public ToxicAttribute_ABC
    {
    public:
        MockToxicAttribute() 
            : ChainableMockObject( MOCKPP_PCHAR( "MockToxicAttribute" ) )
            , mocker ( MOCKPP_PCHAR( "GetContaminationEffect" ), this )
        {}

        MIL_ToxicEffectManipulator GetContaminationEffect( const NBCAttribute& nbc, const MT_Vector2D& position ) const
        {
            mocker.forward( &nbc, &position );
            return MIL_ToxicEffectManipulator( stub_, 0 );
        }
        
    public:
        ChainableMockMethod< void, const NBCAttribute*, const MT_Vector2D* > mocker;
    private:
        MIL_ToxicEffectManipulator::T_NBCAgents stub_;
    };
}

// -----------------------------------------------------------------------------
/* Name: Test VerifyObjectCapacity_Interaction_Contamination_NBC
   Create an object prototype able to contaminate.
   Instanciate the object and test if the interaction mechanism is triggered
   with nbc/toxic attributes
*/
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Interaction_Contamination_NBC )
{
    MIL_ObjectLoader loader;
    
    {
        xml::xistringstream xis( "<objects>" 
                "<object type='object'>"                    
                    "<contamination type='nbc' max-toxic='1'/>"
                "</object>" 
            "</objects>"
            ); 
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = loader.GetType( "object" );
    
    MockArmy army;
    MOCKPP_CHAINER_FOR( MockArmy, RegisterObject ) ( &army ).expects( mockpp::once() );

    MIL_Object_ABC* pObject = CreateObject( type, army, loader );
    CheckCapacity< ContaminationCapacity >( *pObject );

    // The following lines are used to force the Instanciation of attributes
    BOOST_CHECK_NO_THROW( ( static_cast< Object* >( pObject )->GetAttribute< MockToxicAttribute, ToxicAttribute_ABC >() ) );
    BOOST_CHECK_NO_THROW( ( static_cast< Object* >( pObject )->GetAttribute< NBCAttribute >() ) );

    MT_Vector2D position;
    MockAgent agent;
    agent.RegisterRole< MockRoleLocation >();    
    {
        MOCKPP_CHAINER_FOR( MockRoleLocation, NotifyObjectCollision ) ( &agent.GetRole< MockRoleLocation >() ).expects( mockpp::once() );
        MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )     ( &agent.GetRole< MockRoleLocation >() )
            .expects( mockpp::once() ).will( returnValue( static_cast< const MT_Vector2D* >( &position ) ) );
    }
    agent.RegisterRole< MockRoleNBC >();
    {
        // TODO : mockpp::once()
        MOCKPP_CHAINER_FOR( MockRoleNBC, Contaminate ) ( &agent.GetRole< MockRoleNBC >() ).expects( mockpp::never() );
    }
    BOOST_CHECK_NO_THROW( pObject->ProcessAgentInside( agent ) );

    agent.GetRole< MockRoleLocation >().verify();
    agent.GetRole< MockRoleNBC >().verify();
    agent.verify();
    army.verify();
}

// -----------------------------------------------------------------------------
/* Name: Test VerifyObjectCapacity_Interaction_Protection
Create an object prototype able to be an implantation area.
Instanciate the object and test if the interaction mechanism is triggered
*/
// Created: MGD 2009-03-04
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Interaction_Protection )
{
    //@TODO test à renforcer après le merge
    MIL_ObjectLoader loader;

    {
        xml::xistringstream xis( "<objects>" 
                "<object type='implantationArea'>"                    
                    "<protection max-size='1' geniePrepared='false'/>"
                "</object>" 
            "</objects>"
            ); 
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = loader.GetType( "implantationArea" );

    MockArmy army;
    MOCKPP_CHAINER_FOR( MockArmy, RegisterObject ) ( &army ).expects( mockpp::once() );

    MIL_Object_ABC* pObject = CreateObject( type, army, loader );
    CheckCapacity< ProtectionCapacity >( *pObject );

    //First add
    MT_Vector2D position;
    MockAgent agent;
    agent.RegisterRole< MockRoleLocation >();    
    {
        MOCKPP_CHAINER_FOR( MockRoleLocation, NotifyObjectCollision ) ( &agent.GetRole< MockRoleLocation >() ).expects( mockpp::once() );
        MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )     ( &agent.GetRole< MockRoleLocation >() )
            .expects( mockpp::once() ).will( returnValue( static_cast< const MT_Vector2D* >( &position ) ) );
    }
    agent.RegisterRole< MockRoleInterface_Posture >();
    {
        MOCKPP_CHAINER_FOR( MockRoleInterface_Posture, SetTimingFactor ) ( &agent.GetRole< MockRoleInterface_Posture >() ).expects( mockpp::once() );
    }

    pObject->ProcessAgentEntering( agent );
    BOOST_CHECK_NO_THROW( pObject->ProcessAgentInside( agent ) );

    agent.GetRole< MockRoleInterface_Posture >().verify();

    agent.verify();
    army.verify();
}


// -----------------------------------------------------------------------------
/* Name: Test VerifyObjectCapacity_Interaction_InteractIfEquipped
    Create an object prototype with supply-route capacity and checks it "can interacti with"
    only if it is equipeed.
*/
// Created: LDC 2009-03-05
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Interaction_InteractIfEquipped )
{
    MIL_ObjectLoader loader;

    {
        xml::xistringstream xis( "<objects>" 
                "<object geometry='line' type='itineraire logistique'>"
                    "<supply-route/>"
                "</object>"
            "</objects>"
            ); 
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = loader.GetType( "itineraire logistique" );

    MockArmy army;
    MOCKPP_CHAINER_FOR( MockArmy, RegisterObject ) ( &army ).expects( mockpp::once() );

    MIL_Object_ABC* pObject = CreateObject( type, army, loader );
    CheckCapacity< InteractIfEquippedCapacity >( *pObject );

    MockAgent agent;
    
    // Force creation of attribute SupplyRouteAttribute...
    BOOST_CHECK_NO_THROW( ( static_cast< Object* >( pObject )->GetAttribute< SupplyRouteAttribute >() ) );
    BOOST_CHECK_EQUAL( pObject->GetAttribute< SupplyRouteAttribute >().IsEquipped(), pObject->CanInteractWith( agent ) );

    army.verify();
}

// -----------------------------------------------------------------------------
/* Name: Test VerifyObjectCapacity_Interaction_Supply
    Create an object prototype able to be a supply area.
    Instanciate the object and test if the interaction mechanism is triggered
*/
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Interaction_Supply )
{
    //@TODO test à renforcer après le merge
    MIL_ObjectLoader loader;
    {
        xml::xistringstream xis( "<objects>" 
                "<object type='plot ravitaillement'>"
                    "<supply/>"
                "</object>"
            "</objects>"
            ); 
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = loader.GetType( "plot ravitaillement" );

    MockArmy army;
    MOCKPP_CHAINER_FOR( MockArmy, RegisterObject )( &army ).expects( mockpp::once() );

    MIL_Object_ABC* pObject = CreateObject( type, army, loader );
    CheckCapacity< SupplyCapacity >( *pObject );

    //First add
    MT_Vector2D position;
    MockAgent agent;
    {
        MOCKPP_CHAINER_FOR( MockAgent, GetArmyShadow )( &agent ).expects( mockpp::once() );
    }
    agent.RegisterRole< MockRoleLocation >();
    {
        MOCKPP_CHAINER_FOR( MockRoleLocation, NotifyObjectCollision )( &agent.GetRole< MockRoleLocation >() ).expects( mockpp::once() );
        MOCKPP_CHAINER_FOR( MockRoleLocation, GetPositionShadow )    ( &agent.GetRole< MockRoleLocation >() )
            .expects( mockpp::once() ).will( returnValue( static_cast< const MT_Vector2D* >( &position ) ) );
    }
    BOOST_CHECK_NO_THROW( pObject->ProcessAgentEntering( agent ) );
    agent.verify();
    army.verify();
}

// -----------------------------------------------------------------------------
/* Name: Test VerifyObjectCapacity_Interaction_InteractWithEnemy
    Create an object prototype with interact-with-enemy capacity and checks it "can interact with"
    only with other armies.
*/
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Interaction_InteractWithEnemy )
{
    MIL_ObjectLoader loader;

    {
        xml::xistringstream xis( "<objects>" 
                "<object geometry='line' type='barricade'>"
                    "<interact-with-enemy/>"
                "</object>"
            "</objects>"
            ); 
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = loader.GetType( "barricade" );

    MockArmy army;
    MockArmy enemyArmy;
    MOCKPP_CHAINER_FOR( MockArmy, RegisterObject ) ( &army ).expects( mockpp::once() );

    MIL_Object_ABC* pObject = CreateObject( type, army, loader );
    CheckCapacity< InteractWithEnemyCapacity >( *pObject );

    MockAgent agent;
    MOCKPP_CHAINER_FOR( MockAgent, GetArmyShadow ) ( &agent ).expects( mockpp::once() ).will( returnValue( static_cast< MIL_Army_ABC* >( &army ) ) );
    BOOST_CHECK_EQUAL( false, pObject->CanInteractWith( agent ) );
    MOCKPP_CHAINER_FOR( MockAgent, GetArmyShadow ) ( &agent ).expects( mockpp::once() ).will( returnValue( static_cast< MIL_Army_ABC* >( &enemyArmy ) ) );
    BOOST_CHECK_EQUAL( true, pObject->CanInteractWith( agent ) );

    agent.verify();
    army.verify();
}

// -----------------------------------------------------------------------------
// Name: VerifyObjectCapacity_Interaction_Detection
// Created: SBO 2009-06-08
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Interaction_Detection )
{
    MIL_ObjectLoader loader;
    {
        xml::xistringstream xis( "<objects>"
                "<object geometry='point' type='checkpoint'>"
                    "<detection/>"
                "</object>"
            "</objects>"
        );
        loader.Initialize( xis );
    }
    const MIL_ObjectType_ABC& type = loader.GetType( "checkpoint" );
    MockArmy army;
    MOCKPP_CHAINER_FOR( MockArmy, RegisterObject ) ( &army ).expects( mockpp::once() );

    MIL_Object_ABC* object = CreateObject( type, army, loader );
    CheckCapacity< DetectionCapacity >( *object );
    BOOST_CHECK_NO_THROW( static_cast< Object* >( object )->GetAttribute< AnimatorAttribute >() );

    MockAgent animator;
    animator.RegisterRole< MockRolePerceiver >();
    static_cast< Object* >( object )->GetAttribute< AnimatorAttribute >().AddAnimator( animator );

    MockAgent intruder;
    BOOST_CHECK_NO_THROW( object->ProcessAgentEntering( intruder ) );
    animator.GetRole< MockRolePerceiver >().NotifyExternalPerception_mocker.expects( once() )
                                                                           .with( same< MIL_Agent_ABC >( intruder )
                                                                                , same< const PHY_PerceptionLevel >( PHY_PerceptionLevel::identified_ ) );
    army.verify();
    animator.verify();
    intruder.verify();
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: SBO 2009-06-08
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Interaction_Mine )
{
    MIL_ObjectLoader loader;
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
        loader.Initialize( xis );
    }

    const MIL_ObjectType_ABC& type = loader.GetType( "mines" );
    MockArmy army;
    MOCKPP_CHAINER_FOR( MockArmy, RegisterObject ) ( &army ).expects( mockpp::once() );

    MIL_Object_ABC* object = CreateObject( type, army, loader );
    BOOST_CHECK_NO_THROW( static_cast< Object* >( object )->GetAttribute< ObstacleAttribute >() );
    BOOST_CHECK_NO_THROW( static_cast< Object* >( object )->GetAttribute< TimeLimitedAttribute >() );
    BOOST_CHECK_NO_THROW( static_cast< Object* >( object )->GetAttribute< MineAttribute >() );

    army.verify();
}


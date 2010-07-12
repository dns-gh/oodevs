// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "integration_decisionnal_test_pch.h"
#include "Fixture.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace
{
    class BrainFixture : public Fixture
    {
    public:
        BrainFixture()
        {
            brain.RegisterFunction( "DEC_ConnaissanceAgent_EstUnEnnemi", boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsEnemy, boost::cref( this ), _1  ) ) );
            brain.RegisterFunction( "DEC_ConnaissanceAgent_EstUnAllie", boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsAllie, boost::cref( this ), _1  ) ) );
            brain.RegisterFunction( "DEC_ConnaissanceObject_EstUnEnnemi", boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsEnemy, boost::cref( this ), _1  ) ) );
            brain.RegisterFunction( "DEC_ConnaissanceObject_EstUnAllie", boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsAllie, boost::cref( this ), _1  ) ) );
            brain.RegisterFunction( "DEC_ConnaissanceObjet_NiveauDePerceptionCourant", boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_GetPerceptionLevel, boost::cref( this ), _1  ) ) );
            brain.RegisterFunction( "DEC_ConnaissanceAgent_NiveauDePerceptionCourant", boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_GetPerceptionLevel, boost::cref( this ), _1  ) ) );
            brain.RegisterFunction( "DEC_ConnaissanceUrbanBlock_NiveauDePerceptionCourant", boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_GetPerceptionLevel, boost::cref( this ), _1  ) ) );
        }
        int Mock_IsEnemy( const std::string& name )
        {
            if( name == "enemy" )
                return 0;
            if( name == "allie" )
                return 1;
            return 2;
        }
        int Mock_IsAllie( const std::string& name )
        {
            if( name == "allie" )
                return 0;
            if( name == "enemy" )
                return 1;
            return 2;
        }
        int Mock_GetPerceptionLevel( const std::string& name )
        {
            if( name == "enemyDetected" )
                return 1;
            if( name == "enemyRecognized" )
                return 2;
            if( name == "enemyIdentified" )
                return 3;
            return 0;
        }
        void ComputeRelationAgentTest( directia::ScriptRef unit, double expected )
        {
            directia::ScriptRef computeRelation = *brain.GetScriptFunction( "integration.computeRelationAgent" );
            BOOST_CHECK( computeRelation( unit ) );
            brain.GetScriptFunction( "check" )( computeRelation, expected );
        }
        void ComputeRelationObjectTest( directia::ScriptRef unit, double expected )
        {
            directia::ScriptRef computeRelation = *brain.GetScriptFunction( "integration.computeRelationAgent" );
            BOOST_CHECK( computeRelation( unit ) );
            brain.GetScriptFunction( "check" )( computeRelation, expected );
        }
        void GetAgentPerceptionLevelTest( directia::ScriptRef unit, double expected )
        {
            directia::ScriptRef getAgentPerceptionLevel = *brain.GetScriptFunction( "integration.getAgentPerception" );
            BOOST_CHECK( getAgentPerceptionLevel( unit ) );
            brain.GetScriptFunction( "check" )( getAgentPerceptionLevel, expected );
        }
        void GetObjectPerceptionLevelTest( directia::ScriptRef unit, double expected )
        {
            directia::ScriptRef getObjectPerceptionLevel = *brain.GetScriptFunction( "integration.getObjectPerception" );
            BOOST_CHECK( getObjectPerceptionLevel( unit ) );
            brain.GetScriptFunction( "check" )( getObjectPerceptionLevel, expected );
        }
        void GetUrbanBlockPerceptionLevelTest( directia::ScriptRef unit, double expected )
        {
            directia::ScriptRef getUrbanBlockPerceptionLevel = *brain.GetScriptFunction( "integration.getUrbanBlockPerception" );
            BOOST_CHECK( getUrbanBlockPerceptionLevel( unit ) );
            brain.GetScriptFunction( "check" )( getUrbanBlockPerceptionLevel, expected );
        }
        directia::ScriptRef CreateAgent( const std::string& name )
        {
            directia::ScriptRef agent = brain.RegisterObject();
            agent.RegisterObject( "source", name );
            return agent;
        }
    };
}
// -----------------------------------------------------------------------------
// Name: ComputeRelationAgent
// Created: MGD 2010-01-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ComputeRelationAgent, BrainFixture )
{
    ComputeRelationAgentTest( CreateAgent( "enemy" ), 0. );
    ComputeRelationAgentTest( CreateAgent( "neutre" ), 50. );
    ComputeRelationAgentTest( CreateAgent( "allie" ), 100. );
}

// -----------------------------------------------------------------------------
// Name: ComputeRelationObject
// Created: MGD 2010-01-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ComputeRelationObject, BrainFixture )
{
    ComputeRelationObjectTest( CreateAgent( "enemy" ), 0. );
    ComputeRelationObjectTest( CreateAgent( "neutre" ), 50. );
    ComputeRelationObjectTest( CreateAgent( "allie" ), 100. );
}

// -----------------------------------------------------------------------------
// Name: GetAgentPerceptionLevel
// Created: MGD 2010-01-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( GetAgentPerceptionLevel, BrainFixture )
{
    GetAgentPerceptionLevelTest( CreateAgent( "enemyNotSeen"    ), 0. );
    GetAgentPerceptionLevelTest( CreateAgent( "enemyDetected"   ), 30. );
    GetAgentPerceptionLevelTest( CreateAgent( "enemyRecognized" ), 60. );
    GetAgentPerceptionLevelTest( CreateAgent( "enemyIdentified" ), 100. );
}

// -----------------------------------------------------------------------------
// Name: GetObjectPerceptionLevel
// Created: MGD 2010-01-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( GetObjectPerceptionLevel, BrainFixture )
{
    GetObjectPerceptionLevelTest( CreateAgent( "enemyDetected"   ), 30. );
    GetObjectPerceptionLevelTest( CreateAgent( "enemyRecognized" ), 60. );
    GetObjectPerceptionLevelTest( CreateAgent( "enemyIdentified" ), 100. );
    GetObjectPerceptionLevelTest( CreateAgent( "enemyNotSeen"    ), 0. );
}

// -----------------------------------------------------------------------------
// Name: GetObjectPerceptionLevel
// Created: MGD 2010-01-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( GetUrbanPerceptionLevel, BrainFixture )
{
    GetUrbanBlockPerceptionLevelTest( CreateAgent( "enemyDetected"   ), 30. );
    GetUrbanBlockPerceptionLevelTest( CreateAgent( "enemyRecognized" ), 60. );
    GetUrbanBlockPerceptionLevelTest( CreateAgent( "enemyIdentified" ), 100. );
    GetUrbanBlockPerceptionLevelTest( CreateAgent( "enemyNotSeen"    ), 0. );
}

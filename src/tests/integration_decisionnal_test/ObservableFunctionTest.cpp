// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "integration_decisionnal_test_pch.h"
#include <directia/brain/Brain.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace
{
    class BrainFixture
    {
    public:
        BrainFixture()
        : brain( BRAIN_INIT )
        {
            brain[ "include" ]( std::string("Integration.lua") );
            brain[ "DEC_ConnaissanceAgent_EstUnEnnemi" ] = boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsEnemy, boost::cref(this), _1  ) );
            brain[ "DEC_ConnaissanceAgent_EstUnAllie" ] = boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsAllie, boost::cref(this), _1  ) );
            brain[ "DEC_ConnaissanceObject_EstUnEnnemi" ] = boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsEnemy, boost::cref(this), _1  ) );
            brain[ "DEC_ConnaissanceObject_EstUnAllie" ] = boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsAllie, boost::cref(this), _1  ) );
            brain[ "DEC_ConnaissanceObjet_NiveauDePerceptionCourant" ] = boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_GetPerceptionLevel, boost::cref(this), _1  ) );
            brain[ "DEC_ConnaissanceAgent_EstPercuParUnite" ] = boost::function< bool ( const std::string& ) >( boost::bind( &BrainFixture::Mock_PerceiveUnit, boost::cref(this), _1  ) );
            brain[ "DEC_ConnaissanceUrbanBlock_NiveauDeReconnaissanceCourant" ] = boost::function< float ( const std::string& ) >( boost::bind( &BrainFixture::Mock_GetRecceLevel, boost::cref(this), _1 ) );
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
        bool Mock_PerceiveUnit( const std::string& name )
        {
            return( name == "enemySeen" );
        }
        float Mock_GetRecceLevel( const std::string& name )
        {
            if( name == "enemyDetected" )
                return 0.3f;
            if( name == "enemyRecognized" )
                return 0.6f;
            if( name == "enemyIdentified" )
                return 1.f;
            return 0;
        }
        void ComputeRelationAgentTest( directia::tools::binders::ScriptRef unit, double expected )
        {
            directia::tools::binders::ScriptRef computeRelation = brain[ "integration.computeRelationAgent" ];
            BOOST_CHECK_EQUAL( computeRelation.Call< double >( unit ), expected );
        }
        void ComputeRelationObjectTest( directia::tools::binders::ScriptRef unit, double expected )
        {
            directia::tools::binders::ScriptRef computeRelation = brain[ "integration.computeRelationAgent" ];
            BOOST_CHECK_EQUAL( computeRelation.Call< double >( unit ), expected );
        }
        void GetAgentPerceptionLevelTest( directia::tools::binders::ScriptRef unit, double expected )
        {
            directia::tools::binders::ScriptRef getAgentPerceptionLevel = brain[ "integration.getAgentPerceptionLevel" ];
            BOOST_CHECK_EQUAL( getAgentPerceptionLevel.Call< double >( unit ), expected );
        }
        void GetObjectPerceptionLevelTest( directia::tools::binders::ScriptRef unit, double expected )
        {
            directia::tools::binders::ScriptRef getObjectPerceptionLevel = brain[ "integration.getObjectPerception" ];
            BOOST_CHECK_CLOSE( getObjectPerceptionLevel.Call< double >( unit ), expected, 0.0001 );
        }
        void GetUrbanBlockPerceptionLevelTest( directia::tools::binders::ScriptRef unit, double expected )
        {
            directia::tools::binders::ScriptRef getUrbanBlockPerceptionLevel = brain[ "integration.getUrbanBlockPerception" ];
            BOOST_CHECK_CLOSE( getUrbanBlockPerceptionLevel.Call< double >( unit ), expected, 0.0001 );
        }
        directia::tools::binders::ScriptRef CreateAgent( const std::string& name )
        {
            directia::tools::binders::ScriptRef enemy( brain );
            enemy[ "source" ] = name;
            return enemy;
        }
    public:
        directia::brain::Brain brain;
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
    GetAgentPerceptionLevelTest( CreateAgent( "enemySeen"   ), 100. );
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

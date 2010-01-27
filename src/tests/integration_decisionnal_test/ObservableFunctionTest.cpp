// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "integration_decisionnal_test_pch.h"

#include <directia/Brain.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
using namespace mockpp;

namespace
{
    void Check( double result, double expected )
    {
        BOOST_CHECK_CLOSE( result, expected, std::numeric_limits<float>::epsilon() );
    }
    class BrainFixture
    {
    public:
        BrainFixture()
        : brain( BOOST_RESOLVE( "." ) ) 
        {
            brain.RegisterFunction< boost::function< void( double, double ) > >( "check", boost::bind( &Check, _1, _2 ) );
            brain.GetScriptFunction( "include" )( std::string("Integration.lua") );
            brain.RegisterFunction( "DEC_ConnaissanceAgent_EstUnEnnemi", boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsEnemy, boost::cref(this), _1  ) ) );
            brain.RegisterFunction( "DEC_ConnaissanceAgent_EstUnAllie", boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsAllie, boost::cref(this), _1  ) ) );
            brain.RegisterFunction( "DEC_ConnaissanceObject_EstUnEnnemi", boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsEnemy, boost::cref(this), _1  ) ) );
            brain.RegisterFunction( "DEC_ConnaissanceObject_EstUnAllie", boost::function< int ( const std::string& ) >( boost::bind( &BrainFixture::Mock_IsAllie, boost::cref(this), _1  ) ) );
        }
        int Mock_IsEnemy( const std::string& name )
        {
            if ( name == "enemy" )
                return 0;
            if ( name == "allie" )
                return 1;
            return 2;
        }
        int Mock_IsAllie( const std::string& name )
        {
            if ( name == "allie" )
                return 0;
            if ( name == "enemy" )
                return 1;
            return 2;
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
        directia::ScriptRef CreateAgent( const std::string& name )
        {
            directia::ScriptRef enemy = brain.RegisterObject();
            enemy.RegisterObject( "source", name );
            return enemy;
        }
    public:
        directia::Brain brain;
    };
}
// -----------------------------------------------------------------------------
// Name: ComputeRelationAgent
// Created: MGD 2010-01-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ComputeRelationAgent, BrainFixture )
{
    ComputeRelationAgentTest( CreateAgent( "allie" ), 100. );
    ComputeRelationAgentTest( CreateAgent( "enemy" ), 0. );
    ComputeRelationAgentTest( CreateAgent( "neutre" ), 50. );
}
// -----------------------------------------------------------------------------
// Name: ComputeRelationObject
// Created: MGD 2010-01-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ComputeRelationObject, BrainFixture )
{
    ComputeRelationObjectTest( CreateAgent( "allie" ), 100. );
    ComputeRelationObjectTest( CreateAgent( "enemy" ), 0. );
    ComputeRelationObjectTest( CreateAgent( "neutre" ), 50. );
}
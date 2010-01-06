
#include "simulation_kernel_test_pch.h"

#include "Tools/MIL_Config.h"
#include "Decision/DEC_Workspace.h"
#include "Decision/DEC_Model.h"
#include "Decision/Functions/DEC_MiscFunctions.h"
#include "Decision/Functions/DEC_PopulationFunctions.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "MT_Tools/MT_ScipioException.h"
#include "Fixture.h"
#include "MockMIL_MissionType_ABC.h"
#include "StubDEC_Database.h"
#include "StubMIL_AgentPion.h"
#include "StubMIL_AgentTypePion.h"
#include "StubMIL_Automate.h"
#include "StubMIL_AutomateType.h"
#include "StubDEC_KnowledgeResolver_ABC.h"
#include "StubMIL_Mission_ABC.h"
#include "StubMIL_PopulationType.h"
#include "StubMIL_Population.h"
#include <xeumeuleu/xml.h>

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: InstantiateBrainForMIL_AgentPion
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( InstantiateBrainForMIL_AgentPion )
{
    FixturePion fixture;
    DEC_RolePion_Decision decision ( *fixture.pPion_, StubDEC_Database() );
}

// -----------------------------------------------------------------------------
// Name: InstantiateDEC_AutomateDecision
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( InstantiateDEC_AutomateDecision )
{
    FixtureAutomate fixture;
    DEC_AutomateDecision decision( *fixture.pAutomat_, StubDEC_Database() );
}

// -----------------------------------------------------------------------------
// Name: InstantiateDEC_PopulationDecision
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( InstantiateDEC_PopulationDecision )
{
    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
    xis.start( "main" );
    std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > missionTypes;
    DEC_Model model( "test", xis, BOOST_RESOLVE( "." ), "prefix", missionTypes );
    StubMIL_PopulationType type( model );
    StubMIL_Population population( type );
    DEC_PopulationDecision decision( population, StubDEC_Database() );
}

class ScriptMocker : public mockpp::ChainableMockObject
{
public:
    ScriptMocker()
        : mockpp::ChainableMockObject( "ScriptMocker", 0 )
        , Call_mocker( "Call", this )
        , Mission_mocker( "Mission", this )
    {}
    mockpp::ChainableMockMethod< void, std::string > Call_mocker;
    mockpp::ChainableMockMethod< void, MIL_Mission_ABC* > Mission_mocker;
};

class DEC_TestPopulationDecision;

namespace
{
    ScriptMocker scriptMocker_;
    void NotifyCallFromScript( const std::string& text )
    {
        scriptMocker_.Call_mocker.forward( text );
    }
    void NotifyMissionCallFromScript( MIL_Mission_ABC* pMission )
    {
        scriptMocker_.Mission_mocker.forward( pMission );
    }
    MIL_Mission_ABC* GetRawMission( DEC_TestPopulationDecision* pAgent );
}

class DEC_TestPopulationDecision : public DEC_Decision<MIL_Population>
{
public:
    DEC_TestPopulationDecision( MIL_Population& population, DEC_TestPopulationDecision* pOther )
        : DEC_Decision( population, StubDEC_Database() ) 
        , pOther_     ( pOther )
    {
        const DEC_Model_ABC& model = population.GetType().GetModel();
        SetModel( model );
        StartDefaultBehavior();
    }

    void StartMissionBehavior( MIL_Mission_ABC& mission )
    {
        const std::string& strBehavior = mission.GetType().GetDIABehavior();
        ActivateOrder( strBehavior, mission );
    }
    
    virtual DEC_AutomateDecision* GetDecAutomate() const { return 0; }
    virtual std::string GetName() const { return "Test Decision"; }
    virtual void EndCleanStateAfterCrash() {}
    void RegisterSelf( directia::Brain& brain )
    {
        brain.RegisterObject( "myself", this );
    }
    void UpdateMeKnowledge( directia::Brain& brain ) {}

    void UsedByDIA() {}
    void ReleasedByDIA () {}

protected:
    virtual void RegisterUserFunctions( directia::Brain& brain )
    {
        brain.RegisterFunction( "DEC_TestBehaviorCalled",    &NotifyCallFromScript );
        brain.RegisterFunction( "DEC_TestMissionCalled",     &NotifyMissionCallFromScript );
        brain.RegisterFunction( "DEC_GetRawMission",         &GetRawMission );
        brain.RegisterFunction( "DEC_FillMissionParameters",
            boost::function< void( const directia::ScriptRef&, MIL_Mission_ABC* ) >( boost::bind( &DEC_MiscFunctions::FillMissionParameters, boost::ref( brain.GetScriptFunction("InitTaskParameter") ), _1 , _2 ) ) );
        if( pOther_ )
            brain.RegisterObject< DEC_TestPopulationDecision* >( "other", pOther_ );
    }

private:
    DEC_TestPopulationDecision* pOther_;
};

namespace
{  
    MIL_Mission_ABC* GetRawMission( DEC_TestPopulationDecision* pAgent )
    {
        return pAgent->GetMission();
    }
}

// -----------------------------------------------------------------------------
// Name: ActivateOrderExecutesBehaviour
// Created: LDC 2009-04-27
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ActivateOrderExecutesBehaviour )
{
    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
    xis.start( "main" );
    std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > missionTypes;
    DEC_Model model( "test", xis, BOOST_RESOLVE( "." ), "prefix", missionTypes );
    StubMIL_PopulationType type( model );
    StubMIL_Population population( type );
    DEC_TestPopulationDecision decision( population, 0 );
    MockMIL_MissionType_ABC missionType;
    std::string typeString( "missionBehavior" );
    missionType.GetDIABehavior_mocker.expects( once() ).will( returnValue( &typeString ) );
    StubDEC_KnowledgeResolver_ABC resolver;
    StubMIL_Mission_ABC mission( missionType, resolver );
    decision.StartMissionBehavior( mission );
    missionType.verify();
    scriptMocker_.Call_mocker.expects( once() ).with(  eq< std::string >( "missionBehavior called" ) );
    decision.UpdateDecision( 1.f );
    scriptMocker_.verify();
}

// -----------------------------------------------------------------------------
// Name: ActivateOrderPassesMissionToDecisional
// Created: LDC 2009-04-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ActivateOrderPassesMissionToDecisional )
{
    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
    xis.start( "main" );
    std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > missionTypes;
    DEC_Model model( "test", xis, BOOST_RESOLVE( "." ), "prefix", missionTypes );
    StubMIL_PopulationType type( model );
    StubMIL_Population population( type );
    DEC_TestPopulationDecision decision( population, 0 );
    MockMIL_MissionType_ABC missionType;
    std::string typeString( "missionBehaviorWithArg" );
    missionType.GetDIABehavior_mocker.expects( once() ).will( returnValue( &typeString ) );
    StubDEC_KnowledgeResolver_ABC resolver;
    StubMIL_Mission_ABC mission( missionType, resolver );
    decision.StartMissionBehavior( mission );
    missionType.verify();
    scriptMocker_.Mission_mocker.expects( once() ).with( eq< MIL_Mission_ABC* >( &mission ) );
    decision.UpdateDecision( 1.f );
    scriptMocker_.verify();
}

// -----------------------------------------------------------------------------
// Name: DecisionalCanUseMissionParameters
// Created: LDC 2009-04-29
// Tests that if a MIL_Mission_ABC is passed to lua through a function, 
// its parameters can be accessed from lua
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( DecisionalCanUseMissionParameters )
{
    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
    xis.start( "main" );
    std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > missionTypes;
    DEC_Model model( "test", xis, BOOST_RESOLVE( "." ), "prefix", missionTypes );
    StubMIL_PopulationType type( model );
    StubMIL_Population population( type );
    DEC_TestPopulationDecision decision( population, 0 );
    MockMIL_MissionType_ABC missionType;
    std::string typeString( "missionBehavior" );
    missionType.GetDIABehavior_mocker.expects( once() ).will( returnValue( &typeString ) );
    StubDEC_KnowledgeResolver_ABC resolver;
    std::string missionParameter( "param" );
    StubMIL_Mission_ABC mission( missionType, resolver, missionParameter );
    decision.StartMissionBehavior( mission );
    missionType.verify();
    scriptMocker_.Call_mocker.expects( once() ).with( eq( std::string( "missionBehavior called" ) ) );
    decision.UpdateDecision( 1.f );
    scriptMocker_.verify();
}

// -----------------------------------------------------------------------------
// Name: DEC_GetMissionPassesParametersToLua
// Created: LDC 2009-04-30
// Tests that after a call to DEC_GetMission( pion/popu/automat ), the retrieved
// lua object parameters can be accessed from lua
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( DEC_GetMissionPassesParametersToLua )
{
    xml::xistringstream xis( "<main dia-type='PionTest' file='MissionParamTestBrain.bms'/>" );
    xis.start( "main" );
    std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > missionTypes;
    DEC_Model model( "test", xis, BOOST_RESOLVE( "." ), "prefix", missionTypes );
    StubMIL_PopulationType type( model );
    StubMIL_Population population( type );
    DEC_TestPopulationDecision decision( population, 0 );
    MockMIL_MissionType_ABC missionType;
    std::string typeString( "testDEC_GetMission" );
    missionType.GetDIABehavior_mocker.expects( once() ).will( returnValue( &typeString ) );
    StubDEC_KnowledgeResolver_ABC resolver;
    std::string missionParameter( "parameterValue" );
    StubMIL_Mission_ABC mission( missionType, resolver, missionParameter );
    decision.StartMissionBehavior( mission );
    missionType.verify();
    scriptMocker_.Call_mocker.expects( once() ).with( eq( missionParameter ) );
    decision.UpdateDecision( 1.f );
    scriptMocker_.verify();
}

// -----------------------------------------------------------------------------
// Name: DEC_GetMissionPassesParametersToLuaMission
// Created: LDC 2009-04-30
// Tests that after a call to DEC_GetMission( pion/popu/automat ), the retrieved
// lua object parameters can be accessed from lua
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( DEC_GetMissionPassesParametersToLuaMission )
{
    xml::xistringstream xis( "<main dia-type='PionTest' file='MissionParamTestBrain.bms'/>" );
    xis.start( "main" );
    std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > missionTypes;
    DEC_Model model( "test", xis, BOOST_RESOLVE( "." ), "prefix", missionTypes );
    StubMIL_PopulationType type( model );
    StubMIL_Population population( type );
    DEC_TestPopulationDecision decision( population, 0 );
    MockMIL_MissionType_ABC missionType;
    std::string typeString( "mission" );
    missionType.GetDIABehavior_mocker.expects( once() ).will( returnValue( &typeString ) );
    StubDEC_KnowledgeResolver_ABC resolver;
    std::string missionParameter( "parameterValue" );
    StubMIL_Mission_ABC mission( missionType, resolver, missionParameter );
    decision.StartMissionBehavior( mission );
    missionType.verify();
    scriptMocker_.Call_mocker.expects( once() ).with( eq( missionParameter ) );
    decision.UpdateDecision( 1.f );
    scriptMocker_.verify();
}

// -----------------------------------------------------------------------------
// Name: DEC_GetMissionPassesOtherMissionsParametersToLua
// Created: LDC 2009-04-30
// Tests that after a call to DEC_GetMission( pion/popu/automat ), the retrieved
// lua object parameters can be accessed from lua even if the DEC_GetMission is not
// the current object's mission.
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( DEC_GetMissionPassesOtherMissionsParametersToLua )
{
    xml::xistringstream xis( "<main dia-type='PionTest' file='MissionParamTestBrain.bms'/>" );
    xis.start( "main" );
    std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > missionTypes;
    DEC_Model model( "test", xis, BOOST_RESOLVE( "." ), "prefix", missionTypes );
    StubMIL_PopulationType type( model );
    StubMIL_Population population( type );
    DEC_TestPopulationDecision otherDecision( population, 0 );
    DEC_TestPopulationDecision decision( population, &otherDecision );

    MockMIL_MissionType_ABC missionType;
    std::string typeString( "testDEC_GetOtherMission" );
    missionType.GetDIABehavior_mocker.expects( exactly( 2 ) ).will( returnValue( &typeString ) );

    StubDEC_KnowledgeResolver_ABC resolver;
    StubMIL_Mission_ABC mission( missionType, resolver );

    std::string otherMissionParameter( "otherParameterValue" );
    StubMIL_Mission_ABC otherMission( missionType, resolver, otherMissionParameter );

    otherDecision.StartMissionBehavior( otherMission );
    decision.StartMissionBehavior( mission );
    missionType.verify();
    scriptMocker_.Call_mocker.expects( once() ).with( eq( otherMissionParameter ) );
    decision.UpdateDecision( 1.f );
    scriptMocker_.verify();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision_GetterTest
// Created: LDC 2009-07-31
// Tests that a brain member variables can be set and retrieved from the C++
// (For compatibility when migrating from dia3 to dia4)
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( DEC_Decision_GetterTest )
{
    xml::xistringstream xis( "<main dia-type='PionTest' file='MissionParamTestBrain.bms'/>" );
    xis.start( "main" );
    std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > missionTypes;
    DEC_Model model( "test", xis, BOOST_RESOLVE( "." ), "prefix", missionTypes );
    StubMIL_PopulationType type( model );
    StubMIL_Population population( type );
    DEC_TestPopulationDecision decision( population, 0 );

    decision.SetVariable( "myself.AValue", 42 );
    BOOST_CHECK_EQUAL( 42, decision.GetScalarVariable<int>( "myself.AValue" ) );
}

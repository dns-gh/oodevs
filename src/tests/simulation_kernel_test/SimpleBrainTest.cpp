#include "simulation_kernel_test_pch.h"
#include "Decision/DEC_Workspace.h"
#include "Decision/DEC_Model.h"
#include "Decision/DEC_MiscFunctions.h"
#include "Decision/DEC_PopulationFunctions.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_PopulationOrderManager.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "ActionManager.h"
#include "MissionController.h"
#include "Fixture.h"
#include "MockMIL_MissionType_ABC.h"
#include "MockArmy.h"
#include "StubMIL_AgentPion.h"
#include "StubMIL_AgentTypePion.h"
#include "StubMIL_Automate.h"
#include "StubMIL_AutomateType.h"
#include "StubDEC_KnowledgeResolver_ABC.h"
#include "StubMIL_Mission_ABC.h"
#include "StubMIL_PopulationType.h"
#include "StubMIL_Population.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: InstantiateBrainForMIL_AgentPion
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( InstantiateBrainForMIL_AgentPion )
{
    ActionManager actions;
    MissionController controller( actions );
    MIL_EffectManager effectManager;
    FixturePion fixture( controller, effectManager );
    DEC_RolePion_Decision decision( *fixture.pPion_, 100, 100, false );
    decision.Reload( true );
}

// -----------------------------------------------------------------------------
// Name: InstantiateDEC_AutomateDecision
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( InstantiateDEC_AutomateDecision )
{
    ActionManager actions;
    MissionController controller( actions );
    FixtureAutomate fixture( controller );
    DEC_AutomateDecision decision( *fixture.pAutomat_, 100, 100, false );
}

// -----------------------------------------------------------------------------
// Name: InstantiateDEC_PopulationDecision
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( InstantiateDEC_PopulationDecision )
{
    xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
    xis >> xml::start( "main" );
    std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
    DEC_Model model( "test", xis, testOptions.GetDataPath( "." ), missionTypes, false, testOptions.GetDataPath( "resources" ) );
    ActionManager actions;
    MissionController controller( actions );
    MockArmy army;
    StubMIL_PopulationType type( model );
    StubMIL_Population population( type, controller, army );
    DEC_PopulationDecision decision( population, 100, 100, false );
}

class DEC_TestPopulationDecision;

namespace
{
    MOCK_FUNCTION( NotifyCallFromScript, 1, void( const std::string& ), NotifyCallFromScript );
    MOCK_FUNCTION( NotifyMissionCallFromScript, 1, void( boost::shared_ptr< MIL_Mission_ABC > ), NotifyMissionCallFromScript );
    boost::shared_ptr< MIL_Mission_ABC > GetRawMission( DEC_TestPopulationDecision* pAgent );
}

class DEC_TestPopulationDecision : public DEC_Decision< MIL_Population >
{
public:
    DEC_TestPopulationDecision( MIL_Population& population, DEC_TestPopulationDecision* pOther )
        : DEC_Decision( population, 100, 100, false )
        , pOther_( pOther )
    {
        const DEC_Model_ABC& model = population.GetType().GetModel();
        SetModel( model );
        StartDefaultBehavior();
    }
    void SetModel( const DEC_Model_ABC& model )
    {
        InitBrain( model.GetScriptFile(), model.GetName(), model.GetIncludePath(), "stubPopulation", false, false, model.GetIntegrationDir() );
    }
    void StartMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
    {
        const std::string& strBehavior = mission->GetType().GetDIABehavior();
        ActivateOrder( strBehavior, mission );
    }
    virtual DEC_AutomateDecision* GetDecAutomate() { return 0; }
    virtual std::string GetName() const { return "Test Decision"; }
    virtual void EndCleanStateAfterCrash() {}
    virtual std::string GetGroupName() { return std::string(); };
    virtual MIL_OrderManager_ABC& GetOrderManager() const { throw MASA_EXCEPTION_NOT_IMPLEMENTED; };
    virtual void RegisterSelf( sword::Brain& brain )
    {
        brain.GetScriptRef( "myself" ) = this;
    }
    virtual void RegisterSpecific( sword::Brain& /*brain*/, bool /*isMasalife*/, const std::string& /*groupName*/ ) {}

    void UsedByDIA() {}
    void ReleasedByDIA () {}

protected:
    virtual void RegisterUserFunctions( sword::Brain& brain )
    {
        brain.RegisterFunction( "DEC_TestBehaviorCalled", &NotifyCallFromScript );
        brain.RegisterFunction( "DEC_TestMissionCalled", &NotifyMissionCallFromScript );
        brain.RegisterFunction( "DEC_GetRawMission", &GetRawMission );
        directia::tools::binders::ScriptRef initFunction = brain.GetScriptRef( "InitTaskParameter" );
        bool isMasalife = false;
        brain.RegisterFunction( "DEC_FillMissionParameters",
            boost::function< void( const directia::tools::binders::ScriptRef&, boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_MiscFunctions::FillMissionParameters, boost::ref( brain ), boost::ref( initFunction ), _1 , _2, isMasalife ) ) );
        if( pOther_ )
            brain.GetScriptRef( "other" ) = pOther_;
    }
    virtual void RegisterUserArchetypeFunctions( sword::Brain& ) {}
    virtual void PostStartMission() {}
    virtual void PostStopMission() {}

private:
    DEC_TestPopulationDecision* pOther_;
};

namespace
{
    boost::shared_ptr< MIL_Mission_ABC > GetRawMission( DEC_TestPopulationDecision* pAgent )
    {
        return pAgent->GetMission();
    }
    class Pion
    {
    public:
        Pion()
            : xis       ( "<main dia-type='PionTest' file='PionTest.bms'/>" )
            , model     ( "test", xis >> xml::start( "main" ), testOptions.GetDataPath( "." ), missionTypes, false, testOptions.GetDataPath( "resources" ) )
            , type      ( model )
            , controller( actions )
            , population( type, controller, army )
            , decision  ( population, 0 )
        {
            // NOTHING
        }
        xml::xistringstream xis;
        std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
        DEC_Model model;
        ActionManager actions;
        MissionController controller;
        MockArmy army;
        StubMIL_PopulationType type;
        StubMIL_Population population;
        DEC_TestPopulationDecision decision;
        MockMIL_MissionType_ABC missionType;
    };
}

// -----------------------------------------------------------------------------
// Name: ActivateOrderExecutesBehaviour
// Created: LDC 2009-04-27
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ActivateOrderExecutesBehaviour, Pion )
{
    MOCK_EXPECT( missionType.GetDIABehavior ).once().returns( "missionBehavior" );
    StubDEC_KnowledgeResolver_ABC resolver;
    boost::shared_ptr< MIL_Mission_ABC > mission ( new StubMIL_Mission_ABC( missionType, resolver ) );
    decision.StartMissionBehavior( mission );
    mock::verify( missionType );
    MOCK_EXPECT( NotifyCallFromScript ).once().with( "missionBehavior called" );
    decision.UpdateDecision( 1.f );
}

// -----------------------------------------------------------------------------
// Name: ActivateOrderPassesMissionToDecisional
// Created: LDC 2009-04-29
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ActivateOrderPassesMissionToDecisional, Pion )
{
    MOCK_EXPECT( missionType.GetDIABehavior ).once().returns( "missionBehaviorWithArg" );
    StubDEC_KnowledgeResolver_ABC resolver;
    boost::shared_ptr< MIL_Mission_ABC > mission ( new StubMIL_Mission_ABC( missionType, resolver ) );
    decision.StartMissionBehavior( mission );
    mock::verify( missionType );
    MOCK_EXPECT( NotifyMissionCallFromScript ).once().with( mission );
    decision.UpdateDecision( 1.f );
}

// -----------------------------------------------------------------------------
// Name: DecisionalCanUseMissionParameters
// Created: LDC 2009-04-29
// Tests that if a MIL_Mission_ABC is passed to lua through a function,
// its parameters can be accessed from lua
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( DecisionalCanUseMissionParameters, Pion )
{
    MOCK_EXPECT( missionType.GetDIABehavior ).once().returns( "missionBehavior" );
    StubDEC_KnowledgeResolver_ABC resolver;
    std::string missionParameter( "param" );
    boost::shared_ptr< MIL_Mission_ABC > mission ( new StubMIL_Mission_ABC( missionType, resolver, missionParameter ) );
    decision.StartMissionBehavior( mission );
    mock::verify( missionType );
    MOCK_EXPECT( NotifyCallFromScript ).once().with( "missionBehavior called" );
    decision.UpdateDecision( 1.f );
}

namespace
{
    class Mission
    {
    public:
        Mission()
            : xis       ( "<main dia-type='PionTest' file='MissionParamTestBrain.bms'/>" )
            , model     ( "test", xis >> xml::start( "main" ), testOptions.GetDataPath( "." ), missionTypes, false, testOptions.GetDataPath( "resources" ) )
            , controller( actions )
            , type      ( model )
            , population( type, controller, army )
            , decision  ( population, 0 )
        {
            // NOTHING
        }
        xml::xistringstream xis;
        std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
        DEC_Model model;
        ActionManager actions;
        MissionController controller;
        MockArmy army;
        StubMIL_PopulationType type;
        StubMIL_Population population;
        DEC_TestPopulationDecision decision;
        MockMIL_MissionType_ABC missionType;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_GetMissionPassesParametersToLua
// Created: LDC 2009-04-30
// Tests that after a call to DEC_GetMission( pion/popu/automat ), the retrieved
// lua object parameters can be accessed from lua
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( DEC_GetMissionPassesParametersToLua, Mission )
{
    MOCK_EXPECT( missionType.GetDIABehavior ).once().returns( "testDEC_GetMission" );
    StubDEC_KnowledgeResolver_ABC resolver;
    std::string missionParameter( "parameterValue" );
    boost::shared_ptr< MIL_Mission_ABC > mission ( new StubMIL_Mission_ABC( missionType, resolver, missionParameter ) );
    decision.StartMissionBehavior( mission );
    mock::verify( missionType );
    MOCK_EXPECT( NotifyCallFromScript ).once().with( missionParameter );
    decision.UpdateDecision( 1.f );
}

// -----------------------------------------------------------------------------
// Name: DEC_GetMissionPassesParametersToLuaMission
// Created: LDC 2009-04-30
// Tests that after a call to DEC_GetMission( pion/popu/automat ), the retrieved
// lua object parameters can be accessed from lua
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( DEC_GetMissionPassesParametersToLuaMission, Mission )
{
    MOCK_EXPECT( missionType.GetDIABehavior ).once().returns( "mission" );
    StubDEC_KnowledgeResolver_ABC resolver;
    std::string missionParameter( "parameterValue" );
    boost::shared_ptr< MIL_Mission_ABC > mission ( new StubMIL_Mission_ABC( missionType, resolver, missionParameter ) );
    decision.StartMissionBehavior( mission );
    mock::verify( missionType );
    MOCK_EXPECT( NotifyCallFromScript ).once().with( missionParameter );
    decision.UpdateDecision( 1.f );
}

// -----------------------------------------------------------------------------
// Name: DEC_GetMissionPassesOtherMissionsParametersToLua
// Created: LDC 2009-04-30
// Tests that after a call to DEC_GetMission( pion/popu/automat ), the retrieved
// lua object parameters can be accessed from lua even if the DEC_GetMission is not
// the current object's mission.
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( DEC_GetMissionPassesOtherMissionsParametersToLua, Mission )
{
    DEC_TestPopulationDecision otherDecision( population, 0 );
    DEC_TestPopulationDecision populationDecision( population, &otherDecision );
    MOCK_EXPECT( missionType.GetDIABehavior ).exactly( 2 ).returns( "testDEC_GetOtherMission" );
    StubDEC_KnowledgeResolver_ABC resolver;
    boost::shared_ptr< MIL_Mission_ABC > mission ( new StubMIL_Mission_ABC( missionType, resolver ) );
    std::string otherMissionParameter( "otherParameterValue" );
    boost::shared_ptr< MIL_Mission_ABC > otherMission ( new StubMIL_Mission_ABC( missionType, resolver, otherMissionParameter ) );
    otherDecision.StartMissionBehavior( otherMission );
    populationDecision.StartMissionBehavior( mission );
    mock::verify( missionType );
    MOCK_EXPECT( NotifyCallFromScript ).once().with( otherMissionParameter );
    populationDecision.UpdateDecision( 1.f );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision_GetterTest
// Created: LDC 2009-07-31
// Tests that a brain member variables can be set and retrieved from the C++
// (For compatibility when migrating from dia3 to dia4)
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( DEC_Decision_GetterTest, Mission )
{
    decision.SetVariable( "myself.AValue", 42 );
    BOOST_CHECK_EQUAL( 42, decision.GetVariable< int >( "myself.AValue" ) );
}

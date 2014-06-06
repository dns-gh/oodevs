#ifndef __StubDEC_Decision_h_
#define __StubDEC_Decision_h_

#include <tools/Exception.h>
#include "simulation_kernel/Decision/DEC_Decision.h"
#include "simulation_kernel/Decision/DEC_Model.h"

template< class T >
class StubDEC_Decision : public DEC_Decision< T >
{
public:
    StubDEC_Decision( T& entity )
        : DEC_Decision< T >( entity, 100, 100, false )
    {
        xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
        xis.start( "main" );
        std::map< std::string, const MIL_MissionType_ABC* > missionTypes;
        model_.reset( new DEC_Model( "test", xis, testOptions.GetDataPath( "." ), missionTypes, false, testOptions.GetDataPath( "resources" ) ) );
        SetModel( *model_ );
    }
    virtual ~StubDEC_Decision() {}

    void SetModel( const DEC_Model_ABC& model )
    {
        InitBrain( model.GetScriptFile(), model.GetName(), model.GetIncludePath(), "stubAutomate", false, false, model.GetIntegrationDir() );
    }
    virtual DEC_AutomateDecision* GetDecAutomate() { return 0; }
    virtual std::string GetName() const { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual std::string GetGroupName() { return std::string(); }
    virtual MIL_OrderManager_ABC& GetOrderManager() const { throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual void EndCleanStateAfterCrash() {}
    virtual void RegisterUserFunctions( sword::Brain& ) {}
    virtual void RegisterSelf( sword::Brain& /*brain*/ ) {}
    virtual void RegisterSpecific( sword::Brain& /*brain*/, bool /*isMasalife*/, const std::string& /*groupName */ ) {}

    template< typename FunctionType >
    void RegisterFunction( const char* const strFunctionName, FunctionType function )
    {
        DEC_Decision< T >::RegisterFunction( strFunctionName, function );
    }
    virtual void RegisterUserArchetypeFunctions ( sword::Brain& ) {}

protected:
    virtual void PostStartMission() {}
    virtual void PostStopMission() {}

private:
    std::unique_ptr< DEC_Model > model_;
};

#endif

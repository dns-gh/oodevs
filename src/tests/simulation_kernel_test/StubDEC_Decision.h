#ifndef __StubDEC_Decision_h_
#define __StubDEC_Decision_h_

#include "simulation_kernel/Decision/DEC_Decision.h"
#include "simulation_kernel/Decision/DEC_Model.h"

template< class T >
class StubDEC_Decision : public DEC_Decision< T >
{
public:
    StubDEC_Decision( T& entity, DEC_DataBase& database )
        : DEC_Decision< T >( entity, database )
    {
        xml::xistringstream xis( "<main dia-type='PionTest' file='PionTest.bms'/>" );
        xis.start( "main" );
        std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > missionTypes;
        model_.reset( new DEC_Model( "test", xis, BOOST_RESOLVE( "." ), "prefix", missionTypes ) );
        SetModel( *model_ );
    }
    virtual ~StubDEC_Decision() {}
    
    virtual DEC_AutomateDecision* GetDecAutomate() const { return 0; }
    virtual std::string GetName() const { throw; }
    virtual void EndCleanStateAfterCrash() {}
    virtual void RegisterUserFunctions( directia::Brain& brain ) {}
    virtual void RegisterSelf( directia::Brain& brain ) {}
    virtual void UpdateMeKnowledge( directia::Brain& brain ) {}
    template< typename FunctionType >
    void RegisterFunction( const std::string& strFunctionName, FunctionType function )
    {
        DEC_Decision< T >::RegisterFunction( strFunctionName, function );
    }
private:
    std::auto_ptr< DEC_Model > model_;
};

#endif
//*****************************************************************************
//
// $Created: FBD 03-07-31 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_TestManager.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 4 $
// $Workfile: MOS_TestManager.inl $
//
//*****************************************************************************

#include "MOS_AgentTest.h"


//-----------------------------------------------------------------------------
// Name: MOS_TestManager::CreateAgentTest
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
inline
MOS_AgentTest& MOS_TestManager::CreateAgentTest( MOS_Agent& agent )
{
    MOS_AgentTest& agentTest = *new MOS_AgentTest( agent );
    RegisterAgentTest( agentTest);
    return agentTest;
}


//-----------------------------------------------------------------------------
// Name: MOS_TestManager::DeleteAgentTest
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
inline
void MOS_TestManager::DeleteAgentTest( MOS_Agent& agent )
{
    for ( IT_AgentTestPtrSet itAgentTest = agentTestPtrSet_.begin() ; itAgentTest != agentTestPtrSet_.end() ; ++itAgentTest )
    {
        if ( &(**itAgentTest).GetAgent() == &agent )
        {
            DeleteAgentTest( **itAgentTest );
            return;
        }

    }

}


//-----------------------------------------------------------------------------
// Name: MOS_TestManager::DeleteAgentTest
// Created: FBD 03-08-18
//-----------------------------------------------------------------------------
inline
void MOS_TestManager::DeleteAgentTest( MOS_AgentTest& agentTest )
{
    UnregisterAgentTest( agentTest );
    delete &agentTest;
}

//-----------------------------------------------------------------------------
// Name: MOS_TestManager::RegisterAgentTest
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
inline
void MOS_TestManager::RegisterAgentTest( MOS_AgentTest& agentTest )
{
    bool bOk = agentTestPtrSet_.insert( &agentTest ).second;
    assert( bOk );

}



//-----------------------------------------------------------------------------
// Name: MOS_TestManager::UnregisterAgentTest
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
inline
void MOS_TestManager::UnregisterAgentTest( MOS_AgentTest& agentTest )
{
    IT_AgentTestPtrSet itAgent = agentTestPtrSet_.find( &agentTest );
    assert( itAgent != agentTestPtrSet_.end() );
    agentTestPtrSet_.erase( itAgent );
}


//-----------------------------------------------------------------------------
// Name: MOS_TestManager::SayMissionFinished
// Created: FBD 03-09-02
//-----------------------------------------------------------------------------
inline
void MOS_TestManager::SayMissionFinished( MOS_Agent& agent, uint nCurTime )
{
    for ( IT_AgentTestPtrSet itAgentTest = agentTestPtrSet_.begin() ; itAgentTest != agentTestPtrSet_.end() ; ++itAgentTest )
    {
        if ( &(**itAgentTest).GetAgent() == &agent )
        {
            (**itAgentTest).SayMissionFinished( nCurTime );
        }
    }
    
}





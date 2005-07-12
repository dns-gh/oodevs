//*****************************************************************************
//
// $Created: FBD 03-07-31 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_TestManager.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_TestManager.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_TestManager.h"

#ifndef MOS_USE_INLINE
#	include "MOS_TestManager.inl"
#endif

#include "MOS_AgentManager.h"
#include "MOS_App.h"
#include "MOS_Agent.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentEditor.h"
#include "MOS_AgentListView.h"

// $$$$ CBX - temp - until FlushBuffer() is corrected
class TmpOutputArchive : public MT_FlatBinaryOutputArchive
{
public:
    TmpOutputArchive() : MT_FlatBinaryOutputArchive() {}

    virtual ~TmpOutputArchive() {}

    virtual bool FlushBuffer() 
    { 
        if( !MT_FlatBinaryOutputArchive::FlushBuffer() )
            return false;

        fflush( *pOutputFile_ );
        return true;
    }

private:
    TmpOutputArchive( const TmpOutputArchive& );
    TmpOutputArchive& operator=( const TmpOutputArchive& );
};

TmpOutputArchive             MOS_TestManager::archiveOutput_;
MT_FlatBinaryInputArchive    MOS_TestManager::archiveInput_;

//-----------------------------------------------------------------------------
// Name: MOS_TestManager constructor
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
MOS_TestManager::MOS_TestManager()
:   bTest_              ( false )
,   bReplay_            ( false )
,   bRec_               ( false )
,   nMinimumTimeTest_   ( 0 )
,   sFileNameOut_        ( "Test.bin" )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_TestManager destructor
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
MOS_TestManager::~MOS_TestManager()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_TestManager::Initialize
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
void MOS_TestManager::Initialize()
{
    MOS_AgentManager::CT_AgentMap&  listAgent = MOS_App::GetApp().GetAgentManager().GetAgentList();
    for ( MOS_AgentManager::CIT_AgentMap itAgent = listAgent.begin() ; itAgent != listAgent.end() ; ++itAgent )
    {
        MOS_Agent& agent = *itAgent->second;
        CreateAgentTest( agent );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_TestManager::Terminate
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
void MOS_TestManager::Terminate()
{
    while ( agentTestPtrSet_.size() )
    {
        MOS_AgentTest& agentTest = **agentTestPtrSet_.begin();
        DeleteAgentTest( agentTest );
    }
    if ( bTest_ )
        SetFlagTest( false );

    if ( bReplay_ )
        SetFlagReplay( false );
}



//-----------------------------------------------------------------------------
// Name: MOS_TestManager::Update
// Created: FBD 03-09-16
//-----------------------------------------------------------------------------
void MOS_TestManager::Update( int nTime )
{
    if ( bTest_ && nTime )
    {
        RecordRandom( nTime );
    }
    else
    if( bReplay_ && nTime )
    {
        Replay( nTime );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_TestManager::RecordRandom
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
void MOS_TestManager::RecordRandom( int nTime )
{

    if (agentTestPtrSet_.size() )
    {
        if( nOldFrame_ == nTime )
            return;

        nOldFrame_ = nTime;

        archiveOutput_.WriteField( "StartFrame", std::string( "StartFrame" ) );
        archiveOutput_.WriteField( "nTime", nTime );

        for ( CIT_AgentTestPtrSet itAgentTest = agentTestPtrSet_.begin() ; itAgentTest != agentTestPtrSet_.end() ; ++itAgentTest )
        {
            MOS_AgentTest& agentTest = **itAgentTest;
            agentTest.Update( nTime, archiveOutput_, nMinimumTimeTest_ );
        }
        archiveOutput_.WriteField( "EndFrame", std::string( "EndFrame" ) );
        archiveOutput_.FlushBuffer();
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_TestManager::RecordManual
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
void MOS_TestManager::RecordManual( int nTime, MOS_Agent& agent, MOS_Mission_ABC& mission )
{
    if (agentTestPtrSet_.size() )
    {
        if( nOldFrame_ == nTime )
            return;

        nOldFrame_ = nTime;

        archiveOutput_.WriteField( "StartFrame", std::string( "StartFrame" ) );
        archiveOutput_.WriteField( "nTime", nTime );


        for ( CIT_AgentTestPtrSet itAgentTest = agentTestPtrSet_.begin() ; itAgentTest != agentTestPtrSet_.end() ; ++itAgentTest )
        {
            MOS_AgentTest& agentTest = **itAgentTest;
            if ( agentTest.GetAgent().GetAgentID() == agent.GetAgentID() )
            {
                agentTest.UpdateRec( nTime, archiveOutput_, mission );
                break;
            }
        }
        archiveOutput_.WriteField( "EndFrame", std::string( "EndFrame" ) );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_TestManager::ResetReplay
// Created: FBD 03-08-20
//-----------------------------------------------------------------------------
void MOS_TestManager::InitReplay()
{
    nNextId_ = 0;  
    nCurTime_ = 0;
    nTimeNextOrder_ = 0;
    nState_ = 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_TestManager::InitRecord
// Created: FBD 03-08-20
//-----------------------------------------------------------------------------
void MOS_TestManager::InitRecord()
{
    nOldFrame_ = 0;
}



//-----------------------------------------------------------------------------
// Name: MOS_TestManager::Update
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
void MOS_TestManager::Replay( int nTime )
{
    if ( agentTestPtrSet_.size() == 0 )
        return;

    if (  agentTestPtrSet_.size() != MOS_App::GetApp().GetAgentManager().GetAgentList().size() )
        return;


    if( nState_ == 255 )
        return;

    while( 1 )
    {
        // Si Etat Lire new frame
        if( nState_ == 0 )
        {
            std::string sName;
            archiveInput_.ReadField( "StartFrame", sName );
            if (  sName == "StartFrame" )
            {
                archiveInput_.ReadField( "nTime", nCurTime_ );
                nState_ = 1;
            }
            else
            {
                // Sinon etat fini
                nState_  = 255;
                return;

            }
        }

        // Si Etat Attente Synchro
        if ( nState_ == 1 )
        {
            if ( nCurTime_<= nTime  )
            {
                nState_ = 2;
            }
            else
                return;
        }

        // Si Etat Lire Agent
        if  ( nState_ == 2 )
        {
            std::string sName;
            archiveInput_.ReadField( "sName", sName );
            if( sName == "EndFrame" )
            {
                // Alors on Passe en Etat lire new frame
                nState_ = 0;
                return;
            }
            else
            {
                assert( sName == "MOS_AgentTest|nIdAgent" );
                archiveInput_.ReadField( "nIdAgent", nNextId_ );
            
                archiveInput_.ReadField( "sName", sName );
                assert( sName == "MOS_AgentTest|nTimeNextOrder" );
                archiveInput_.ReadField( "nTimeNextOrder", nTimeNextOrder_ );
        
                for ( CIT_AgentTestPtrSet itAgentTest = agentTestPtrSet_.begin() ; itAgentTest != agentTestPtrSet_.end() ; ++itAgentTest )
                {
                    MOS_AgentTest& agentTest = **itAgentTest;
                    if ( agentTest.GetAgent().GetAgentID() == nNextId_ )
                    {
//                        MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().ReplayAMission( agentTest, archiveInput_ );
                        break;
                    }
                }
            }
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_TestManager::SetFlagTest
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
void MOS_TestManager::SetFlagTest( bool bTest )
{
    if( !bTest_&& bTest )
    {
        archiveOutput_.OpenOutputFile( sFileNameOut_ );
        archiveOutput_.SetFlushSize( 100000 );
        MOS_App::GetApp().GetTestManager().InitRecord();
    }
    else
    if ( bTest_&& !bTest )
    {
        archiveOutput_.FlushBuffer();
        archiveOutput_.CloseOutputFile();
    }
    bTest_ = bTest;
}


//-----------------------------------------------------------------------------
// Name: MOS_TestManager::GetFlagTest
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
bool MOS_TestManager::GetFlagTest() const
{
    return bTest_;   
}



//-----------------------------------------------------------------------------
// Name: MOS_TestManager::SetFlagReplay
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
void MOS_TestManager::SetFlagReplay( bool bReplay )
{
    if( !bReplay_&& bReplay )
    {
        archiveInput_.Open( sFileNameOut_ );
        InitReplay();
    }
    else
    if ( bReplay_&& !bReplay )
    {
    }
    
    bReplay_ = bReplay;
}




//-----------------------------------------------------------------------------
// Name: MOS_TestManager::SetFlagReplay
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
void MOS_TestManager::SetFlagRec( bool bRec )
{
    if( !bRec_&& bRec )
    {
        archiveOutput_.OpenOutputFile( "Test.bin" );
        archiveOutput_.SetFlushSize( 100000 );
        InitRecord();

    }
    else
    if ( bRec_ && !bRec )
    {
        archiveOutput_.FlushBuffer();
        archiveOutput_.CloseOutputFile();
    }
    
    bRec_ = bRec;
}



//-----------------------------------------------------------------------------
// Name: MOS_TestManager::GetFlagRec
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
bool MOS_TestManager::GetFlagRec() const
{
    return bRec_;   
}



//-----------------------------------------------------------------------------
// Name: MOS_TestManager::GetRecArchive
// Created: FBD 03-09-15
//-----------------------------------------------------------------------------
MT_FlatBinaryOutputArchive& MOS_TestManager::GetRecArchive()
{
    return archiveOutput_;
}

//-----------------------------------------------------------------------------
// Name: MOS_TestManager::GetFlagReplay
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
bool MOS_TestManager::GetFlagReplay() const
{
    return bRec_;   
}


//-----------------------------------------------------------------------------
// Name: MOS_TestManager::SetMinimumTimeTest
// Created: FBD 03-09-16
//-----------------------------------------------------------------------------
void MOS_TestManager::SetMinimumTimeTest( int nTime )
{
    nMinimumTimeTest_ = nTime;
}



//-----------------------------------------------------------------------------
// Name: std::string 
// Created: FBD 03-09-16
//-----------------------------------------------------------------------------
std::string MOS_TestManager::GetFileNameOut() const
{
    return sFileNameOut_;
}


//-----------------------------------------------------------------------------
// Name: MOS_TestManager::SetNameOutDir
// Created: FBD 03-09-16
//-----------------------------------------------------------------------------
void MOS_TestManager::SetFileNameOut( std::string& sName )
{
    sFileNameOut_ = sName;
}


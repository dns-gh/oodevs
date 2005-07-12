//*****************************************************************************
//
// $Created: FBD 03-07-31 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentTest.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 5 $
// $Workfile: MOS_AgentTest.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_AgentTest.h"

#ifndef MOS_USE_INLINE
#	include "MOS_AgentTest.inl"
#endif

#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentEditor.h"
#include "MOS_AgentListView.h"

//-----------------------------------------------------------------------------
// Name: MOS_AgentTest constructor
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
MOS_AgentTest::MOS_AgentTest( MOS_Agent& agent )
:   agent_( agent )
,   nTimeNextOrder_( 0 )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentTest destructor
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
MOS_AgentTest::~MOS_AgentTest()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentTest::Update
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------

void MOS_AgentTest::Update( int nCurtime, MT_OutputArchive_ABC& archive, int nMinimumTime )
{
    if ( nTimeNextOrder_ > nCurtime )
        return;

    nTimeNextOrder_ = nCurtime + nMinimumTime + ( rand() % TIMEMAXINTERVAL ); // Toutes les 60 secondes

    archive.WriteField( "sName", std::string( "MOS_AgentTest|nIdAgent" ) );
    archive.WriteField( "nIdAgent", agent_.GetAgentID() );

    archive.WriteField( "sName", std::string( "MOS_AgentTest|nTimeNextOrder" ) );
    archive.WriteField( "nTimeNextOrder", nTimeNextOrder_ );
    
//    MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().GiveAMission( *this, archive );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentTest::Update
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------

void MOS_AgentTest::UpdateRec( int nCurtime, MT_OutputArchive_ABC& archive, MOS_Mission_ABC& mission )
{

    nTimeNextOrder_ = nCurtime + 1;

    archive.WriteField( "sName", std::string( "MOS_AgentTest|nIdAgent" ) );
    archive.WriteField( "nIdAgent", agent_.GetAgentID() );

    archive.WriteField( "sName", std::string( "MOS_AgentTest|nTimeNextOrder" ) );
    archive.WriteField( "nTimeNextOrder", nTimeNextOrder_ );

    
//    MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().GiveThisMission( *this, archive, mission );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentTest::SayMissionFinished
// Created: FBD 03-09-02
//-----------------------------------------------------------------------------
void MOS_AgentTest::SayMissionFinished( uint nCurTime )
{
    nTimeNextOrder_ = nCurTime;
}


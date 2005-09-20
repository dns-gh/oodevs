//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MOSServer.cpp $
// $Author: Ape $
// $Modtime: 6/09/04 16:03 $
// $Revision: 3 $
// $Workfile: MOS_MOSServer.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_MOSServer.h"
#include "MOS_AgentServerController.h"
#include "MOS_AgentServerConnectionMgr.h"

#include "DIN/DIN_Statistics.h"

//-----------------------------------------------------------------------------
// Name: MOS_MOSServer constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
MOS_MOSServer::MOS_MOSServer()
    : dinEngine_()
{
    pASController_ = new MOS_AgentServerController( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_MOSServer destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
MOS_MOSServer::~MOS_MOSServer()
{
    delete pASController_;
}



//=============================================================================
// MAIN METHODS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_MOSServer::Connect
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void MOS_MOSServer::Connect( const std::string& strHostName, uint16 nPort )
{
    pASController_->GetConnectionMgr().ConnectToServer( strHostName, nPort);
   
}


//-----------------------------------------------------------------------------
// Name: MOS_MOSServer::Disconnect
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_MOSServer::Disconnect()
{
    pASController_ ->GetConnectionMgr().DisconnectFromServer();   
}


//-----------------------------------------------------------------------------
// Name: MOS_MOSServer::Update
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void MOS_MOSServer::Update()
{
    dinEngine_.Update();
//APE    MOS_App::GetApp().GetMainWindow().NotifyNetStatistics( dinEngine_.GetStatistics().GetReceivedBytes(), dinEngine_.GetStatistics().GetSentBytes() );
}


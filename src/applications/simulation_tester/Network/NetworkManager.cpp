// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "NetworkManager.h"

#include "ConnectionHandler.h"
#include "Messages/MessageManager.h"
#include "Workspace.h"

#include "DIN/DIN_EventManager.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: NetworkManager constructor
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
NetworkManager::NetworkManager( Workspace& workspace, const std::string& strHostName, uint nPort )
    : dinEngine_     ()
    , pLink_         ( 0 )
    , strServerHost_ ( strHostName )
    , nServerPort_   ( nPort )
{
    pConnectionHandler_ = new ConnectionHandler( *this );
	pMessageMgr_        = new MessageManager   ( workspace, *this );
}

//-----------------------------------------------------------------------------
// Name: NetworkManager destructor
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
NetworkManager::~NetworkManager()
{
    delete pConnectionHandler_;
    delete pMessageMgr_;
}


//=============================================================================
// MAIN METHODS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NetworkManager::Connect
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
void NetworkManager::Connect()
{
    pConnectionHandler_->ConnectToServer( strServerHost_, nServerPort_ );
}

//-----------------------------------------------------------------------------
// Name: NetworkManager::Disconnect
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
void NetworkManager::Disconnect()
{
    pConnectionHandler_->DisconnectFromServer();   
}

//-----------------------------------------------------------------------------
// Name: NetworkManager::Update
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
void NetworkManager::Update()
{
    dinEngine_.Update();
}

//-----------------------------------------------------------------------------
// Name: NetworkManager::BindLink
// Created: SBO 2005-05-12
//-----------------------------------------------------------------------------
void NetworkManager::BindLink( DIN::DIN_Link& link )
{
    pLink_ = &link;
}

//-----------------------------------------------------------------------------
// Name: NetworkManager::UnbindLink
// Created: SBO 2005-05-12
//-----------------------------------------------------------------------------
void NetworkManager::UnbindLink()
{
    pLink_ = 0;
}


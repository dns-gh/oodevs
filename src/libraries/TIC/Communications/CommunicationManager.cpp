// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/CommunicationManager.cpp $
// $Author: Sbo $
// $Modtime: 20/07/05 18:24 $
// $Revision: 8 $
// $Workfile: CommunicationManager.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"
#include "Types.h"

#include "CommunicationManager.h"
#include "Communication_CR.h"
#include "Communication_Intervisibility.h"
#include "MessageReport.h"

using namespace TIC;

//-----------------------------------------------------------------------------
// Name: CommunicationManager::CommunicationManager
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
CommunicationManager::CommunicationManager()
{
}

//-----------------------------------------------------------------------------
// Name: CommunicationManager::~CommunicationManager
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
CommunicationManager::~CommunicationManager()
{
}

//-----------------------------------------------------------------------------
// Name: CommunicationManager::Initialize
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
void CommunicationManager::Initialize()
{
    MessageReport    ::Initialize();
    CommunicationType::Initialize();
    CommunicationLink::Initialize();
}

//-----------------------------------------------------------------------------
// Name: CommunicationManager::Terminate
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
void CommunicationManager::Terminate()
{
    MessageReport    ::Terminate();
    CommunicationType::Terminate();
    CommunicationLink::Terminate();
}

//-----------------------------------------------------------------------------
// Name: CommunicationManager::OnReceiveMsgUnitInterVisibility
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
void CommunicationManager::OnReceiveMsgUnitInterVisibility( DIN::DIN_Input& input )
{
    Communication_Intervisibility::OnReceiveUnit( input );
}

//-----------------------------------------------------------------------------
// Name: CommunicationManager::OnReceiveMsgObjectInterVisibility
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
void CommunicationManager::OnReceiveMsgObjectInterVisibility( DIN::DIN_Input& input )
{
    Communication_Intervisibility::OnReceiveObject( input );
}

//-----------------------------------------------------------------------------
// Name: CommunicationManager::OnReceiveMsgFragOrder
// Created: SBO 2005-06-03
//-----------------------------------------------------------------------------
void CommunicationManager::OnReceiveMsgFragOrder( const ASN1T_MsgOrderConduiteAck& asnMsg )
{
    Communication_CR::OnReceiveFragOrder( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: CommunicationManager::OnReceiveMsgPawnOrder
// Created: SBO 2005-06-03
//-----------------------------------------------------------------------------
void CommunicationManager::OnReceiveMsgPawnOrder( const ASN1T_MsgPionOrder& asnMsg )
{
    Communication_CR::OnReceivePawnOrder( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: CommunicationManager::OnReceiveMsgPawnOrder
// Created: SBO 2005-06-03
//-----------------------------------------------------------------------------
void CommunicationManager::OnReceiveMsgCR( const ASN1T_MsgCR& asnMsg )
{
    Communication_CR::OnReceiveCR( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: CommunicationManager::GetCommunicationCount
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
void CommunicationManager::Flush()
{
    CommunicationLink::Flush();
}
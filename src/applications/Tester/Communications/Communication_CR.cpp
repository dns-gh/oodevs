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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/Communication_CR.cpp $
// $Author: Sbo $
// $Modtime: 20/07/05 18:02 $
// $Revision: 7 $
// $Workfile: Communication_CR.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"
#include "Types.h"

#include "Communication_CR.h"
#include "CommunicationManager.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"

using namespace TIC;


//-----------------------------------------------------------------------------
// Name: Communication_CR::Communication_CR
// Created: SBO 2005-06-28
//-----------------------------------------------------------------------------
Communication_CR::Communication_CR( CommunicationLink&            link, 
                                    T_EntityId                    nSubjectId, 
                                    const CommunicationType&      type, 
                                    CommunicationManager::ESource eSource )
    : Communication_ABC()
    , nSubjectId_      ( nSubjectId )
{
    pType_   = &type;
    eSource_ = eSource;
    nArity_  = 1;
    pLink_   = &link;
    // register communication to link
    pLink_->RegisterCommunication( eSource, *this );
    pLink_->OnCommunicationUpdated();
}

//-----------------------------------------------------------------------------
// Name: Communication_CR::~Communication_CR
// Created: SBO 2005-06-28
//-----------------------------------------------------------------------------
Communication_CR::~Communication_CR()
{
}

//-----------------------------------------------------------------------------
// Name: Communication_CR::OnReceiveFragOrder
// Created: SBO 2005-06-28
//-----------------------------------------------------------------------------
void Communication_CR::OnReceiveFragOrder( const ASN1T_MsgOrderConduiteAck& asnMsg )
{
    Pawn* pPawn = Pawn::Find( asnMsg.unit_id );
    assert( pPawn );

    CommunicationLink* pLink = CommunicationLink::Find( pPawn->GetAutomat(), *pPawn );
    if( pLink == 0 )
    {
        pLink = new CommunicationLink( pPawn->GetAutomat(), *pPawn );
        CommunicationLink::Register( *pLink );
    }

    const CommunicationType& comType = CommunicationType::Find( CommunicationType::eOrder, CommunicationType::eConduite );

    // create a communication
    new Communication_CR( *pLink, asnMsg.order_id, comType, CommunicationManager::eSourceAutomat );
}

//-----------------------------------------------------------------------------
// Name: Communication_CR::OnReceivePawnOrder
// Created: SBO 2005-06-28
//-----------------------------------------------------------------------------
void Communication_CR::OnReceivePawnOrder( const ASN1T_MsgPionOrder& asnMsg )
{
    Pawn* pPawn = Pawn::Find( asnMsg.oid_unite_executante );
    assert( pPawn );


    CommunicationLink* pLink = CommunicationLink::Find( pPawn->GetAutomat(), *pPawn );
    if( pLink == 0 )
    {
        pLink = new CommunicationLink( pPawn->GetAutomat(), *pPawn );
        CommunicationLink::Register( *pLink );
    }

    const CommunicationType& comType = CommunicationType::Find( CommunicationType::eOrder, CommunicationType::eMission );

    // create a communication
    new Communication_CR( *pLink, asnMsg.order_id, comType, CommunicationManager::eSourceAutomat );
}

//-----------------------------------------------------------------------------
// Name: Communication_CR::OnReceiveCR
// Created: SBO 2005-06-29
//-----------------------------------------------------------------------------
void Communication_CR::OnReceiveCR( const ASN1T_MsgCR& asnMsg )
{
    Pawn* pPawn = Pawn::Find( asnMsg.unit_id );
    assert( pPawn );

    CommunicationLink* pLink = CommunicationLink::Find( pPawn->GetAutomat(), *pPawn );
    if( pLink == 0 )
    {
        pLink = new CommunicationLink( pPawn->GetAutomat(), *pPawn );
        CommunicationLink::Register( *pLink );
    }

    const CommunicationType& comType = CommunicationType::Find( CommunicationType::eReport, asnMsg.cr.t );
    
    // create a communication
    new Communication_CR( *pLink, asnMsg.cr.t, comType, CommunicationManager::eSourcePawn );
}

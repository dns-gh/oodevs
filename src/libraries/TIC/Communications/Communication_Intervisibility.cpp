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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/Communication_Intervisibility.cpp $
// $Author: Sbo $
// $Modtime: 12/07/05 18:51 $
// $Revision: 6 $
// $Workfile: Communication_Intervisibility.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"
#include "Types.h"

#include "Communication_Intervisibility.h"
#include "CommunicationManager.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"
#include "Entities/Team.h"

using namespace TIC;

//-----------------------------------------------------------------------------
// Name: Communication_CR::Communication_CR
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
Communication_Intervisibility::Communication_Intervisibility( CommunicationLink&            link, 
                                                              T_EntityId                    nSubjectId, 
                                                              const CommunicationType&      type, 
                                                              CommunicationManager::ESource eSource )
    : Communication_ABC()
{
    pType_   = &type;
    eSource_ = eSource;
    nArity_  = 0;
    // insert subject in communication's map. 'true' => need to be exported
    subjects_.insert( std::make_pair( nSubjectId, true ) );

    pLink_ = &link;
    // register communication to link
    pLink_->RegisterCommunication( eSource, *this );
    pLink_->OnCommunicationUpdated();
}

//-----------------------------------------------------------------------------
// Name: Communication_Intervisibility::~Communication_Intervisibility
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
Communication_Intervisibility::~Communication_Intervisibility()
{
    subjects_.clear();
}

//-----------------------------------------------------------------------------
// Name: Communication_Intervisibility::OnReceiveUnit
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
void Communication_Intervisibility::OnReceiveUnit( DIN::DIN_Input& input )
{
    T_EntityId  nViewerId;
    T_EntityId  nSubjectId;
    uint8       nVisType;

    input >> nViewerId;
    input >> nSubjectId;
    input >> nVisType;

    Pawn* pViewerPawn = Pawn::Find( nViewerId );
    assert( pViewerPawn );
    
    Pawn* pSubjectPawn = Pawn::Find( nSubjectId );
    assert( pSubjectPawn );

    CommunicationType::ESubType   eSubType = CommunicationType::eNoSubType;
    CommunicationManager::ESource eSource  = CommunicationManager::eSourcePawn;

    switch( nVisType )
    {
        case eVisTypeRecognized:
        case eVisTypeIdentified:
            // check relation with observed pawn
            switch( pViewerPawn->GetAutomat().GetTeam().GetRelationWith( pSubjectPawn->GetAutomat().GetTeam() ) )
            {
                case Team::eEnemy:
                    eSubType = CommunicationType::eUnitEnemy;
                    break;
                case Team::eFriend:
                    // filter communication into the same automat
                    if( pViewerPawn->GetAutomat().GetId() == pSubjectPawn->GetAutomat().GetId() )
                        return;
                    eSubType = CommunicationType::eUnitFriend;
                    break;
                case Team::eNeutral:
                    eSubType = CommunicationType::eUnitNeutral;
                    break;
                case Team::eUnknown:
                    eSubType = CommunicationType::eUnitUnknown;
                    break;
                default:
                    assert( false );
            }
            break;
        case eVisTypeDetected:
            eSubType = CommunicationType::eUnitUnknown;
            break;
        case eVisTypeInvisible:
            eSubType = CommunicationType::eUnitLost;
            break;
        default:
            assert( false );
    }

    const CommunicationType& comType = CommunicationType::Find( CommunicationType::ePerception, eSubType );

    // Get Communication about this subject if exists
    CommunicationLink* pLink = CommunicationLink::Find( pViewerPawn->GetAutomat(), *pViewerPawn );
    if( pLink == 0 )
    {
        pLink = new CommunicationLink( pViewerPawn->GetAutomat(), *pViewerPawn );
        CommunicationLink::Register( *pLink );
    }

    Communication_Intervisibility* existingCom = 
            static_cast< Communication_Intervisibility* >( pLink->GetCommunicationBySubject( eSource, pSubjectPawn->GetId() ) );

    // if a communication matches subject remove previous subject
    if( existingCom )
        existingCom->subjects_.erase( pSubjectPawn->GetId() );

    // look for a communication matching type
    existingCom = static_cast< Communication_Intervisibility *>( pLink->GetCommunicationByType( eSource, comType ) );
    if( existingCom != 0 )
        existingCom->subjects_.insert( std::make_pair( pSubjectPawn->GetId(), true ) );
    else
        new Communication_Intervisibility( *pLink, pSubjectPawn->GetId(), comType, eSource );
    pLink->OnCommunicationUpdated();
}

//-----------------------------------------------------------------------------
// Name: Communication_Intervisibility::OnReceiveObject
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
void Communication_Intervisibility::OnReceiveObject( DIN::DIN_Input& input )
{
    T_EntityId  nViewerId;
    T_EntityId  nObjectId;
    uint8       nObjectVisType;

    input >> nViewerId;
    input >> nObjectId;
    input >> nObjectVisType;

    Pawn* pViewerPawn = Pawn::Find( nViewerId );
    assert( pViewerPawn );
    
    CommunicationType::ESubType   eSubType = CommunicationType::eNoSubType;
    CommunicationManager::ESource eSource  = CommunicationManager::eSourcePawn;

    switch( nObjectVisType )
    {
        case eVisTypeInvisible:
            eSubType = CommunicationType::eObjectLost;
            break;
        case eVisTypeRecognized:
        case eVisTypeDetected  :
        case eVisTypeIdentified:
            eSubType = CommunicationType::eObject;
            break;
        default:
            assert( false );
    }

    const CommunicationType& comType = CommunicationType::Find( CommunicationType::ePerception, eSubType );

    // Get Communication about this subject if exists
    CommunicationLink* pLink = CommunicationLink::Find( pViewerPawn->GetAutomat(), *pViewerPawn );
    if( pLink == 0 )
    {
        pLink = new CommunicationLink( pViewerPawn->GetAutomat(), *pViewerPawn );
        CommunicationLink::Register( *pLink );
    }

    Communication_Intervisibility* existingCom = 
        static_cast< Communication_Intervisibility * >( pLink->GetCommunicationBySubject( eSource, nObjectId ) );

    // if a communication matches subject remove previous subject
    if( existingCom )
        existingCom->subjects_.erase( nObjectId );

    // look for a communication matching type
    existingCom = static_cast< Communication_Intervisibility *>( pLink->GetCommunicationByType( eSource, comType ) );
    if( existingCom != 0 )
        existingCom->subjects_.insert( std::make_pair( nObjectId, true ) );
    else
        new Communication_Intervisibility( *pLink, nObjectId, comType, eSource );
    pLink->OnCommunicationUpdated();
}

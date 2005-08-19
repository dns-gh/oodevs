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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/CommunicationLink.cpp $
// $Author: Sbo $
// $Modtime: 12/07/05 18:53 $
// $Revision: 3 $
// $Workfile: CommunicationLink.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"
#include "Types.h"
#include "CommunicationLink.h"
#include "CommunicationManager.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"

using namespace TIC;

CommunicationLink::T_ComLinkSet CommunicationLink::links_;

//-----------------------------------------------------------------------------
// Name: CommunicationLink::CommunicationLink
// Created: SBO 2005-07-08
//-----------------------------------------------------------------------------
CommunicationLink::CommunicationLink( const Automat& automat, const Pawn& pawn )
    : pAutomat_ ( &automat )
    , pPawn_    ( &pawn )
    , bUpdated_ ( true )
    , coms_     ()
{
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::~CommunicationLink
// Created: SBO 2005-07-08
//-----------------------------------------------------------------------------
CommunicationLink::~CommunicationLink()
{
    for( CIT_ComSourceMap it = coms_.begin(); it != coms_.end(); ++it )
        delete it->second;
    coms_.clear();
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::Initialize
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
void CommunicationLink::Initialize()
{
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::Terminate
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
void CommunicationLink::Terminate()
{
    for( CIT_ComLinkSet it = links_.begin(); it != links_.end(); ++it )
        delete *it;
    links_.clear();
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::operator==
// Created: SBO 2005-07-08
//-----------------------------------------------------------------------------
bool CommunicationLink::operator==( const CommunicationLink& link ) const
{
    return pPawn_ == link.pPawn_ && pAutomat_ == link.pAutomat_;
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::Terminate
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
Communication_ABC* CommunicationLink::GetCommunicationBySubject( CommunicationManager::ESource eSource, 
                                                                 T_EntityId                    nSubjectId )
{
    for( std::pair< IT_ComSourceMap, IT_ComSourceMap > range = coms_.equal_range( eSource );
        range.first != coms_.end() && range.first != range.second;
        ++( range.first ) )
        if( range.first->second->IsSameSubject( nSubjectId ) )
            return range.first->second;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::Terminate
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
Communication_ABC* CommunicationLink::GetCommunicationByType( CommunicationManager::ESource eSource, 
                                                              const CommunicationType&      type  )
{
    for( std::pair< IT_ComSourceMap, IT_ComSourceMap > range = coms_.equal_range( eSource );
        range.first != coms_.end() && range.first != range.second;
        ++( range.first ) )
        if( range.first->second->IsSameType( type ) )
            return range.first->second;
    return 0;    
}
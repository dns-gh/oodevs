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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/CommunicationLink.inl $
// $Author: Sbo $
// $Modtime: 12/07/05 17:12 $
// $Revision: 2 $
// $Workfile: CommunicationLink.inl $
//
// *****************************************************************************

#include "Entities/Automat.h"
#include "Entities/Pawn.h"
#include "Communications/CommunicationManager.h"
#include "Communications/Communication_ABC.h"

namespace TIC
{

//-----------------------------------------------------------------------------
// Name: CommunicationLink::Register
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
void CommunicationLink::Register( CommunicationLink& link )
{
    bool bOut = links_.insert( &link ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::Flush
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
void CommunicationLink::Flush()
{
    for( CIT_ComLinkSet it = links_.begin(); it != links_.end(); ++it )
    {
        CommunicationLink& link = **it;
        link.bUpdated_ = false;
        for( IT_ComSourceMap itSrc = link.coms_.begin(); itSrc != link.coms_.end(); )
            if( itSrc->second->ResetStatus() )
            {
                delete itSrc->second;
                itSrc = link.coms_.erase( itSrc );
            }
            else
                ++itSrc;
    }
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::Find
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
inline
CommunicationLink* CommunicationLink::Find( const Automat& automat, const Pawn& pawn )
{
    CommunicationLink comTmp( automat, pawn );
    IT_ComLinkSet it = links_.find( &comTmp );
    if( it != links_.end() )
        return *it;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::GetCount
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
uint CommunicationLink::GetCount()
{
    uint nCount = 0;
    for( CIT_ComLinkSet it = links_.begin(); it != links_.end(); ++it )
        if( ( *it )->bUpdated_ )
            ++nCount;
    return nCount;
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::GetAt
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
const CommunicationLink* CommunicationLink::GetAt( uint nIdx )
{
    uint nCount = 0;
    for( CIT_ComLinkSet it = links_.begin(); it != links_.end(); ++it )
    {
        if( ( *it )->bUpdated_ )
        {
            if( nCount == nIdx )
                return *it;
            ++nCount;
        }
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Name: CommunicationLink::GetAutomatId
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
inline
uint CommunicationLink::GetAutomatId() const
{
    assert( pAutomat_ );
    return pAutomat_->GetId();
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::GetPawnId
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
inline
uint CommunicationLink::GetPawnId() const
{
    assert( pPawn_ );
    return pPawn_->GetId();
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::GetCommunicationCount
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
uint CommunicationLink::GetCommunicationCount( CommunicationManager::ESource eSource ) const
{
    uint nCount = 0;
    for( std::pair< CIT_ComSourceMap, CIT_ComSourceMap > range = coms_.equal_range( eSource );
        range.first != coms_.end() && range.first != range.second;
        ++range.first )
        if( range.first->second->GetArity() > 0 )
            ++nCount;
    return nCount;
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::GetCommunicationAt
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
const Communication_ABC* CommunicationLink::GetCommunicationAt( CommunicationManager::ESource eSource, uint nIdx ) const
{
    uint nCount = 0;
    for( std::pair< CIT_ComSourceMap, CIT_ComSourceMap > range = coms_.equal_range( eSource );
        range.first != coms_.end() && range.first != range.second;
        ++range.first )
        if( range.first->second->GetArity() > 0 )
        {
            if( nIdx == nCount )
                return range.first->second;
            ++nCount;
        }
    return 0;
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::Register
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
inline
void CommunicationLink::RegisterCommunication( CommunicationManager::ESource eSource, Communication_ABC& com )
{
    bool bOut = coms_.insert( std::make_pair( eSource, &com ) )->second ? true : false;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::OnCommunicationUpdated
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
inline
void CommunicationLink::OnCommunicationUpdated()
{
    bUpdated_ = true;
}

} // end namespace TIC
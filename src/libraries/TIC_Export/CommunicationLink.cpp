// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/CommunicationLink.cpp $
// $Author: Sbo $
// $Modtime: 12/07/05 9:50 $
// $Revision: 2 $
// $Workfile: CommunicationLink.cpp $
//
// *****************************************************************************

#include "TIC/Types.h"
#include "CommunicationLink.h"
#include "Communication.h"
#include "TIC/Communications/CommunicationLink.h"
#include "TIC/Communications/CommunicationManager.h"

namespace TICExport
{

//-----------------------------------------------------------------------------
// Name: CommunicationLink::CommunicationLink
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
CommunicationLink::CommunicationLink()
: pLink_ ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::CommunicationLink
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
CommunicationLink::CommunicationLink( uint nIdx )
{
    pLink_ = TIC::CommunicationLink::GetAt( nIdx );
    assert( pLink_ );
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::~CommunicationLink
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
CommunicationLink::~CommunicationLink()
{
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::GetAutomatId
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
uint CommunicationLink::GetAutomatId() const
{
    assert( pLink_ );
    return pLink_->GetAutomatId();
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::GetPawnId
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
uint CommunicationLink::GetPawnId() const
{
    assert( pLink_ );
    return pLink_->GetPawnId();
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::GetCommunicationCount
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
uint CommunicationLink::GetCommunicationCount( CommunicationLink::ESource eSource ) const
{
    return pLink_->GetCommunicationCount( static_cast< TIC::CommunicationManager::ESource >( eSource ) );
}

//-----------------------------------------------------------------------------
// Name: CommunicationLink::GetCommunicationAt
// Created: SBO 2005-07-11
//-----------------------------------------------------------------------------
const Communication CommunicationLink::GetCommunicationAt( CommunicationLink::ESource eSource, uint nIdx ) const
{
    const TIC::Communication_ABC* pCom = pLink_->GetCommunicationAt( static_cast< TIC::CommunicationManager::ESource >( eSource ), nIdx );
    assert( pCom );
    return Communication( *pCom );
}


} // namespace TICExport
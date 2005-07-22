// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-09 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/TicManager.inl $
// $Author: Sbo $
// $Modtime: 16/06/05 11:46 $
// $Revision: 11 $
// $Workfile: TicManager.inl $
//
// *****************************************************************************

#include <time.h>

namespace TIC
{

//-----------------------------------------------------------------------------
// Name: TicManager::GetTicManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
TicManager& TicManager::GetTicManager()
{
    assert( pTicManager_ );
	return *pTicManager_;
}

// -----------------------------------------------------------------------------
// Name: TicManager::GetCallback
// Created: NLD 2005-06-08
// -----------------------------------------------------------------------------
inline
TICExport::Callback_ABC& TicManager::GetCallback() const
{
    return callback_;
}

//-----------------------------------------------------------------------------
// Name: TicManager::GetServer
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
NetworkManager& TicManager::GetNetworkManager() const
{
	return *pNetworkManager_;
}

//-----------------------------------------------------------------------------
// Name: TicManager::GetEntityManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
EntityManager&	TicManager::GetEntityManager() const
{
	return *pEntityManager_;
}

//-----------------------------------------------------------------------------
// Name: TicManager::GetTime
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
inline
uint32	TicManager::GetTick() const
{
	return nTick_;
}

//-----------------------------------------------------------------------------
// Name: TicManager::SetTick
// Created: SBO 2005-05-30
//-----------------------------------------------------------------------------
inline
void	TicManager::SetTick( uint32 nTick )
{
    nTick_ = nTick;

    if( nCurrentSimTime_ == 0 )
        nCurrentSimTime_ = time( 0 );
    else
        nCurrentSimTime_ += nTickDuration_;
}

//-----------------------------------------------------------------------------
// Name: TicManager::SetTickDuration
// Created: SBO 2005-05-30
//-----------------------------------------------------------------------------
inline
void	TicManager::SetTickDuration( uint32 nTickDuration )
{
    nTickDuration_ = nTickDuration;
}

//-----------------------------------------------------------------------------
// Name: TicManager::SetTickDuration
// Created: SBO 2005-06-16
//-----------------------------------------------------------------------------
inline
uint32 TicManager::GetTickDuration() const
{
    return nTickDuration_;
}

//-----------------------------------------------------------------------------
// Name: TicManager::GetSimTime
// Created: SBO 2005-06-03
//-----------------------------------------------------------------------------
inline
uint32	TicManager::GetSimTime() const
{
    return nCurrentSimTime_;
}

} // end namespace TIC
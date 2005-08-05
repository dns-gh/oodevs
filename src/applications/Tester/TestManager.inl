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
// $Archive: /MVW_v10/Build/SDK/TIC/src/TestManager.inl $
// $Author: Sbo $
// $Modtime: 16/06/05 11:46 $
// $Revision: 11 $
// $Workfile: TestManager.inl $
//
// *****************************************************************************

#include <time.h>

namespace TEST
{

//-----------------------------------------------------------------------------
// Name: TestManager::GetTestManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
TestManager& TestManager::GetTestManager()
{
    assert( pTestManager_ );
	return *pTestManager_;
}

//-----------------------------------------------------------------------------
// Name: TestManager::GetNetworkManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
NetworkManager& TestManager::GetNetworkManager() const
{
	return *pNetworkManager_;
}

//-----------------------------------------------------------------------------
// Name: TestManager::GetEntityManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
EntityManager&	TestManager::GetEntityManager() const
{
	return *pEntityManager_;
}

// -----------------------------------------------------------------------------
// Name: TestManager::GetScheduler
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
inline
Scheduler& TestManager::GetScheduler() const
{
    return *pScheduler_;
}

// -----------------------------------------------------------------------------
// Name: TestManager::GetTestSet
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
inline
TestSet_ABC& TestManager::GetTestSet() const
{
    return *pTestSet_;
}

//-----------------------------------------------------------------------------
// Name: TestManager::GetTime
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
inline
uint32	TestManager::GetTick() const
{
	return nTick_;
}

//-----------------------------------------------------------------------------
// Name: TestManager::SetTick
// Created: SBO 2005-05-30
//-----------------------------------------------------------------------------
inline
void	TestManager::SetTick( uint32 nTick )
{
    nTick_ = nTick;

    if( nCurrentSimTime_ == 0 )
        nCurrentSimTime_ = time( 0 );
    else
        nCurrentSimTime_ += nTickDuration_;
}

//-----------------------------------------------------------------------------
// Name: TestManager::SetTickDuration
// Created: SBO 2005-05-30
//-----------------------------------------------------------------------------
inline
void	TestManager::SetTickDuration( uint32 nTickDuration )
{
    nTickDuration_ = nTickDuration;
}

//-----------------------------------------------------------------------------
// Name: TestManager::SetTickDuration
// Created: SBO 2005-06-16
//-----------------------------------------------------------------------------
inline
uint32 TestManager::GetTickDuration() const
{
    return nTickDuration_;
}

//-----------------------------------------------------------------------------
// Name: TestManager::GetSimTime
// Created: SBO 2005-06-03
//-----------------------------------------------------------------------------
inline
uint32	TestManager::GetSimTime() const
{
    return nCurrentSimTime_;
}

} // end namespace TEST
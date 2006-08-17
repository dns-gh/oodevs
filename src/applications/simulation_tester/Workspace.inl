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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Workspace.inl $
// $Author: Sbo $
// $Modtime: 16/06/05 11:46 $
// $Revision: 11 $
// $Workfile: Workspace.inl $
//
// *****************************************************************************

#include <time.h>

namespace TEST
{

//-----------------------------------------------------------------------------
// Name: Workspace::GetWorkspace
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
Workspace& Workspace::GetWorkspace()
{
    assert( pWorkspace_ );
	return *pWorkspace_;
}

//-----------------------------------------------------------------------------
// Name: Workspace::GetNetworkManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
NetworkManager& Workspace::GetNetworkManager() const
{
    assert( pNetworkManager_ );
	return *pNetworkManager_;
}

//-----------------------------------------------------------------------------
// Name: Workspace::GetEntityManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
EntityManager&	Workspace::GetEntityManager() const
{
    assert( pEntityManager_ );
	return *pEntityManager_;
}

// -----------------------------------------------------------------------------
// Name: Workspace::GetTypeManager
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
TypeManager& Workspace::GetTypeManager() const
{
    assert( pTypeManager_ );
    return *pTypeManager_;
}

// -----------------------------------------------------------------------------
// Name: Workspace::GetPositionManager
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
PositionManager& Workspace::GetPositionManager() const
{
    assert( pPositionManager_ );
    return *pPositionManager_;
}


// -----------------------------------------------------------------------------
// Name: workspace::GetTacticalLineManager
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
inline
TacticalLineManager& Workspace::GetTacticalLineManager() const
{
    assert( pTacticalLineManager_ );
    return *pTacticalLineManager_;
}

// -----------------------------------------------------------------------------
// Name: Workspace::GetScheduler
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
inline
Scheduler& Workspace::GetScheduler() const
{
    assert( pScheduler_ );
    return *pScheduler_;
}

// -----------------------------------------------------------------------------
// Name: Workspace::GetTestSet
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
inline
TestSet_ABC& Workspace::GetTestSet() const
{
    assert( pTestSet_ );
    return *pTestSet_;
}

//-----------------------------------------------------------------------------
// Name: Workspace::GetTime
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
inline
uint32	Workspace::GetTick() const
{
	return nTick_;
}

//-----------------------------------------------------------------------------
// Name: Workspace::SetTick
// Created: SBO 2005-05-30
//-----------------------------------------------------------------------------
inline
void	Workspace::SetTick( uint32 nTick )
{
    nTick_ = nTick;

    if( nCurrentSimTime_ == 0 )
        nCurrentSimTime_ = time( 0 );
    else
        nCurrentSimTime_ += nTickDuration_;
}

//-----------------------------------------------------------------------------
// Name: Workspace::SetTickDuration
// Created: SBO 2005-05-30
//-----------------------------------------------------------------------------
inline
void	Workspace::SetTickDuration( uint32 nTickDuration )
{
    nTickDuration_ = nTickDuration;
}

//-----------------------------------------------------------------------------
// Name: Workspace::SetTickDuration
// Created: SBO 2005-06-16
//-----------------------------------------------------------------------------
inline
uint32 Workspace::GetTickDuration() const
{
    return nTickDuration_;
}

//-----------------------------------------------------------------------------
// Name: Workspace::GetSimTime
// Created: SBO 2005-06-03
//-----------------------------------------------------------------------------
inline
uint32	Workspace::GetSimTime() const
{
    return nCurrentSimTime_;
}

} // end namespace TEST
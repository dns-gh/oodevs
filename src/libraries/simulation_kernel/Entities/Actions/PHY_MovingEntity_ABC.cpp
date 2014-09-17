// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MovingEntity_ABC.h"
#include "Decision/DEC_PathWalker.h"
#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
PHY_MovingEntity_ABC::PHY_MovingEntity_ABC()
    : pathWalker_( new DEC_PathWalker( *this ))
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC destructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
PHY_MovingEntity_ABC::~PHY_MovingEntity_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::Clean
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_MovingEntity_ABC::Clean()
{
    pathWalker_->Clean();
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::Move
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
int PHY_MovingEntity_ABC::Move( boost::shared_ptr< DEC_PathResult > pPath )
{
    return pathWalker_->Move( pPath );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::MoveSuspended
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_MovingEntity_ABC::MoveSuspended( boost::shared_ptr< DEC_PathResult > pPath )
{
    pathWalker_->MoveSuspended( pPath );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::MoveCanceled
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_MovingEntity_ABC::MoveCanceled( boost::shared_ptr< DEC_PathResult > pPath )
{
    pathWalker_->MoveCanceled( pPath );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::SerializeEnvironmentType
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void PHY_MovingEntity_ABC::SerializeEnvironmentType( sword::UnitEnvironmentType& msg ) const
{
    pathWalker_->SerializeEnvironmentType( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::SerializeCurrentPath
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void PHY_MovingEntity_ABC::SerializeCurrentPath( sword::Path& asn ) const
{
    pathWalker_->SerializeCurrentPath( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::ExtrapolatePosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Vector2D PHY_MovingEntity_ABC::ExtrapolatePosition( const MT_Vector2D& position, const double rSpeed, const double rTime, const bool bBoundOnPath ) const
{
    return pathWalker_->ExtrapolatePosition( position, rSpeed, rTime, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::IsMovingOn
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool PHY_MovingEntity_ABC::IsMovingOn( const DEC_Path_ABC& path ) const
{
    return pathWalker_->IsMovingOn( path );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::ComputeFutureObjectCollision
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool PHY_MovingEntity_ABC::ComputeFutureObjectCollision( const T_KnowledgeObjectVector& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject, const MIL_Agent_ABC& agent, bool blockedByObject, bool applyScale ) const
{
    return pathWalker_->ComputeFutureObjectCollision( GetPosition(), objectsToTest, rDistance, pObject, agent, blockedByObject, applyScale );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::HasCurrentPath
// Created: LGY 2013-07-18
// -----------------------------------------------------------------------------
bool PHY_MovingEntity_ABC::HasCurrentPath() const
{
    return pathWalker_->HasCurrentPath();
}

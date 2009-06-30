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

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
PHY_MovingEntity_ABC::PHY_MovingEntity_ABC()
    : pathWalker_( *this )
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
    pathWalker_.Clean();
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::Move
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
int PHY_MovingEntity_ABC::Move( DEC_PathResult& path )
{
    return pathWalker_.Move( path );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::MoveSuspended
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_MovingEntity_ABC::MoveSuspended( DEC_PathResult& path )
{
    pathWalker_.MoveSuspended( path );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::MoveCanceled
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_MovingEntity_ABC::MoveCanceled( DEC_PathResult& path )
{
    pathWalker_.MoveCanceled( path );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::SerializeEnvironmentType
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void PHY_MovingEntity_ABC::SerializeEnvironmentType( ASN1T_MsgUnitEnvironmentType& msg ) const
{
    pathWalker_.SerializeEnvironmentType( msg );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::SerializeCurrentPath
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool PHY_MovingEntity_ABC::SerializeCurrentPath( ASN1T_Path& asn ) const
{
    return pathWalker_.SerializeCurrentPath( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::ExtrapolatePosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Vector2D PHY_MovingEntity_ABC::ExtrapolatePosition( const MT_Vector2D& position, const MT_Float rSpeed, const MT_Float rTime, const bool bBoundOnPath ) const
{
    return pathWalker_.ExtrapolatePosition( position, rSpeed, rTime, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::IsMovingOn
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool PHY_MovingEntity_ABC::IsMovingOn( const DEC_Path_ABC& path ) const
{
    return pathWalker_.IsMovingOn( path );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::ComputeFutureObjectCollision
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool PHY_MovingEntity_ABC::ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, MT_Float& rDistance, const DEC_Knowledge_Object** pObject ) const
{
    return pathWalker_.ComputeFutureObjectCollision( vStartPos, objectsToTest, rDistance, pObject );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-30 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::Move
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
inline
int PHY_MovingEntity_ABC::Move( DEC_PathResult& path )
{
    return pathWalker_.Move( path );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::MoveSuspended
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
inline
void PHY_MovingEntity_ABC::MoveSuspended( DEC_PathResult& path )
{
    pathWalker_.MoveSuspended( path );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::MoveCanceled
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
inline
void PHY_MovingEntity_ABC::MoveCanceled( DEC_PathResult& path )
{
    pathWalker_.MoveCanceled( path );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::SerializeEnvironmentType
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void PHY_MovingEntity_ABC::SerializeEnvironmentType( DIN::DIN_BufferedMessage& msg ) const
{
    pathWalker_.SerializeEnvironmentType( msg );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::SerializeCurrentPath
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
bool PHY_MovingEntity_ABC::SerializeCurrentPath( ASN1T_Path& asn ) const
{
    return pathWalker_.SerializeCurrentPath( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::ExtrapolatePosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
MT_Vector2D PHY_MovingEntity_ABC::ExtrapolatePosition( const MT_Vector2D& position, const MT_Float rSpeed, const MT_Float rTime, const bool bBoundOnPath ) const
{
    return pathWalker_.ExtrapolatePosition( position, rSpeed, rTime, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::IsMovingOn
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
bool PHY_MovingEntity_ABC::IsMovingOn( const DEC_Path_ABC& path ) const
{
    return pathWalker_.IsMovingOn( path );
}

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::ComputeFutureObjectCollision
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
bool PHY_MovingEntity_ABC::ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, MT_Float& rDistance, const DEC_Knowledge_Object** pObject ) const
{
    return pathWalker_.ComputeFutureObjectCollision( vStartPos, objectsToTest, rDistance, pObject );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MovingEntity_ABC::serialize
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void PHY_MovingEntity_ABC::serialize( Archive& /*file*/, const uint )
{
    // NOTHING
}
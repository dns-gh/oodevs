// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: DEC_PathResult::GetResult
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
inline
const DEC_PathResult::T_PathPoints& DEC_PathResult::GetResult( bool useCheck ) const
{
    if( useCheck && GetState() == eComputing )
        MT_LOG_ERROR_MSG( "Path is being computed" );
    return resultList_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::GetPathType
// Created: CMA 2012-03-29
// -----------------------------------------------------------------------------
inline
const DEC_PathType& DEC_PathResult::GetPathType() const
{
    return pathType_;
}

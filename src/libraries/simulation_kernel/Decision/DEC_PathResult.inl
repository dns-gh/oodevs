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
const DEC_PathResult::T_PathPointList& DEC_PathResult::GetResult( bool useCheck ) const
{
    if( useCheck && GetState() == eComputing )
        throw std::runtime_error( "Path is being computed" );
    return resultList_;
}

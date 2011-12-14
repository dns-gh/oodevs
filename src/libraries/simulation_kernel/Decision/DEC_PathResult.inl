// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 Math�matiques Appliqu�es SA (MASA)
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
        MT_LOG_ERROR_MSG( "Path is being computed" );
    return resultList_;
}

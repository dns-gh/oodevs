// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/Path.cpp $
// $Author: Sbo $
// $Modtime: 19/07/05 11:45 $
// $Revision: 3 $
// $Workfile: Path.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"
#include "Types.h"
#include "Tools/Path.h"
#include "Tools/Position.h"

using namespace TIC;

//-----------------------------------------------------------------------------
// Name: Path::Path
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
Path::Path()
    : path_ ()
{
}

//-----------------------------------------------------------------------------
// Name: Path::~Path
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
Path::~Path()
{
    Clear();
}

//-----------------------------------------------------------------------------
// Name: Path::operator=
// Created: SBO 2005-07-19
//-----------------------------------------------------------------------------
Path& Path::operator=( const Path& path )
{
    Clear();
    for( CIT_PositionVector it = path.path_.begin(); it != path.path_.end(); ++it )
    {
        Position* pPos = new Position( ( *it )->GetMgrsCoordinate() );
        AddNode( *pPos );
    }
    return *this;
}

//-----------------------------------------------------------------------------
// ITERATORS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name: Path::const_reverse_iterator::const_reverse_iterator
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
Path::const_reverse_iterator::const_reverse_iterator( const Path& path, const Position& position )
    : cursor_     ( position )
    , itBegin_    ( path.path_.rbegin() )
    , itEnd_      ( path.path_.rend() )
{
}

//-----------------------------------------------------------------------------
// Name: Path::const_reverse_iterator::~const_reverse_iterator
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
Path::const_reverse_iterator::~const_reverse_iterator()
{
}

//-----------------------------------------------------------------------------
// Name: Path::const_reverse_iterator::operator+
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
Path::const_reverse_iterator& Path::const_reverse_iterator::operator+( double rDistance )
{
    // find previous and next nodes around cursor_
    CRIT_PositionVector itNext = itBegin_;
    ++itNext;

    uint i = 0;
    for( CRIT_PositionVector itPrev = itBegin_; itNext != itEnd_; ++itPrev, ++itNext, i++ )
        if( cursor_.IsOnSegment( **itPrev, **itNext ) )
            break;

    // check that we have found the segment containing current position
    if( itNext == itEnd_ )
        throw std::exception();

    double rDistanceToNext = 0.0;

    while( rDistance >= 0 )
    {
        rDistanceToNext = cursor_.GetDistanceTo( **itNext );
        if( rDistance > rDistanceToNext )
        {
            rDistance -= rDistanceToNext;
            cursor_.Set( ( *itNext )->X(), ( *itNext )->Y() );
            itNext++;
            if( itNext == itEnd_ )
                throw std::exception();
        }
        else
        {
            if( rDistanceToNext == 0 )
                return *this;
            double rRatio = rDistance / rDistanceToNext;
            cursor_.Set( ( ( *itNext )->X() - cursor_.X() ) * rRatio + cursor_.X()
                       , ( ( *itNext )->Y() - cursor_.Y() ) * rRatio + cursor_.Y() );
            assert( !_isnan( cursor_.X() ) );
            return *this;
        }
    }
    // moving after the end of the path
    throw std::exception();
}

//-----------------------------------------------------------------------------
// Name: Path::const_reverse_iterator::operator*
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
const Position& Path::const_reverse_iterator::operator*() const
{
    return cursor_;
}
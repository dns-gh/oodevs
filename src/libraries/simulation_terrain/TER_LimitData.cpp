// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_LimitData.h"
#include "TER_DynamicData.h"

// -----------------------------------------------------------------------------
// Name: TER_LimitData::DistanceData constructor
// Created: AGE 2005-05-12
// -----------------------------------------------------------------------------
TER_LimitData::DistanceData::DistanceData( const MT_Vector2D& from, const MT_Vector2D& to )
    : origin_       ( from )
    , direction_    ( to - from )
    , rSquareLength_( direction_.SquareMagnitude() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData::DistanceData::SquareDistance
// Created: AGE 2005-05-12
// -----------------------------------------------------------------------------
double TER_LimitData::DistanceData::SquareDistance( const MT_Vector2D& p ) const
{
    // $$$$ AGE 2007-07-04: buggued since 1992
    const MT_Vector2D v( p - origin_ );
    const double rDotProduct = DotProduct( direction_, v );
    if( rDotProduct <= 0 )
        return p.SquareDistance( origin_ );
    if( rDotProduct >= rSquareLength_ )
        return std::numeric_limits< double >::max();
    return v.SquareMagnitude() - rDotProduct * rDotProduct / rSquareLength_;
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData constructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
TER_LimitData::TER_LimitData( const T_PointVector& points )
{
    InitializeDistancesData( points );
    data_ = CreateAndRegisterDynamicData( points, "" );
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData destructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
TER_LimitData::~TER_LimitData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData::InitializeDistanceData
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
void TER_LimitData::InitializeDistancesData( const T_PointVector& points )
{
    distancesData_.clear();
    distancesData_.reserve( points.size() );
    CIT_PointVector itCur  = points.begin();
    CIT_PointVector itNext = itCur;
    ++ itNext;
    distancesData_.reserve( points.end() - itNext );
    for( ; itNext != points.end(); ++itCur, ++itNext )
        distancesData_.push_back( DistanceData( *itCur, *itNext ) );
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData::Distance
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
double TER_LimitData::SquareDistance( const MT_Vector2D& p ) const
{
    double rResult = std::numeric_limits< double >::max();
    for( auto it = distancesData_.begin(); it != distancesData_.end(); ++it )
    {
        const DistanceData& data = *it;
        const double rSquareDistance = data.SquareDistance( p );
        if( rResult > rSquareDistance )
            rResult = rSquareDistance;
    }
    return rResult;
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData::Distance
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
double TER_LimitData::Distance( const MT_Vector2D& p ) const
{
    return std::sqrt( SquareDistance( p ) );
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData::GetPoints
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
const T_PointVector& TER_LimitData::GetPoints() const
{
    return data_->GetPoints();
}

//-----------------------------------------------------------------------------
// Name: TER_LimitData::GetLength
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
double TER_LimitData::GetLength() const
{
    double rLength = 0.;
    const MT_Vector2D* pPrevPoint = 0;
    const T_PointVector& points = data_->GetPoints();
    for( CIT_PointVector itPoint = points.begin(); itPoint != points.end(); ++itPoint )
    {
        const MT_Vector2D& curPoint = *itPoint;

        if( pPrevPoint )
            rLength += pPrevPoint->Distance( curPoint );
        pPrevPoint = &curPoint;
    }
    return rLength;
}

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

double TER_LimitData::DistanceData::SquareLength() const
{
    return rSquareLength_;
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData::DistanceData::SquareDistance
// Created: AGE 2005-05-12
// -----------------------------------------------------------------------------
double TER_LimitData::DistanceData::SquareDistance( const MT_Vector2D& p, bool last ) const
{
    const MT_Vector2D v( p - origin_ );
    const double rDotProduct = DotProduct( direction_, v );
    if( rDotProduct <= 0 )
        return p.SquareDistance( origin_ );
    if( rDotProduct >= rSquareLength_ )
    {
        if( last )
            return p.SquareDistance( origin_ + direction_ );
        return std::numeric_limits< double >::max();
    }
    return v.SquareMagnitude() - rDotProduct * rDotProduct / rSquareLength_;
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData constructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
TER_LimitData::TER_LimitData( const T_PointVector& points )
{
    if( points.size() > 1 )
    {
        distancesData_.reserve( points.size() - 1 );
        for( size_t i = 0; i != points.size() - 1; ++i )
            distancesData_.push_back( DistanceData( points[i], points[i+1] ));
    }
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
// Name: TER_LimitData::Distance
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
double TER_LimitData::SquareDistance( const MT_Vector2D& p ) const
{
    double rResult = std::numeric_limits< double >::max();
    for( size_t i = 0; i != distancesData_.size(); ++i )
    {
        const double rSquareDistance = distancesData_[i].SquareDistance(
               p, i + 1 == distancesData_.size() );
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

const TER_LimitData::T_DistancesData& TER_LimitData::GetDistances() const
{
    return distancesData_;
}

//-----------------------------------------------------------------------------
// Name: TER_LimitData::GetLength
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
double TER_LimitData::GetLength() const
{
    double length = 0.;
    for( auto i = distancesData_.cbegin(); i != distancesData_.cend(); ++i )
        length += std::sqrt( i->SquareLength() );
    return length;
}

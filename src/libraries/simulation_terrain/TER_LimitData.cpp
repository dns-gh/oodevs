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

namespace
{

double SquareDistance( const MT_Vector2D& p, const MT_Vector2D& start,
        const MT_Vector2D& end, double squareLength, bool last )
{
    const MT_Vector2D v1 = end - start;
    const MT_Vector2D v2 = p - start;
    const double rDotProduct = DotProduct( v1, v2 );
    if( rDotProduct <= 0 )
        return p.SquareDistance( start );
    if( rDotProduct >= squareLength )
    {
        if( last )
            return p.SquareDistance( end );
        return std::numeric_limits< double >::max();
    }
    return v2.SquareMagnitude() - rDotProduct * rDotProduct / squareLength;
}

}  // namespace

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
        {
            const double d = (points[i+1] - points[i]).SquareMagnitude();
            distancesData_.push_back( d );
        }
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
    const T_PointVector& points = GetPoints();
    double rResult = std::numeric_limits< double >::max();
    for( size_t i = 0; i != distancesData_.size(); ++i )
    {
        const double rSquareDistance = ::SquareDistance(p, points[i], points[i+1],
                distancesData_[i], i + 1 == distancesData_.size() );
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

const std::vector< double >& TER_LimitData::GetDistances() const
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
        length += std::sqrt( *i );
    return length;
}

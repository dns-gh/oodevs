// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "TER_LimitData.h"
#include "Network/NET_ASN_Tools.h"
#include "Entities/MIL_Formation.h"
#include "Entities/MIL_EntityManager.h"
#include "MIL_TacticalLineManager.h"
#include "MIL_AgentServer.h"
#include "simulation_terrain/TER_PathFindManager.h"
#include "simulation_terrain/TER_AnalyzerManager.h"
#include "simulation_terrain/TER_DynamicData.h"

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
    : points_       ( points )
    , pPathFindData_( new TER_DynamicData( points_, TER_AnalyzerManager::DefaultTerrainData() ) )
    , distancesData_()
    , nNbRefs_      ( 0 )
{
    InitializeDistancesData();

    TER_PathFindManager::GetPathFindManager().AddDynamicData( *pPathFindData_ );
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData destructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
TER_LimitData::~TER_LimitData()
{
    assert( nNbRefs_ == 0 );
    // Asynchronous deletion of TER_DynamicData
    TER_PathFindManager::GetPathFindManager().RemoveDynamicData( *pPathFindData_ );
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData::InitializeDistanceData
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
void TER_LimitData::InitializeDistancesData()
{
    distancesData_.clear();
    distancesData_.reserve( points_.size() );
    CIT_PointVector itCur  = points_.begin();
    CIT_PointVector itNext = itCur;
    ++ itNext;
    distancesData_.reserve( points_.end() - itNext );
    for( ; itNext != points_.end(); ++itCur, ++itNext )
        distancesData_.push_back( DistanceData( *itCur, *itNext ) );
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData::Distance
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
double TER_LimitData::SquareDistance( const MT_Vector2D& p ) const
{
    double rResult = std::numeric_limits< double >::max();
    for( CIT_DistancesData it = distancesData_.begin(); it != distancesData_.end(); ++it )
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
// Name: TER_LimitData::AddRef
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void TER_LimitData::AddRef( const MIL_Fuseau& /*fuseau*/ ) const
{
    ++ nNbRefs_;
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData::DecRef
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void TER_LimitData::DecRef( const MIL_Fuseau& /*fuseau*/ ) const
{
    assert( nNbRefs_ > 0 );
    --nNbRefs_;
    if( nNbRefs_ == 0 )
        MIL_AgentServer::GetWorkspace().GetTacticalLineManager().DestroyLimitData( *this );
    // delete this ...
}

// -----------------------------------------------------------------------------
// Name: TER_LimitData::GetPoints
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
const T_PointVector& TER_LimitData::GetPoints() const
{
    return points_;
}

//-----------------------------------------------------------------------------
// Name: TER_LimitData::GetLength
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
double TER_LimitData::GetLength() const
{
    double rLength = 0.;
    const MT_Vector2D* pPrevPoint = 0;
    for( CIT_PointVector itPoint = points_.begin(); itPoint != points_.end(); ++itPoint )
    {
        const MT_Vector2D& curPoint = *itPoint;

        if( pPrevPoint )
            rLength += pPrevPoint->Distance( curPoint );
        pPrevPoint = &curPoint;
    }
    return rLength;
}

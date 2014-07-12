// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_Localisation.h"
#include "TER_World.h"
#include "MT_Tools/MT_Circle.h"
#include "MT_Tools/MT_Droite.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Ellipse.h"
#include "MT_Tools/MT_FormatString.h"
#include <xeumeuleu/xml.hpp>

const double TER_Localisation::rPrecision_ = 2.1242;

//-----------------------------------------------------------------------------
// Name: TER_Localisation constructor
// Created: NLD 2003-07-22
//-----------------------------------------------------------------------------
TER_Localisation::TER_Localisation()
    : nType_        ( eNone )
    , bWasCircle_   ( false )
    , rCircleRadius_( 0. )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation constructor
// Created: JVT 04-05-28
//-----------------------------------------------------------------------------
TER_Localisation::TER_Localisation( const TER_Localisation& localisation )
    : bWasCircle_   ( false )
    , rCircleRadius_( 0. )
{
    nType_       = localisation.GetType();
    pointVector_ = localisation.GetPoints();
    if( localisation.bWasCircle_ )
    {
        bWasCircle_ = true;
        rCircleRadius_ = localisation.rCircleRadius_;
        vCircleCenter_ = localisation.vCircleCenter_;
    }
    if( nType_ == ePolygon )
        polygon_ = localisation.polygon_;
    else if( nType_ == eLine )
        polyline_ = localisation.polyline_;
    else if( nType_ == ePoint )
        polygon_ = localisation.polygon_;
    boundingBox_ = localisation.boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation constructor
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
TER_Localisation::TER_Localisation( const TER_Polygon& polygon )
    : nType_        ( ePolygon )
    , polygon_      ( polygon )
    , pointVector_  ( polygon.GetBorderPoints() )
    , boundingBox_  ( polygon.GetBoundingBox() )
    , bWasCircle_   ( false )
    , rCircleRadius_( 0. )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation constructor
// Created: JVT 04-06-07
//-----------------------------------------------------------------------------
TER_Localisation::TER_Localisation( E_LocationType nType, const T_PointVector& pointVector )
    : nType_        ( nType )
    , pointVector_  ( pointVector )
    , bWasCircle_   ( false )
    , rCircleRadius_( 0. )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation constructor
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
TER_Localisation::TER_Localisation( const MT_Vector2D& vPos, double rRadius )
    : nType_        ( eCircle )
    , bWasCircle_   ( true )
    , vCircleCenter_( vPos )
    , rCircleRadius_( rRadius )
{
    pointVector_.reserve( 2 );
    pointVector_.push_back( vPos );
    pointVector_.push_back( vPos + MT_Vector2D( rRadius, 0. ) );
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation constructor
// Created: LDC 2013-02-13
// -----------------------------------------------------------------------------
TER_Localisation::TER_Localisation( const MT_Ellipse& ellipse )
    : nType_        ( ePolygon )
    , bWasCircle_   ( false )
    , rCircleRadius_( 0. )
{
    T_PointVector vector;
    const MT_Vector2D major = ellipse.GetMajorAxisHighPoint();
    const MT_Vector2D minor = ellipse.GetMinorAxisHighPoint();
    vector.push_back( MT_Vector2D( major.rX_ + minor.rX_ - ellipse.GetCenter().rX_, major.rY_ + minor.rY_ - ellipse.GetCenter().rY_ ) );
    vector.push_back( MT_Vector2D( major.rX_ - minor.rX_ + ellipse.GetCenter().rX_, major.rY_ - minor.rY_ + ellipse.GetCenter().rY_ ) );
    vector.push_back( MT_Vector2D( 3 * ellipse.GetCenter().rX_ - major.rX_ - minor.rX_, 3 * ellipse.GetCenter().rY_ - major.rY_ - minor.rY_ ) );
    vector.push_back( MT_Vector2D( ellipse.GetCenter().rX_ - major.rX_ + minor.rX_, ellipse.GetCenter().rY_ - major.rY_ + minor.rY_ ) );
    polygon_.Reset( vector );
    pointVector_ = polygon_.GetBorderPoints();
    InitializeBoundingBox( pointVector_ );
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation destructor
// Created: NLD 2003-07-22
//-----------------------------------------------------------------------------
TER_Localisation::~TER_Localisation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::InitializeBoundingBox
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
void TER_Localisation::InitializeBoundingBox( CT_PointVector& pointVector )
{
    // bounding box
    MT_Vector2D vDownLeft( std::numeric_limits< double >::max(), std::numeric_limits< double >::max() );
    MT_Vector2D vUpRight ( std::numeric_limits< double >::min(), std::numeric_limits< double >::min() );
    for( CIT_PointVector itPoint = pointVector.begin(); itPoint != pointVector.end(); ++itPoint )
    {
        const MT_Vector2D& vPos = *itPoint;
        if( vPos.rX_ < vDownLeft.rX_ )
            vDownLeft.rX_ = vPos.rX_;
        if( vPos.rY_ < vDownLeft.rY_ )
            vDownLeft.rY_ = vPos.rY_;
        if( vPos.rX_ > vUpRight.rX_ )
            vUpRight.rX_ = vPos.rX_;
        if( vPos.rY_ > vUpRight.rY_ )
            vUpRight.rY_ = vPos.rY_;
    }
    boundingBox_.Set( vDownLeft, vUpRight );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::InitializePolygon
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
bool TER_Localisation::InitializePolygon()
{
    for( IT_PointVector it = pointVector_.begin(); it != pointVector_.end(); ++it )
        TER_World::GetWorld().ClipPointInsideWorld( *it );
    // Make the polygon loop if it wasn't done
    if( !pointVector_.empty() && *pointVector_.begin() != *pointVector_.rbegin() )
        pointVector_.push_back( *pointVector_.begin() );
    polygon_.Reset( pointVector_ );
    if( polygon_.IsNull() )
        return false;
    InitializeBoundingBox( pointVector_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::InitializeLine
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
bool TER_Localisation::InitializeLine()
{
    if( pointVector_.size() < 2 )
        return false;
    polyline_ = pointVector_;
    InitializeBoundingBox( pointVector_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::InitializePoint
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
bool TER_Localisation::InitializePoint( double rRectSize )
{
    if( pointVector_.size() < 1 )
        return false;
    // Transformation du point en rectangle
    const MT_Vector2D& vPos = *pointVector_.begin();
    T_PointVector pointsTmp; pointsTmp.reserve( 5 );
    pointsTmp.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ - rRectSize, vPos.rY_ - rRectSize ) ));
    pointsTmp.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ + rRectSize, vPos.rY_ - rRectSize ) ));
    pointsTmp.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ + rRectSize, vPos.rY_ + rRectSize ) ));
    pointsTmp.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ - rRectSize, vPos.rY_ + rRectSize ) ));
    pointsTmp.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ - rRectSize, vPos.rY_ - rRectSize ) ));
    polygon_.Reset( pointsTmp );
    boundingBox_.Set( pointsTmp[0], pointsTmp[2] );
    return true;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::InitializeCircle
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
bool TER_Localisation::InitializeCircle()
{
    if( pointVector_.size() != 2 )
        return false;
    bWasCircle_    = true;
    vCircleCenter_ = pointVector_[0];
    rCircleRadius_ = ( vCircleCenter_.Distance( pointVector_[1] ) );
    T_PointVector pointsTmp; pointsTmp.reserve( 10 );
    for( double rAngle = 0; rAngle < ( MT_PI * 2 ); rAngle += (MT_PI/8) )
    {
        double rX_ = vCircleCenter_.rX_ + ( rCircleRadius_ * cos( rAngle ) );
        double rY_ = vCircleCenter_.rY_ + ( rCircleRadius_ * sin( rAngle ) );
        pointsTmp.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( rX_, rY_ ) ) );
    }
    pointsTmp.push_back( pointsTmp.front() );
    polygon_.Reset( pointsTmp );
    InitializeBoundingBox( pointsTmp );
    //$$$ test
    pointVector_ = pointsTmp;
    nType_ = ePolygon;
    return true;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::ComputeAngle
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
double ComputeAngle( const MT_Vector2D& v1Tmp, const MT_Vector2D& v2Tmp ) //$$$$$ a deplacer dans MT_Vector2D
{
    MT_Vector2D v1 = v1Tmp.Normalized();
    MT_Vector2D v2 = v2Tmp.Normalized();
    double rCosAngle = v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_; // (cos angle)
    double rSinAngle = v1.rX_ * v2.rY_ - v1.rY_ * v2.rX_; // (sin angle)
    double rAngle = acos(rCosAngle);
    if( rSinAngle >= 0.0 )
        return rAngle ;
    return -rAngle;
}

#define MT_ANGLE_POINT_INTERVAL   ( 30 * MT_PI / 180 ) // 30°

// -----------------------------------------------------------------------------
// Name: TER_Localisation::InitializeSector
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
bool TER_Localisation::InitializeSector()
{
    if( pointVector_.size() != 3 )
        return false;

    MT_Vector2D& vCenter  = pointVector_[0];
    double     rRadiusA = ( vCenter.Distance( pointVector_[1] ) );
    double     rRadiusB = ( vCenter.Distance( pointVector_[2] ) );
    MT_Vector2D  vRadiusA( pointVector_[1] - vCenter );
    MT_Vector2D  vRadiusB( pointVector_[2] - vCenter );

    double rSectorAngle = ComputeAngle( vRadiusA, vRadiusB );
    double rOriginAngle = ComputeAngle( vRadiusA, MT_Vector2D( 1, 0 ) );

    if( rSectorAngle < 0 )
        rSectorAngle = 2 * MT_PI + rSectorAngle;

    T_PointVector pointsTmp;
    pointsTmp.push_back( vCenter );

    double rAngleTmp = 0;
    while( rAngleTmp <= rSectorAngle )
    {
        MT_Vector2D vNewPoint( rRadiusA * cos( rAngleTmp ), rRadiusB * sin( rAngleTmp ) );
        vNewPoint.Rotate( rOriginAngle );
        vNewPoint += vCenter;
        TER_World::GetWorld().ClipPointInsideWorld( vNewPoint );
        pointsTmp.push_back( vNewPoint );

        if( rAngleTmp == rSectorAngle )
            break;

        rAngleTmp += MT_ANGLE_POINT_INTERVAL;
        if( rAngleTmp > rSectorAngle )
            rAngleTmp = rSectorAngle;
    }
    pointsTmp.push_back( vCenter );
    polygon_.Reset( pointsTmp );
    InitializeBoundingBox( pointsTmp );
    //$$$ test
    pointVector_ = pointsTmp;
    nType_ = ePolygon;
    return true;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::InitializeEllipse
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
bool TER_Localisation::InitializeEllipse()
{
    if( pointVector_.size() != 3 )
        return false;

    MT_Vector2D& vCenter  = pointVector_[0];
    double     rRadiusA = ( vCenter.Distance( pointVector_[1] ) );
    double     rRadiusB = ( vCenter.Distance( pointVector_[2] ) );
    MT_Vector2D  vRadiusA( pointVector_[1] - vCenter );
    double rOriginAngle = ComputeAngle( vRadiusA, MT_Vector2D( 1, 0 ) );

    T_PointVector pointsTmp;
    for( double rAngleTmp = 0; rAngleTmp < ( MT_PI * 2 ); rAngleTmp += MT_ANGLE_POINT_INTERVAL )
    {
        MT_Vector2D vNewPoint( rRadiusA * cos( rAngleTmp ), rRadiusB * sin( rAngleTmp ) );
        vNewPoint.Rotate( rOriginAngle );
        vNewPoint += vCenter;
        TER_World::GetWorld().ClipPointInsideWorld( vNewPoint );
        pointsTmp.push_back( vNewPoint );
    }
    pointsTmp.push_back( pointsTmp.front() );
    polygon_.Reset( pointsTmp );
    InitializeBoundingBox( pointsTmp );
    //$$$ test
    pointVector_ = pointsTmp;
    nType_ = ePolygon;
    return true;
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Initialize
// Created: NLD 2003-07-25
// Last modified: JVT 03-09-02
//-----------------------------------------------------------------------------
bool TER_Localisation::Initialize( double rPointSize )
{
    switch( nType_ )
    {
        case eCircle  : return InitializeCircle ();
        case eEllipse : return InitializeEllipse();
        case eLine    : return InitializeLine   ();
        case ePolygon : return InitializePolygon();
        case ePoint   : return InitializePoint  ( rPointSize );
        case eSector  : return InitializeSector ();
        case eNone:
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Reset
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
void TER_Localisation::Reset()
{
    if( nType_ != eLine )
        polygon_.Reset();
    pointVector_.clear();
    nType_ = eNone;
    boundingBox_.Reset();
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Reset
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
bool TER_Localisation::Reset( E_LocationType nType, const T_PointVector& pointVector, double rPointSize )
{
    Reset();
    nType_       = nType;
    pointVector_ = pointVector;
    return Initialize( rPointSize );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::Reset
// Created: JSR 2012-11-06
// -----------------------------------------------------------------------------
bool TER_Localisation::Reset( E_LocationType nType, const T_PointList& pointList, double rPointSize )
{
    Reset( pointList );
    nType_ = nType;
    return Initialize( rPointSize );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::Reset
// Created: NLD 2003-08-19
// -----------------------------------------------------------------------------
void TER_Localisation::Reset( const MT_Vector2D& vPos )
{
    Reset();
    nType_ = ePoint;
    pointVector_.clear();
    pointVector_.push_back( vPos );
    if( ! Initialize() )
        MT_LOG_ERROR_MSG( __FUNCTION__ << ": Initialization failed" );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::Reset
// Created: NLD 2003-08-21
// -----------------------------------------------------------------------------
bool TER_Localisation::Reset( const T_PointVector& pointVector )
{
    pointVector_ = pointVector;
    if( pointVector_.size() == 1 )
        nType_ = ePoint;
    else if( pointVector_.size() == 2 )
        nType_ = eLine;
    else if( pointVector_.size() > 2 )
        nType_ = ( *pointVector.begin() == pointVector[ pointVector.size() - 1 ] ) ? ePolygon : eLine;
    return Initialize();
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::Reset
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
bool TER_Localisation::Reset( const T_PointList& pointList )
{
    pointVector_.clear();
    pointVector_.reserve( pointList.size() );
    std::copy( pointList.begin(), pointList.end(), std::back_inserter( pointVector_ ) );
    return Reset( pointVector_ );
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Reset
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
void TER_Localisation::Reset( const TER_Localisation& localisation, double pointSize )
{
    nType_         = localisation.GetType        ();
    pointVector_   = localisation.GetPoints      ();
    bWasCircle_    = localisation.WasACircle     ();
    vCircleCenter_ = localisation.GetCircleCenter();
    rCircleRadius_ = localisation.GetCircleRadius();
    if( nType_ == ePoint )
    {
        if( !pointSize || pointSize*2 == localisation.boundingBox_.GetWidth() )
        {
            polygon_ = localisation.polygon_;
            boundingBox_ = localisation.boundingBox_;
        }
        else
        {
            MT_Vector2D point;
            if( localisation.GetType() == ePoint )
            {
                if( !localisation.GetPoints().empty() )
                    point = localisation.GetPoints()[0];
                else
                {
                    point = localisation.boundingBox_.GetCenter();
                    MT_LOG_ERROR_MSG( __FUNCTION__ << ": Reset point error: uninitialised point" );
                }
            }
            else
                point = localisation.boundingBox_.GetCenter();
            T_PointVector pointsTmp;
            pointsTmp.reserve( 5 );
            pointsTmp.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( point.rX_ - pointSize, point.rY_ - pointSize ) ));
            pointsTmp.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( point.rX_ + pointSize, point.rY_ - pointSize ) ));
            pointsTmp.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( point.rX_ + pointSize, point.rY_ + pointSize ) ));
            pointsTmp.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( point.rX_ - pointSize, point.rY_ + pointSize ) ));
            pointsTmp.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( point.rX_ - pointSize, point.rY_ - pointSize ) ));
            polygon_.Reset( pointsTmp );
            boundingBox_.Set( pointsTmp[0], pointsTmp[2] );
        }
    }
    else
    {
        if( nType_ == ePolygon )
            polygon_ = localisation.polygon_;
        else if( nType_ == eLine )
            polyline_ = localisation.polyline_;
        boundingBox_ = localisation.boundingBox_;
    }
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Read
// Created: NLD 2003-07-22
//-----------------------------------------------------------------------------
void TER_Localisation::Read( xml::xistream& xis )
{
    Reset();
    std::string strType;
    xis >> xml::start( "shape" )
            >> xml::attribute( "type", strType );
    nType_ = ConvertLocalisationType( strType );
    // Points
    xis     >> xml::start( "points" )
                >> xml::list( "point", *this, &TER_Localisation::ReadPoint )
            >> xml::end// points
        >> xml::end; // shape
    if( !Initialize() )
        throw MASA_EXCEPTION( "Invalid location" ); // $$$$ ABL 2007-07-09: error context
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::ReadPoint
// Created: ABL 2007-07-09
// -----------------------------------------------------------------------------
void TER_Localisation::ReadPoint( xml::xistream& xis )
{
    std::string strPoint;
    xis >> strPoint;
    MT_Vector2D vPoint;
    TER_World::GetWorld().MosToSimMgrsCoord( strPoint, vPoint );
    pointVector_.push_back( vPoint );
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Write
// Created: JVT 03-07-31
//-----------------------------------------------------------------------------
void TER_Localisation::Write( xml::xostream& xos ) const
{
    xos << xml::start( "shape" );
    if( bWasCircle_ )
    {
        xos << xml::attribute( "type", "cercle" )
            << xml::start( "points" );
        std::string strPoint;
        TER_World::GetWorld().SimToMosMgrsCoord( vCircleCenter_, strPoint );
        xos << xml::content( "point", strPoint );

        MT_Vector2D vDir( 0., 1. );
        TER_World::GetWorld().SimToMosMgrsCoord( vCircleCenter_ + vDir * rCircleRadius_, strPoint );
        xos << xml::content( "point", strPoint )
            << xml::end; // points
    }
    else
    {
        xos << xml::attribute( "type", ConvertLocalisationType( nType_ ) )
            << xml::start( "points" );
        std::string strPoint;
        for( auto it = pointVector_.begin(); it != pointVector_.end(); ++it )
        {
            TER_World::GetWorld().SimToMosMgrsCoord( *it, strPoint );
            xos << xml::content( "point", strPoint );
        }
        xos << xml::end; // points
    }
    xos << xml::end; // shape
}

std::string TER_Localisation::GetTypeString() const
{
    return TER_Localisation::ConvertLocalisationType( GetType() );
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::ConvertLocalisationType
// Created: NLD 2003-07-22
// Last modified: JVT 03-10-07
//-----------------------------------------------------------------------------
// static
TER_Localisation::E_LocationType TER_Localisation::ConvertLocalisationType( const std::string& strType )
{
    if( !::_strcmpi( strType.c_str(), "cercle" ) )
        return eCircle;
    else if( !::_strcmpi( strType.c_str(), "ellipse" ) )
        return eEllipse;
    else if( !::_strcmpi( strType.c_str(), "ligne" ) )
        return eLine;
    else if( !::_strcmpi( strType.c_str(), "polygone" ) )
        return ePolygon;
    else if( !::_strcmpi( strType.c_str(), "point" ) )
        return ePoint;
    else if( !::_strcmpi( strType.c_str(), "secteur" ) )
        return eSector;
    else
        throw MASA_EXCEPTION( "Invalid location type: '" + strType + "'" );
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: JVT 03-07-31
//-----------------------------------------------------------------------------
std::string TER_Localisation::ConvertLocalisationType( E_LocationType nType )         // A deplacer dans TER_Tools si nécessaire
{
    switch ( nType )
    {
        case eCircle  : return std::string( "cercle" );
        case eEllipse : return std::string( "ellipse" );
        case eLine    : return std::string( "ligne" );
        case ePolygon : return std::string( "polygone" );
        case ePoint   : return std::string( "point" );
        case eSector  : return std::string( "secteur" );
        default : break;
    }
    throw MASA_EXCEPTION( MT_FormatString( "Invalid location type: '%d'", nType ) );
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::ComputeNearestPoint
// Calcule la position de la localisation la plus proche de vSrc
// Created: NLD 2003-07-24
// Last modified: JVT 03-09-04
//-----------------------------------------------------------------------------
void TER_Localisation::ComputeNearestPoint( const MT_Vector2D& vSrc, MT_Vector2D& vResult ) const
{
    if( IsInside( vSrc ) )
        vResult = vSrc;
    else
        ComputeNearestOutsidePoint( vSrc, vResult );
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::ComputeNearestOutsidePoint
// Calcule la position sur le périmètre de la localisation la plus proche de vSrc
// Created: MGD 2011-01-20
//-----------------------------------------------------------------------------
void TER_Localisation::ComputeNearestOutsidePoint( const MT_Vector2D& vSrc, MT_Vector2D& vResult ) const
{
    if( pointVector_.size() == 1 )
    {
        vResult = pointVector_[ 0 ];
        return;
    }
    double rShortestDist   = std::numeric_limits< double >::max();
    CIT_PointVector itPoint  = pointVector_.begin();
    const MT_Vector2D* pPos1 = &*itPoint;
    for( ++itPoint; itPoint != pointVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pPos2 = &*itPoint;
        MT_Line lineTmp( *pPos1, *pPos2 );

        MT_Vector2D vResultTmp = lineTmp.ClosestPointOnLine( vSrc );
        double    rDistTmp   = vResultTmp.Distance( vSrc );
        if( rDistTmp < rShortestDist )
        {
            rShortestDist = rDistTmp;
            vResult       = vResultTmp;
        }
        pPos1 = pPos2;
    }
    assert( rShortestDist != std::numeric_limits< double >::max() );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::ComputeNearestPoint
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
void TER_Localisation::ComputeNearestPoint( const TER_Localisation& localisation, MT_Vector2D& vResult, double& rMinDistance ) const
{
    const T_PointVector& points = localisation.GetPoints();
    rMinDistance = std::numeric_limits< double >::max();
    for( auto it = points.begin(); it != points.end(); ++it )
    {
        MT_Vector2D nearestPointTmp;
        ComputeNearestPoint( *it, nearestPointTmp );
        double rDistance = it->Distance( nearestPointTmp );
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            vResult = *it;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetPointsClippedByPolygon
// Created: NLD 2003-08-20
// -----------------------------------------------------------------------------
void TER_Localisation::GetPointsClippedByPolygon( const TER_Polygon& polygon, T_PointVector& clippedPointVector ) const
{
    clippedPointVector.clear();
    CIT_PointVector itPoint = pointVector_.begin();
    const MT_Vector2D* pPrevPoint = &*itPoint;
    for( ++itPoint; itPoint != pointVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pCurPoint = &*itPoint;
        if( polygon.IsInside( *pPrevPoint, rPrecision_ ) )
            clippedPointVector.push_back( *pPrevPoint );
        // Intersection
        TER_DistanceLess cmp( *pPrevPoint );
        T_PointSet collisionSet( cmp );
        MT_Line lineTmp( *pPrevPoint, *pCurPoint );
        polygon.IntersectWithBorder( lineTmp, collisionSet, rPrecision_ );
        for( CIT_PointSet itStepPoint = collisionSet.begin(); itStepPoint != collisionSet.end(); ++itStepPoint )
        {
            if( polygon.IsInside( *itStepPoint, rPrecision_ ) )
                clippedPointVector.push_back( *itStepPoint );
        }
        pPrevPoint = pCurPoint;
    }
    if( polygon.IsInside( *pPrevPoint, rPrecision_ ) )
        clippedPointVector.push_back( *pPrevPoint );
    // Make clipped polygon looped
    if( nType_ == ePolygon && clippedPointVector.size() >= 2 && clippedPointVector[0] != clippedPointVector[clippedPointVector.size() - 1] )
        clippedPointVector.push_back( clippedPointVector[0] );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetPointsClippedByLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void TER_Localisation::GetPointsClippedByLocalisation( const TER_Localisation& localisation, T_PointVector& clippedPointVector ) const
{
    GetPointsClippedByPolygon( localisation.polygon_, clippedPointVector );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetPointsClippedBetweenTwoLines
// Created: NLD 2003-08-21
// -----------------------------------------------------------------------------
void TER_Localisation::GetPointsClippedBetweenTwoLines( const MT_Droite& leftDroite, const MT_Droite& rightDroite, const MT_Vector2D& vDroitesIntersection, T_PointVector& clippedPointVector ) const
{
    clippedPointVector.clear();
    CIT_PointVector itPoint = pointVector_.begin();
    const MT_Vector2D* pPrevPoint = &*itPoint;
    bool bPrevPointOnLeftDroite  = false;
    bool bPrevPointOnRightDroite = false;
    for( ++itPoint; itPoint != pointVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pCurPoint = &*itPoint;
        // Stockage de tous les points à ajouter dans le polygone clippé
        TER_DistanceLess cmp( *pPrevPoint );
        T_PointSet collisionSet( cmp );
        if( MT_IsPointBetweenTwoLines( leftDroite, rightDroite, *pPrevPoint ) )
            collisionSet.insert( *pPrevPoint );
        MT_Line lineTmp( *pPrevPoint, *pCurPoint );
        MT_Vector2D vPosIntersect;
        if( leftDroite.Intersect2D( lineTmp, vPosIntersect ) == eDoIntersect && MT_IsPointBetweenTwoLines( leftDroite, rightDroite, vPosIntersect ) )
            collisionSet.insert( vPosIntersect );
        if( rightDroite.Intersect2D( lineTmp, vPosIntersect ) == eDoIntersect && MT_IsPointBetweenTwoLines( leftDroite, rightDroite, vPosIntersect ) )
            collisionSet.insert( vPosIntersect );
        if( !collisionSet.empty() )
        {
            // Si on passe d'une intersection avec une droite vers une autre droite, (point précédent => 1er nouveau point)
            //  ajout de l'intersection des deux droites comme nouveau point du polygone clippé
            const MT_Vector2D& vFirstPoint = *collisionSet.begin();
            if( bPrevPointOnLeftDroite && rightDroite.IsInside( vFirstPoint ) )
                clippedPointVector.push_back( vDroitesIntersection );
            else if( bPrevPointOnRightDroite && leftDroite.IsInside( vFirstPoint ) )
                clippedPointVector.push_back( vDroitesIntersection );
            // Insertion des points
            std::copy( collisionSet.begin(), collisionSet.end(), std::back_inserter( clippedPointVector ) );
            // Sauvegarde état du dernier point (se trouve dans une des deux droites ?)
            const MT_Vector2D& vLastPoint = *collisionSet.rbegin();
            bPrevPointOnLeftDroite  = leftDroite .IsInside( vLastPoint );
            bPrevPointOnRightDroite = rightDroite.IsInside( vLastPoint );
        }
        pPrevPoint = pCurPoint;
    }
    if( !clippedPointVector.empty() )
    {
        // Gestion du bouclage (dernier point -> premier point)
        const MT_Vector2D& vLoopPoint = clippedPointVector[0];
        if( bPrevPointOnLeftDroite && rightDroite.IsInside( vLoopPoint ) )
            clippedPointVector.push_back( vDroitesIntersection );
        else if( bPrevPointOnRightDroite && leftDroite.IsInside( vLoopPoint ) )
            clippedPointVector.push_back( vDroitesIntersection );
        if( *clippedPointVector.begin() != *clippedPointVector.rbegin() )
            clippedPointVector.push_back( *clippedPointVector.begin() );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetPointsClippedBetweenTwoLines
// Created: JVT 2004-11-04
// -----------------------------------------------------------------------------
void TER_Localisation::GetPointsClippedBetweenTwoLines( const MT_Droite& leftDroite, const MT_Droite& rightDroite, T_PointVector& clippedPointVector ) const
{
    clippedPointVector.clear();
    if( pointVector_.empty() )
        return;
    CIT_PointVector itPoint = pointVector_.begin();
    const MT_Vector2D* pPrevPoint = &*itPoint;
    for( ++itPoint; itPoint != pointVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pCurPoint = &*itPoint;
        // Stockage de tous les points à ajouter dans le polygone clippé
        TER_DistanceLess cmp( *pPrevPoint );
        T_PointSet collisionSet( cmp );
        if( MT_IsPointBetweenTwoLines( leftDroite, rightDroite, *pPrevPoint ) )
            collisionSet.insert( *pPrevPoint );
        MT_Line lineTmp( *pPrevPoint, *pCurPoint );
        MT_Vector2D vPosIntersect;
        if( leftDroite.Intersect2D( lineTmp, vPosIntersect ) == eDoIntersect )
            collisionSet.insert( vPosIntersect );
        if( rightDroite.Intersect2D( lineTmp, vPosIntersect ) == eDoIntersect )
            collisionSet.insert( vPosIntersect );
        std::copy( collisionSet.begin(), collisionSet.end(), std::back_inserter( clippedPointVector ) );
        pPrevPoint = pCurPoint;
    }
    // bouclage (dernier point -> premier point)
    if( !clippedPointVector.empty() )
        clippedPointVector.push_back( *clippedPointVector.begin() );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::Split
// Created: NLD 2003-08-22
// -----------------------------------------------------------------------------
void TER_Localisation::Split( unsigned int nNbrParts, T_LocalisationPtrVector& locVector, const MT_Vector2D* splitDirection ) const
{
    if( nNbrParts == 0 )
        locVector.clear();
    else if( nNbrParts == 1 )
    {
        boost::shared_ptr< TER_Localisation > pLocalisation( new TER_Localisation( *this ) ); //$$$ RAM
        locVector.push_back( pLocalisation );
    }
    else if( nType_ == ePoint )
    {
        for( unsigned int i = 0; i < nNbrParts; ++i )
        {
            boost::shared_ptr< TER_Localisation > pLocalisation( new TER_Localisation() ); //$$$ RAM
            pLocalisation->Reset( *pointVector_.begin() );
            locVector.push_back( pLocalisation );
        }
    }
    else if( nType_ == eLine )
    {
        assert( false );
//        double rLineLength = polyline_.Magnitude();
//        for( unsigned int i = 0; i < nNbrParts; ++i )
//        {
//            TER_Localisation* pLocalisation = new TER_Localisation(); //$$$ RAM
//            pLocalisation->Reset( *pointVector_.begin() );
//        }
    }
    else if( nType_ == ePolygon )
    {
        // 1. Barycentre
        MT_Vector2D vBarycenter = MT_ComputeBarycenter( pointVector_ );
        assert( TER_World::GetWorld().IsValidPosition( vBarycenter ) );

        // 2. Stockage des droites de découpe
        typedef std::vector< MT_Droite > T_DroiteVector;
        T_DroiteVector droiteVector;
        MT_Vector2D vDir = splitDirection ? *splitDirection : MT_Vector2D( 0, 1 );
        double rAngle = 2. * MT_PI / nNbrParts;
        droiteVector.push_back( MT_Droite( vBarycenter, vDir + vBarycenter ) );
        for( unsigned int i = 0; i < nNbrParts; ++i )
        {
            vDir.Rotate( rAngle );
            droiteVector.push_back( MT_Droite( vBarycenter, vDir + vBarycenter ) );
        }
        // 3. Création des localisation découpées
        T_DroiteVector::const_iterator itDroite = droiteVector.begin();
        const MT_Droite* pPrevDroite = &*itDroite;
        for( ++itDroite; itDroite != droiteVector.end(); ++itDroite )
        {
            const MT_Droite* pCurDroite = &*itDroite;
            T_PointVector splitPolyPoints;
            GetPointsClippedBetweenTwoLines( *pPrevDroite, *pCurDroite, vBarycenter, splitPolyPoints );

            if( !splitPolyPoints.empty() )
            {
                boost::shared_ptr< TER_Localisation > pLocalisation( new TER_Localisation() ); //$$$ RAM
                pLocalisation->Reset( splitPolyPoints );
                locVector.push_back( pLocalisation );
            }
            pPrevDroite = pCurDroite;
        }
    }
    else
        assert( false );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::Convexify
// Created: NLD 2007-05-07
// -----------------------------------------------------------------------------
void TER_Localisation::Convexify()
{
    if( nType_ == ePolygon )
    {
        polygon_.Reset( pointVector_, true ); // 'true' is for generate 'convex hull'
        pointVector_ = polygon_.GetBorderPoints();
        InitializeBoundingBox( pointVector_ );
    }
}

namespace
{
    MT_Vector2D GetMidPoint( MT_Vector2D a, MT_Vector2D b )
    {
        return ( a + b ) / 2;
    }

    T_PointVector GetVectorWithAdditionalPoint( const T_PointVector& pointVector )
    {
        T_PointVector result;
        size_t pointVectorSize = pointVector.size();
        if( pointVectorSize > 0 )
        {
            for( size_t i = 0; i < pointVectorSize - 1; ++i )
            {
                result.push_back( pointVector[i] );
                result.push_back( GetMidPoint( pointVector[i], pointVector[i+1] ) );
            }
            result.push_back( pointVector.back() );
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::Scale
// Created: NLD 2003-10-03
// -----------------------------------------------------------------------------
void TER_Localisation::Scale( double rDist )
{
    ///$$$$$ NLD - A DECOUPER

    if( nType_ == ePolygon )
    {
        MT_Vector2D vBarycenter = ComputeBarycenter();
        // Scale (homothety from the barycenter)
        T_PointVector newPointVector;        
        if( pointVector_.size() == 3 ) // Flat polygon with only 2 points...
        {
            MT_Vector2D direction = pointVector_[1] - pointVector_[0];
            direction.Rotate90();
            direction.Normalize();
            T_PointVector newVector;
            newVector.push_back( pointVector_[0] );
            newVector.push_back( vBarycenter + direction );
            newVector.push_back( pointVector_[1] );
            newVector.push_back( vBarycenter - direction );
            newVector.push_back( pointVector_[2] );
            polygon_.Reset( newVector, false );
            pointVector_ = newVector;
        }
        else if( pointVector_.size() == 4 ) // Triangles
        {
            T_PointVector polygonWithAtLeastFourSides = GetVectorWithAdditionalPoint( pointVector_ );
            polygon_.Reset( polygonWithAtLeastFourSides, false );
            pointVector_ = polygonWithAtLeastFourSides;
        }
        for( CIT_PointVector itPoint = pointVector_.begin(); itPoint != pointVector_.end(); ++itPoint )
        {
            MT_Vector2D vDir( *itPoint - vBarycenter );
            double rDirLength  = vDir.Magnitude();
            if( rDirLength > 0 )
                vDir *= ( ( rDirLength + rDist ) / rDirLength );
            TER_World::GetWorld().ClipPointInsideWorld( vDir += vBarycenter );
            newPointVector.push_back( vDir );
        }
        polygon_.Reset( newPointVector, true ); // 'true' is for generate 'convex hull'
        pointVector_ = polygon_.GetBorderPoints();
        InitializeBoundingBox( pointVector_ );
    }
    else if( nType_ == eLine )
    {
        T_PointVector points1Vector;
        T_PointVector points2Vector;
        MT_Vector2D vNormDirCurLine;
        for( CIT_PointVector itPoint = pointVector_.begin(); itPoint != pointVector_.end(); ++ itPoint )
        {
            const MT_Vector2D& vCurPoint = *itPoint;
            MT_Vector2D vNormDirNextLine;
            if( (itPoint+1) != pointVector_.end() )
            {
                vNormDirNextLine = MT_Vector2D( *(itPoint+1) - *itPoint );
                vNormDirNextLine.Normalize();
                vNormDirNextLine.Rotate90ClockWise();
            }
            MT_Vector2D vDir = vNormDirNextLine + vNormDirCurLine;
            vDir.Normalize();
            vDir *= rDist;
            points1Vector.push_back( vCurPoint + vDir );
            vDir.Rotate180();
            points2Vector.push_back( vCurPoint + vDir );
            vNormDirCurLine = vNormDirNextLine;
        }
        pointVector_ = points1Vector;
        std::copy( points2Vector.rbegin(), points2Vector.rend(), std::back_inserter( pointVector_ ) );
        pointVector_.push_back( pointVector_.front() );
        polygon_.Reset( pointVector_ );
        nType_ = ePolygon;
        InitializeBoundingBox( pointVector_ );
    }
    else if( nType_ == ePoint )
    {
            double rNewRectSize = boundingBox_.GetWidth()/2 + rDist;
            // Transformation du point en rectangle
            MT_Vector2D vPos = pointVector_.front();
            pointVector_.clear(); pointVector_.reserve( 5 );
            pointVector_.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ - rNewRectSize, vPos.rY_ - rNewRectSize )) );
            pointVector_.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ + rNewRectSize, vPos.rY_ - rNewRectSize )) );
            pointVector_.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ + rNewRectSize, vPos.rY_ + rNewRectSize )) );
            pointVector_.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ - rNewRectSize, vPos.rY_ + rNewRectSize )) );
            pointVector_.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ - rNewRectSize, vPos.rY_ - rNewRectSize )) );
            polygon_.Reset( pointVector_ );
            nType_ = ePolygon;
            boundingBox_.Set( pointVector_[0], pointVector_[2] );
    }
    else
        assert( false );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::IsIntersecting
// Created: NLD 2007-04-17
// -----------------------------------------------------------------------------
bool TER_Localisation::IsIntersecting( const TER_Polygon& polygon, double rPrecision ) const
{
    CIT_PointVector itPoint = pointVector_.begin();
    const MT_Vector2D* pPrevPoint = &*itPoint;
    for( ++itPoint; itPoint != pointVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pCurPoint = &*itPoint;
        if( polygon.IsInside( *pPrevPoint, rPrecision ) || polygon.Intersect2D( MT_Line( *pPrevPoint, *pCurPoint ), rPrecision ) )
            return true;
        pPrevPoint = pCurPoint;
    }
    if( polygon.IsInside( *pPrevPoint, rPrecision ) )
        return true;
    // ici, les deux localisations sont disjointes OU polygon est inclu dans *this
    return IsInside( polygon.GetBorderPoints().front(), rPrecision );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::IsIntersecting
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
bool TER_Localisation::IsIntersecting( const TER_Localisation& localisation, double rPrecision ) const
{
    if( localisation.GetPoints().empty() || pointVector_.empty() ) // Nécessaire au dernier test...
        return false;
    CIT_PointVector itPoint = pointVector_.begin();
    const MT_Vector2D* pPrevPoint = &*itPoint;
    for( ++itPoint; itPoint != pointVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pCurPoint = &*itPoint;
        if( localisation.IsInside( *pPrevPoint, rPrecision ) || localisation.Intersect2D( MT_Line( *pPrevPoint, *pCurPoint ), rPrecision ) )
            return true;
        pPrevPoint = pCurPoint;
    }
    if( localisation.IsInside( *pPrevPoint, rPrecision ) )
        return true;
    // ici, les deux localisations sont dijointes OU localisation est inclu dans *this
    return IsInside( localisation.GetPoints().front(), rPrecision );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::IsIntersectingWithBorderLines
// Created: MMC 2013-06-12
// -----------------------------------------------------------------------------
bool TER_Localisation::IsIntersectingWithBorderLines( const MT_Line& line ) const
{
    if( pointVector_.size() > 1 )
        for( std::size_t index = 0; index < (pointVector_.size() - 1); ++index )
            if( MT_Line( pointVector_[ index ], pointVector_[ index + 1 ] ).Intersect2D( line ) )
                return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::Contains
// Created: BCI 2011-02-24
// -----------------------------------------------------------------------------
bool TER_Localisation::Contains( const TER_Localisation& other, double rPrecision ) const
{
    for( auto it = other.pointVector_.begin(); it != other.pointVector_.end(); ++it )
        if( !IsInside( *it, rPrecision ) )
            return false;
    if( pointVector_.size() > 2 && other.pointVector_.size() > 1 )
        for( std::size_t index = 0; index < (other.pointVector_.size() - 1); ++index )
            if( IsIntersectingWithBorderLines( MT_Line( other.pointVector_[ index ], other.pointVector_[ index + 1 ] ) ) )
                return false;
    return true;
}

namespace
{
    double CirclesIntersectionArea( const TER_Localisation& lhs, const MT_Circle& rhs )
    {
        return MT_Circle( lhs.GetCircleCenter(), lhs.GetCircleRadius() ).IntersectionArea( rhs );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetIntersectionAreaWithCircle
// Created: SBO 2006-01-19
// -----------------------------------------------------------------------------
double TER_Localisation::GetIntersectionAreaWithCircle( const MT_Circle& circle ) const
{
    switch( nType_ )
    {
    case ePolygon:
        if( bWasCircle_ )
            return CirclesIntersectionArea( *this, circle );
        else // TODO
            return 0.;
    case eCircle:
        return CirclesIntersectionArea( *this, circle );
    case eLine:
        {
            T_PointVector shape;
            if( ! polyline_.Intersect2DWithCircle( circle.Center(), circle.Radius(), shape ) )
                return 0.;
            double rDistance = 0.;
            CIT_PointVector itCurrent = shape.begin();
            CIT_PointVector itNext = shape.begin();
            ++itNext;
            for( ; itNext != shape.end(); ++itCurrent, ++itNext )
                rDistance += itCurrent->Distance( *itNext );
            return rDistance;
        }
    default:
        return 0.;
    }
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::GetPoints
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
const T_PointVector& TER_Localisation::GetPoints() const
{
    return pointVector_;
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::GetType
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
TER_Localisation::E_LocationType TER_Localisation::GetType() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::operator==
// Created: NLD 2003-09-01
// -----------------------------------------------------------------------------
bool TER_Localisation::operator == ( const TER_Localisation& localisation ) const
{
    return localisation.nType_ == nType_ && localisation.pointVector_ == pointVector_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::operator!=
// Created: CMA 2011-10-13
// -----------------------------------------------------------------------------
bool TER_Localisation::operator != ( const TER_Localisation& localisation ) const
{
    return ! ( *this == localisation );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::ComputeBarycenter
// Created: NLD 2003-08-22
// -----------------------------------------------------------------------------
MT_Vector2D TER_Localisation::ComputeBarycenter() const
{
    return MT_ComputeBarycenter( pointVector_ );
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::GetBoundingBox
// Created: JVT 03-09-02
//-----------------------------------------------------------------------------
MT_Rect TER_Localisation::GetBoundingBox() const
{
    return boundingBox_;
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Intersect2DWithCircle
// Created: NLD 2003-07-23
//-----------------------------------------------------------------------------
bool TER_Localisation::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const
{
    switch( nType_ )
    {
//        case ePoint:   return vCircleCenter.Distance( pointVector_[0] ) <= rRadius;
        case ePoint:   return polygon_ .Intersect2DWithCircle( vCircleCenter, rRadius );
        case ePolygon: return polygon_ .Intersect2DWithCircle( vCircleCenter, rRadius );
        case eLine:    return polyline_.Intersect2DWithCircle( vCircleCenter, rRadius );
        default:
            return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::Intersect2DWithCircle
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool TER_Localisation::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, T_PointVector& shape ) const
{
    switch( nType_ )
    {
        case ePoint:
        case ePolygon:
            {
                assert( false );
                return false;
            }
        case eLine: return polyline_.Intersect2DWithCircle( vCircleCenter, rRadius, shape );
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::IsInside
// Created: NLD 2003-07-24
// Last modified: JVT 03-09-02
//-----------------------------------------------------------------------------
bool TER_Localisation::IsInside( const MT_Vector2D& vPos ) const
{
    return IsInside( vPos, rPrecision_ );
}

bool TER_Localisation::IsInside( const MT_Vector2D& vPos, double rPrecision ) const
{
    switch( nType_ )
    {
//        case ePoint:   return vPos.SquareDistance( pointVector_[0] ) <= ( rPrecision_ * rPrecision_ );
        case ePoint:   return polygon_ .IsInside( vPos, rPrecision );
        case ePolygon: return polygon_ .IsInside( vPos, rPrecision );
        case eLine:    return boundingBox_.IsInside( vPos, rPrecision ) && polyline_.IsInside( vPos, rPrecision );
        default:
            return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::IsOnBorder
// Created: NLD 2003-10-03
// -----------------------------------------------------------------------------
bool TER_Localisation::IsOnBorder( const MT_Vector2D& vPos, double rPrecision ) const
{
    switch( nType_ )
    {
//        case ePoint:   return vPos.SquareDistance( pointVector_[0] ) <= ( rPrecision * rPrecision );
        case ePoint:   return boundingBox_.IsInside( vPos, rPrecision ) && polygon_ .IsOnBorder( vPos, rPrecision );
        case ePolygon: return boundingBox_.IsInside( vPos, rPrecision ) && polygon_ .IsOnBorder( vPos, rPrecision );
        case eLine:    return boundingBox_.IsInside( vPos, rPrecision ) && polyline_.IsInside  ( vPos, rPrecision );
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Intersect2D
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
bool TER_Localisation::Intersect2D( const MT_Line& orientedLine, T_PointSet& collisions, double rPrecision /*= rPrecision_*/ ) const
{
    switch( nType_ )
    {
        /*case ePoint:
            {
                if( orientedLine.IsInside( pointVector_[0], rPrecision_ ) )
                {
                    collisions.insert( pointVector_[0] );
                    return true;
                }
                return false;
            }                */
        case ePoint:   return polygon_ .Intersect2D( orientedLine, collisions, rPrecision );
        case ePolygon: return polygon_ .Intersect2D( orientedLine, collisions, rPrecision );
        case eLine:    return polyline_.Intersect2D( orientedLine, collisions, rPrecision );
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Intersect2D
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
bool TER_Localisation::Intersect2D( const MT_Line& line, double rPrecision /*= rPrecision_*/ ) const
{
    switch( nType_ )
    {
//        case ePoint:   return line.IsInside( pointVector_[0], rPrecision );
        case ePoint:   return polygon_ .Intersect2D( line, rPrecision );
        case ePolygon: return polygon_ .Intersect2D( line, rPrecision );
        case eLine:    return polyline_.Intersect2D( line, rPrecision );
        default:
            return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetArea
// Created: NLD 2003-10-17
// -----------------------------------------------------------------------------
double TER_Localisation::GetArea() const
{
    switch( nType_ )
    {
        case ePoint:   return 1.f;
        case ePolygon: return polygon_ .GetArea  ();
        case eLine:    return polyline_.Magnitude();
        default:
            return 0.f;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetLength
// Created: NLD 2003-10-17
// -----------------------------------------------------------------------------
double TER_Localisation::GetLength() const
{
    switch( nType_ )
    {
        case ePoint:   return 1.; // ???
        case ePolygon: return MT_Vector2D( boundingBox_.GetPointUpLeft() - boundingBox_.GetPointDownRight() ).Magnitude();
        case eLine:    return polyline_.Magnitude();
        default:
            return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::WasACircle
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
bool TER_Localisation::WasACircle() const
{
    return bWasCircle_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetCircleRadius
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
double TER_Localisation::GetCircleRadius() const
{
    return rCircleRadius_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetCircleCenter
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
const MT_Vector2D& TER_Localisation::GetCircleCenter() const
{
    return vCircleCenter_;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::DefaultPrecision
// Created: LDC 2013-11-06
// -----------------------------------------------------------------------------
double TER_Localisation::DefaultPrecision()
{
    return rPrecision_;
}

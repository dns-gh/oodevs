//*****************************************************************************
//
// $Created: NLD 2003-07-22 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_Localisation.cpp $
// $Author: Age $
// $Modtime: 16/06/05 11:23 $
// $Revision: 5 $
// $Workfile: TER_Localisation.cpp $
//
//*****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_Localisation.h"
#include "TER_World.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_InputArchive_Logger.h"
#include "MT/MT_Archive/MT_OutputArchive_ABC.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"
#include "MT/MT_Archive/MT_Error.h"
#include "MT/MT_IO/MT_FormatString.h"

// $$$$ JVT : CRADE
#include "../../src/libraries/simulation_kernel/CheckPoints/MIL_CheckPointSerializationHelpers.h"

const MT_Float TER_Localisation::rPrecision_ = 2.1242;

#include "TER_Localisation.inl"

//MT_BOOSTPOOLING_FUNCTIONS( TER_Localisation )

//-----------------------------------------------------------------------------
// Name: TER_Localisation constructor
// Created: NLD 2003-07-22
//-----------------------------------------------------------------------------
TER_Localisation::TER_Localisation()
    : nType_        ( eNone )
    , bWasCircle_   ( false )
    , vCircleCenter_()
    , rCircleRadius_( 0. )
{

}

//-----------------------------------------------------------------------------
// Name: TER_Localisation constructor
// Created: JVT 04-05-28
//-----------------------------------------------------------------------------
TER_Localisation::TER_Localisation( const TER_Localisation& localisation )
    : bWasCircle_   ( false )
    , vCircleCenter_()
    , rCircleRadius_( 0. )
{
    nType_       = localisation.GetType();
    pointVector_ = localisation.GetPoints();

    if( nType_ == ePolygon )
        polygon_ = localisation.polygon_;
	else if( nType_ == eLine )
		polyline_ = localisation.polyline_;
    else if( nType_ == ePoint )
        polygon_ = localisation.polygon_;

    boundingBox_ = localisation.boundingBox_;   
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void TER_Localisation::load( boost::archive::binary_iarchive& file, const uint )
{
    file >> nType_
         >> pointVector_
         >> bWasCircle_
         >> vCircleCenter_
         >> rCircleRadius_;

    Initialize();
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void TER_Localisation::save( boost::archive::binary_oarchive& file, const uint ) const
{
    file << nType_
         << pointVector_
         << bWasCircle_
         << vCircleCenter_
         << rCircleRadius_;
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation constructor
// Created: JVT 04-06-07
//-----------------------------------------------------------------------------
TER_Localisation::TER_Localisation( E_LocationType nType, const T_PointVector& pointVector )
    : nType_        ( nType )
    , pointVector_  ( pointVector )
    , bWasCircle_   ( false )
    , vCircleCenter_()
    , rCircleRadius_( 0. )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation constructor
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
TER_Localisation::TER_Localisation( const MT_Vector2D& vPos, MT_Float rRadius )
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


//-----------------------------------------------------------------------------
// Name: TER_Localisation destructor
// Created: NLD 2003-07-22
//-----------------------------------------------------------------------------
TER_Localisation::~TER_Localisation()
{

}

//=============================================================================
// MAIN
//=============================================================================

// -----------------------------------------------------------------------------
// Name: TER_Localisation::InitializeBoundingBox
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
void TER_Localisation::InitializeBoundingBox( CT_PointVector& pointVector )
{
    // bounding box
    MT_Vector2D vDownLeft( std::numeric_limits<MT_Float>::max(), std::numeric_limits<MT_Float>::max() );
    MT_Vector2D vUpRight ( std::numeric_limits<MT_Float>::min(), std::numeric_limits<MT_Float>::min() ); 

    for ( CIT_PointVector itPoint = pointVector.begin(); itPoint != pointVector.end(); ++itPoint )
    {
        const MT_Vector2D& vPos = *itPoint;
        if ( vPos.rX_ < vDownLeft.rX_ )
            vDownLeft.rX_ = vPos.rX_;
        if ( vPos.rY_ < vDownLeft.rY_ )
            vDownLeft.rY_ = vPos.rY_;
        if ( vPos.rX_ > vUpRight.rX_ )
            vUpRight.rX_ = vPos.rX_;
        if ( vPos.rY_ > vUpRight.rY_ )
            vUpRight.rY_ = vPos.rY_;
    }
    boundingBox_.Set( vDownLeft, vUpRight );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::InitializePolygon
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
inline
bool TER_Localisation::InitializePolygon()
{
    if( pointVector_.size() < 3 )
        return false;

    for( IT_PointVector it = pointVector_.begin(); it != pointVector_.end(); ++it )
        TER_World::GetWorld().ClipPointInsideWorld( *it );

    // Make the polygon loop if it wasn't done
    if( *pointVector_.begin() != *pointVector_.rbegin() )
        pointVector_.push_back( *pointVector_.begin() );
    polygon_.Reset( pointVector_ );

    InitializeBoundingBox( pointVector_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::InitializeLine
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
inline
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
inline
bool TER_Localisation::InitializePoint()
{
    static MT_Float rRectSize = 250 / TER_World::GetWorld().GetMeterPerPixel();

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
inline
bool TER_Localisation::InitializeCircle()
{
    if( pointVector_.size() != 2 )
        return false;

    bWasCircle_    = true;
    vCircleCenter_ = pointVector_[0];
    rCircleRadius_ = ( vCircleCenter_.Distance( pointVector_[1] ) );

    T_PointVector pointsTmp; pointsTmp.reserve( 10 );
    for( MT_Float rAngle = 0; rAngle < ( MT_PI * 2 ); rAngle += (MT_PI/8) )
    {
        MT_Float rX_ = vCircleCenter_.rX_ + ( rCircleRadius_ * cos( rAngle ) );
        MT_Float rY_ = vCircleCenter_.rY_ + ( rCircleRadius_ * sin( rAngle ) );
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

//$$$$$ a deplacer dans MT_Vector2D

MT_Float ComputeAngle( const MT_Vector2D& v1Tmp, const MT_Vector2D& v2Tmp )
{
    MT_Vector2D v1 = v1Tmp.Normalized();
    MT_Vector2D v2 = v2Tmp.Normalized();

    MT_Float rCosAngle = v1.rX_ * v2.rX_ + v1.rY_ * v2.rY_; // (cos angle)
    MT_Float rSinAngle = v1.rX_ * v2.rY_ - v1.rY_ * v2.rX_; // (sin angle)

    MT_Float rAngle = acos(rCosAngle);
     
    if( rSinAngle >= 0.0 )
        return rAngle ;
    else
        return -rAngle;
}

#define MT_ANGLE_POINT_INTERVAL   ( 30 * MT_PI / 180 ) // 30°

// -----------------------------------------------------------------------------
// Name: TER_Localisation::InitializeSector
// Created: NLD 2003-09-24
// -----------------------------------------------------------------------------
inline
bool TER_Localisation::InitializeSector()
{
    if( pointVector_.size() != 3 )
        return false;

    MT_Vector2D& vCenter  = pointVector_[0];
    MT_Float     rRadiusA = ( vCenter.Distance( pointVector_[1] ) );
    MT_Float     rRadiusB = ( vCenter.Distance( pointVector_[2] ) );
    MT_Vector2D  vRadiusA( pointVector_[1] - vCenter );
    MT_Vector2D  vRadiusB( pointVector_[2] - vCenter );

    MT_Float rSectorAngle = ComputeAngle( vRadiusA, vRadiusB );
    MT_Float rOriginAngle = ComputeAngle( vRadiusA, MT_Vector2D( 1, 0 ) );

    if( rSectorAngle < 0 )
        rSectorAngle = 2 * MT_PI + rSectorAngle;

    T_PointVector pointsTmp;
    pointsTmp.push_back( vCenter );

    MT_Float rAngleTmp = 0;
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
inline
bool TER_Localisation::InitializeEllipse()
{
    if( pointVector_.size() != 3 )
        return false;

    MT_Vector2D& vCenter  = pointVector_[0];
    MT_Float     rRadiusA = ( vCenter.Distance( pointVector_[1] ) );
    MT_Float     rRadiusB = ( vCenter.Distance( pointVector_[2] ) );
    MT_Vector2D  vRadiusA( pointVector_[1] - vCenter );

    MT_Float rOriginAngle = ComputeAngle( vRadiusA, MT_Vector2D( 1, 0 ) );

    T_PointVector pointsTmp;
    for( MT_Float rAngleTmp = 0; rAngleTmp < ( MT_PI * 2 ); rAngleTmp += MT_ANGLE_POINT_INTERVAL )
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
bool TER_Localisation::Initialize()
{
    switch( nType_ )
    {
        case eCircle  : return InitializeCircle ();
        case eEllipse : return InitializeEllipse();
        case eLine    : return InitializeLine   ();
        case ePolygon : return InitializePolygon();
        case ePoint   : return InitializePoint  ();
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
bool TER_Localisation::Reset( E_LocationType nType, const T_PointVector& pointVector )
{
    Reset();
    nType_       = nType;
    pointVector_ = pointVector;
    return Initialize();
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
    bool bOut = Initialize();
    assert( bOut );
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

    else if( pointVector_.size() > 2 )
    {
        if( ( *pointVector.begin() == pointVector[ pointVector.size() - 1 ] ) )
            nType_ = ePolygon;
        else
            nType_ = eLine;
    }

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
void TER_Localisation::Reset( const TER_Localisation& localisation )
{
    Reset();
    nType_         = localisation.GetType        ();
    pointVector_   = localisation.GetPoints      ();
    bWasCircle_    = localisation.WasACircle     ();
    vCircleCenter_ = localisation.GetCircleCenter();
    rCircleRadius_ = localisation.GetCircleRadius();

    if( nType_ == ePolygon )
        polygon_ = localisation.polygon_;
	else if( nType_ == eLine )
		polyline_ = localisation.polyline_;
    else if( nType_ == ePoint )
        polygon_ = localisation.polygon_;

    boundingBox_ = localisation.boundingBox_;
}

//-----------------------------------------------------------------------------
// Name: TER_Localisation::Read
// Created: NLD 2003-07-22
//-----------------------------------------------------------------------------
void TER_Localisation::Read( MT_InputArchive_Logger< MT_XXmlInputArchive >& archive )
{
    Reset();
    archive.Section( "shape" );

    std::string strType;
    archive.ReadAttribute( "type", strType );
    nType_ = ConvertLocalisationType( strType );

    // Points
    archive.BeginList( "points" );
    while ( archive.NextListElement() )
    {
        std::string strPoint;
        archive.Section( "point" );
        archive.Read( strPoint );
        archive.EndSection();

        MT_Vector2D vPoint;
        TER_World::GetWorld().MosToSimMgrsCoord( strPoint, vPoint );
        pointVector_.push_back( vPoint );
    }   
    archive.EndList(); // points
   
    archive.EndSection(); // shape

    if( !Initialize() )
        throw MT_ScipioException( "TER_Localisation::Read", __FILE__, __LINE__, "Invalid localisation", archive.GetContext() );
}


//-----------------------------------------------------------------------------
// Name: TER_Localisation::Write
// Created: JVT 03-07-31
//-----------------------------------------------------------------------------
void TER_Localisation::Write( MT_OutputArchive_ABC& archive ) const
{
    archive.Section( "shape" );

    if( bWasCircle_ )
    {
        archive.WriteAttribute( "type", "cercle" );
        archive.Section( "points" );
        std::string strPoint;
        TER_World::GetWorld().SimToMosMgrsCoord( vCircleCenter_, strPoint );
        archive.WriteField( "point", strPoint );

        MT_Vector2D vDir( 0., 1. );
        TER_World::GetWorld().SimToMosMgrsCoord( vCircleCenter_ + vDir * rCircleRadius_, strPoint );
        archive.WriteField( "point", strPoint );

        archive.EndSection(); // points
    }
    else
    {
        archive.WriteAttribute( "type", ConvertLocalisationType( nType_ ) );

        archive.Section( "points" );
        std::string strPoint;
        for ( CIT_PointVector it = pointVector_.begin(); it != pointVector_.end(); ++it )
        {
            TER_World::GetWorld().SimToMosMgrsCoord( *it, strPoint );
            archive.WriteField( "point", strPoint );
           
        }
        archive.EndSection(); // points        
    }
    archive.EndSection(); // shape
}


//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: TER_Localisation::ConvertLocalisationType
// Created: NLD 2003-07-22
// Last modified: JVT 03-10-07
//-----------------------------------------------------------------------------
// static
TER_Localisation::E_LocationType TER_Localisation::ConvertLocalisationType( const std::string& strType )
{
    if ( !::strcmpi( strType.c_str(),"cercle" ) )
        return eCircle;
    else if ( !::strcmpi( strType.c_str(), "ellipse" ) )
        return eEllipse;
    else if ( !::strcmpi( strType.c_str(), "ligne" ) )
        return eLine;
    else if ( !::strcmpi( strType.c_str(), "polygone" ) )
        return ePolygon;
    else if ( !::strcmpi( strType.c_str(), "point" ) )
        return ePoint;
    else if ( !::strcmpi( strType.c_str(), "secteur" ) )
        return eSector;
    else
        throw MT_ScipioException( "TER_Localisation::ConvertLocalisationType", __FILE__, __LINE__, MT_FormatString( "Invalid localisation type: '%s'", strType.c_str() ) );
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

    throw MT_ScipioException( "TER_Localisation::ConvertLocalisationType", __FILE__, __LINE__, MT_FormatString( "Invalid localisation type: '%d'", nType ) );
}


//=============================================================================
// GEO TOOLS
//=============================================================================
            
//-----------------------------------------------------------------------------
// Name: TER_Localisation::ComputeNearestPoint
// Calcule la position de la localisation la plus proche de vSrc
// Created: NLD 2003-07-24
// Last modified: JVT 03-09-04
//-----------------------------------------------------------------------------
bool TER_Localisation::ComputeNearestPoint( const MT_Vector2D& vSrc, MT_Vector2D& vResult ) const
{
    if( IsInside( vSrc ) )
    {
        vResult = vSrc;
        return true;
    }

    if( pointVector_.size() == 1 )
    {
        vResult = pointVector_[ 0 ];
        return true;
    }

    MT_Float rShortestDist   = std::numeric_limits< MT_Float >::max();
    CIT_PointVector itPoint  = pointVector_.begin();
    const MT_Vector2D* pPos1 = &*itPoint;
    for( ++itPoint; itPoint != pointVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pPos2 = &*itPoint;
        MT_Line lineTmp( *pPos1, *pPos2 );

        MT_Vector2D vResultTmp = lineTmp.ClosestPointOnLine( vSrc );
        MT_Float    rDistTmp   = vResultTmp.Distance( vSrc );
        if( rDistTmp < rShortestDist )
        {
            rShortestDist = rDistTmp;
            vResult       = vResultTmp;
        }
        pPos1 = pPos2;
    }
    assert( rShortestDist != std::numeric_limits< MT_Float >::max() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::ComputeNearestPoint
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
bool TER_Localisation::ComputeNearestPoint( const TER_Localisation& localisation, MT_Vector2D& vResult, MT_Float& rMinDistance ) const
{
    const T_PointVector& points = localisation.GetPoints();
    rMinDistance = std::numeric_limits< MT_Float >::max();

    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
    {
        MT_Vector2D nearestPointTmp;
        if( ComputeNearestPoint( *it, nearestPointTmp ) )
        {
            MT_Float rDistance = it->Distance( nearestPointTmp );
            if( rDistance < rMinDistance )
            {
                rMinDistance = rDistance;
                vResult = *it;
            }
        }
        else
            return false;
    }
    return true;
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
    if ( !clippedPointVector.empty() )
        clippedPointVector.push_back( *clippedPointVector.begin() );    
}


// -----------------------------------------------------------------------------
// Name: TER_Localisation::Split
// Created: NLD 2003-08-22
// -----------------------------------------------------------------------------
void TER_Localisation::Split( uint nNbrParts, T_LocalisationPtrVector& locVector ) const
{
    if( nNbrParts == 0 )
        locVector.clear();

    else if( nNbrParts == 1 )
    {
        TER_Localisation* pLocalisation = new TER_Localisation( *this ); //$$$ RAM
        locVector.push_back( pLocalisation );  
    }

    else if( nType_ == ePoint )
    {
        for( uint i = 0; i < nNbrParts; ++i )
        {
            TER_Localisation* pLocalisation = new TER_Localisation(); //$$$ RAM
            pLocalisation->Reset( *pointVector_.begin() );
            locVector.push_back( pLocalisation );
        }
    }

    else if( nType_ == eLine )
    {
        assert( false );
//        MT_Float rLineLength = polyline_.Magnitude();
//        for( uint i = 0; i < nNbrParts; ++i )
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
        T_DroiteVector droiteVector;
        MT_Vector2D vDir( 0, 1 );
        MT_Float rAngle = 2. * MT_PI / nNbrParts;
        droiteVector.push_back( MT_Droite( vBarycenter, vDir + vBarycenter ) );
        for( uint i = 0; i < nNbrParts; ++i )
        {
            vDir.Rotate( rAngle );
            droiteVector.push_back( MT_Droite( vBarycenter, vDir + vBarycenter ) );
        }
    
        // 3. Création des localisation découpées
        CIT_DroiteVector itDroite = droiteVector.begin();
        const MT_Droite* pPrevDroite = &*itDroite;
        for( ++itDroite; itDroite != droiteVector.end(); ++itDroite )
        {
            const MT_Droite* pCurDroite = &*itDroite;
            T_PointVector splitPolyPoints;
            GetPointsClippedBetweenTwoLines( *pPrevDroite, *pCurDroite, vBarycenter, splitPolyPoints );

            if( !splitPolyPoints.empty() )
            {
                TER_Localisation* pLocalisation = new TER_Localisation(); //$$$ RAM
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

// -----------------------------------------------------------------------------
// Name: TER_Localisation::Scale
// Created: NLD 2003-10-03
// -----------------------------------------------------------------------------
void TER_Localisation::Scale( MT_Float rDist )
{
    ///$$$$$ NLD - A DECOUPER

    if( nType_ == ePolygon )
    {
        MT_Vector2D vBarycenter = ComputeBarycenter();

        // Scale (homothety from the barycenter)
        T_PointVector newPointVector;
        for( CIT_PointVector itPoint = pointVector_.begin(); itPoint != pointVector_.end(); ++itPoint )
        {
            MT_Vector2D vDir( *itPoint - vBarycenter );
            MT_Float rDirLength  = vDir.Magnitude();
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
            static MT_Float rRectSize = 250 / TER_World::GetWorld().GetMeterPerPixel();
            MT_Float rNewRectSize = rRectSize + rDist;

            // Transformation du point en rectangle
            MT_Vector2D vPos = pointVector_.front();

            pointVector_.clear(); pointVector_.reserve( 5 );
            pointVector_.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ - rNewRectSize, vPos.rY_ - rNewRectSize )) );
            pointVector_.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ + rNewRectSize, vPos.rY_ - rNewRectSize )) );
            pointVector_.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ + rNewRectSize, vPos.rY_ + rNewRectSize )) );
            pointVector_.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ - rNewRectSize, vPos.rY_ + rNewRectSize )) );
            pointVector_.push_back( TER_World::GetWorld().ClipPointInsideWorld( MT_Vector2D( vPos.rX_ - rNewRectSize, vPos.rY_ - rNewRectSize )) );
            polygon_.Reset( pointVector_ );
            boundingBox_.Set( pointVector_[0], pointVector_[2] );
    }
    else
        assert( false );
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::IsIntersecting
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
bool TER_Localisation::IsIntersecting( const TER_Localisation& localisation ) const
{
    if ( localisation.GetPoints().empty() ) // Nécessaire au dernier test...
        return false;

    CIT_PointVector itPoint = pointVector_.begin();
    const MT_Vector2D* pPrevPoint = &*itPoint;
    for ( ++itPoint; itPoint != pointVector_.end(); ++itPoint )
    {
        const MT_Vector2D* pCurPoint = &*itPoint;
        
        if ( localisation.IsInside( *pPrevPoint ) || localisation.Intersect2D( MT_Line( *pPrevPoint, *pCurPoint ) ) )
            return true;
            
        pPrevPoint = pCurPoint;
    }

    if ( localisation.IsInside( *pPrevPoint ) )
        return true;

    // ici, les deux localisations sont dijointes OU localisation est inclu dans *this
    return IsInside( localisation.GetPoints().front() );
}

namespace
{
    MT_Float CirclesIntersectionArea( const TER_Localisation& lhs, const MT_Circle& rhs )
    {
        return MT_Circle( lhs.GetCircleCenter(), lhs.GetCircleRadius() ).IntersectionArea( rhs );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::GetIntersectionAreaWithCircle
// Created: SBO 2006-01-19
// -----------------------------------------------------------------------------
MT_Float TER_Localisation::GetIntersectionAreaWithCircle( const MT_Circle& circle ) const
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
            MT_Float rDistance = 0.;
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

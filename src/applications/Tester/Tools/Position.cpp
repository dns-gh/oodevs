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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/Position.cpp $
// $Author: Sbo $
// $Modtime: 18/07/05 17:55 $
// $Revision: 15 $
// $Workfile: Position.cpp $
//
// *****************************************************************************

#include "Tester_Pch.h"
#include "Types.h"
#include "Position.h"

#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"
#include "geometry/Point2.h"
#include "geometry/Rectangle2.h"
#include "geocoord/PlanarCartesian.h"
#include "geocoord/MGRS.h"
#include "geocoord/Geodetic.h"


using namespace TEST;
using namespace geocoord;
using namespace geometry;

PlanarCartesian*              Position::pPlanar_;
PlanarCartesian::Parameters*  Position::pParameters_;
Point2< double >*             Position::pTranslation_;
Geodetic*                     Position::pGeodetic_;

//-----------------------------------------------------------------------------
// Name: Position::Position
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
Position::Position()
    : Point2< double > ()
    , strMgrs_         ()
    , rLongitude_      ( 0.0 )
    , rLatitude_       ( 0.0 )
{
    // nothing
}

//-----------------------------------------------------------------------------
// Name: Position::Position
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
Position::Position( const std::string& strMgrs )
    : Point2< double > ( Point2FromMgrs( strMgrs ) )
    , strMgrs_         ( strMgrs )
    , rLongitude_      ( 0.0 )
    , rLatitude_       ( 0.0 )
{
    UpdateWGS84();
}

//-----------------------------------------------------------------------------
// Name: Position::Position
// Created: SBO 2005-06-29
//-----------------------------------------------------------------------------
Position::Position( double rLatitude, double rLongitude )
    : Point2< double > ()
    , strMgrs_         ()
    , rLatitude_       ( rLatitude  )
    , rLongitude_      ( rLongitude )
{
    pGeodetic_->Set( RadianFromDegree( rLatitude_ ), RadianFromDegree( rLongitude_ ) );
    MGRS mgrs;
    pGeodetic_->GetCoordinates( mgrs );
    strMgrs_ = mgrs.GetString();
    Point2< double > pt( Point2FromMgrs( strMgrs_ ) );
    Set( pt.X(), pt.Y() );
}

//-----------------------------------------------------------------------------
// Name: Position::Position
// Created: SBO 2005-06-29
//-----------------------------------------------------------------------------
Position::Position( const Point2< double >& pt )
    : Point2< double > ( pt )
    , strMgrs_         ( MgrsFromPoint2( pt ) )
    , rLongitude_      ( 0.0 )
    , rLatitude_       ( 0.0 )
{
    UpdateWGS84();
}

//-----------------------------------------------------------------------------
// Name: Position::~Position
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
Position::~Position()
{
}

//-----------------------------------------------------------------------------
// Name: Position::Initialize
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
void Position::Initialize( const std::string& strWorldConfigFile )
{
    try
    {
        std::string       strCurrentDir = MT_GetCurrentDir();
        std::string       strDir;
        std::string       strFile;
        MT_ExtractFilePath( strWorldConfigFile, strDir  );
        MT_ExtractFileName( strWorldConfigFile, strFile );
        MT_ChangeDir      ( strDir );

        XmlInputArchive   archive;

        float             rMiddleLatitude;
        float             rMiddleLongitude;
        uint              nWorldWidth;
        uint              nWorldHeight;

        archive.Open      ( strWorldConfigFile            );
        archive.ReadField ( "Latitude" , rMiddleLatitude  );
        archive.ReadField ( "Longitude", rMiddleLongitude );
        archive.ReadField ( "Width"    , nWorldWidth      );
        archive.ReadField ( "Height"   , nWorldHeight     );
        archive.Close     (                               );

        // mgrs conversion
        pParameters_  = new PlanarCartesian::Parameters( rMiddleLatitude  * std::acos( -1. ) / 180., 
                                                         rMiddleLongitude * std::acos( -1. ) / 180. );
        pPlanar_      = new PlanarCartesian            ( *pParameters_ );
        pTranslation_ = new Point2< double >( nWorldWidth * 0.5, nWorldHeight * 0.5 );
        
        // wgs84 conversion
        pGeodetic_    = new Geodetic();

        MT_ChangeDir     ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        throw exception;
    }
}

//-----------------------------------------------------------------------------
// Name: Position::Terminate
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
void Position::Terminate()
{
    delete pGeodetic_;
    delete pTranslation_;
    delete pPlanar_;
    delete pParameters_;
}

//-----------------------------------------------------------------------------
// Name: Position::operator=
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
Position& Position::operator=( const std::string& strMgrs )
{
    strMgrs_ = strMgrs;

    try
    {
        MGRS        mgrs;
        mgrs.SetString( strMgrs_ );
        pPlanar_->SetCoordinates( mgrs );
        Set( pPlanar_->GetX() + pTranslation_->X(), pPlanar_->GetY() + pTranslation_->Y() );
        UpdateWGS84();
        return *this;
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() << ": " << strMgrs );
        throw;
    }
}

//-----------------------------------------------------------------------------
// Name: Position::operator=
// Created: SBO 2005-05-31
//-----------------------------------------------------------------------------
Position& Position::operator=( const Point2< double >& pt )
{
    strMgrs_ = MgrsFromPoint2( pt );
    Set( pt.X(), pt.Y() );
    UpdateWGS84();
    return *this;
}

//-----------------------------------------------------------------------------
// Name: Position::operator=
// Created: SBO 2005-06-16
//-----------------------------------------------------------------------------
Position& Position::operator=( const Position& position )
{
    Set( position.X(), position.Y() );
    strMgrs_    = position.strMgrs_;
    rLatitude_  = position.rLatitude_;
    rLongitude_ = position.rLongitude_;
    return *this;
}

//-----------------------------------------------------------------------------
// Name: Position::operator+
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
Position Position::operator+( const Point2< double >& pt ) const
{
    geometry::Point2< double > tmpPt( pt.X() + X(), pt.Y() + Y() );
    Position tmpPos;
    tmpPos = tmpPt;
    return tmpPos;
}

//-----------------------------------------------------------------------------
// Name: Position::Point2FromMgrs
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
geometry::Point2< double > Position::Point2FromMgrs( const std::string& strMgrs )
{
    MGRS mgrs;

    try
    {
        mgrs.SetString( strMgrs );
        pPlanar_->SetCoordinates( mgrs );
        return geometry::Point2< double >( pPlanar_->GetX() + pTranslation_->X(),
                                           pPlanar_->GetY() + pTranslation_->Y() );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() << ": " << strMgrs );
        throw;
    }
}

//-----------------------------------------------------------------------------
// Name: Position::Point2FromPolar
// Created: SBO 2005-05-31
//-----------------------------------------------------------------------------
Point2< double > Position::Point2FromPolar( double rRadius, double rDegAngle )
{
    double x = rRadius * std::cos( RadianFromDegree( rDegAngle ) );
    double y = rRadius * std::sin( RadianFromDegree( rDegAngle ) );

    return geometry::Point2< double >( x, y );
}

//-----------------------------------------------------------------------------
// Name: Position::MgrsFromPoint2
// Created: SBO 2005-05-31
//-----------------------------------------------------------------------------
std::string Position::MgrsFromPoint2( const Point2< double >& pt )
{
    MGRS mgrs;

    try
    {
        pPlanar_->Set( pt.X() - pTranslation_->X(), pt.Y() - pTranslation_->Y() );
        mgrs.SetCoordinates( *pPlanar_ );
        return mgrs.GetString();
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception caught in " __FUNCTION__ " converting (" << pt.X() << 
                          ", " << pt.Y() << ") to mgrs : " << e.what() );
        throw;
    }
}

//-----------------------------------------------------------------------------
// Name: Position::UpdateWGS84
// Created: SBO 2005-06-06
//-----------------------------------------------------------------------------
void Position::UpdateWGS84()
{
    pPlanar_->GetCoordinates( *pGeodetic_ );
    rLatitude_  = pGeodetic_->GetLatitude ();
    rLongitude_ = pGeodetic_->GetLongitude();
}

//-----------------------------------------------------------------------------
// Name: Position::IsOnLine
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
bool Position::IsOnSegment( const Position& start, const Position& end ) const
{
    double rDistanceDelta = start.GetDistanceTo( end ) - GetDistanceTo( start ) - GetDistanceTo( end );
    return fabs( rDistanceDelta ) < 5; // 5 meters tolerance
}

// -----------------------------------------------------------------------------
// Name: Position::SetSimCoordinates
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void Position::SetSimCoordinates( double rX, double rY )
{
    geometry::Point2< double > pt( rX, rY );
    *this = pt;
}

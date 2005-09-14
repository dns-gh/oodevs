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

#include "TIC_pch.h"
#include "Types.h"
#include "Position.h"

#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"
#include "geometry/Point2.h"
#include "geometry/Rectangle2.h"
#include "geocoord/PlanarCartesian.h"
#include "geocoord/MGRS.h"
#include "geocoord/Geodetic.h"


using namespace TIC;
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
    : geometry::Point2< double > ( 0.0, 0.0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Position constructor
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
Position::Position( const Position& position )
    : geometry::Point2< double >( position.X(), position.Y() )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Position::Position
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
Position::Position( const std::string& strMgrs )
    : geometry::Point2< double >( 0.0, 0.0 )
{
    Position& pos = PositionFromMgrs( strMgrs );
    SetSimCoordinates( pos.X(), pos.Y() );
    delete &pos;
}

//-----------------------------------------------------------------------------
// Name: Position::Position
// Created: SBO 2005-06-29
//-----------------------------------------------------------------------------
Position::Position( double rLatitude, double rLongitude )
    : geometry::Point2< double >( 0.0, 0.0 )
{
    Position& pos = PositionFromWGS( rLatitude, rLongitude );
    SetSimCoordinates( pos.X(), pos.Y() );
    delete &pos;
}

//-----------------------------------------------------------------------------
// Name: Position::~Position
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
Position::~Position()
{
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: Position::operator=
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
Position& Position::operator=( const Position& position )
{
    SetSimCoordinates( position.X(), position.Y() );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Position::operator+
// Created: SBO 2005-08-29
// -----------------------------------------------------------------------------
Position Position::operator+( const Position& position ) const
{
    Position tmpPos;
    tmpPos.SetSimCoordinates( X() + position.X(), Y() + position.Y() );
    return Position( tmpPos );
}

//-----------------------------------------------------------------------------
// Name: Position::IsOnSegment
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
bool Position::IsOnSegment( const Position& start, const Position& end ) const
{
    double rDistanceDelta = start.GetDistanceTo( end ) - GetDistanceTo( start ) - GetDistanceTo( end );
    return fabs( rDistanceDelta ) < 5; // 5 meters tolerance
}

// -----------------------------------------------------------------------------
// Name: Position::PositionFromMgrs
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
Position& Position::PositionFromMgrs( const std::string& strMgrs  )
{
    MGRS mgrs;

    try
    {
        mgrs.SetString( strMgrs );
        pPlanar_->SetCoordinates( mgrs );
        Position& position = *new Position();
        position.SetSimCoordinates( pPlanar_->GetX() + pTranslation_->X()
                                  , pPlanar_->GetY() + pTranslation_->Y() );
        return position;
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() << ": " << strMgrs );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: Position::MgrsFromPosition
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
std::string Position::MgrsFromPosition( const Position& position ) const
{
    MGRS mgrs;

    try
    {
        pPlanar_->Set( position.GetSimX() - pTranslation_->X(), position.GetSimY() - pTranslation_->Y() );
        mgrs.SetCoordinates( *pPlanar_ );
        return mgrs.GetString();
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception caught in " __FUNCTION__ " converting (" << position.GetSimX() << 
                          ", " << position.GetSimY() << ") to mgrs : " << e.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: Position::PositionFromWGS
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
Position& Position::PositionFromWGS( double rLatitude, double rLongitude )
{
    pGeodetic_->Set( RadianFromDegree( rLatitude ), RadianFromDegree( rLongitude ) );
    pPlanar_->SetCoordinates( *pGeodetic_ );
    Position& pos = *new Position();
    pos.SetSimCoordinates( pPlanar_->GetX() + pTranslation_->X(), pPlanar_->GetY() + pTranslation_->Y() );
    return pos;
}

// -----------------------------------------------------------------------------
// Name: Position::SetSimCoordinates
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void Position::SetSimCoordinates( double rX, double rY )
{
    Set( rX, rY );
}

//-----------------------------------------------------------------------------
// Name: Position::PositionFromPolar
// Created: SBO 2005-05-31
//-----------------------------------------------------------------------------
Position Position::PositionFromPolar( double rRadius, double rDegAngle )
{
    /*
    double x = rRadius * std::cos( RadianFromDegree( rDegAngle ) );
    double y = rRadius * std::sin( RadianFromDegree( rDegAngle ) );
    */
    double x = rRadius * std::sin( RadianFromDegree( rDegAngle ) );
    double y = rRadius * std::cos( RadianFromDegree( rDegAngle ) );
    Position tmpPos;
    tmpPos.SetSimCoordinates( x, y );
    return Position( tmpPos );
}

//-----------------------------------------------------------------------------
// Name: Position::GetMgrsCoordinate
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
const std::string Position::GetMgrsCoordinate() const
{
    return MgrsFromPosition( *this );
}

//-----------------------------------------------------------------------------
// Name: Position::GetLatitude
// Created: SBO 2005-06-06
//-----------------------------------------------------------------------------
double Position::GetLatitude() const
{
    pPlanar_->Set( GetSimX() - pTranslation_->X(), GetSimY() - pTranslation_->Y() );
    pGeodetic_->SetCoordinates( *pPlanar_ );
    return DegreeFromRadian( pGeodetic_->GetLatitude() );
}

//-----------------------------------------------------------------------------
// Name: Position::GetLongitude
// Created: SBO 2005-06-06
//-----------------------------------------------------------------------------
double Position::GetLongitude() const
{
    pPlanar_->Set( GetSimX() - pTranslation_->X(), GetSimY() - pTranslation_->Y() );
    pGeodetic_->SetCoordinates( *pPlanar_ );
    return DegreeFromRadian( pGeodetic_->GetLongitude() );
}
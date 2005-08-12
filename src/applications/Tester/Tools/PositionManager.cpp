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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/PositionManager.cpp $
// $Author: Sbo $
// $Modtime: 18/07/05 15:07 $
// $Revision: 8 $
// $Workfile: PositionManager.cpp $
//
// *****************************************************************************

#include "Tester_Pch.h"
#include "Types.h"
#include "Tools/PositionManager.h"
#include "Tools/Position.h"
#include "geometry/Point2.h"
#include "geometry/Rectangle2.h"
#include "geocoord/PlanarCartesian.h"
#include "geocoord/MGRS.h"
#include "geocoord/Geodetic.h"

#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

using namespace TEST;
using namespace geocoord;
using namespace geometry;


//-----------------------------------------------------------------------------
// Name: PositionManager::PositionManager
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
PositionManager::PositionManager( const std::string& strWorldConfigFile )
    : rWorldWidth_  ( 0. )
    , rWorldHeight_ ( 0. )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strWorldConfigFile, strDir  );
        MT_ExtractFileName  ( strWorldConfigFile, strFile );
        MT_ChangeDir        ( strDir );

        XmlInputArchive     archive;

        std::string         strTmp;
        archive.Open        ( strFile );
        archive.ReadField   ( "World", strTmp );
        archive.Close       ();

        InitializeCoordinateConverter( strTmp );

        MT_ChangeDir        ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        throw exception;
    }

    Position::SetPositionManager( *this );
}

//-----------------------------------------------------------------------------
// Name: PositionManager::~PositionManager
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
PositionManager::~PositionManager()
{
    delete pGeodetic_;
    delete pTranslation_;
    delete pPlanar_;
    delete pParameters_;
}


// -----------------------------------------------------------------------------
// Name: PositionManager::InitializeCoordinateConverter
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
void PositionManager::InitializeCoordinateConverter( const std::string& strWorldConfigFile )
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

        archive.Open      ( strWorldConfigFile            );
        archive.ReadField ( "Latitude" , rMiddleLatitude  );
        archive.ReadField ( "Longitude", rMiddleLongitude );
        archive.ReadField ( "Width"    , rWorldWidth_     );
        archive.ReadField ( "Height"   , rWorldHeight_    );
        archive.Close     (                               );

        // mgrs conversion
        pParameters_  = new PlanarCartesian::Parameters( rMiddleLatitude  * std::acos( -1. ) / 180., 
                                                         rMiddleLongitude * std::acos( -1. ) / 180. );
        pPlanar_      = new PlanarCartesian            ( *pParameters_ );
        pTranslation_ = new Point2< double >( rWorldWidth_ * 0.5, rWorldHeight_ * 0.5 );
        
        // wgs84 conversion
        pGeodetic_    = new Geodetic();

        MT_ChangeDir     ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        throw exception;
    }
}

// -----------------------------------------------------------------------------
// Name: PositionManager::PositionFromMgrs
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
Position& PositionManager::PositionFromMgrs( const std::string& strMgrs  )
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
// Name: PositionManager::MgrsFromPosition
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
std::string PositionManager::MgrsFromPosition( const Position& position )
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
// Name: PositionManager::PositionFromWGS
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
Position& PositionManager::PositionFromWGS( double rLatitude, double rLongitude )
{
    pGeodetic_->Set( RadianFromDegree( rLatitude ), RadianFromDegree( rLongitude ) );
    pPlanar_->SetCoordinates( *pGeodetic_ );
    Position& pos = *new Position();
    pos.SetSimCoordinates( pPlanar_->GetX() + pTranslation_->X(), pPlanar_->GetY() + pTranslation_->Y() );
    return pos;
}

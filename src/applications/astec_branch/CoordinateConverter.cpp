// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "CoordinateConverter.h"

#include "geometry/Types.h"
#include "geocoord/Geoid.h"
#include "MT_Tools/MT_Rect.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: CoordinateConverter constructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
CoordinateConverter::CoordinateConverter( const std::string& scipioXml )
    : planar_        ( parameters_ )
{
    xml::xifstream scipio( scipioXml );
    std::string terrain;
    scipio >> start( "Scipio" )
                >> start( "Donnees" )
                    >> content( "Terrain", terrain );
    ReadTerrain( terrain );
}
    
// -----------------------------------------------------------------------------
// Name: CoordinateConverter destructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
CoordinateConverter::~CoordinateConverter()
{
}

//-------------------------------------------------------------------------
// Initialization
//-------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ReadTerrain
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void CoordinateConverter::ReadTerrain( const std::string& terrainFile )
{
    xifstream xis( terrainFile );

    std::string geoidFile, worldFile;
    xis >> content( "Geoid", geoidFile )
        >> content( "CoordinateConverter", worldFile );

    geocoord::Geoid::Instance().Initialize( geoidFile );

    ReadWorld( QFileInfo( terrainFile.c_str() ).filePath().ascii() + ( "/" + worldFile ) );
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ReadWorld
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void CoordinateConverter::ReadWorld( const std::string& worldFile )
{
    xifstream xis( worldFile );

    double rLatitude, rLongitude, rWidth, rHeight;
    xis >> start( "CoordinateConverter" )
        >> content( "Latitude", rLatitude )
        >> content( "Longitude", rLongitude )
        >> content( "Width", rWidth )
        >> content( "Height", rHeight );

    extent_.Set( MT_Vector2D( 0, 0 ), MT_Vector2D( rWidth, rHeight ) );
    translation_.rX_ = rWidth * 0.5;
    translation_.rY_ = rHeight * 0.5;
    const double rPiOver180 = std::acos( -1. ) / 180.;
    parameters_.SetOrigin( rLatitude * rPiOver180, rLongitude * rPiOver180 );
}

//-------------------------------------------------------------------------
// Coordinate CoordinateConverter
//-------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ConvertToMgrs
// Created: SBO 2006-03-14
// -----------------------------------------------------------------------------
std::string CoordinateConverter::ConvertToMgrs( const MT_Vector2D& pos ) const
{
    const MT_Vector2D translated = pos - translation_;
    planar_.Set( translated.rX_, translated.rY_ );
    mgrs_.SetCoordinates( planar_ );
    return mgrs_.GetString();
}
    
// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ConvertToXY
// Created: SBO 2006-03-14
// -----------------------------------------------------------------------------
MT_Vector2D CoordinateConverter::ConvertToXY( const ASN1T_CoordUTM& mgrs ) const
{
    const std::string coord( (const char*)( mgrs.data ), 15 );
    mgrs_.SetString( coord );
    planar_.SetCoordinates( mgrs_ );
    MT_Vector2D pos( planar_.GetX(), planar_.GetY() );
    return pos + translation_;
}

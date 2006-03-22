//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/World.cpp $
// $Author: Age $
// $Modtime: 24/06/05 17:47 $
// $Revision: 9 $
// $Workfile: World.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "World.h"
#undef min
#undef max

#include "geometry/Types.h"
#include "MT/MT_IO/MT_DirEntry.h"
#include "MT/MT_IO/MT_DirectoryBrowser.h"
#include "MT/MT_IO/MT_Dir.h"

#include "geocoord/Geoid.h"
#include "graphics/GraphicShape.h"
#include "graphics/DrawDetection.h"
#include "graphics/DataFactory.h"
#include "graphics/GraphicFactory.h"

#include "terrain/TesselatedShape.h"
#include "MainWindow.h"
#include "Options.h"

// -----------------------------------------------------------------------------
// Name: World constructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
World::World( const std::string& strArchive )
    : planar_              ( parameters_ )
    , strArchiveName_      ( strArchive )
    , pDetection_          ( 0 )
    , isInitialized_       ( false )
    , strTerrainDirectory_ ( "" )
{
    Read( strArchiveName_, true );
}
    
// -----------------------------------------------------------------------------
// Name: World destructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
World::~World()
{
    delete pDetection_;
}

//-------------------------------------------------------------------------
/** @name Conversion */
//-------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: World::SimToMosMgrsCoord
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void World::SimToMosMgrsCoord( const MT_Vector2D& vPos, std::string& sMgrs )
{
    const MT_Vector2D translated = vPos - translation_;
    planar_.Set( translated.rX_, translated.rY_ );
    mgrs_.SetCoordinates( planar_ );
    sMgrs = mgrs_.GetString();
}

// -----------------------------------------------------------------------------
// Name: World::MosToSimMgrsCoord
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void World::MosToSimMgrsCoord( const std::string& sMgrs, MT_Vector2D& vPos )
{
    mgrs_.SetString( sMgrs );
    planar_.SetCoordinates( mgrs_ );
    vPos.rX_ = planar_.GetX();
    vPos.rY_ = planar_.GetY();
    vPos += translation_;
}

// -----------------------------------------------------------------------------
// Name: World::MosToSimMgrsCoord
// Created: AGE 2005-02-21
// -----------------------------------------------------------------------------
void World::MosToSimMgrsCoord( const char sMgrs[15], MT_Vector2D& vPos )
{
    return MosToSimMgrsCoord( std::string( sMgrs, 15 ), vPos );
}

// -----------------------------------------------------------------------------
// Name: World::MgrsToGeoCoord
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void World::MgrsToGeoCoord( const std::string& sMgrs, MT_Vector2D& vPos )
{
    static const MT_Float r180OverPi = 180. / std::acos( -1. );
    mgrs_.SetString( sMgrs );
    geodetic_.SetCoordinates( mgrs_ );
    vPos.rX_ = geodetic_.GetLongitude() * r180OverPi;
    vPos.rY_ = geodetic_.GetLatitude()  * r180OverPi;
}
    
// -----------------------------------------------------------------------------
// Name: World::GeoToMgrsCoord
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void World::GeoToMgrsCoord( const MT_Vector2D& vPos, std::string& sMgrs )
{
    static const MT_Float rPiOver180 = std::acos( -1. ) / 180.;
    geodetic_.Set( vPos.rX_ * rPiOver180, vPos.rY_ * rPiOver180 );
    mgrs_.SetCoordinates( geodetic_ );
    sMgrs = mgrs_.GetString();
}

//-------------------------------------------------------------------------
/** @name Accessors */
//-------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: World::GetRect
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
const MT_Rect& World::GetRect() const
{
    return extent_;
}
   
//-------------------------------------------------------------------------
/** @name Initialization */
//-------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: World::Initialize
// Created: AGE 2005-03-16
// -----------------------------------------------------------------------------
void World::Initialize()
{
    if( ! isInitialized_ )
        Read( strArchiveName_ );
    isInitialized_ = true;
}

// -----------------------------------------------------------------------------
// Name: World::Read
// Created: AGE 2005-03-14
// Modified: SBO 2005-07-26 (Added support for cmd line specified initial directory)
// -----------------------------------------------------------------------------
void World::Read( const std::string& strArchive, bool bWorldOnly )
{
    std::string strDir;
    MT_ExtractFilePath( strArchive, strDir );
    std::string strFile;
    MT_ExtractFileName( strArchive, strFile );

    const std::string strInitialDir = MT_GetCurrentDir();
    if( strTerrainDirectory_.empty() )
        strTerrainDirectory_ = strInitialDir + "\\" + strDir;
    MT_ChangeDir( strTerrainDirectory_ );
    
    InputArchive archive;
    archive.Open( strFile );

    archive.Section( "Terrain" );
    std::string strGeoid;
    std::string strWorld;
    std::string strGraphics;
    std::string strDetection;
    archive.ReadField( "Geoid"    , strGeoid     );
    archive.ReadField( "World"    , strWorld     );
    archive.ReadField( "Graphics" , strGraphics  );
    archive.ReadField( "RawVision", strDetection );

    geocoord::Geoid::Instance().Initialize( strGeoid );

    if( bWorldOnly )
        ReadWorld( strWorld );
    else
    {
        ReadGraphics( strGraphics );
        ReadDetection( strDetection + "/detection.dat" );
    }

    MT_ChangeDir( strInitialDir );
}

// -----------------------------------------------------------------------------
// Name: World::ReadWorld
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void World::ReadWorld( const std::string& strArchive )
{
    InputArchive archive;
    archive.Open( strArchive );
    archive.Section( "World" );
    double rLatitude, rLongitude;
    archive.ReadField( "Latitude", rLatitude );
    archive.ReadField( "Longitude", rLongitude );
    double rWidth, rHeight;
    archive.ReadField( "Width", rWidth );
    archive.ReadField( "Height", rHeight );
    extent_.Set( MT_Vector2D( 0, 0 ), MT_Vector2D( rWidth, rHeight ) );
    translation_.rX_ = rWidth * 0.5;
    translation_.rY_ = rHeight * 0.5;
    const double rPiOver180 = std::acos( -1. ) / 180.;
    parameters_.SetOrigin( rLatitude * rPiOver180, rLongitude * rPiOver180 );
}

// -----------------------------------------------------------------------------
// Name: World::ReadGraphics
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void World::ReadGraphics( const std::string& strArchive )
{
    MT_DirectoryBrowser browser;
    
    browser.FindFiles( strArchive, "*.bin", MT::eFilesOnly, false );
    const MT::T_DirEntryList& entryList = browser.GetEntryList();
    for( MT::T_DirEntryList::const_iterator entryIt = entryList.begin(); entryIt != entryList.end(); ++entryIt )
    {
        const MT_DirEntry& entry = *entryIt;
        const std::string& strName = entry.fullName_;
        std::string strFileName;
        MT_ExtractFileName( strName, strFileName );
        if( strFileName != "Preview.bin" )
            ReadGraphicFile( strName );
    }
}

// -----------------------------------------------------------------------------
// Name: World::ReadGraphicFile
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void World::ReadGraphicFile( const std::string& strName )
{
    DataFactory factory;
    GraphicFactory graphicfactory( *this, setup_, factory );
    try
    {
        graphicfactory.LoadGraphicFile( strName );
    } 
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: World::AddShape
// -----------------------------------------------------------------------------
void World::AddShape( GraphicShape& shape )
{
    lodshapes_[ setup_.GetLastLevelOfDetail() ].push_back( &shape );
}

// -----------------------------------------------------------------------------
// Name: World::ShouldUseList
// Created: AGE 2006-02-01
// -----------------------------------------------------------------------------
bool World::ShouldUseList( const std::string& filename )
{
    return ! MainWindow::GetMainWindow().GetOptions().bNoList_ && filename.find( "list" ) != std::string::npos;
}


// -----------------------------------------------------------------------------
// Name: World::ReadDetection
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void World::ReadDetection( const std::string& strName )
{
    pDetection_ = new DrawDetection( strName, geometry::Rectangle2f( extent_.GetLeft(), extent_.GetBottom(), extent_.GetRight(), extent_.GetTop() ) );
}

// -----------------------------------------------------------------------------
// Name: World::GetTerrainDirectory
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
std::string World::GetTerrainDirectory() const
{
    return strTerrainDirectory_;
}


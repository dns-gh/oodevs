//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_World.cpp $
// $Author: Age $
// $Modtime: 24/06/05 17:47 $
// $Revision: 9 $
// $Workfile: MOS_World.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_World.h"
#undef min
#undef max

#include "geometry/Types.h"
#include "MT/MT_IO/MT_DirEntry.h"
#include "MT/MT_IO/MT_DirectoryBrowser.h"
#include "MT/MT_IO/MT_Dir.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

#include "geocoord/Geoid.h"
#include "graphics/GraphicShape.h"
#include "graphics/DrawDetection.h"
#include "graphics/DataFactory.h"
#include "terrain/TesselatedShape.h"

#include "MOS_GraphicSetup.h"
#include "MOS_MainWindow.h"
#include "MOS_Options.h"

// -----------------------------------------------------------------------------
// Name: MOS_World constructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
MOS_World::MOS_World( const std::string& strArchive )
    : planar_( parameters_ )
    , strArchiveName_( strArchive )
    , pDetection_( 0 )
    , isInitialized_( false )
{
    Read( strArchiveName_, true );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_World destructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
MOS_World::~MOS_World()
{
    for( unsigned int i = 0; i < 3; ++i )
        for( CIT_Shapes it = lodshapes_[i].begin(); it != lodshapes_[i].end(); ++it )
            delete *it;
    delete pDetection_;
}

//-------------------------------------------------------------------------
/** @name Conversion */
//-------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: MOS_World::SimToMosMgrsCoord
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_World::SimToMosMgrsCoord( const MT_Vector2D& vPos, std::string& sMgrs )
{
    const MT_Vector2D translated = vPos - translation_;
    planar_.Set( translated.rX_, translated.rY_ );
    mgrs_.SetCoordinates( planar_ );
    sMgrs = mgrs_.GetString();
}

// -----------------------------------------------------------------------------
// Name: MOS_World::MosToSimMgrsCoord
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_World::MosToSimMgrsCoord( const std::string& sMgrs, MT_Vector2D& vPos )
{
    mgrs_.SetString( sMgrs );
    planar_.SetCoordinates( mgrs_ );
    vPos.rX_ = planar_.GetX();
    vPos.rY_ = planar_.GetY();
    vPos += translation_;
}

// -----------------------------------------------------------------------------
// Name: MOS_World::MosToSimMgrsCoord
// Created: AGE 2005-02-21
// -----------------------------------------------------------------------------
void MOS_World::MosToSimMgrsCoord( const char sMgrs[15], MT_Vector2D& vPos )
{
    return MosToSimMgrsCoord( std::string( sMgrs, 15 ), vPos );
}

// -----------------------------------------------------------------------------
// Name: MOS_World::MgrsToGeoCoord
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_World::MgrsToGeoCoord( const std::string& sMgrs, MT_Vector2D& vPos )
{
    static const MT_Float r180OverPi = 180. / std::acos( -1. );
    mgrs_.SetString( sMgrs );
    geodetic_.SetCoordinates( mgrs_ );
    vPos.rX_ = geodetic_.GetLongitude() * r180OverPi;
    vPos.rY_ = geodetic_.GetLatitude()  * r180OverPi;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_World::GeoToMgrsCoord
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_World::GeoToMgrsCoord( const MT_Vector2D& vPos, std::string& sMgrs )
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
// Name: MOS_World::GetRect
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
const MT_Rect& MOS_World::GetRect() const
{
    return extent_;
}
   
//-------------------------------------------------------------------------
/** @name Initialization */
//-------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: MOS_World::Initialize
// Created: AGE 2005-03-16
// -----------------------------------------------------------------------------
void MOS_World::Initialize()
{
    if( ! isInitialized_ )
        Read( strArchiveName_ );
    isInitialized_ = true;
}

// -----------------------------------------------------------------------------
// Name: MOS_World::Read
// Created: AGE 2005-03-14
// Modified: SBO 2005-07-26 (Added support for cmd line specified initial directory)
// -----------------------------------------------------------------------------
void MOS_World::Read( const std::string& strArchive, bool bWorldOnly )
{
    std::string strDir;
    MT_ExtractFilePath( strArchive, strDir );
    std::string strFile;
    MT_ExtractFileName( strArchive, strFile );

    const std::string strInitialDir = MT_GetCurrentDir();
    if( strTerrainDirectory_.empty() )
        strTerrainDirectory_ = strInitialDir + "\\" + strDir;
    MT_ChangeDir( strTerrainDirectory_ );
    
    MT_XXmlInputArchive archive;
    archive.EnableExceptions( true );
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
// Name: MOS_World::ReadWorld
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void MOS_World::ReadWorld( const std::string& strArchive )
{
    MT_XXmlInputArchive archive;
    archive.EnableExceptions( true );
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
// Name: MOS_World::ReadGraphics
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void MOS_World::ReadGraphics( const std::string& strArchive )
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
// Name: MOS_World::ReadGraphicFile
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void MOS_World::ReadGraphicFile( const std::string& strName )
{
    MOS_GraphicSetup setup;
    DataFactory factory;
    try
    {
        MT_FlatBinaryInputArchive archive;
        archive.EnableExceptions( true );
        archive.Open( strName );
        while( ! archive.EndOfBuffer() )
        {
            if( MOS_MainWindow::GetMainWindow().GetOptions().bNoList_ )
            {
                noListShapes_.push_back( new TesselatedShape( archive, factory ) );
            }
            else
            {
                GraphicShape* pShape = new GraphicShape( archive, factory, setup );
                lodshapes_[ setup.GetLastLevelOfDetail() ].push_back( pShape );
            }
        }
    } 
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_World::ReadDetection
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void MOS_World::ReadDetection( const std::string& strName )
{
    MT_FlatBinaryInputArchive archive;
    archive.EnableExceptions( true );
    archive.Open( strName );    
    pDetection_ = new DrawDetection( archive, geometry::Rectangle2f( extent_.GetLeft(), extent_.GetBottom(), extent_.GetRight(), extent_.GetTop() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_World::GetTerrainDirectory
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
std::string MOS_World::GetTerrainDirectory() const
{
    return strTerrainDirectory_;
}

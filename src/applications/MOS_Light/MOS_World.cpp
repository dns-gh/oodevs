//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_World.cpp $
// $Author: Age $
// $Modtime: 24/06/05 17:06 $
// $Revision: 18 $
// $Workfile: MOS_World.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_World.h"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentEditor.h"
#include "MOS_AgentListView.h"
#include "MT/MT_IO/MT_DirectoryBrowser.h"
#include "MT/MT_IO/MT_Dir.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"
#include "geocoord/Geoid.h"
#include "graphics/GraphicShape.h"
#include "graphics/DrawDetection.h"
#include "graphics/DataFactory.h"
#include "MOS_GraphicSetup.h"

// -----------------------------------------------------------------------------
// Name: MOS_World constructor
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
MOS_World::MOS_World( const std::string& strArchiveName )
    : planar_( parameters_ )
{
    Read( strArchiveName );
}

// -----------------------------------------------------------------------------
// Name: MOS_World destructor
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
MOS_World::~MOS_World()
{
    for( unsigned int i = 0; i < 3; ++i )
        for( CIT_Shapes it = lodshapes_[i].begin(); it != lodshapes_[i].end(); ++it )
            delete *it;
    delete pDetection_;
}

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
    MosToSimMgrsCoord( sMgrs, vPos );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_World::GeoToMgrsCoord
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_World::GeoToMgrsCoord( const MT_Vector2D& vPos, std::string& sMgrs )
{
    SimToMosMgrsCoord( vPos, sMgrs );
}

// -----------------------------------------------------------------------------
// Name: MOS_World::DrawAreas
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
void MOS_World::DrawAreas( unsigned int nMaxLod, const geometry::Rectangle2f& extent ) const
{
    for( unsigned int nLod = 0; nLod < nMaxLod; ++nLod )
    {
        for( CIT_Shapes it = lodshapes_[nLod].begin(); it != lodshapes_[nLod].end(); ++it )
        {
            assert( *it );
            const GraphicShape& shape = **it;
            shape.DrawArea( extent );
            glTranslatef( 0.f, 0.f, 0.0001f );
        }
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_World::DrawBorders
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
void MOS_World::DrawBorders( unsigned int nMaxLod, const geometry::Rectangle2f& extent ) const
{
    glClear( GL_STENCIL_BUFFER_BIT );
    glEnable( GL_STENCIL_TEST );
    glDisable( GL_DEPTH_TEST );
    glStencilFunc( GL_NEVER, 1, 0xFFFFFFFF );
    glStencilOp( GL_INCR, GL_INCR, GL_INCR );

    for( unsigned int nLod = 0; nLod < nMaxLod; ++nLod )
    {
        for( CIT_Shapes it = lodshapes_[nLod].begin(); it != lodshapes_[nLod].end(); ++it )
        {
            assert( *it );
            const GraphicShape& shape = **it;
            shape.DrawBorders( extent );
        }
    }

    glStencilFunc( GL_GEQUAL, 1, 0xFFFFFFFF );
    for( unsigned int nLod = 0; nLod < nMaxLod; ++nLod )
    {
        for( CIT_Shapes it = lodshapes_[nLod].begin(); it != lodshapes_[nLod].end(); ++it )
        {
            assert( *it );
            const GraphicShape& shape = **it;
            shape.DrawBorders( extent );
        }
    }

    glEnable( GL_DEPTH_TEST );
    glDisable( GL_STENCIL_TEST );
    glTranslatef( 0.f, 0.f, 0.0001f );
}

// -----------------------------------------------------------------------------
// Name: MOS_World::DrawLines
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
void MOS_World::DrawLines( unsigned int nMaxLod, const geometry::Rectangle2f& extent ) const
{
    for( unsigned int nLod = 0; nLod < nMaxLod; ++nLod )
    {
        for( CIT_Shapes it = lodshapes_[nLod].begin(); it != lodshapes_[nLod].end(); ++it )
        {
            assert( *it );
            const GraphicShape& shape = **it;
            shape.DrawLines( extent );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_World::Draw
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_World::Draw( const MT_Rect& viewport ) const
{
    if( MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().IsDrawAltitude() )
        pDetection_->Draw();

    const geometry::Rectangle2f extent( viewport.GetLeft(), viewport.GetBottom(), viewport.GetRight(), viewport.GetTop() );

    unsigned int nMaxLod = 3;
    if( extent.Width() > 400000.f ) // $$$$ AGE 2005-02-28: 
        nMaxLod = 0; // draw nothing
    else if( extent.Width() > 200000.f )
        nMaxLod = 1;
    else if( extent.Width() > 80000.f )
        nMaxLod = 2;

    DrawAreas( nMaxLod, extent );
    if( nMaxLod > 2 )
        DrawBorders( nMaxLod, extent );
    DrawLines( nMaxLod, extent );
}

// -----------------------------------------------------------------------------
// Name: MOS_World::DrawNameObjects
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_World::DrawNameObjects( const MT_Rect& viewport, QGLWidget& widget ) const
{
    geometry::Rectangle2f extent( viewport.GetLeft(), viewport.GetBottom(), viewport.GetRight(), viewport.GetTop() );
    glDisable( GL_DEPTH_TEST );
    glColor4f( 0, 0, 0, 1 );
    for( unsigned int nLod = 0; nLod < 3; ++nLod )
    {
        for( CIT_Shapes it = lodshapes_[nLod].begin(); it != lodshapes_[nLod].end(); ++it )
        {
            assert( *it );
            const GraphicShape& shape = **it;
            shape.DrawName( extent, widget );
        }
    }
    glEnable( GL_DEPTH_TEST );
}

// -----------------------------------------------------------------------------
// Name: MOS_World::ReadGraphics
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_World::ReadGraphics( const std::string& strArchive )
{
    MT_DirectoryBrowser browser;
    browser.FindFiles( strArchive, "*.bin", MT_DirectoryBrowser::eFilesOnly, false );
    const MT_DirEntryList& entryList = browser.GetEntryList();
    for( MT_DirEntryList::const_iterator entryIt = entryList.begin(); entryIt != entryList.end(); ++entryIt )
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
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_World::ReadGraphicFile( const std::string& strName )
{
    std::string strFeature;
    MT_ExtractBaseName( strName, strFeature );
    MT_FlatBinaryInputArchive archive;
    MOS_GraphicSetup setup;
    DataFactory factory;

    try
    {
        archive.EnableExceptions( true );
        archive.Open( strName );
        while( ! archive.EndOfBuffer() )
        {
            GraphicShape* pShape = new GraphicShape( archive, factory, setup );
            lodshapes_[ setup.GetLastLevelOfDetail() ].push_back( pShape );
        }
    } 
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_World::Read
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_World::Read( const std::string& strArchive )
{
    std::string strTerrainDataPath;
    MT_ExtractFilePath( strArchive, strTerrainDataPath );

    MT_XXmlInputArchive archive;
    archive.EnableExceptions( true );
    archive.Open( strArchive );

    archive.Section( "Terrain" );
    std::string strGeoid;
    std::string strWorld;
    std::string strGraphics;
    std::string strDetection;
    archive.ReadField( "Geoid", strGeoid );
    archive.ReadField( "World", strWorld);
    archive.ReadField( "Graphics", strGraphics);
    archive.ReadField( "RawVision", strDetection );

    const std::string strInitialDir = MT_GetCurrentDir();
    MT_ChangeDir( strTerrainDataPath );

    geocoord::Geoid::Instance().Initialize( strGeoid );
    ReadGraphics( strGraphics );
    ReadWorld( strWorld );
    ReadDetection( strDetection + "/detection.dat" );

    MT_ChangeDir( strInitialDir );
}

// -----------------------------------------------------------------------------
// Name: MOS_World::ReadWorld
// Created: AGE 2005-02-07
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
// Name: MOS_World::ReadDetection
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_World::ReadDetection( const std::string& strName )
{
    MT_FlatBinaryInputArchive archive;
    archive.EnableExceptions( true );
    archive.Open( strName );    
    pDetection_ = new DrawDetection( archive, geometry::Rectangle2f( extent_.GetLeft(), extent_.GetBottom(), extent_.GetRight(), extent_.GetTop() ) );
}



// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainLayer.h"
#include "GraphicPreferences.h"
#include "TerrainPicker.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/OptionVariant.h"
#include <graphics/RawShapeLayer.h>
#include <graphics/NoVBOShapeLayer.h>
#include <graphics/ShapeCollector.h>
#include <graphics/extensions.h>
#include <graphics/DataFactory.h>

#include <boost/filesystem/operations.hpp>
namespace bfs = boost::filesystem;

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainLayer constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TerrainLayer::TerrainLayer( Controllers& controllers, const GlTools_ABC& tools, GraphicPreferences& setup, TerrainPicker& picker )
    : controllers_( controllers )
    , tools_      ( tools )
    , setup_      ( setup )
    , picker_     ( picker )
    , pickingEnabled_( true )
{
    picker_.RegisterLayer( *this );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TerrainLayer::~TerrainLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::NotifyUpdated
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::NotifyUpdated( const ModelLoaded& modelLoaded )
{
    parameters_.Load( modelLoaded.config_ );
    LoadGraphics();
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void TerrainLayer::SetAlpha( float alpha )
{
    setup_.SetAlpha( alpha );
    Layer2d_ABC::SetAlpha( alpha );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Paint
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !ShouldDrawPass() || GetAlpha() == 0 )
        return;

    if( layer_.get() || noVBOlayer_.get() )
    {
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT );
            glBindTexture( GL_TEXTURE_2D, 0 );
            glDisable( GL_TEXTURE_GEN_S );
            glDisable( GL_TEXTURE_GEN_T );
            if( layer_.get() )
                layer_->Paint( viewport );
            else
                noVBOlayer_->Paint( viewport );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::OptionChanged
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void TerrainLayer::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "SmallText" )
        smallNames_ = value.To< TristateOption >();
    if( name == "BigText" )
        bigNames_ = value.To< TristateOption >();
    if( name == "3D" )
        pickingEnabled_ = ! value.To< bool >();

    const std::string strMinScale( "VisuScaleMin" );
    const std::string strMaxScale( "VisuScaleMax" );

    if( name.size() > strMinScale.size() && name.substr( 0, strMinScale.size() ) == strMinScale )
    {
        std::stringstream stream( name.substr( strMinScale.size() ) );
        int index = -1;
        stream >> index;
        if( index >= 0 && index < 13 )
            minVisuScale_[ index ] = value.To< int >();
    }

    if( name.size() > strMaxScale.size() && name.substr( 0, strMaxScale.size() ) == strMaxScale )
    {
        std::stringstream stream( name.substr( strMaxScale.size() ) );
        int index = -1;
        stream >> index;
        if( index >= 0 && index < 13 )
            maxVisuScale_[ index ] = value.To< int >();
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Reset
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void TerrainLayer::Reset()
{
    parameters_.Purge();
    layer_.reset();
    noVBOlayer_.reset();
}

namespace
{
    float PixelsPerMeter()
    {
        HDC screen = GetDC( NULL );
        const int hSize = GetDeviceCaps( screen, HORZSIZE );
        const int hRes = GetDeviceCaps( screen, HORZRES );
        ReleaseDC( NULL, screen );
        return 1000.f * hRes / hSize;
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::MyLayer
// Created: AGE 2007-05-10
// -----------------------------------------------------------------------------
template< typename Base >
class TerrainLayer::MyLayer : public Base
{
public:
    MyLayer( TerrainLayer& parent, const std::string& filename )
        : Base( parent.setup_, filename )
        , parent_( parent ) {}

    virtual bool ShouldDisplay( const TerrainData& data, const geometry::Rectangle2f& /*viewport*/ )
    {
        if( data.Border() )
            return true;

        const float scale = parent_.tools_.Pixels() * PixelsPerMeter();

        switch( data.Linear() )
        {
        case 0:
        default:
            return false;
        // see in VisualisationScalesPanel.cpp for indexes
        case TerrainData::cliff_:
            return parent_.minVisuScale_[ 3 ] <= scale && parent_.maxVisuScale_[ 3 ] > scale;
        case TerrainData::motorway_:
            return parent_.minVisuScale_[ 4 ] <= scale && parent_.maxVisuScale_[ 4 ] > scale;
        case TerrainData::largeroad_:
            return parent_.minVisuScale_[ 5 ] <= scale && parent_.maxVisuScale_[ 5 ] > scale;
        case TerrainData::mediumroad_:
            return parent_.minVisuScale_[ 6 ] <= scale && parent_.maxVisuScale_[ 6 ] > scale;
        case TerrainData::smallroad_:
            return parent_.minVisuScale_[ 7 ] <= scale && parent_.maxVisuScale_[ 7 ] > scale;
        case TerrainData::bridge_:
            return parent_.minVisuScale_[ 8 ] <= scale && parent_.maxVisuScale_[ 8 ] > scale;
        case TerrainData::railroad_:
            return parent_.minVisuScale_[ 9 ] <= scale && parent_.maxVisuScale_[ 9 ] > scale;
        case TerrainData::largeriver_:
            return parent_.minVisuScale_[ 10 ] <= scale && parent_.maxVisuScale_[ 10 ] > scale;
        case TerrainData::mediumriver_:
            return parent_.minVisuScale_[ 11 ] <= scale && parent_.maxVisuScale_[ 11 ] > scale;
        case TerrainData::smallriver_:
            return parent_.minVisuScale_[ 12 ] <= scale && parent_.maxVisuScale_[ 12 ] > scale;
        }
    }
    virtual bool ShouldDisplayBorder( const TerrainData& data, const geometry::Rectangle2f& /*viewport*/ )
    {
        const float scale = parent_.tools_.Pixels() * PixelsPerMeter();
        return data.Border() && parent_.minVisuScale_[ 2 ] <= scale && parent_.maxVisuScale_[ 2 ] > scale;
    }
    virtual bool ShouldDisplayNames( const TerrainData& data, const geometry::Rectangle2f& /*viewport*/ )
    {
        const float scale = parent_.tools_.Pixels() * PixelsPerMeter();
        return (  data.Linear() && parent_.smallNames_.IsSet( parent_.minVisuScale_[ 1 ] <= scale && parent_.maxVisuScale_[ 1 ] > scale ) )
            || ( !data.Linear() && parent_.bigNames_.IsSet( parent_.minVisuScale_[ 0 ] <= scale && parent_.maxVisuScale_[ 0 ] > scale ) );
    }
    virtual void DrawName( const geometry::Point2f& at, const std::string& name )
    {
        glPushAttrib( GL_CURRENT_BIT );
            glColor3f( 0, 0, 0 );
            parent_.tools_.Print( name, at );
        glPopAttrib();
    }

private:
    TerrainLayer& operator=( const TerrainLayer& );
    TerrainLayer& parent_;
};

// -----------------------------------------------------------------------------
// Name: TerrainLayer::LoadGraphics
// Created: AGE 2007-05-10
// -----------------------------------------------------------------------------
void TerrainLayer::LoadGraphics()
{
    world_.Set( 0, 0, parameters_.width_, parameters_.height_ );
    try
    {
        const bfs::path aggregated = bfs::path( parameters_.graphicsDirectory_, bfs::native ) / "shapes.dump";
        if( ! bfs::exists( aggregated ) )
        {
            DataFactory factory;
            ShapeCollector collector( factory );
            collector.LoadGraphicDirectory( parameters_.graphicsDirectory_ );
            collector.Finalize( aggregated.native_file_string() );
        }
        if( bfs::exists( aggregated ) )
        {
            if( gl::HasVBO() )
            {
                layer_.reset( new MyLayer< RawShapeLayer >  ( *this, aggregated.native_file_string() ) );
                layer_->Initialize( world_ );
            }
            else
            {
                noVBOlayer_.reset( new MyLayer< NoVBOShapeLayer >( *this, aggregated.native_file_string() ) );
                noVBOlayer_->Initialize( world_ );
            }
        }
    }
    catch( ... )
    {
        // $$$$ AGE 2007-05-10:
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::HandleMouseMove
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
bool TerrainLayer::HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& terrainCoordinates )
{
    picker_.Pick( mouse->x(), mouse->y(), terrainCoordinates );
    return false;
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Pick
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
TerrainData TerrainLayer::Pick( int x, int y )
{
    if( !pickingEnabled_ )
        return TerrainData();
    if( layer_.get() )
        return layer_->PickTerrain( x, y );
    else if( noVBOlayer_.get() )
        return noVBOlayer_->PickTerrain( x, y );
    return TerrainData();
}

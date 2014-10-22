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

#include "GLOptions.h"
#include "GLView_ABC.h"
#include "TerrainSettings.h"
#include "TerrainPicker.h"

#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/OptionsController.h"
#include "tools/ExerciseConfig.h"
#include "ENT/ENT_Tr.h"

#include <graphics/extensions.h>
#include <graphics/NoVBOShapeLayer.h>
#include <graphics/RawShapeLayer.h>
#include <graphics/ShapeCollector.h>
#include <terrain/DataFactory.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainLayer constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TerrainLayer::TerrainLayer( Controllers& controllers,
                            GLView_ABC& tools,
                            TerrainPicker& picker )
    : Layer2D( controllers, tools, eLayerTypes_Terrain )
    , picker_( picker )
    , pickingEnabled_( true )
{
    picker_.RegisterLayer( *this );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TerrainLayer::~TerrainLayer()
{
    picker_.UnregisterLayer( *this );
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::NotifyUpdated
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::NotifyUpdated( const ModelLoaded& modelLoaded )
{
    graphicsDirectory_ = modelLoaded.config_.GetGraphicsDirectory();
    width_ = modelLoaded.config_.GetTerrainWidth();
    height_ = modelLoaded.config_.GetTerrainHeight();
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void TerrainLayer::SetAlpha( float alpha )
{
    view_.GetOptions().GetTerrainSettings()->SetAlpha( alpha );
    Layer2D::SetAlpha( alpha );
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Paint
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void TerrainLayer::Paint( const geometry::Rectangle2f& viewport )
{
    const auto& options = view_.GetOptions();
    settings_ = options.GetTerrainSettings();
    if( !ShouldDrawPass() || GetAlpha() == 0 || !settings_ )
        return;
    if( !layer_ && !noVBOlayer_ && !graphicsDirectory_.IsEmpty() )
        LoadGraphics();
    if( !layer_ && !noVBOlayer_ )
        return;
    smallNames_ = options.Get( "SmallText" ).To< TristateOption >();
    bigNames_ = options.Get( "BigText" ).To< TristateOption >();
    pickingEnabled_ = !options.Get( "3D" ).To< bool >();
    for( int i = 0; i < eNbrVisualisationScale; ++i )
    {
        const auto name = "VisualisationScales/" + ENT_Tr::ConvertFromVisualisationScale( static_cast< E_VisualisationScale >( i ) );
        minVisuScale_[ i ] = options.Get( name + "/Min" ).To< int >();
        maxVisuScale_[ i ] = options.Get( name + "/Max" ).To< int >();
    }
    if( layer_ )
        layer_->SetGraphicSetup( settings_ );
    else
        noVBOlayer_->SetGraphicSetup( settings_ );
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable( GL_TEXTURE_GEN_S );
    glDisable( GL_TEXTURE_GEN_T );
    if( layer_ )
        layer_->Paint( viewport );
    else
        noVBOlayer_->Paint( viewport );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Reset
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void TerrainLayer::Reset()
{
    width_ = 0;
    height_ = 0;
    graphicsDirectory_.Clear();
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
    MyLayer( TerrainLayer& parent, const tools::Path& filename )
        : Base( parent.settings_, filename )
        , parent_( parent ) {}

    virtual bool ShouldDisplay( const TerrainData& data, const geometry::Rectangle2f& /*viewport*/ )
    {
        if( data.Border() )
            return true;

        const float scale = parent_.view_.Pixels() * PixelsPerMeter();

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
        const float scale = parent_.view_.Pixels() * PixelsPerMeter();
        return data.Border() && parent_.minVisuScale_[ 2 ] <= scale && parent_.maxVisuScale_[ 2 ] > scale;
    }
    virtual bool ShouldDisplayNames( const TerrainData& data, const geometry::Rectangle2f& /*viewport*/ )
    {
        const float scale = parent_.view_.Pixels() * PixelsPerMeter();
        return (  data.Linear() && parent_.smallNames_.IsSet( parent_.minVisuScale_[ 1 ] <= scale && parent_.maxVisuScale_[ 1 ] > scale ) )
            || ( !data.Linear() && parent_.bigNames_.IsSet( parent_.minVisuScale_[ 0 ] <= scale && parent_.maxVisuScale_[ 0 ] > scale ) );
    }
    virtual void DrawName( const geometry::Point2f& at, const std::string& name )
    {
        glPushAttrib( GL_CURRENT_BIT );
            glColor3f( 0, 0, 0 );
            parent_.view_.Print( name, at );
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
    world_.Set( 0, 0, width_, height_ );
    try
    {
        const tools::Path aggregated = graphicsDirectory_ / "shapes.dump";
        if( !aggregated.Exists() )
        {
            DataFactory factory;
            ShapeCollector collector( factory );
            collector.LoadGraphicDirectory( graphicsDirectory_ );
            collector.Finalize( aggregated );
        }
        else
        {
            if( gl::HasVBO() )
            {
                layer_.reset( new MyLayer< RawShapeLayer >  ( *this, aggregated ) );
                layer_->Initialize( world_ );
            }
            else
            {
                noVBOlayer_.reset( new MyLayer< NoVBOShapeLayer >( *this, aggregated ) );
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
    if( layer_ )
        return layer_->PickTerrain( x, y );
    else if( noVBOlayer_ )
        return noVBOlayer_->PickTerrain( x, y );
    return TerrainData();
}

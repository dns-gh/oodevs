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
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/OptionVariant.h"
#include "graphics/RawShapeLayer.h"
#include "graphics/NoVBOShapeLayer.h"
#include "graphics/ShapeCollector.h"
#include "graphics/extensions.h"
#include "graphics/DataFactory.h"

#include <boost/filesystem/operations.hpp>
namespace bfs = boost::filesystem;

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainLayer constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
TerrainLayer::TerrainLayer( Controllers& controllers, const GlTools_ABC& tools, GraphicPreferences& setup )
    : controllers_  ( controllers )
    , tools_        ( tools )
    , setup_        ( setup )
{
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

    if( !layer_.get() && !noVBOlayer_.get() && !parameters_.graphicsDirectory_.empty() )
        LoadGraphics();

    if( layer_.get() || noVBOlayer_.get() )
    {
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT );
            glBindTexture( GL_TEXTURE_2D, 0 );
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
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
}

// -----------------------------------------------------------------------------
// Name: TerrainLayer::Reset
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void TerrainLayer::Reset()
{
    layer_.reset();
    noVBOlayer_.reset();
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

    virtual bool ShouldDisplay( const TerrainData& data, const geometry::Rectangle2f& viewport )
    {
        if( data.Border() )
            return true;
        const float width = std::max( viewport.Width(), viewport.Height() );
        switch( data.Linear() )
        {
        case 0:
        default:
            return false;
        case TerrainData::cliff_:
        case TerrainData::smallriver_:
        case TerrainData::smallroad_:
        case TerrainData::railroad_:
            return width < 100000.f;
        case TerrainData::mediumriver_:
        case TerrainData::mediumroad_:
        case TerrainData::bridge_:
            return width < 150000.f;
        case TerrainData::largeriver_:
        case TerrainData::largeroad_:
        case TerrainData::motorway_:
            return true;
        }
    }
    virtual bool ShouldDisplayBorder( const TerrainData& data, const geometry::Rectangle2f& viewport )
    {
        const float width = std::max( viewport.Width(), viewport.Height() );
        return data.Border() && width < 200000.f;
    }
    virtual bool ShouldDisplayNames( const TerrainData& data, const geometry::Rectangle2f& viewport )
    {
        const float width = std::max( viewport.Width(), viewport.Height() );
        return (  data.Linear() && parent_.smallNames_.IsSet( width < 10000.f ) )
            || ( !data.Linear() && parent_.bigNames_.IsSet( width < 30000.f ) );
    }
    virtual void DrawName( const geometry::Point2f& at, const std::string& name )
    {
        glColor3f( 0, 0, 0 );
        parent_.tools_.Print( name, at );
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
                layer_     .reset( new MyLayer< RawShapeLayer >  ( *this, aggregated.native_file_string() ) );
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

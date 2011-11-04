// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ContourLinesLayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/GLTools_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/OptionVariant.h"
#include <graphics/extensions.h>
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
// Name: ContourLinesLayer constructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
ContourLinesLayer::ContourLinesLayer( kernel::Controllers& controllers )
    : controllers_    ( controllers )
    , reset_          ( false )
    , modelLoaded_    ( false )
    , enabled_        ( false )
    , color_          ( 245, 245, 220 )
    , setup_          ( *new ContourGraphicSetup() )
{
    setup_.SetColor( color_.red() / 256.f, color_.green() / 256.f, color_.blue() / 256.f );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer destructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
ContourLinesLayer::~ContourLinesLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void ContourLinesLayer::NotifyUpdated( const ModelLoaded& modelLoaded )
{
    parameters_.Load( modelLoaded.config_ );
    LoadGraphics();
    reset_       = true;
    modelLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void ContourLinesLayer::SetAlpha( float alpha )
{
    setup_.alpha_ = alpha ;
    Layer2d_ABC::SetAlpha( alpha );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::OptionChanged
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void ContourLinesLayer::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "ContourLinesColor" )
    {
        color_.setNamedColor( value.To< QString >() );
        setup_.SetColor( color_.red() / 256.f, color_.green() / 256.f, color_.blue() / 256.f );
    }
    else if( name == "ContourLinesEnabled" )
        enabled_ = value.To< bool >();
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void ContourLinesLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !ShouldDrawPass() || GetAlpha() == 0 || !enabled_  )
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
// Name: ContourLinesLayer::Reset
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void ContourLinesLayer::Reset()
{
    parameters_.Purge();
    layer_.reset();
    noVBOlayer_.reset();
    modelLoaded_ = false;
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::LoadGraphics
// Created: AGE 2007-05-10
// -----------------------------------------------------------------------------
void ContourLinesLayer::LoadGraphics()
{
    world_.Set( 0, 0, parameters_.width_, parameters_.height_ );
    try
    {
        const bfs::path aggregated = bfs::path( parameters_.detectionDirectory_, bfs::native ) / "shapes.dump";
        if( ! bfs::exists( aggregated ) )
        {
            DataFactory factory;
            ShapeCollector collector( factory );
            collector.LoadGraphicFile( ( bfs::path( parameters_.detectionDirectory_, bfs::native ) / "contours.bin" ).string() );
            collector.Finalize( aggregated.native_file_string() );
        }
        if( bfs::exists( aggregated ) )
        {
            if( gl::HasVBO() )
            {
                layer_.reset( new RawShapeLayer( setup_, aggregated.native_file_string() ) );
                layer_->Initialize( world_ );
            }
            else
            {
                noVBOlayer_.reset( new NoVBOShapeLayer( setup_, aggregated.native_file_string() ) );
                noVBOlayer_->Initialize( world_ );
            }
        }
    }
    catch( ... )
    {
        // $$$$ AGE 2007-05-10:
    }
}
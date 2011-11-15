// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SvglRenderer.h"
#include <svgl/svgl.h>
#include <svgl/Opacity.h>
#include <svgl/ListPaint.h>
#include <svgl/ListLength.h>
#include <svgl/ListLengthFactory.h>
#include <svgl/PropertyFactory.h>
#include <svgl/Style.h>
#include <xeumeuleu/xml.hpp>

using namespace geometry;
using namespace gui;
using namespace svg;

std::auto_ptr< TextRenderer > SvglRenderer::renderer_( new TextRenderer() );
unsigned int SvglRenderer::colorList_ = 0;

// -----------------------------------------------------------------------------
// Name: SvglRenderer constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
SvglRenderer::SvglRenderer()
    : current_         ( new Color( "black" ) )
    , opacity_         ( new Opacity() )
    , references_      ( new References() )
    , renderingContext_( new RenderingContext() )
    , listLenghts_     ( new ListLengthFactory() )
    , colorDirty_      ( true )
{
    r_ = g_ = b_ = a_ = 1;
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer destructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
SvglRenderer::~SvglRenderer()
{
    for( CIT_Lists it = lists_.begin(); it != lists_.end(); ++it )
        glDeleteLists( it->second, 1 );
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::SetCurrentColor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void SvglRenderer::SetCurrentColor( float r, float g, float b, float a )
{
    if( r!=r_ || g_!=g || b_!=b || a_!=a )
    {
        r_ = r; g_ = g; b_ = b; a_ = a;
        colorDirty_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::DefaultStyle
// Created: AGE 2008-05-07
// -----------------------------------------------------------------------------
std::string SvglRenderer::DefaultStyle()
{
    return "stroke:black;fill:currentColor;stroke-width:4";
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::Compile
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
svg::Node_ABC* SvglRenderer::Compile( xml::xistream& input, float lod )
{
    CreateStaticLists();
    SVGFactory factory( *renderer_ );
    factory.ChangePropertyFactory( svg::RenderingContext_ABC::strokeWidth, *listLenghts_ );
    std::auto_ptr< Style > border( CreateStyle( "" ) );
    references_->Register( "border", *border );
    return factory.Compile( input, *references_, lod );
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::Render
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void SvglRenderer::Render( svg::Node_ABC* node, const std::string& style, const geometry::Rectangle2f& viewport, unsigned vWidth, unsigned vHeight )
{
    CreateStaticLists();

    // $$$$ JSR 2011-10-12: HACK temporaire : Désactivation des display lists, on dessiner les symboles pour contourner le bug 6421. A investiguer...
    ConfigureColorList();
    ConfigureWidthList( viewport, vWidth, vHeight );
    const BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
    ListPaint color( colorList_ );
    renderingContext_->SetViewport( box, vWidth, vHeight );
    renderingContext_->PushProperty( RenderingContext::color, color );
    std::auto_ptr< Style > border( CreateStyle( style ) );
    references_->Register( "border", *border );
    node->Draw( *renderingContext_, *references_ );
    renderingContext_->PopProperty( RenderingContext::color );
    /*unsigned int listId = 0;
    CIT_Lists it = lists_.find( node );
    if( it == lists_.end() )
    {
        listId = GenerateList( node, style, viewport, vWidth, vHeight );
        if( !listId )
            return;
        lists_[ node ] = listId;
    }
    else
        listId = it->second;

    ConfigureColorList();
    ConfigureWidthList( viewport, vWidth, vHeight );
    glCallList( listId );*/
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::GenerateList
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
unsigned int SvglRenderer::GenerateList( svg::Node_ABC* node, const std::string& style, const geometry::Rectangle2f& viewport, unsigned vWidth, unsigned vHeight )
{
    unsigned int result = glGenLists( 1 );
    if( result )
    {
        glNewList( result, GL_COMPILE );
            const BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
            ListPaint color( colorList_ );
            renderingContext_->SetViewport( box, vWidth, vHeight );
            renderingContext_->PushProperty( RenderingContext::color, color );
            std::auto_ptr< Style > border( CreateStyle( style ) );
            references_->Register( "border", *border );
            node->Draw( *renderingContext_, *references_ );
            renderingContext_->PopProperty( RenderingContext::color );
        glEndList();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::ConfigureColorList
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void SvglRenderer::ConfigureColorList()
{
    if( colorDirty_ )
    {
        glNewList( colorList_, GL_COMPILE );
        glColor4fv( &r_ );
        glEndList();
        colorDirty_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::ConfigureWidthList
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void SvglRenderer::ConfigureWidthList( const geometry::Rectangle2f& viewport, unsigned vWidth, unsigned vHeight )
{
    if( viewport != previousViewport_ || vWidth   != previousWidth_ || vHeight  != previousHeight_ )
    {
        const BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
        listLenghts_->SetViewport( box, vWidth, vHeight );
        previousViewport_ = viewport;
        previousWidth_    = vWidth;
        previousHeight_   = vHeight;
    }
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< Style > SvglRenderer::CreateStyle
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
std::auto_ptr< Style > SvglRenderer::CreateStyle( const std::string& style )
{
    PropertyFactory factory;
    factory.ChangeFactory( RenderingContext_ABC::strokeWidth, *listLenghts_ );
    return std::auto_ptr< Style >( new Style( style, factory ) );
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::CreateStaticLists
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void SvglRenderer::CreateStaticLists()
{
    if( ! colorList_ )
    {
        renderer_->InitializeFont( "Arial", 700 ); // $$$$ AGE 2007-05-24:
        colorList_ = glGenLists( 1 );
    }
}

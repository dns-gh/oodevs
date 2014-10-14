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

std::unique_ptr< TextRenderer > SvglRenderer::renderer_( new TextRenderer() );
unsigned int SvglRenderer::colorList_ = 0;

// -----------------------------------------------------------------------------
// Name: SvglRenderer constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
SvglRenderer::SvglRenderer()
    : references_      ( new References() )
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
    for( auto it = lists_.begin(); it != lists_.end(); ++it )
        glDeleteLists( it->second, 1 );
    if( colorList_ )
    {
        glDeleteLists( colorList_, 1 );
        colorList_ = 0;
        renderer_.reset( new TextRenderer() );
    }
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::SetCurrentColor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void SvglRenderer::SetCurrentColor( float r, float g, float b, float a )
{
    if( r != r_ || g_ != g || b_ != b || a_ != a )
    {
        r_ = r;
        g_ = g;
        b_ = b;
        a_ = a;
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
    std::unique_ptr< Style > border( CreateStyle( "" ) );
    references_->Register( "border", *border );
    return factory.Compile( input, *references_, lod );
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::Render
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void SvglRenderer::Render( const std::shared_ptr< svg::Node_ABC >& node, const std::string& style, const geometry::Rectangle2f& viewport,
                           unsigned vWidth, unsigned vHeight, bool pickingMode )
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
    CreateStaticLists();
    if( pickingMode )
    {
        ConfigureColorList();
        ConfigureWidthList( viewport, vWidth, vHeight );
        Draw( node, style, viewport, vWidth, vHeight, true );
        return;
    }
    unsigned int listId = RetrieveListId( node, style, viewport, vWidth, vHeight, lists_ );
    if( !listId )
        return;
    ConfigureColorList();
    ConfigureWidthList( viewport, vWidth, vHeight );
    glCallList( listId );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::RetrieveListId
// Created: LGY 2013-03-07
// -----------------------------------------------------------------------------
unsigned int SvglRenderer::RetrieveListId( const std::shared_ptr< svg::Node_ABC >& node,
    const std::string& style, const geometry::Rectangle2f& viewport,
    unsigned vWidth, unsigned vHeight, T_Lists& lists )
{
    auto it = lists.find( node );
    if( it != lists.end() && !colorDirty_ )
        return it->second;
    const auto listId = it == lists.end() ? glGenLists( 1 ) : it->second;
    if( listId )
    {
        glNewList( listId, GL_COMPILE );
        Draw( node, style, viewport, vWidth, vHeight, false );
        glEndList();
        lists[ node ] = listId;
    }
    return listId;
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::Draw
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void SvglRenderer::Draw( const std::shared_ptr< svg::Node_ABC >& node, const std::string& style, const geometry::Rectangle2f& viewport, unsigned vWidth, unsigned vHeight, bool pickingMode )
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    const BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
    ListPaint color( colorList_ );
    if( pickingMode )
        renderingContext_->EnablePickingMode( 5.f );
    renderingContext_->SetViewport( box, vWidth, vHeight );
    renderingContext_->PushProperty( RenderingContext::color, color );
    svg::Opacity opacity( a_ );
    renderingContext_->PushProperty( svg::RenderingContext_ABC::fillOpacity, opacity );
    renderingContext_->PushProperty( svg::RenderingContext_ABC::strokeOpacity, opacity );
    std::unique_ptr< Style > border( CreateStyle( style ) );
    references_->Register( "border", *border );
    node->Draw( *renderingContext_, *references_ );
    renderingContext_->DisablePickingMode();
    renderingContext_->PopProperty( svg::RenderingContext_ABC::strokeOpacity );
    renderingContext_->PopProperty( svg::RenderingContext_ABC::fillOpacity );
    renderingContext_->PopProperty( RenderingContext::color );
    glPopAttrib();
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
// Name: SvglRenderer::CreateStyle
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
std::unique_ptr< Style > SvglRenderer::CreateStyle( const std::string& style )
{
    PropertyFactory factory;
    factory.ChangeFactory( RenderingContext_ABC::strokeWidth, *listLenghts_ );
    return std::unique_ptr< Style >( new Style( style, factory ) );
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::CreateStaticLists
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void SvglRenderer::CreateStaticLists()
{
    if( ! colorList_ )
    {
        renderer_->InitializeFont( "Arial", 700 );
        colorList_ = glGenLists( 1 );
    }
}

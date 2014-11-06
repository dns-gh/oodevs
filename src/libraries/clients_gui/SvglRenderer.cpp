// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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

// -----------------------------------------------------------------------------
// Name: SvglRenderer constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
SvglRenderer::SvglRenderer()
    : references_      ( new References() )
    , renderingContext_( new RenderingContext() )
    , listLenghts_     ( new ListLengthFactory() )
    , previousWidth_   ( 0 )
    , previousHeight_  ( 0 )
    , colorList_       ( 0 )
    , r_               ( 1 )
    , g_               ( 1 )
    , b_               ( 1 )
    , a_               ( 1 )
    , colorDirty_      ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer destructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
SvglRenderer::~SvglRenderer()
{
    for( auto it = lists_.begin(); it != lists_.end(); ++it )
        glDeleteLists( it->second.first, 1 );
    glDeleteLists( colorList_, 1 );
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::SetCurrentColor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void SvglRenderer::SetCurrentColor( float r, float g, float b, float a )
{
    // This stands for the 'currentColor' of SVG
    if( r != r_ || g_ != g || b_ != b )
    {
        r_ = r;
        g_ = g;
        b_ = b;
        colorDirty_ = true;
    }
    a_ = a;
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
    if( !node )
        return;
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    CreateStaticLists();
    if( pickingMode )
    {
        ConfigureColorList();
        ConfigureWidthList( viewport, vWidth, vHeight );
        Draw( node, style, viewport, vWidth, vHeight, true );
    }
    else if( auto listId = RetrieveListId( node, style, viewport, vWidth, vHeight ) )
    {
        ConfigureColorList();
        ConfigureWidthList( viewport, vWidth, vHeight );
        glCallList( listId );
    }
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::RetrieveListId
// Created: LGY 2013-03-07
// -----------------------------------------------------------------------------
unsigned int SvglRenderer::RetrieveListId( const std::shared_ptr< svg::Node_ABC >& node,
    const std::string& style, const geometry::Rectangle2f& viewport,
    unsigned vWidth, unsigned vHeight )
{
    auto it = lists_.find( node );
    // Alpha cannot simply be injected into the compiled display lists
    // so just rebuild them whenever it changes.
    if( it != lists_.end() && it->second.second == a_ )
        return it->second.first;
    const auto listId = it == lists_.end() ? glGenLists( 1 ) : it->second.first;
    if( listId )
    {
        glNewList( listId, GL_COMPILE );
        Draw( node, style, viewport, vWidth, vHeight, false );
        glEndList();
        lists_[ node ] = std::make_pair( listId, a_ );
    }
    return listId;
}

// -----------------------------------------------------------------------------
// Name: SvglRenderer::Draw
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void SvglRenderer::Draw( const std::shared_ptr< svg::Node_ABC >& node, const std::string& style,
    const geometry::Rectangle2f& viewport, unsigned vWidth, unsigned vHeight, bool pickingMode )
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    const BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
    ListPaint color( colorList_ );
    if( pickingMode )
        renderingContext_->EnablePickingMode( 5.f );
    renderingContext_->SetViewport( box, vWidth, vHeight );
    renderingContext_->PushProperty( RenderingContext::color, color );
    renderingContext_->SetOpacity( a_ );
    std::unique_ptr< Style > border( CreateStyle( style ) );
    references_->Register( "border", *border );
    node->Draw( *renderingContext_, *references_ );
    renderingContext_->DisablePickingMode();
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
        glColor4f( r_, g_, b_, a_ );
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
    if( viewport != previousViewport_ || vWidth != previousWidth_ || vHeight  != previousHeight_ )
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
    if( !renderer_ )
    {
        renderer_.reset( new TextRenderer() );
        renderer_->InitializeFont( "Arial", 700 );
    }
    if( !colorList_ )
        colorList_ = glGenLists( 1 );
}

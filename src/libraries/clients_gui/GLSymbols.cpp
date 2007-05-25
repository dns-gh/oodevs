// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GLSymbols.h"
#include "tools/GeneralConfig.h"
#include "svgl/svgl.h"
#include "svgl/Opacity.h"
#include "svgl/ListPaint.h"
#include "svgl/ListLength.h"
#include "svgl/ListLengthFactory.h"
#include "zipstream/zipstream.h"
#include "xeumeuleu/xml.h"

using namespace geometry;
using namespace gui;
using namespace svg;

// -----------------------------------------------------------------------------
// Name: GLSymbols constructor
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
GLSymbols::GLSymbols()
    : zipFile_( new zip::izipfile( tools::GeneralConfig::BuildWorkingDirectoryChildFile( "symbols.pak" ).c_str() ) )
    , current_( new Color( "black" ) )
    , opacity_( new Opacity() )
    , renderer_( new TextRenderer() )
    , references_( new References() )
    , renderingContext_( new RenderingContext() )
    , listLenghts_( new ListLengthFactory( 1, 3, 5 ) )
    , colorList_  ( 0 )
    , colorDirty_ ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GLSymbols destructor
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
GLSymbols::~GLSymbols()
{
    for( CIT_Symbols it = symbols_.begin(); it != symbols_.end(); ++it )
    {
        delete it->second.first;
        delete it->second.second;
    }
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::PrintApp6
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
void GLSymbols::PrintApp6( const std::string& symbol, const geometry::Rectangle2f& viewport, unsigned vWidth /*= 640*/, unsigned vHeight /*= 480*/ )
{
    CreateStaticLists();

    const bool create = ! symbol.empty() && symbols_.find( symbol ) == symbols_.end();
    T_LodSymbol& node = symbols_[ symbol ];
    if( create )
    {
        // $$$$ AGE 2006-09-11: error management !
        const std::string filename = symbol + ".svg";
        try
        {
            node.first  = Compile( filename, 10 );
            node.second = Compile( filename, 100 );
        }
        catch( ... )
        {
            std::cout << "Could not open svg symbol '" << filename << "'" << std::endl;// $$$$ AGE 2006-10-23:
        };
    }
    Node_ABC* renderNode = viewport.Width() > 30000 ? node.second : node.first;  // $$$$ AGE 2006-09-11: hardcoded lod
    if( renderNode )
        Render( renderNode, viewport, vWidth, vHeight );
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::Compile
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
svg::Node_ABC* GLSymbols::Compile( const std::string& filename, float lod ) const
{
    SVGFactory factory( *renderer_ );
    factory.ChangePropertyFactory( svg::RenderingContext_ABC::strokeWidth, *listLenghts_ );
    zip::izipstream zipStream( *zipFile_, filename.c_str() );
    xml::xistreamstream xis( zipStream );
    return factory.Compile( xis, *references_, lod );
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::Render
// Created: AGE 2007-05-24
// -----------------------------------------------------------------------------
void GLSymbols::Render( svg::Node_ABC* renderNode, const geometry::Rectangle2f& viewport, unsigned vWidth, unsigned vHeight )
{
    unsigned int& list = lists_[ renderNode ];
    if( ! list )
        list = GenerateList( renderNode, viewport, vWidth, vHeight );
    if( list )
    {
        ConfigureColorList();
        ConfigureWidthList( viewport, vWidth, vHeight );
        glCallList( list );
    }
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::CreateStaticLists
// Created: AGE 2007-05-24
// -----------------------------------------------------------------------------
void GLSymbols::CreateStaticLists()
{
    if( ! colorList_ )
    {
        renderer_->InitializeFont( "Arial", 700 ); // $$$$ AGE 2007-05-24:
        colorList_ = glGenLists( 1 );
        listLenghts_->InitializeLists();
    }
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::SetCurrentColor
// Created: AGE 2007-05-25
// -----------------------------------------------------------------------------
void GLSymbols::SetCurrentColor( float r, float g, float b, float a )
{
    if( r!=r_ || g_!=g || b_!=b || a_!=a )
    {
        r_ = r; g_ = g; b_ = b; a_ = a;
        colorDirty_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::ConfigureColorList
// Created: AGE 2007-05-24
// -----------------------------------------------------------------------------
void GLSymbols::ConfigureColorList()
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
// Name: GLSymbols::ConfigureWidthList
// Created: AGE 2007-05-25
// -----------------------------------------------------------------------------
void GLSymbols::ConfigureWidthList( const geometry::Rectangle2f& viewport, unsigned vWidth, unsigned vHeight )
{
    if( viewport != previousViewport_ 
     || vWidth   != previousWidth_
     || vHeight  != previousHeight_ )
    {   
        const BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
        listLenghts_->SetViewport( box, vWidth, vHeight );
        previousViewport_ = viewport;
        previousWidth_    = vWidth;
        previousHeight_   = vHeight;
    }
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::GenerateList
// Created: AGE 2007-05-24
// -----------------------------------------------------------------------------
unsigned int GLSymbols::GenerateList( svg::Node_ABC* renderNode, const geometry::Rectangle2f& viewport, unsigned vWidth, unsigned vHeight )
{
    unsigned int result = glGenLists( 1 );
    if( result )
    {
        glNewList( result, GL_COMPILE );
            const BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
            ListPaint color( colorList_ );
            renderingContext_->SetViewport( box, vWidth, vHeight );
            renderingContext_->PushProperty( RenderingContext::color, color );
            renderNode->Draw( *renderingContext_, *references_ );
            renderingContext_->PopProperty( RenderingContext::color );
        glEndList();
    }
    return result;
}

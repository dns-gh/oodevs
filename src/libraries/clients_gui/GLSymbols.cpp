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
#include "svgl/svgl.h"
#include "zipstream/zipstream.h"
#include "clients_kernel/PathTools.h"
#include "xeumeuleu/xml.h"

using namespace geometry;
using namespace gui;
using namespace svg;

// -----------------------------------------------------------------------------
// Name: GLSymbols constructor
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
GLSymbols::GLSymbols()
    : zipFile_( new zip::izipfile( kernel::path_tools::BuildWorkingDirectoryPath( "symbols.pak" ).c_str() ) )
    , current_( new Color( "black" ) )
    , renderer_( new TextRenderer() )
    , references_( new References() )
    , renderingContext_( new RenderingContext() )
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

namespace
{
    void SetCurrentColor( Color& col )
    {
        // $$$$ AGE 2006-10-25: pas terrible
        float color[4];
        glGetFloatv( GL_CURRENT_COLOR, color );
        col.Set( color[0], color[1], color[2] );
    }
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::Compile
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
svg::Node_ABC* GLSymbols::Compile( const std::string& filename, float lod ) const
{
    SVGFactory factory( *renderer_ );
    zip::izipstream zipStream( *zipFile_, filename.c_str() );
    xml::xistreamstream xis( zipStream );
    return factory.Compile( xis, *references_, lod );
}

// $$$$ AGE 2007-02-09: cette méthode est un champ de $

// -----------------------------------------------------------------------------
// Name: GLSymbols::PrintApp6
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
void GLSymbols::PrintApp6( const std::string& symbol, const geometry::Rectangle2f& viewport, unsigned vWidth /*= 640*/, unsigned vHeight /*= 480*/ )
{
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
    {
        const BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
        SetCurrentColor( *current_ ); // $$$$ AGE 2006-10-25: 
        renderingContext_->SetViewport( box, vWidth, vHeight );
        renderingContext_->PushProperty( RenderingContext::color, *current_ );
        renderNode->Draw( *renderingContext_, *references_ );
        renderingContext_->PopProperty( RenderingContext::color );
    }
}

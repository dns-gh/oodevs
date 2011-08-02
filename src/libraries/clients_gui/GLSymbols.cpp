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
#include "SvglRenderer.h"
#include "tools/GeneralConfig.h"
#pragma warning( push, 0 )
#include <zipstream/zipstream.h>
#pragma warning( pop )
#include <svgl/Node_ABC.h>
#include <xeumeuleu/xml.hpp>

using namespace geometry;
using namespace gui;
using namespace svg;

// -----------------------------------------------------------------------------
// Name: GLSymbols constructor
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
GLSymbols::GLSymbols( SvglRenderer& renderer )
    : renderer_( renderer )
    , zipFile_ ( new zip::izipfile( tools::GeneralConfig::BuildResourceChildFile( "symbols.pak" ).c_str() ) )
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
void GLSymbols::PrintApp6( const std::string& symbol, const std::string& style, const geometry::Rectangle2f& viewport, unsigned vWidth /* = 640*/, unsigned vHeight /* = 480*/ )
{
    const T_SymbolKey key( symbol, style );
    const bool create = ! symbol.empty() && symbols_.find( key ) == symbols_.end();
    T_LodSymbol& node = symbols_[ key ];
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
        }
    }
    Node_ABC* renderNode = viewport.Width() > 30000 ? node.second : node.first;  // $$$$ AGE 2006-09-11: hardcoded lod
    if( renderNode )
        renderer_.Render( renderNode, style, viewport, vWidth, vHeight );
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::Compile
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
svg::Node_ABC* GLSymbols::Compile( const std::string& filename, float lod ) const
{
    zip::izipstream zipStream( *zipFile_, filename.c_str() );
    xml::xistreamstream xis( zipStream );
    return renderer_.Compile( xis, lod );
}

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
#include "MT_Tools/MT_Logger.h"
#include <tools/Zip.h>
#include <svgl/Node_ABC.h>

using namespace geometry;
using namespace gui;
using namespace svg;

GLSymbols::GLSymbols( SvglRenderer& renderer, const tools::Path& symbolPath )
    : renderer_( renderer )
    , symbolPath_( symbolPath )
    , archive_( new tools::zip::InputArchive( tools::GeneralConfig::BuildResourceChildFile( "symbols.pak" ) ) )
{
    // NOTHING
}

GLSymbols::~GLSymbols()
{
    // NOTHING
}

void GLSymbols::PrintApp6( const std::string& symbol, const std::string& style, const geometry::Rectangle2f& viewport,
                           unsigned vWidth /* = 640*/, unsigned vHeight /* = 480*/, bool pickingMode /* = false*/ )
{
    if( symbol.empty() )
        return;
    const T_SymbolKey key( symbol, style );
    const bool create = symbols_.find( key ) == symbols_.end();
    auto& node = symbols_[ key ];
    if( create )
    {
        node.first.reset( Compile( symbol, 10, true ) );
        node.second.reset( Compile( symbol, 100, false ) );
        if( !node.first || !node.second )
            MT_LOG_ERROR_MSG( "Could not open svg symbol '" << symbol << ".svg', and cannot find the closest symbol." );
    }
    const auto& renderNode = viewport.Width() > 30000 ? node.second : node.first;  // $$$$ AGE 2006-09-11: hardcoded lod
    renderer_.Render( renderNode, style, viewport, vWidth, vHeight, pickingMode );
}

svg::Node_ABC* GLSymbols::Compile( const std::string& symbol, float lod, bool first )
{
    for( std::size_t i = symbol.size(); i > 0; --i )
    {
        auto* node = Compile( symbol.substr( 0, i ), lod );
        if( node )
            return node;
        if( first )
        {
            notFoundSymbols_.push_back( symbol.substr( 8, symbol.size() - 8 ) );
            first = false;
        }
    }
    return 0;
}

svg::Node_ABC* GLSymbols::Compile( const std::string& symbol, float lod ) const
{
    const auto symbolFile = tools::Path::FromUTF8( symbol ) + ".svg";
    if( symbolPath_.Exists() )
    {
        const auto fullPath = symbolPath_ / symbolFile;
        if( !fullPath.Exists() )
            return 0;
        tools::Xifstream xis( fullPath );
        return renderer_.Compile( xis, lod );
    }
    svg::Node_ABC* node = 0;
    archive_->ReadFile( symbolFile,
        [&]( std::istream& s )
        {
            xml::xistreamstream xis( s );
            node = renderer_.Compile( xis, lod );
        } );
    return node;
}

const std::vector< std::string >& GLSymbols::GetNotFoundSymbol() const
{
    return notFoundSymbols_;
}

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
        try
        {
            node.first.reset( Compile( symbol, 10, true ) );
            node.second.reset( Compile( symbol, 100, false ) );
        }
        catch( ... )
        {
            MT_LOG_ERROR_MSG( "Could not open svg symbol '" << symbol << ".svg', and cannot find the closest symbol." );
        }
    }
    const auto& renderNode = viewport.Width() > 30000 ? node.second : node.first;  // $$$$ AGE 2006-09-11: hardcoded lod
    renderer_.Render( renderNode, style, viewport, vWidth, vHeight, pickingMode );
}

svg::Node_ABC* GLSymbols::Compile( std::string symbol, float lod, bool first )
{
    while( symbol.size() )
    {
        try
        {
            const tools::Path symbolFile = tools::Path::FromUTF8( symbol ) + ".svg";
            if( !symbolPath_.IsEmpty() && symbolPath_.Exists() )
            {
                tools::Path symbolPath = symbolPath_ / symbolFile;
                tools::Xifstream xis( symbolPath );
                return renderer_.Compile( xis, lod );
            }
            else
            {
                svg::Node_ABC* node = 0;
                if( !archive_->ReadFile( symbolFile,
                    [&]( std::istream& s )
                    {
                        xml::xistreamstream xis( s );
                        node = renderer_.Compile( xis, lod );
                    } ) )
                    throw "not found";
                return node;
            }
        }
        catch( ... )
        {
            if( first )
            {
                notFoundSymbols_.push_back( symbol.substr( 8, symbol.size() - 8 ) );
                first = false;
            }
            symbol.pop_back();
        }
    }
    throw "not found";
}

const std::vector< std::string >& GLSymbols::GetNotFoundSymbol() const
{
    return notFoundSymbols_;
}

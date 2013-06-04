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
#include "tools/ExerciseConfig.h"
#pragma warning( push, 0 )
#include <zipstream/zipstream.h>
#pragma warning( pop )
#include <svgl/Node_ABC.h>
#include <zipstream/zipstream.h>

using namespace geometry;
using namespace gui;
using namespace svg;

// -----------------------------------------------------------------------------
// Name: GLSymbols constructor
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
GLSymbols::GLSymbols( SvglRenderer& renderer )
    : renderer_( renderer )
    , zipFile_ ( new zip::izipfile( tools::GeneralConfig::BuildResourceChildFile( "symbols.pak" ).ToUnicode() ) )
    , symbolsPath_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GLSymbols destructor
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
GLSymbols::~GLSymbols()
{
    for( auto it = symbols_.begin(); it != symbols_.end(); ++it )
    {
        delete it->second.first;
        delete it->second.second;
    }
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::PrintApp6
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
void GLSymbols::PrintApp6( const std::string& symbol, const std::string& style, const geometry::Rectangle2f& viewport,
                           unsigned vWidth /* = 640*/, unsigned vHeight /* = 480*/, bool pickingMode /* = false*/ )
{
    const T_SymbolKey key( symbol, style );
    auto it = alphaSymbols_.find( key );
    const bool create = ( !symbol.empty() && ( symbols_.find( key ) == symbols_.end() ) ) ||
                        ( !pickingMode && it != alphaSymbols_.end() && it->second != alpha_ );
    T_LodSymbol& node = symbols_[ key ];
    if( create )
    {
        try
        {
            node.first  = Compile( symbol, 10, true );
            node.second = Compile( symbol, 100, false );
            alphaSymbols_[ key ] = alpha_;
        }
        catch( ... )
        {
            std::cout << "Could not open svg symbol '" << symbol << ".svg', and cannot find the closest symbol." << std::endl;
        }
    }
    Node_ABC* renderNode = viewport.Width() > 30000 ? node.second : node.first;  // $$$$ AGE 2006-09-11: hardcoded lod
    if( renderNode )
        renderer_.Render( renderNode, style, viewport, vWidth, vHeight, pickingMode );
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::Compile
// Created: SBO 2006-12-15
// -----------------------------------------------------------------------------
svg::Node_ABC* GLSymbols::Compile( std::string symbol, float lod, bool firstNode )
{
    bool firstTime = true;
    while( symbol.size() )
    {
        try
        {
            const tools::Path symbolFile = tools::Path::FromUTF8( symbol ) + ".svg";
            if( !symbolsPath_.IsEmpty() && symbolsPath_.Exists() )
            {
                tools::Path symbolPath = symbolsPath_ / symbolFile;
                tools::Xifstream xis( symbolPath );
                return renderer_.Compile( xis, lod );
            }
            else
            {
                zip::izipstream zipStream( *zipFile_, symbolFile.ToUTF8().c_str() );
                xml::xistreamstream xis( zipStream );
                return renderer_.Compile( xis, lod );
            }
        }
        catch( ... )
        {
            if( firstNode && firstTime )
            {
                notFoundSymbols_.push_back( symbol.substr( 8, symbol.size() - 8 ) );
                firstTime = false;
            }
            symbol.pop_back();
        }
    }
    throw MASA_EXCEPTION( "Symbol not found" );
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::SetSymbolsPath
// Created: ABR 2013-01-22
// -----------------------------------------------------------------------------
void GLSymbols::SetSymbolsPath( const tools::Path& symbolPath )
{
    symbolsPath_ = symbolPath;
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::Load
// Created: ABR 2013-01-21
// -----------------------------------------------------------------------------
void GLSymbols::Load( const tools::ExerciseConfig& config )
{
    SetSymbolsPath( config.GetPhysicalChildPath( "symbols-directory" ) );
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::GetNotFoundSymbol
// Created: ABR 2013-01-21
// -----------------------------------------------------------------------------
const std::vector< std::string >& GLSymbols::GetNotFoundSymbol() const
{
    return notFoundSymbols_;
}

// -----------------------------------------------------------------------------
// Name: GLSymbols::SetCurrentColor
// Created: LGY 2013-06-03
// -----------------------------------------------------------------------------
void GLSymbols::SetCurrentColor( float /*r*/, float /*g*/, float /*b*/, float a )
{
    alpha_ = a;
}

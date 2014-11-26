// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GLSymbols_h_
#define __GLSymbols_h_

#include <boost/noncopyable.hpp>

namespace svg
{
    class Node_ABC;
}

namespace tools
{
namespace zip
{
    class InputArchive;
}
}

namespace gui
{
    class SvglRenderer;

// =============================================================================
/** @class  GLSymbols
    @brief  GLSymbols
*/
// Created: SBO 2006-12-15
// =============================================================================
class GLSymbols : private boost::noncopyable
{
public:
             GLSymbols( SvglRenderer& renderer, const tools::Path& symbolPath );
    virtual ~GLSymbols();

    void PrintApp6( const std::string& symbol,
                    const std::string& style,
                    const geometry::Rectangle2f& viewport,
                    unsigned vWidth = 640,
                    unsigned vHeight = 480,
                    bool pickingMode = false );
    const std::vector< std::string >& GetNotFoundSymbol() const;

private:
    svg::Node_ABC* Compile( std::string symbol, float lod, bool firstNode );

private:
    typedef std::pair< std::string, std::string > T_SymbolKey;
    typedef std::pair< std::shared_ptr< svg::Node_ABC >, std::shared_ptr< svg::Node_ABC > > T_LodSymbol;
    typedef std::map< T_SymbolKey, T_LodSymbol > T_Symbols;

private:
    SvglRenderer& renderer_;
    const tools::Path symbolPath_;
    std::unique_ptr< tools::zip::InputArchive > archive_;
    T_Symbols symbols_;
    std::vector< std::string > notFoundSymbols_;
};

}

#endif // __GLSymbols_h_

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
    class ExerciseConfig;

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
    //! @name Constructors/Destructor
    //@{
    explicit GLSymbols( SvglRenderer& renderer );
    virtual ~GLSymbols();
    //@}

    //! @name Operations
    //@{
    void PrintApp6( const std::string& symbol, const std::string& style, const geometry::Rectangle2f& viewport,
                     unsigned vWidth = 640, unsigned vHeight = 480, bool pickingMode = false, bool checkAlpha = true );
    void Load( const tools::ExerciseConfig& config );
    void SetSymbolsPath( const tools::Path& symbolPath );
    const std::vector< std::string >& GetNotFoundSymbol() const;
    void SetCurrentColor( float r, float g, float b, float a );
    //@}

private:
    //! @name Helpers
    //@{
    svg::Node_ABC* Compile( std::string symbol, float lod, bool firstNode );
    //@}

    //! @name Types
    //@{
    typedef std::pair< std::string, std::string >       T_SymbolKey;
    typedef std::pair< svg::Node_ABC*, svg::Node_ABC* > T_LodSymbol;
    typedef std::map< T_SymbolKey, T_LodSymbol >        T_Symbols;
    typedef std::map< T_SymbolKey, float >              T_AlphaSymbols;
    //@}

private:
    //! @name Member data
    //@{
    SvglRenderer& renderer_;
    tools::Path symbolsPath_;
    std::unique_ptr< tools::zip::InputArchive > archive_;
    T_Symbols                      symbols_;
    T_AlphaSymbols                 alphaSymbols_;
    std::vector< std::string >     notFoundSymbols_;
    float                          alpha_;
    //@}
};

}

#endif // __GLSymbols_h_

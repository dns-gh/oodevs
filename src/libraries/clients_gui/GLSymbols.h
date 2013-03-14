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

namespace zip
{
    class izipfile;
}

namespace tools
{
    class ExerciseConfig;
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
                    unsigned vWidth = 640, unsigned vHeight = 480, bool pickingMode = false );
    void Load( const tools::ExerciseConfig& config );
    void SetSymbolsPath( const tools::Path& symbolPath );
    const std::vector< std::string >& GetNotFoundSymbol() const;
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
    typedef T_Symbols::const_iterator                 CIT_Symbols;
    //@}

private:
    //! @name Member data
    //@{
    SvglRenderer& renderer_;
    tools::Path symbolsPath_;
    std::auto_ptr< zip::izipfile > zipFile_;
    T_Symbols                      symbols_;
    std::vector< std::string >     notFoundSymbols_;
    //@}
};

}

#endif // __GLSymbols_h_

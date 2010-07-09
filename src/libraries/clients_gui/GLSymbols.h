// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __GLSymbols_h_
#define __GLSymbols_h_

namespace svg
{
    class Node_ABC;
}

namespace zip
{
    class izipfile;
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
class GLSymbols
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit GLSymbols( SvglRenderer& renderer );
    virtual ~GLSymbols();
    //@}

    //! @name Operations
    //@{
    void PrintApp6( const std::string& symbol, const std::string& style, const geometry::Rectangle2f& viewport, unsigned vWidth = 640, unsigned vHeight = 480 );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GLSymbols( const GLSymbols& );            //!< Copy constructor
    GLSymbols& operator=( const GLSymbols& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    svg::Node_ABC* Compile( const std::string& filename, float lod ) const;
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
    std::auto_ptr< zip::izipfile > zipFile_;
    T_Symbols                      symbols_;
    //@}
};

}

#endif // __GLSymbols_h_

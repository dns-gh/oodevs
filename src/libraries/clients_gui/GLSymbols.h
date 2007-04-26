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

namespace svg
{
    class Node_ABC;
    class References;
    class TextRenderer;
    class RenderingContext;
    class Color;
    class Opacity;
}

namespace zip
{
    class izipfile;
}

namespace gui
{

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
             GLSymbols();
    virtual ~GLSymbols();
    //@}

    //! @name Operations
    //@{
    void PrintApp6( const std::string& symbol, const geometry::Rectangle2f& viewport, unsigned vWidth = 640, unsigned vHeight = 480 );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GLSymbols( const GLSymbols& );            //!< Copy constructor
    GLSymbols& operator=( const GLSymbols& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    svg::Node_ABC* Compile( const std::string& filename, float lod ) const;
    //@}

    //! @name Types
    //@{
    typedef std::pair< svg::Node_ABC*, svg::Node_ABC* > T_LodSymbol;
    typedef std::map< std::string, T_LodSymbol >        T_Symbols;
    typedef T_Symbols::const_iterator                 CIT_Symbols;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< zip::izipfile >         zipFile_;
    std::auto_ptr< svg::Color >            current_;
    std::auto_ptr< svg::Opacity >          opacity_;
    std::auto_ptr< svg::TextRenderer >     renderer_;
    std::auto_ptr< svg::References >       references_;
    std::auto_ptr< svg::RenderingContext > renderingContext_;
    T_Symbols                              symbols_;
    //@}
};

}

#endif // __GLSymbols_h_

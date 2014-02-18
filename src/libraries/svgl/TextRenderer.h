// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TextRenderer_h_
#define __TextRenderer_h_

#include <map>
#include <string>

namespace svg
{
    class RenderingContext_ABC;
    class Font;

// =============================================================================
/** @class  TextRenderer
    @brief  TextRenderer
*/
// Created: AGE 2006-10-20
// =============================================================================
class TextRenderer
{

public:
    //! @name Constructors/Destructor
    //@{
             TextRenderer();
    virtual ~TextRenderer();
    //@}

    //! @name Operations
    //@{
    void InitializeFont( const std::string& fontFamily, int fontWeight );
    void Render( const std::string& text, RenderingContext_ABC& context );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TextRenderer( const TextRenderer& );            //!< Copy constructor
    TextRenderer& operator=( const TextRenderer& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    struct FontId
    {
        FontId( const std::string& name, int weight )
            : name( name ), weight( weight ) {};

        std::string name;
        int weight;
        bool operator<( const FontId& rhs ) const
        {
            return name < rhs.name 
                || ( !( rhs.name < name )
                  && weight < rhs.weight );
        }
    };
    typedef std::map< FontId, Font* > T_Fonts;
    //@}

private:
    //! @name Member data
    //@{
    T_Fonts fonts_;
    //@}
};

}

#endif // __TextRenderer_h_

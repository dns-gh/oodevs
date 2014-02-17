// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Font_h_
#define __Font_h_

namespace svg
{
    class RenderingContext_ABC;

// =============================================================================
/** @class  Font
    @brief  Font
*/
// Created: AGE 2006-10-20
// =============================================================================
class Font
{

public:
    //! @name Constructors/Destructor
    //@{
             Font( const std::string& name, int weight );
    virtual ~Font();
    //@}

    //! @name Operations
    //@{
    void Render( const std::string& text, RenderingContext_ABC& context );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Font( const Font& );            //!< Copy constructor
    Font& operator=( const Font& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AlignText( const std::string& text, RenderingContext_ABC& context );
    float GetTextLength( const std::string& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int        baseList_;
    GLYPHMETRICSFLOAT	gmfPoly_[256];
    //@}
};

}

#endif // __Font_h_

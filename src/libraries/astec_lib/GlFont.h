// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlFont_h_
#define __GlFont_h_

// =============================================================================
/** @class  GlFont
    @brief  Gl font
*/
// Created: SBO 2006-03-20
// =============================================================================
class GlFont
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit GlFont( const std::string& font, bool outlines = false );
    virtual ~GlFont();
    //@}

    //! @name Operations
    //@{
    void Print( const std::string& message );
    geometry::Vector2f GetTextSize( const std::string& message ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GlFont( const GlFont& );            //!< Copy constructor
    GlFont& operator=( const GlFont& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned int        baseList_;
    GLYPHMETRICSFLOAT	gmfPoly_[256];
    //@}
};

#endif // __GlFont_h_

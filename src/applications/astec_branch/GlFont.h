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
             GlFont( const std::string& font );
    virtual ~GlFont();
    //@}

    //! @name Operations
    //@{
    void Print( const geometry::Point2f& where, const std::string& message, float rFontSize = 1.0f );
    geometry::Point2f GetTextSize( const std::string& message ) const;
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
    unsigned int        nBasePoly_;
    GLYPHMETRICSFLOAT	gmfPoly_[256];
    //@}
};

#endif // __GlFont_h_

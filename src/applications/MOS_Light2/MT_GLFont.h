// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-30 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLFont.h $
// $Author: Ape $
// $Modtime: 13/07/04 16:06 $
// $Revision: 1 $
// $Workfile: MT_GLFont.h $
//
// *****************************************************************************

#ifndef __MT_GLFont_h_
#define __MT_GLFont_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  MT_GLFont
    @brief  MT_GLFont
    @par    Using example
    @code
    MT_GLFont;
    @endcode
*/
// Created: APE 2004-06-30
// =============================================================================
class MT_GLFont
{
    MT_COPYNOTALLOWED( MT_GLFont );

public:
    enum E_Alignement
    {
        eVTop       = 1,
        eVCenter    = 2,
        eVBottom    = 4,
        eHLeft      = 8,
        eHCenter    = 16,
        eHRight     = 32,
        eDefault    = eVTop | eHRight,
        eCenter     = eVCenter | eHCenter
    };

public:
    //! @name Constructors/Destructor
    //@{
     MT_GLFont( const std::string& strFontName, float rPrecision = 0.0f, uint nCharSet = ANSI_CHARSET, bool bUseOutline = false, int nTypeSize = FW_THIN );
    ~MT_GLFont();
    //@}

    //! @name Operations
    //@{
    bool IsValid() const;

    void Print( MT_Vector2D vPos, const std::string& strText, float rFontSize = 1.0f, int nOutlineSize = 0, E_Alignement nAlign = eDefault );
    MT_Vector2D GetTextSize( const std::string& sText ) const;
    //@}

    //! @name Static easy-acess functions
    //@{
    static MT_GLFont* Font( const std::string& strFontName, float rPrecision = 0.0f, uint nCharSet = ANSI_CHARSET, bool bUseOutline = false, int nTypeSize = FW_THIN );
    static void Print( const std::string& strFontName, MT_Vector2D vPos, const std::string& strText, float rFontSize = 1.0f, int nOutlineSize = 0, E_Alignement nAlign = eDefault );
    static void DeleteFonts();
    //@}

private:
    //! @name Member data
    //@{
    const std::string   strFontName_;
    uint                nCharset_;
    bool                bUseOutline_;
    int                 nTypeSize_;
    float               rPrecision_;

    GLuint              nBasePoly_;
    GLuint              nBaseLine_;

    GLYPHMETRICSFLOAT	gmfPoly_[256];
    GLYPHMETRICSFLOAT	gmfLine_[256];

    HDC	                hDC_;

    bool                bValid_;

    //@}
};

#   include "MT_GLFont.inl"

#endif // __MT_GLFont_h_

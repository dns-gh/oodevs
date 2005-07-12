//*****************************************************************************
//
// $Created: FBD 02-12-13 $
// $Archive: /MVW_v10/Build/SDK/GFX/src/GFX_Text.h $
// $Author: Jvt $
// $Modtime: 4/02/03 17:50 $
// $Revision: 3 $
// $Workfile: GFX_Text.h $
//
//*****************************************************************************

#ifndef __GFX_Text_h_
#define __GFX_Text_h_

#include "GFX.h"

#include <qgl.h>

class GFX_Color;

//*****************************************************************************
// Created: FBD 02-12-13
//*****************************************************************************
class GFX_Text
{
    MT_COPYNOTALLOWED( GFX_Text );

public:
    explicit GFX_Text( const std::string& sFontName, MT_Float rPrecision = 0.0, uint nCharSet = ANSI_CHARSET, bool bUseLine = false, int nTypeSize = FW_THIN );
    virtual ~GFX_Text();

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

    void              InitFont();
    void              Print( MT_Vector2D vPos, MT_Float rSizeFont, const GFX_Color& color, bool bCenter, const char *fmt, ...);
    void              PrintLine( MT_Vector2D vPos, MT_Float rSizeFont, MT_Float rSizeLine, const GFX_Color& color, bool bCenter, const char *fmt, ...);
    MT_Vector2D       GetSizeText( const std::string& sText ) const;
    void              DeleteFont();

private:
    const std::string   sFontName_;
    uint                nCharset_;
    bool                bUseLine_;
    int                 nTypeSize_;
    GLuint              nBasePoly_;
    MT_Float            rPrecision_;
    GLYPHMETRICSFLOAT	gmfPoly_[256];
    GLYPHMETRICSFLOAT	gmfLine_[256];

    GLuint              nBaseLine_;
    HDC	                hDC_;

};

#ifdef USE_INLINE
#	include "GFX_Text.inl"
#endif


#endif // __GFX_Text_h_
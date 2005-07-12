//*****************************************************************************
//
// $Created: FBD 02-11-22 $
// $Archive: /MVW_v10/Build/SDK/GFX/src/GFX_Color.h $
// $Author: Nld $
// $Modtime: 8/09/04 11:21 $
// $Revision: 7 $
// $Workfile: GFX_Color.h $
//
//*****************************************************************************

#ifndef __GFX_Color_h_
#define __GFX_Color_h_

#include "GFX.h"

#include <qgl.h>

//*****************************************************************************
// Created: FBD 02-11-22
//*****************************************************************************
class GFX_Color
{
public:
    GFX_Color( MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha = 1.0 );
    ~GFX_Color();

    void SetRGB  ( MT_Float rR, MT_Float rG, MT_Float rB );
    void SetRed  ( MT_Float rVal );
    void SetGreen( MT_Float rVal );
    void SetBlue ( MT_Float rVal );
    void SetAlpha( MT_Float rValue );

    void AddRGB ( MT_Float rR, MT_Float rG, MT_Float rB );

    void AddInvert( MT_Float rValue );

    void      SetGLColor() const;
    MT_Float  GetAlpha  () const;
    GFX_Color GetShadow ( MT_Float rStrength ) const;

private:
    MT_Float rRed_;
    MT_Float rGreen_;
    MT_Float rBlue_;
    MT_Float rAlpha_;
};

#include "GFX_Color.inl"

#endif // __GFX_Color_h_
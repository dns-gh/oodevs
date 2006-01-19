//*****************************************************************************
//
// $Created: FBD 02-11-22 $
// $Archive: /MVW_v10/Build/SDK/GFX/src/GFX_Color.inl $
// $Author: Nld $
// $Modtime: 8/09/04 11:20 $
// $Revision: 6 $
// $Workfile: GFX_Color.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: GFX_Color::SetGLColor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
inline
void GFX_Color::SetGLColor() const
{
    glColor4f( rRed_ / 256., rGreen_ / 256., rBlue_ / 256., rAlpha_ );
}

//-----------------------------------------------------------------------------
// Name: GFX_Color::SetAlpha
// Created: FBD 02-11-25
//-----------------------------------------------------------------------------
inline
void GFX_Color::SetAlpha( MT_Float rValue )
{
    rAlpha_ = rValue;
}


//-----------------------------------------------------------------------------
// Name: GFX_Color::SetRGB
// Created: JVT 02-11-27
//-----------------------------------------------------------------------------
inline
void GFX_Color::SetRGB( MT_Float rR, MT_Float rG, MT_Float rB )
{
    rRed_   = rR;
    rGreen_ = rG;
    rBlue_  = rB;
}


//-----------------------------------------------------------------------------
// Name: GFX_Color::SetRed
// Created: JVT 02-11-27
//-----------------------------------------------------------------------------
inline
void GFX_Color::SetRed( MT_Float rVal )
{
    rRed_ = rVal;
}


//-----------------------------------------------------------------------------
// Name: GFX_Color::SetGreen
// Created: JVT 02-11-27
//-----------------------------------------------------------------------------
inline
void GFX_Color::SetGreen( MT_Float rVal )
{
    rGreen_ = rVal;
}


//-----------------------------------------------------------------------------
// Name: GFX_Color::GetAlpha
// Created: FBD 02-12-03
//-----------------------------------------------------------------------------
inline
MT_Float GFX_Color::GetAlpha() const
{
    return rAlpha_;
}

//-----------------------------------------------------------------------------
// Name: GFX_Color::SetBlue
// Created: JVT 02-11-27
//-----------------------------------------------------------------------------
inline
void GFX_Color::SetBlue( MT_Float rVal )
{
    rBlue_ = rVal;
}


//-----------------------------------------------------------------------------
// Name: GFX_Color::GetShadow
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
inline
GFX_Color GFX_Color::GetShadow( MT_Float rStrength ) const
{
    return GFX_Color( rRed_ / rStrength, rGreen_ / rStrength, rBlue_ / rStrength, rAlpha_  );
}




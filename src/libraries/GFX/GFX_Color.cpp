//*****************************************************************************
//
// $Created: FBD 02-11-22 $
// $Archive: /MVW_v10/Build/SDK/GFX/src/GFX_Color.cpp $
// $Author: Nld $
// $Modtime: 8/09/04 11:21 $
// $Revision: 2 $
// $Workfile: GFX_Color.cpp $
//
//*****************************************************************************

#include "GFX_pch.h"

#include "GFX_Color.h"

// No comment ...
#undef min
#undef max

//-----------------------------------------------------------------------------
// Name: GFX_Color constructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
GFX_Color::GFX_Color( MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha )
    : rRed_  ( rRed )
    , rGreen_( rGreen )
    , rBlue_ ( rBlue )
    , rAlpha_( rAlpha )
{
    
}


//-----------------------------------------------------------------------------
// Name: GFX_Color destructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
GFX_Color::~GFX_Color()
{
    
}


//-----------------------------------------------------------------------------
// Name: GFX_Color::GetShadow
// Created: APE 04-02-02
//-----------------------------------------------------------------------------
void GFX_Color::AddRGB ( MT_Float rR, MT_Float rG, MT_Float rB )
{
    rRed_   = std::min( (MT_Float)255.0, rRed_   + rR );
    rGreen_ = std::min( (MT_Float)255.0, rGreen_ + rG );
    rBlue_  = std::min( (MT_Float)255.0, rBlue_  + rB );
}


// -----------------------------------------------------------------------------
// Name: GFX_Color::AddInvert
/** @param  rValue 
*/
// Created: APE 2004-03-09
// -----------------------------------------------------------------------------
void GFX_Color::AddInvert( MT_Float rValue )
{
    if( rRed_ > 124 )
        rRed_ = std::max( (MT_Float)0, rRed_ - rValue );
    else
        rRed_ = std::min( (MT_Float)255.0, rRed_ + rValue );
    if( rGreen_ > 124 )
        rGreen_ = std::max( (MT_Float)0, rGreen_ - rValue );
    else
        rGreen_ = std::min( (MT_Float)255.0, rGreen_ + rValue );
    if( rBlue_ > 124 )
        rBlue_ = std::max( (MT_Float)0, rBlue_ - rValue );
    else
        rBlue_ = std::min( (MT_Float)255.0, rBlue_ + rValue );
}
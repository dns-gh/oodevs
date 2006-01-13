// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MT_GLToolTipLine.h"


// -----------------------------------------------------------------------------
// Name: MT_GLToolTipLine constructor
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
MT_GLToolTipLine::MT_GLToolTipLine( QString strText, QFont font, GFX_Color color )
: strText_  ( strText )
, font_     ( font )
, color_    ( color )
, fm_       ( QFontMetrics( font_ ) )
{
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTipLine destructor
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
MT_GLToolTipLine::~MT_GLToolTipLine()
{
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTipLine::Draw
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
void MT_GLToolTipLine::Draw( QGLWidget* pWidget, MT_Rect viewRect, MT_Vector2D pos ) const
{
    color_.SetGLColor();
        pWidget->renderText( pos.rX_ , pos.rY_ , 0, strText_, font_ );
}



// -----------------------------------------------------------------------------
// Name: MT_GLToolTipLine::GetWidth
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
uint  MT_GLToolTipLine::GetWidth() const
{
    return  fm_.boundingRect( strText_ ).width();
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTipLine::GetHeight
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
uint  MT_GLToolTipLine::GetHeight() const
{
    return  fm_.boundingRect( strText_ ).height();
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTipLine::GetSpace
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
uint MT_GLToolTipLine::GetSpace() const
{
    return  fm_.boundingRect( " " ).width();
}
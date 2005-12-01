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
#include "MT_GLToolTip.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MT_GLToolTipLine.h"

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip constructor
// Created: HME 2005-11-25
// -----------------------------------------------------------------------------
MT_GLToolTip::MT_GLToolTip( QGLWidget* pWidget )
: rWidth_           ( 0 )
, rHeight_          ( 0 )
, pWidget_          ( pWidget )
, backgroundColor_  ( GFX_Color( 0.0, 0.0, 0.0, 0.5 ) )
{
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip destructor
// Created: HME 2005-11-25
// -----------------------------------------------------------------------------
MT_GLToolTip::~MT_GLToolTip()
{
    lines_.clear();
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip::AddLine
// Created: HME 2005-11-25
// -----------------------------------------------------------------------------
void MT_GLToolTip::AddLine( std::string strText, MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha, bool bBold, float rStretchFactor )
{
    AddLine( QString( strText.c_str() ), GFX_Color( rRed, rGreen, rBlue, rAlpha), bBold, rStretchFactor );
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip::AddLine
// Created: HME 2005-11-25
// -----------------------------------------------------------------------------
void MT_GLToolTip::AddLine( QString strText, MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha, bool bBold, float rStretchFactor )
{
    AddLine( strText, GFX_Color( rRed, rGreen, rBlue, rAlpha), bBold, rStretchFactor );
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip::MT_GLToolTip::AddLine
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
void MT_GLToolTip::AddLine( QString strText, GFX_Color color, bool bBold , float rStretchFactor)
{
    QFont font = pWidget_->font();
    if ( bBold )
        font.setBold( true );
    //font.setPointSize( font.pointSize() * rStretchFactor );  //buggy

    lines_.push_back( MT_GLToolTipLine( strText, font, color ) );
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip::MT_GLToolTip::AddLine
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
void MT_GLToolTip::AddLine( std::string strText, GFX_Color color, bool bBold , float rStretchFactor )
{
    AddLine( QString( strText.c_str() ), color, bBold, rStretchFactor );
}


// -----------------------------------------------------------------------------
// Name: MT_GLToolTip::InsertLineLine
// Created: HME 2005-11-25
// -----------------------------------------------------------------------------
void MT_GLToolTip::InsertLine( std::string strText, MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha, bool bBold, float rStretchFactor )
{
    InsertLine( QString( strText.c_str() ), GFX_Color( rRed, rGreen, rBlue, rAlpha), bBold, rStretchFactor );
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip::InsertLine
// Created: HME 2005-11-25
// -----------------------------------------------------------------------------
void MT_GLToolTip::InsertLine( QString strText, MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha, bool bBold, float rStretchFactor )
{
    InsertLine( strText, GFX_Color( rRed, rGreen, rBlue, rAlpha), bBold, rStretchFactor );
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip::MT_GLToolTip::InsertLine
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
void MT_GLToolTip::InsertLine( QString strText, GFX_Color color, bool bBold , float rStretchFactor)
{
    QFont font = pWidget_->font();
    if ( bBold )
        font.setBold( true );
    //font.setPointSize( font.pointSize() * rStretchFactor );  //buggy

    lines_.insert( lines_.begin(), MT_GLToolTipLine( strText, font, color ) );
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip::MT_GLToolTip::InsertLine
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
void MT_GLToolTip::InsertLine( std::string strText, GFX_Color color, bool bBold , float rStretchFactor )
{
    InsertLine( QString( strText.c_str() ), color, bBold, rStretchFactor );
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip::Draw
// Created: HME 2005-11-25
// -----------------------------------------------------------------------------
void MT_GLToolTip::Draw( MT_Rect viewRect, MT_Vector2D pos, MT_Float zoom ) const 
{
    if ( zoom == 0 )
        zoom = 0.01;

    uint width = 0;
    uint height = 0;
    uint space = 0;
    uint line = 0;
    for( CITLinesVector it = lines_.begin(); it != lines_.end(); ++it )      
    {
        width = std::max< uint >( width, it->GetWidth() );
        height = std::max< uint >( height, it->GetHeight() );
        space = std::max< uint >( space, it->GetSpace() );
        line++;
    }
    space *= zoom;
    width *= zoom;
    height *= zoom;
    width += 2 * space;

    uint totalHeight = height * (1.5 * lines_.size() + 0.2);

    //avoid the corners
    if( ( pos.rX_ + width ) > viewRect.GetPointUpRight().rX_ )
        pos.rX_ =  viewRect.GetPointUpRight().rX_ - width;
    if( ( pos.rY_ - totalHeight ) < viewRect.GetPointUpRight().rY_ )
        pos.rY_ = viewRect.GetPointUpRight().rY_ + totalHeight;

    //draw the background of the box
    backgroundColor_.SetGLColor();
    glBegin( GL_QUADS );
        glVertex3f( pos.rX_, pos.rY_, 0.0 );
        glVertex3f( pos.rX_, pos.rY_ - totalHeight, 0.0 );
        glVertex3f( pos.rX_ + width, pos.rY_ - totalHeight, 0.0 );
        glVertex3f( pos.rX_ + width, pos.rY_, 0.0 );
    glEnd();

    //draw the texts
    MT_Vector2D     linePos       = MT_Vector2D( pos.rX_ + space, pos.rY_ - height );
    for( CITLinesVector it = lines_.begin(); it != lines_.end(); ++it )
    {
        it->Draw( pWidget_, viewRect, linePos );
        linePos.rY_ -= height * 1.5;
    }
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip::SetBackgroundColor
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
void MT_GLToolTip::SetBackgroundColor( MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha )
{
    backgroundColor_ = GFX_Color( rRed, rGreen, rBlue, rAlpha );
}

// -----------------------------------------------------------------------------
// Name: MT_GLToolTip::SetBackgroundColor
// Created: HME 2005-11-28
// -----------------------------------------------------------------------------
void MT_GLToolTip::SetBackgroundColor( GFX_Color color )
{
    backgroundColor_ = color;
}

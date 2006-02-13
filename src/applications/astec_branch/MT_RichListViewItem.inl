// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-09 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_RichListViewItem.inl $
// $Author: Ape $
// $Modtime: 10/09/04 17:51 $
// $Revision: 1 $
// $Workfile: MT_RichListViewItem.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::SetBackgroundColor
/** @param  color 
*/
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
inline
void MT_RichListViewItem::SetBackgroundColor( const QColor& color )
{
    backgroundColor_ = color;
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::SetAlternatingBackgroundColors
/** @param  color1 
    @param  color2 
    */
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
inline
void MT_RichListViewItem::SetAlternatingBackgroundColors( const QColor& color1, const QColor& color2 )
{
    backgroundColor_ = color1;
    backgroundColor2_ = color2;
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::SetFont
/** @param  font 
*/
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
inline
void MT_RichListViewItem::SetFont( const QFont& font )
{
    font_ = font;
    this->InitializeFont();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::SetFontColor
/** @param  color 
*/
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
inline
void MT_RichListViewItem::SetFontColor( const QColor& color )
{
    fontColor_ = color;
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::SetBold
/** @param  b 
*/
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
inline
void MT_RichListViewItem::SetBold( bool b )
{
    font_.setBold( b );
    this->InitializeFont();
}

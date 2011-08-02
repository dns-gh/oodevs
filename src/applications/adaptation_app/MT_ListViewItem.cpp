// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "MT_ListViewItem.h"
#include <QtGui/qpainter.h>

// -----------------------------------------------------------------------------
// Name: MT_ListViewItem constructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MT_ListViewItem::MT_ListViewItem( Q3ListView * parent )
    : Q3ListViewItem( parent )
    , pFont_       ( 0 )
    , pFontColor_  ( 0 )
    , pBackgroundColor_( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem constructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MT_ListViewItem::MT_ListViewItem( Q3ListView * parent, Q3ListViewItem * after )
    : Q3ListViewItem( parent, after )
    , pFont_       ( 0 )
    , pFontColor_  ( 0 )
    , pBackgroundColor_( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem constructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MT_ListViewItem::MT_ListViewItem( Q3ListView * parent, QString label1, QString label2, QString label3, QString label4, QString label5, QString label6, QString label7, QString label8 )
    : Q3ListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
    , pFont_       ( 0 )
    , pFontColor_  ( 0 )
    , pBackgroundColor_( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem constructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MT_ListViewItem::MT_ListViewItem( Q3ListView * parent, Q3ListViewItem * after, QString label1, QString label2, QString label3, QString label4, QString label5, QString label6, QString label7, QString label8 )
    : Q3ListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
    , pFont_       ( 0 )
    , pFontColor_  ( 0 )
    , pBackgroundColor_( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem constructor
/** @param  parent
*/
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MT_ListViewItem::MT_ListViewItem( Q3ListViewItem * parent )
    : Q3ListViewItem( parent )
    , pFont_       ( 0 )
    , pFontColor_  ( 0 )
    , pBackgroundColor_( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem constructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MT_ListViewItem::MT_ListViewItem( Q3ListViewItem * parent, Q3ListViewItem * after )
    : Q3ListViewItem( parent, after )
    , pFont_       ( 0 )
    , pFontColor_  ( 0 )
    , pBackgroundColor_( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem constructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MT_ListViewItem::MT_ListViewItem( Q3ListViewItem * parent, QString label1, QString label2, QString label3, QString label4, QString label5, QString label6, QString label7, QString label8 )
    : Q3ListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
    , pFont_       ( 0 )
    , pFontColor_  ( 0 )
    , pBackgroundColor_( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem constructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MT_ListViewItem::MT_ListViewItem( Q3ListViewItem * parent, Q3ListViewItem * after, QString label1, QString label2, QString label3, QString label4, QString label5, QString label6, QString label7, QString label8 )
    : Q3ListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
    , pFont_       ( 0 )
    , pFontColor_  ( 0 )
    , pBackgroundColor_( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem destructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MT_ListViewItem::~MT_ListViewItem()
{
    delete pFont_;
    delete pFontColor_;
    delete pBackgroundColor_;
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem::SetFont
/** @param  nColumn
    @param  font
*/
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
void MT_ListViewItem::SetFont( uint nColumn, const QFont &font )
{
    if( nColumn >= fonts_.size() )
        if( pFont_ != 0 )
            fonts_.resize( nColumn + 1, *pFont_ );
        else
            fonts_.resize( nColumn + 1, listView()->font() );
    fonts_[nColumn] = font;
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem::SetFontColor
/** @param  nColumn
    @param  color
*/
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
void MT_ListViewItem::SetFontColor( uint nColumn, const QColor &color )
{
    if( nColumn >= fontColors_.size() )
        if( pFontColor_ != 0 )
            fontColors_.resize( nColumn + 1, *pFontColor_ );
        else
            fontColors_.resize( nColumn + 1, listView()->colorGroup().text() );
    fontColors_[nColumn] = color;
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem::SetBackground
/** @param  nColumn
    @param  color
*/
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
void MT_ListViewItem::SetBackground( uint nColumn, const QColor &color )
{
    if( nColumn >= backgrounds_.size() )
        if( pBackgroundColor_ != 0 )
            backgrounds_.resize( nColumn + 1, *pBackgroundColor_ );
        else
            backgrounds_.resize( nColumn + 1, listView()->colorGroup().base() );
    backgrounds_[nColumn] = color;
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem::SetFont
/** @param  font
*/
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
void MT_ListViewItem::SetFont( const QFont &font )
{
    delete pFont_;
    pFont_ = new QFont( font );
    for( uint n = 0; n < fonts_.size(); ++n )
        fonts_[n] = font;
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem::SetFontColor
/** @param  color
*/
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
void MT_ListViewItem::SetFontColor( const QColor &color )
{
    delete pFontColor_;
    pFontColor_ = new QColor( color );
    for( uint n = 0; n < fontColors_.size(); ++n )
        fontColors_[n] = color;
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem::SetBackground
/** @param  color
*/
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
void MT_ListViewItem::SetBackground( const QColor &color )
{
    delete pBackgroundColor_;
    pBackgroundColor_ = new QColor( color );
    for( uint n = 0; n < backgrounds_.size(); ++n )
        backgrounds_[n] = color;
}

// -----------------------------------------------------------------------------
// Name: MT_ListViewItem::paintCell
/** @param  painter
    @param  cg
    @param  column
    @param  width
    @param  align
*/
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
void MT_ListViewItem::paintCell( QPainter *painter, const QColorGroup &cg, int column, int width, int align )
{
    // Save the painter.
    painter->save();

    // Set the font.
    if( pFont_ != 0 )
        painter->setFont( *pFont_ );
    if( column >= 0 && column < (int)fonts_.size() )
        painter->setFont(fonts_[column]);

    QColorGroup grp(cg);

    // Set the font color.
    if( pFontColor_ != 0 )
        grp.setColor(QColorGroup::Text, *pFontColor_ );
    if( column >= 0 && column < (int)fontColors_.size() )
        grp.setColor(QColorGroup::Text, fontColors_[column]);

    // Set the background color.
    if( pBackgroundColor_ != 0 )
        grp.setColor(QColorGroup::Base, *pBackgroundColor_ );
    if( column >= 0 && column < (int)backgrounds_.size() )
        grp.setColor(QColorGroup::Base, backgrounds_[column]);

    // Draw the cell.
    Q3ListViewItem::paintCell(painter, grp, column, width, align);

    // Restore the painter.
    painter->restore();
}


// -----------------------------------------------------------------------------
// Name: MT_ListViewItem::width
/** @param  fm
    @param  lv
    @param  column
    @return
*/
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
int MT_ListViewItem::width( const QFontMetrics &fm, const Q3ListView *lv, int column ) const
{
    int width;
    if( column >= 0 && column < (int)fonts_.size())
    {
        QFontMetrics fm2(fonts_[column]);
        width = Q3ListViewItem::width(fm2, lv, column);
    }
    else if( pFont_ != 0 )
    {
        QFontMetrics fm2( *pFont_ );
        width = Q3ListViewItem::width(fm2, lv, column);
    }
    else
        width = Q3ListViewItem::width(fm, lv, column);

    return width;
}

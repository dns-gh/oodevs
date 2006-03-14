// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/RichListItem.cpp $
// $Author: Ape $
// $Modtime: 22/09/04 17:24 $
// $Revision: 2 $
// $Workfile: RichListItem.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "RichListItem.h"

// -----------------------------------------------------------------------------
// Name: RichListItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
RichListItem::RichListItem( QListView * parent )
    : QListViewItem( parent )
    , font_( parent->font() )
    , fontColor_( Qt::white )
    , even_( InitializeColor() )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: RichListItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
RichListItem::RichListItem( QListView * parent, QListViewItem * after )
    : QListViewItem( parent, after )
    , font_( parent->font() )
    , fontColor_( Qt::white )
    , even_( InitializeColor() )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: RichListItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
RichListItem::RichListItem( QListView * parent, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : QListViewItem( parent )
    , font_( parent->font() )
    , fontColor_( Qt::white )
    , even_( InitializeColor() )
{
    AddColumns( label1, label2, label3, label4, label5, label6, label7, label8 );
}

// -----------------------------------------------------------------------------
// Name: RichListItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
RichListItem::RichListItem( QListView * parent, QListViewItem * after, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : QListViewItem( parent, after )
    , font_( parent->font() )
    , fontColor_( Qt::white )
    , even_( InitializeColor() )
{
    AddColumns( label1, label2, label3, label4, label5, label6, label7, label8 );
}

// -----------------------------------------------------------------------------
// Name: RichListItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
RichListItem::RichListItem( QListViewItem * parent )
    : QListViewItem( parent )
    , font_( listView()->font() )
    , fontColor_( Qt::white )
    , even_( InitializeColor() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichListItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
RichListItem::RichListItem( QListViewItem * parent, QListViewItem * after )
    : QListViewItem( parent, after )
    , font_( listView()->font() )
    , fontColor_( Qt::white )
    , even_( InitializeColor() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichListItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
RichListItem::RichListItem( QListViewItem * parent, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : QListViewItem( parent )
    , font_( listView()->font() )
    , fontColor_( Qt::white )
    , even_( InitializeColor() )
{
    AddColumns( label1, label2, label3, label4, label5, label6, label7, label8 );
}

// -----------------------------------------------------------------------------
// Name: RichListItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
RichListItem::RichListItem( QListViewItem * parent, QListViewItem * after, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : QListViewItem( parent, after )
    , font_( listView()->font() )
    , fontColor_( Qt::white )
    , even_( InitializeColor() )
{
    AddColumns( label1, label2, label3, label4, label5, label6, label7, label8 );
}

// -----------------------------------------------------------------------------
// Name: RichListItem destructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
RichListItem::~RichListItem()
{
    for( IT_RichTexts it = columns_.begin(); it != columns_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: RichListItem::AddColumns
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
void RichListItem::AddColumns( const QString& label1, const QString& label2, const QString& label3, const QString& label4,  const QString& label5, const QString& label6, const QString& label7, const QString& label8 )
{
    if( !label1.isNull() )
        setText( 0, label1 ); 
    if( !label2.isNull() )
        setText( 1, label2 ); 
    if( !label3.isNull() )
        setText( 2, label3 ); 
    if( !label4.isNull() )
        setText( 3, label4 );
    if( !label5.isNull() )
        setText( 4, label5 ); 
    if( !label6.isNull() )
        setText( 5, label6 ); 
    if( !label7.isNull() )
        setText( 6, label7 ); 
    if( !label8.isNull() )
        setText( 7, label8 );
}

// -----------------------------------------------------------------------------
// Name: RichListItem::InitializeColors
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
bool RichListItem::InitializeColor()
{
    const RichListItem* pItem = (RichListItem*)( itemAbove() );
    return backgroundColor2_.isValid()&& pItem && ! pItem->even_;
}

// -----------------------------------------------------------------------------
// Name: RichListItem::SetBackgroundColor
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
void RichListItem::SetBackgroundColor( const QColor& color )
{
    backgroundColor_ = color;
    backgroundColor2_ = QColor();
    even_ = InitializeColor();
}

// -----------------------------------------------------------------------------
// Name: RichListItem::SetBackgroundColor
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
void RichListItem::SetBackgroundColor( const QColor& color1, const QColor& color2 )
{
    backgroundColor_ = color1;
    backgroundColor2_ = color2;
    even_ = InitializeColor();
}

// -----------------------------------------------------------------------------
// Name: RichListItem::SetFont
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
void RichListItem::SetFont( const QFont& font )
{
    font_ = font;
    int height = 0;
    for( IT_RichTexts it = columns_.begin(); it != columns_.end(); ++it ) {
        it->second->setDefaultFont( font_ );
        height = std::max( height, it->second->height() );
    }
    setHeight( height );
    widthChanged();
}

// -----------------------------------------------------------------------------
// Name: RichListItem::SetFontColor
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
void RichListItem::SetFontColor( const QColor& color )
{
    fontColor_ = color;
}

// -----------------------------------------------------------------------------
// Name: RichListItem::SetBold
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
void RichListItem::SetBold( bool b )
{
    QFont newFont( font_ );
    newFont.setBold( b );
    SetFont( newFont );
}

// -----------------------------------------------------------------------------
// Name: RichListItem::GetBackgroundColor
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
const QColor& RichListItem::GetBackgroundColor()
{
    return even_ ? backgroundColor2_ : backgroundColor_;
}

// -----------------------------------------------------------------------------
// Name: RichListItem::paintCell
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
void RichListItem::paintCell( QPainter* pPainter, const QColorGroup& cg, int nColumn, int nWidth, int /*nAlign*/ )
{
    if( nColumn >= (int)columns_.size() )
        return;

    QColorGroup colorGroup( cg );
    QBrush brush;
    if( isSelected() )
    {
        if( listView()->hasFocus() )
        {
            const QColor& color = listView()->colorGroup().highlightedText();
            colorGroup.setColor( QColorGroup::Text, color );
            colorGroup.setColor( QColorGroup::Link, color );
            brush.setColor( listView()->colorGroup().highlight() );
        }
        else
        {
            colorGroup.setColor( QColorGroup::Text, Qt::black );
            colorGroup.setColor( QColorGroup::Link, Qt::black );
            brush.setColor( Qt::lightGray );
        }
    }
    else
    {
        colorGroup.setColor( QColorGroup::Text, fontColor_ );
        brush.setColor( GetBackgroundColor() );
    }

    QSimpleRichText* pRichText = columns_[nColumn].second;
    const QRect rect( 0, 0, nWidth, this->height() );
    pRichText->draw( pPainter, 0, 0, rect, colorGroup, &brush );
}


// -----------------------------------------------------------------------------
// Name: RichListItem::width
// Created: APE 2004-09-07
// -----------------------------------------------------------------------------
int RichListItem::width( const QFontMetrics& /*fm*/, const QListView* /*lv*/, int nColumn ) const
{
    if( nColumn >= (int)columns_.size() )
        return 0;

    return columns_[nColumn].second->widthUsed();
}

// -----------------------------------------------------------------------------
// Name: RichListItem::setText
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
void RichListItem::setText( int column, const QString& text )
{
    if( column < 0 )
        return;
    while( (int)columns_.size() <= column )
        columns_.push_back( T_RichText( "", CreateRichText( "" ) ) );
    T_RichText& richText = columns_[ column ];
    delete richText.second; 
    richText = T_RichText( text, CreateRichText( text ) );
    widthChanged();
}

// -----------------------------------------------------------------------------
// Name: RichListItem::CreateRichText
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
QSimpleRichText* RichListItem::CreateRichText( const QString& label )
{
    QSimpleRichText* text = new QSimpleRichText( label, font_ );
    text->setWidth( 10000 ); // Unlimited width
    if( text->height() > height() )
        setHeight( text->height() );
    return text;
}

// -----------------------------------------------------------------------------
// Name: RichListItem::text
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
QString RichListItem::text( int column ) const
{
    if( column < 0 || column >= (int)columns_.size() )
        return "";
    return columns_[ column ].first;
}

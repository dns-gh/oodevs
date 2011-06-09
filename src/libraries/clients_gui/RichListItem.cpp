// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichListItem.h"
#include <qpainter.h>

using namespace gui;

namespace
{
    const unsigned margin = 2;
}

// -----------------------------------------------------------------------------
// Name: RichListItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
RichListItem::RichListItem( QListView * parent )
    : QListViewItem( parent )
    , font_( parent->font() )
    , fontColor_( parent->palette().color( QPalette::Active, QColorGroup::Text ) )
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
    , fontColor_( parent->palette().color( QPalette::Active, QColorGroup::Text ) )
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
    , fontColor_( parent->palette().color( QPalette::Active, QColorGroup::Text ) )
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
    , fontColor_( parent->palette().color( QPalette::Active, QColorGroup::Text ) )
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
    , fontColor_( listView()->palette().color( QPalette::Active, QColorGroup::Text ) )
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
    , fontColor_( listView()->palette().color( QPalette::Active, QColorGroup::Text ) )
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
    , fontColor_( listView()->palette().color( QPalette::Active, QColorGroup::Text ) )
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
    , fontColor_( listView()->palette().color( QPalette::Active, QColorGroup::Text ) )
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
        delete it->rich;
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
    if( ! backgroundColor_.isValid() )
        backgroundColor_ = listView()->colorGroup().base();
    const RichListItem* pItem = (RichListItem*)( itemAbove() );
    if( !pItem )
        pItem = (RichListItem*)( itemBelow() );
    return backgroundColor2_.isValid() && pItem && ! pItem->even_;
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
    for( IT_RichTexts it = columns_.begin(); it != columns_.end(); ++it )
        it->rich->setDefaultFont( font_ );
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
void RichListItem::paintCell( QPainter* pPainter, const QColorGroup& cg, int nColumn, int nWidth, int nAlign )
{
    if( nColumn < 0 || nColumn >= ( int ) columns_.size() )
        return;

    QColorGroup colorGroup( cg );
    QBrush brush( Qt::SolidPattern );
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
        if( listView()->backgroundOrigin() == QWidget::WindowOrigin )
            brush = listView()->backgroundBrush();
        else
            brush.setColor( GetBackgroundColor() );
    }

    QSimpleRichText* pRichText = columns_[ nColumn ].rich;
    pRichText->setWidth( nWidth );
    setHeight( Height() + 2 * ::margin );

    const QPixmap& pm = columns_[ nColumn ].pixMap;
    QRect rect( 0, 0, nWidth, height() );

    int voffset = ( height() - pRichText->height() ) / 2;
    if( ( nAlign & Qt::AlignTop ) == Qt::AlignTop )
        voffset = ::margin;
    else if( (nAlign & Qt::AlignBottom ) == Qt::AlignBottom )
        voffset = std::max< int >( height() - pRichText->height() - ::margin, 0 );
    int hoffset = pm.isNull() ? ::margin : ( pm.width() + 2 * ::margin );
    int hindent = ( nAlign & Qt::AlignHCenter ) == 0 ? 0 : ( nWidth - pRichText->widthUsed() - hoffset ) / 2;

    pRichText->draw( pPainter, hoffset + hindent, voffset, rect, colorGroup, &brush );
    pPainter->drawPixmap( QPoint( hindent + ::margin, ( height() - pm.height() ) / 2 ), pm );
}

// -----------------------------------------------------------------------------
// Name: RichListItem::width
// Created: APE 2004-09-07
// -----------------------------------------------------------------------------
int RichListItem::width( const QFontMetrics& /*fm*/, const QListView* /*lv*/, int nColumn ) const
{
    return Width( nColumn );
}

// -----------------------------------------------------------------------------
// Name: RichListItem::Width
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
int RichListItem::Width( int nColumn ) const
{
    if( nColumn < 0 || nColumn >= ( int )columns_.size() )
        return 0;
    const QPixmap& pm = columns_[ nColumn ].pixMap;
    const int textWidth = columns_[ nColumn ].rich->widthUsed();
    const int pixWidth  = pm.isNull() ? 0 : ( pm.width() + ::margin );
    return textWidth + pixWidth;
}

// -----------------------------------------------------------------------------
// Name: RichListItem::Height
// Created: SBO 2011-05-26
// -----------------------------------------------------------------------------
int RichListItem::Height() const
{
    int bestHeight = 0;
    for( T_RichTexts::const_iterator it = columns_.begin(); it != columns_.end(); ++it )
        if( it->pixMap.isNull() )
            bestHeight = std::max( bestHeight, it->rich->height() );
        else
            bestHeight = std::max( bestHeight, it->pixMap.height() );
    return bestHeight;
}

// -----------------------------------------------------------------------------
// Name: RichListItem::setText
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
void RichListItem::setText( int column, const QString& text )
{
    if( column < 0 )
        return;
    while( ( int )columns_.size() <= column )
        columns_.push_back( RichText( "", CreateRichText( "" ) ) );
    RichText& richText = columns_[ column ];
    if( richText.base != text )
    {
        richText.base = text;
        delete richText.rich;
        richText.rich = CreateRichText( text );
        widthChanged();
        listView()->triggerUpdate();
    }
}

// -----------------------------------------------------------------------------
// Name: RichListItem::setPixmap
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void RichListItem::setPixmap( int column, const QPixmap & pm )
{
    if( column < 0 )
        return;
    while( ( int )columns_.size() <= column )
        columns_.push_back( RichText( "", CreateRichText( "" ) ) );
    RichText& richText = columns_[ column ];
    richText.pixMap = pm;
    setHeight( std::max( height(), pm.height() ) );
    widthChanged();
    listView()->triggerUpdate();
}

// -----------------------------------------------------------------------------
// Name: RichListItem::CreateRichText
// Created: AGE 2006-03-06
// -----------------------------------------------------------------------------
QSimpleRichText* RichListItem::CreateRichText( const QString& label )
{
    QSimpleRichText* text = new QSimpleRichText( label, font_ );
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
    if( column < 0 || column >= ( int ) columns_.size() )
        return "";
    return columns_[ column ].base;
}

// -----------------------------------------------------------------------------
// Name: RichListItem::pixmap
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
const QPixmap * RichListItem::pixmap( int column ) const
{
    if( column < 0 || column >= ( int ) columns_.size() )
        return 0;
    return & columns_[ column ].pixMap;
}

// -----------------------------------------------------------------------------
// Name: RichListItem::rtti
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
int RichListItem::rtti() const
{
    return 999;
}

// -----------------------------------------------------------------------------
// Name: RichListItem::GetAnchorAt
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
QString RichListItem::GetAnchorAt( const QPoint globalPos, int nColumn ) const
{
    if( nColumn < 0 || nColumn >= ( int ) columns_.size() || ! columns_[ nColumn ].rich )
        return QString::null;

    QListView* pListView = listView();
    QRect itemRect = pListView->itemRect( this );
    QPoint topLeft = itemRect.topLeft();
    QPoint topLeftWorld = pListView->viewport()->mapToGlobal( topLeft );
    QPoint offset( globalPos.x() - topLeftWorld.x(), globalPos.y() - topLeftWorld.y() );

    for( int n = 0; n < nColumn; ++n )
        offset.setX( offset.x() - pListView->columnWidth( n ) );

    return columns_[ nColumn ].rich->anchorAt( offset );
}

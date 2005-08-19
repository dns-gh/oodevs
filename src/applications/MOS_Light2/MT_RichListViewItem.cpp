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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_RichListViewItem.cpp $
// $Author: Ape $
// $Modtime: 22/09/04 17:24 $
// $Revision: 2 $
// $Workfile: MT_RichListViewItem.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MT_RichListViewItem.h"


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
MT_RichListViewItem::MT_RichListViewItem( QListView * parent )
    : QListViewItem( parent )
{
    this->Initialize();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
MT_RichListViewItem::MT_RichListViewItem( QListView * parent, QListViewItem * after )
    : QListViewItem( parent, after )
{
    this->Initialize();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
MT_RichListViewItem::MT_RichListViewItem( QListView * parent, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : QListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
{
    richText_.push_back( new QSimpleRichText( label1, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label2, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label3, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label4, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label5, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label6, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label7, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label8, this->listView()->font() ) );

    this->Initialize();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
MT_RichListViewItem::MT_RichListViewItem( QListView * parent, QListViewItem * after, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : QListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
{
    richText_.push_back( new QSimpleRichText( label1, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label2, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label3, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label4, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label5, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label6, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label7, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label8, this->listView()->font() ) );

    this->Initialize();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
MT_RichListViewItem::MT_RichListViewItem( QListViewItem * parent )
    : QListViewItem( parent )
{
    this->Initialize();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
MT_RichListViewItem::MT_RichListViewItem( QListViewItem * parent, QListViewItem * after )
    : QListViewItem( parent, after )
{
    this->Initialize();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
MT_RichListViewItem::MT_RichListViewItem( QListViewItem * parent, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : QListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
{
    richText_.push_back( new QSimpleRichText( label1, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label2, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label3, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label4, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label5, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label6, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label7, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label8, this->listView()->font() ) );

    this->Initialize();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem constructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
MT_RichListViewItem::MT_RichListViewItem( QListViewItem * parent, QListViewItem * after, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : QListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
{
    richText_.push_back( new QSimpleRichText( label1, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label2, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label3, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label4, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label5, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label6, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label7, this->listView()->font() ) );
    richText_.push_back( new QSimpleRichText( label8, this->listView()->font() ) );

    this->Initialize();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem destructor
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
MT_RichListViewItem::~MT_RichListViewItem()
{
    while( ! richText_.empty() )
    {
        delete richText_.back();
        richText_.pop_back();
    }
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::Initialize
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
void MT_RichListViewItem::Initialize()
{
    backgroundColor_ = Qt::white;
    font_ = this->listView()->font();

    this->InitializeSizes();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::InitializeSizes
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
void MT_RichListViewItem::InitializeSizes()
{
    int nHeight = 0;
    for( uint i = 0; i < richText_.size(); ++i )
    {
        richText_[i]->setWidth( 10000 ); // Unlimited width
        nHeight = max( nHeight, richText_[i]->height() );
    }
    this->setHeight( nHeight );
    this->widthChanged();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::InitializeFont
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
void MT_RichListViewItem::InitializeFont()
{
    for( uint i = 0; i < richText_.size(); ++i )
        richText_[i]->setDefaultFont( font_ );
    this->InitializeSizes();
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::GetBackgroundColor
/** @return 
*/
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
const QColor& MT_RichListViewItem::GetBackgroundColor()
{
    if( ! backgroundColor2_.isValid() )
        return backgroundColor_;

    QListViewItem* pItem = this->itemAbove();
    if( pItem != 0 )
    {
        if ( ((MT_RichListViewItem*)pItem )->currentBackgroundColor_ == backgroundColor_ )
            currentBackgroundColor_ = backgroundColor2_;
        else
            currentBackgroundColor_ = backgroundColor_;
    }
    else
        currentBackgroundColor_= backgroundColor_;

    return currentBackgroundColor_;
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::GetAnchorAt
/** @param  globalPos 
    @return 
*/
// Created: APE 2004-09-07
// -----------------------------------------------------------------------------
QString MT_RichListViewItem::GetAnchorAt( const QPoint globalPos, int nColumn ) const
{
    if( nColumn >= (int)richText_.size() )
        return "";

    QListView* pListView = this->listView();
    QRect itemRect = pListView->itemRect( this );
    QPoint topLeft = itemRect.topLeft();
    QPoint topLeftWorld = pListView->viewport()->mapToGlobal( topLeft );
    QPoint offset( globalPos.x() - topLeftWorld.x(), globalPos.y() - topLeftWorld.y() );

    for( int n = 0; n < nColumn; ++n )
        offset.setX( offset.x() - this->listView()->columnWidth( n ) );

    return richText_[nColumn]->anchorAt( offset );
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::paintCell
/** @param  pPainter 
    @param  cg 
    @param  nColumn 
    @param  nWidth 
    @param  nAlign 
*/
// Created: APE 2004-09-08
// -----------------------------------------------------------------------------
void MT_RichListViewItem::paintCell( QPainter* pPainter, const QColorGroup& cg, int nColumn, int nWidth, int /*nAlign*/ )
{
    if( nColumn >= (int)richText_.size() )
        return;

    QSimpleRichText* pRichText = richText_[nColumn];
    QRect rect( 0, 0, nWidth, this->height() );

    if( this->isSelected() )
    {
        QColorGroup cg2( cg );
        const QColorGroup& parentCg = listView()->colorGroup();
        if( listView()->hasFocus() )
        {

            cg2.setColor(QColorGroup::Text, parentCg.highlightedText() );
            cg2.setColor(QColorGroup::Link, parentCg.highlightedText() );
            pRichText->draw( pPainter, 0, 0, rect, cg2, &QBrush( parentCg.highlight() ) );
        }
        else
        {
            cg2.setColor(QColorGroup::Text, Qt::black );
            cg2.setColor(QColorGroup::Link, Qt::black );
            pRichText->draw( pPainter, 0, 0, rect, cg2, &QBrush( Qt::lightGray ) );
        }
    }
    else
    {
        QColorGroup cg2( cg );
        QBrush backgroundBrush( this->GetBackgroundColor() );
        cg2.setColor(QColorGroup::Text, fontColor_ );
        pRichText->draw( pPainter, 0, 0, rect, cg2, &backgroundBrush );
    }
}


// -----------------------------------------------------------------------------
// Name: MT_RichListViewItem::width
/** @param  fm 
    @param  lv 
    @param  c 
    @return 
*/
// Created: APE 2004-09-07
// -----------------------------------------------------------------------------
int MT_RichListViewItem::width( const QFontMetrics& /*fm*/, const QListView* /*lv*/, int nColumn ) const
{
    if( nColumn >= (int)richText_.size() )
        return 0;

    return richText_[nColumn]->widthUsed();
}


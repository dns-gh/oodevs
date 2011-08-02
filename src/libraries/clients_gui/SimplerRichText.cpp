// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SimplerRichText.h"
#include <QtGui/qpainter.h>
#include <QtCore/qregexp.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SimplerRichText::RichElement::RichElement
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
SimplerRichText::RichElement::RichElement( SimplerRichText* parent, const QString& text, const QString& link )
    : parent_( parent )
    , label_( text )
    , anchor_( link )
    , offset_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::RichElement::draw
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void SimplerRichText::RichElement::draw( QPainter* p, int x, int y, const QColorGroup& cg ) const
{
    if( anchor_.size() ) {
        p->setPen( cg.color( QColorGroup::Link ) );
        p->setFont( parent_->linkFont_ );
    }
    else {
        p->setPen( cg.color( QColorGroup::Text ) );
        p->setFont( parent_->font_ );
    }
    p->drawText( x + offset_, y + parent_->height()-3, label_, -1 );
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::RichElement::width
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
int SimplerRichText::RichElement::width() const
{
    return QFontMetrics( parent_->font_ ).width( label_ );
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::RichElement::widthUsed
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
int SimplerRichText::RichElement::widthUsed() const
{
    return offset_ + width();
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::RichElement::IsAt
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
bool SimplerRichText::RichElement::IsAt( const QPoint& point ) const
{
    return point.x() >= offset_ && point.x() < widthUsed();
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::RichElement::Anchor
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
const QString& SimplerRichText::RichElement::Anchor() const
{
    return anchor_;
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::RichElement::UpdateOffset
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void SimplerRichText::RichElement::UpdateOffset( SimplerRichText::CIT_Elements it )
{
    if( it == parent_->elements_.begin() )
        offset_ = 0;
    else
    {
        const RichElement& previous = *(it-1);
        offset_ = previous.widthUsed();
    }
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText constructor
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
SimplerRichText::SimplerRichText( const QString& text, const QFont& font )
    : font_( font )
    , linkFont_( font )
{
    linkFont_.setUnderline( true );
    Parse( text );
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText destructor
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
SimplerRichText::~SimplerRichText()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::draw
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void SimplerRichText::draw( QPainter* p, int x, int y, const QRect& clipRect, const QColorGroup& cg, const QBrush* paper /* = 0*/ ) const
{
    if( ! clipRect.isNull() )
        p->setClipRect( clipRect, Qt::ReplaceClip );
    if( paper )
        p->fillRect( clipRect, *paper );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->draw( p, x, y, cg );
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::setDefaultFont
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void SimplerRichText::setDefaultFont( const QFont& font )
{
    font_ = linkFont_ = font;
    linkFont_.setUnderline( true );
    UpdateOffset();
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::setText
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void SimplerRichText::setText( const QString& text )
{
    Parse( text );
    UpdateOffset();
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::Parse
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void SimplerRichText::Parse( const QString& text )
{
    QStringList list;
    Breakdown( text, list );
    bool textOnly = true;
    elements_.clear();
    elements_.reserve( list.size() );
    for( QStringList::iterator it = list.begin(); it != list.end(); ++it )
    {
        if( textOnly )
            AddTextElement( *it );
        else
            AddLinkElement( *it );
        textOnly = !textOnly;
    }
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::UpdateOffset
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void SimplerRichText::UpdateOffset()
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->UpdateOffset( it );
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::Breakdown
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void SimplerRichText::Breakdown( const QString& text, QStringList& list )
{
    QStringList first = QStringList::split( "<a ", text, true );
    for( QStringList::iterator it = first.begin(); it != first.end(); ++it )
        list += QStringList::split( "</a>", *it, true );
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::AddTextElement
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void SimplerRichText::AddTextElement( const QString& text )
{
    if( ! text.isEmpty() )
        AddLinkElement( QString(), text );
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::AddLinkElement
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void SimplerRichText::AddLinkElement( const QString& text )
{
    QRegExp regexp( "\\s*href=[\\'\\\"](.*)[\\'\\\"]\\s*>(.*)" );
    if( regexp.search( text ) > -1 )
    {
        QStringList result = regexp.capturedTexts();
        AddLinkElement( result[1], result[2] );
    }
    else
        AddTextElement( text );
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::AddLinkElement
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void SimplerRichText::AddLinkElement( const QString& link, const QString& text )
{
    elements_.push_back( RichElement( this, text, link ) );
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::anchorAt
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
QString SimplerRichText::anchorAt( const QPoint& point ) const
{
    if( point.y() >= 0 && point.y() < height() )
        for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
            if( it->IsAt( point ) )
                return it->Anchor();
    return "";
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::widthUsed
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
int SimplerRichText::widthUsed() const
{
    return elements_.empty() ? 0 : elements_.back().widthUsed();
}

// -----------------------------------------------------------------------------
// Name: SimplerRichText::height
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
int SimplerRichText::height() const
{
    return QFontMetrics( font_ ).height();
}

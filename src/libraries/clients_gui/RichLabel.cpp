// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichLabel.h"
#include "moc_RichLabel.cpp"
#include "SimplerRichText.h"
#include <Qt3Support/q3process.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichLabel constructor
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
RichLabel::RichLabel( QWidget* parent /* = 0*/, const char* name /* = 0*/ )
    : QLabel( parent, name )
    , richText_( new SimplerRichText( "", font() ) )
{
    setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    setMouseTracking( true );
}

// -----------------------------------------------------------------------------
// Name: RichLabel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
RichLabel::RichLabel( const QString& text, QWidget* parent, const char* name )
    : QLabel( text, parent, name )
    , richText_( new SimplerRichText( text, font() ) )
{
    setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    setMouseTracking( true );
}

// -----------------------------------------------------------------------------
// Name: RichLabel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
RichLabel::RichLabel( const QString& text, bool required, QWidget* parent, const char* name )
    : QLabel( text, parent, name )
    , richText_( new SimplerRichText( text, font() ) )
{
    if( required )
    {
        QFont font = this->font();
        font.setBold( true );
        setFont( font );
        richText_->setDefaultFont( font );
    }
    setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    setMouseTracking( true );
}

// -----------------------------------------------------------------------------
// Name: RichLabel destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
RichLabel::~RichLabel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichLabel::Warn
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void RichLabel::Warn( int msec )
{
    setPaletteForegroundColor( Qt::red );
    QTimer::singleShot( msec, this, SLOT( OnWarnStop() ) );
}

// -----------------------------------------------------------------------------
// Name: RichLabel::OnWarnStop
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void RichLabel::OnWarnStop()
{
    setPaletteForegroundColor( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: RichLabel::setText
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
void RichLabel::setText( const QString& message )
{
    delete richText_;
    richText_ = new SimplerRichText( message , font() );
    QLabel::setText( message );
}

// -----------------------------------------------------------------------------
// Name: RichLabel::mouseReleaseEvent
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
void RichLabel::mouseReleaseEvent( QMouseEvent* e )
{
    const QString url = richText_->anchorAt( e->pos() );
    if( ! url.isEmpty() )
    {
        emit LinkClicked( url );
        setCursor( Qt::arrowCursor );
    }
}

// -----------------------------------------------------------------------------
// Name: RichLabel::mouseMoveEvent
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
void RichLabel::mouseMoveEvent( QMouseEvent* e )
{
    const QString url = richText_->anchorAt( e->pos() );
    if( url.isEmpty() )
        setCursor( Qt::arrowCursor );
    else
        setCursor( Qt::pointingHandCursor );
}

// -----------------------------------------------------------------------------
// Name: RichLabel::drawContents
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
void RichLabel::drawContents( QPainter* p )
{
    render( p ) ;
}
 
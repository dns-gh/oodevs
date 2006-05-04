// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "RichLabel.h"
#include "moc_RichLabel.cpp"
#include <qsimplerichtext.h>
#include <qprocess.h>

// -----------------------------------------------------------------------------
// Name: RichLabel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
RichLabel::RichLabel( const QString& text, QWidget* parent, const char* name )
    : QLabel( text, parent, name )
    , richText_( new QSimpleRichText( text, this->font() ) )
{
    setAlignment( AlignVCenter | AlignLeft );
}

// -----------------------------------------------------------------------------
// Name: RichLabel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
RichLabel::RichLabel( const QString& text, bool required, QWidget* parent, const char* name )
    : QLabel( text, parent, name )
    , richText_( new QSimpleRichText( text, this->font() ) )
{
    if( required )
    {
        QFont font = this->font();
        font.setBold( true );
        setFont( font );
        richText_->setDefaultFont( font );
    }
    setAlignment( AlignVCenter | AlignLeft );
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
// Name: RichLabel::mouseReleaseEvent
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
void RichLabel::mouseReleaseEvent( QMouseEvent* e )
{
    const QPoint pos = mapToParent( e->pos() );
    if( richText_->inText( pos ) )
    {
        const QString url = richText_->anchorAt( pos );
        if( url.isEmpty() )
            return;
        QProcess openPage(0);
		openPage.addArgument( "cmd" );
		openPage.addArgument( "/c" );
		openPage.addArgument( "start" );
		openPage.addArgument( url );
		openPage.start();
        setCursor( QCursor::arrowCursor );
    }
}

// -----------------------------------------------------------------------------
// Name: RichLabel::mousePressEvent
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
void RichLabel::mousePressEvent( QMouseEvent* e )
{
    const QPoint pos = mapToParent( e->pos() );
    if( richText_->inText( pos ) )
    {
        const QString url = richText_->anchorAt( pos );
        if( !url.isEmpty() )
            setCursor( QCursor::pointingHandCursor );
    }
}

// -----------------------------------------------------------------------------
// Name: RichLabel::drawContents
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
void RichLabel::drawContents( QPainter* p )
{
    richText_->setWidth( p, width() );
    QLabel::drawContents( p );
}

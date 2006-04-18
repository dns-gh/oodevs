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

// -----------------------------------------------------------------------------
// Name: RichLabel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
RichLabel::RichLabel( const QString& text, QWidget* parent, const char* name )
    : QLabel( text, parent, name )
{
    setAlignment( AlignVCenter | AlignLeft );
}

// -----------------------------------------------------------------------------
// Name: RichLabel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
RichLabel::RichLabel( const QString& text, bool required, QWidget* parent, const char* name )
    : QLabel( text, parent, name )
{
    if( required )
    {
        QFont font = this->font();
        font.setBold( true );
        setFont( font );
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

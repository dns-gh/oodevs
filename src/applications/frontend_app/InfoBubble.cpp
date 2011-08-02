// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "InfoBubble.h"
#include "resources.h"
#include <QtGui/qlabel.h>
#include <QtGui/qpixmap.h>

// -----------------------------------------------------------------------------
// Name: InfoBubble constructor
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
InfoBubble::InfoBubble( QWidget* parent )
    : Q3HGroupBox( parent )
{
    setMargin( 5 );
    icon_ = new QLabel( this );
    icon_->setFixedSize( 60, 48 );

    message_ = new QLabel( this );
    message_->setAlignment( Qt::TextWordWrap );
//    hide();
}

// -----------------------------------------------------------------------------
// Name: InfoBubble destructor
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
InfoBubble::~InfoBubble()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoBubble::ShowError
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
void InfoBubble::ShowError( const QString& message )
{
    icon_->setPixmap( MAKE_PIXMAP( error ) );
    message_->setText( message );
    show();
}

// -----------------------------------------------------------------------------
// Name: InfoBubble::ShowWarning
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
void InfoBubble::ShowWarning( const QString& message )
{
    icon_->setPixmap( MAKE_PIXMAP( warning ) );
    message_->setText( message );
    show();
}

// -----------------------------------------------------------------------------
// Name: InfoBubble::ShowInfo
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
void InfoBubble::ShowInfo( const QString& message )
{
    icon_->setPixmap( MAKE_PIXMAP( info ) );
    message_->setText( message );
    show();
}

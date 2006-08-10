// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AboutDialog.h"
#include "splashscreen.cpp"
#include "astec_gui/RichLabel.h"
#include "astec_gui/ItemFactory_ABC.h"

// -----------------------------------------------------------------------------
// Name: AboutDialog constructor
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
AboutDialog::AboutDialog( QWidget* parent, ItemFactory_ABC& factory )
    : QDialog( parent, 0, FALSE, WStyle_Splash )
{
    setCaption( tr( "A Propos" ) );
    QPixmap pixmap( qembed_findData( "astec.jpg" ) );
    sizeHint_.setWidth ( pixmap.width() );
    sizeHint_.setHeight( pixmap.height() + 50 );
    setMaximumSize( sizeHint_ );
    setMinimumSize( sizeHint_ );

    QVBox* box = new QVBox( this );
    box->setBackgroundPixmap( pixmap );
    box->setMinimumSize( sizeHint_ );
    box->layout()->setAlignment( Qt::AlignBottom | Qt::AlignRight );
    QHBox* hbox = new QHBox( box );
    QString message;
    message += QString( APP_NAME ) + QString( " " ) + QString( APP_VERSION )
            + tr( " © 2006 Masa-SCI <a href=\"http://www.masa-sci.com\">www.masa-sci.com</a>" );
    RichLabel* label = factory.CreateLabel( message, hbox );
    label->setAlignment( Qt::SingleLine );
    label->setBackgroundPixmap( pixmap );
    label->setBackgroundOrigin( QWidget::AncestorOrigin );
    QPushButton* button = new QPushButton( tr( "Fermer" ), hbox );
    connect( button, SIGNAL( clicked() ), this, SLOT( accept() ) );
}
    
// -----------------------------------------------------------------------------
// Name: AboutDialog destructor
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
AboutDialog::~AboutDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AboutDialog::sizeHint
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
QSize AboutDialog::sizeHint() const
{
    return sizeHint_;
}

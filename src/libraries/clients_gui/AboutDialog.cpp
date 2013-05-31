// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AboutDialog.h"
#include "moc_AboutDialog.cpp"
#include "ItemFactory_ABC.h"
#include "RichLabel.h"
#include "RichPushButton.h"
#include "SubObjectName.h"
#include "Tools.h"
#include "ImageWrapper.h"
#include "tools/GeneralConfig.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: AboutDialog constructor
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
AboutDialog::AboutDialog( QWidget* parent, const QString& line, const QString& license )
    : QDialog( parent, 0, true, Qt::WStyle_Splash )
{
    SubObjectName subObject( "AboutDialog" );
    setCaption( tools::translate( "Application", "About" ) );

    QPixmap pixmap;
    const tools::Path filename = tools::Path::FromUnicode( tools::translate( "Application", "images/gui/splash_swordot.png" ).toStdWString() );
    pixmap = gui::Pixmap( filename.IsEmpty()
        ? tools::GeneralConfig::BuildResourceChildFile( "images/gui/splash_swordot.png" )
        : tools::GeneralConfig::BuildResourceChildFile( filename ) );

    setMinimumSize( pixmap.width(), pixmap.height() + 50 );
    Q3VBox* vbox = new Q3VBox( this );
    {
        QLabel* label = new QLabel( vbox );
        label->setPixmap( pixmap );
    }
    Q3HBox* hbox = new Q3HBox( vbox );
    hbox->setMaximumHeight( 50 );
    QString message;
    message = QString( "%1 - %2<br>%3 <a href=\"http://%4\">%5</a>" )
            .arg( tools::translate( "Application", "SWORD" ) )
            .arg( line )
            .arg( tools::translate( "Application", "%1 MASA Group" ).arg( QDateTime::currentDateTime().date().year() ) )
            .arg( tools::translate( "Application", "www.masagroup.net" ) )
            .arg( tools::translate( "Application", "www.masagroup.net" ) );

    if( !license.isEmpty() )
        message += tr( "<br>License will expire on " ) + license;
    RichLabel* label = new RichLabel( "aboutLabel", message, hbox );
    label->setAlignment( Qt::TextSingleLine | Qt::AlignCenter );
    RichPushButton* button = new RichPushButton( "close", tr( "Close" ), hbox );
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

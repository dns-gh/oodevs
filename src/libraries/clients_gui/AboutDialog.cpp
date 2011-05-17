// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::AboutDialog */

#include "clients_gui_pch.h"
#include "AboutDialog.h"
#include "moc_AboutDialog.cpp"
#include "ItemFactory_ABC.h"
#include "RichLabel.h"
#include "Tools.h"
#include "tools/GeneralConfig.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: AboutDialog constructor
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
AboutDialog::AboutDialog( QWidget* parent, ItemFactory_ABC& factory, const QString& line, const QString& license )
: QDialog( parent, 0, true, Qt::WStyle_Splash )
{
    setCaption( tr( "About" ) );
    QPixmap pixmap( GetSplashScreen() );
    sizeHint_.setWidth ( pixmap.width() );
    sizeHint_.setHeight( pixmap.height() + 50 );
    setMaximumSize( sizeHint_ );
    setMinimumSize( sizeHint_ );

    QVBox* vbox = new QVBox( this );
    vbox->setMinimumSize( sizeHint_ );
    QVBox* box = new QVBox( vbox );
    box->setBackgroundPixmap( pixmap );
    QHBox* hbox = new QHBox( vbox );
    hbox->setMaximumHeight( 50 );
    QString message;
    message = QString( "%1 - %2<br>%3 <a href=\"http://%4\">%5</a>" )
            .arg( tools::translate( "Application", "SWORD" ) )
            .arg( line )
            .arg( tools::translate( "Application", "© 2011 MASA Group" ) )
            .arg( tools::translate( "Application", "www.masagroup.net" ) )
            .arg( tools::translate( "Application", "www.masagroup.net" ) );

    if( ! license.isNull() )
        message += tr( "<br>License will expire on " ) + license;
    RichLabel* label = factory.CreateLabel( message, hbox );
    label->setAlignment( Qt::SingleLine | Qt::AlignCenter );
    QPushButton* button = new QPushButton( tr( "Close" ), hbox );
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

// -----------------------------------------------------------------------------
// Name: AboutDialog::GetSplashScreen
// Created: SBO 2008-08-19
// -----------------------------------------------------------------------------
QString AboutDialog::GetSplashScreen() const
{
    const QString filename = tools::translate( "Application", "images/gui/splash_swordot.png" );
    if( filename.isNull() || filename.isEmpty() )
        return tools::GeneralConfig::BuildResourceChildFile( "images/gui/splash_swordot.png" ).c_str();
    return tools::GeneralConfig::BuildResourceChildFile( filename.ascii() ).c_str();
}
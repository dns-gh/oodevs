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
#include "ImageWrapper.h"

#include "clients_kernel/Tools.h"

#include "tools/GeneralConfig.h"
#include "tools/VersionHelper.h"
#include "tools/Version.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: AboutDialog constructor
// Created: SBO 2006-05-04
// -----------------------------------------------------------------------------
AboutDialog::AboutDialog( QWidget* parent, const QString& license )
    : QDialog( parent, 0, true, Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint )
{
    SubObjectName subObject( "AboutDialog" );
    setCaption( tools::translate( "Application", "About" ) );

    QVBoxLayout* vbox = new QVBoxLayout( this );
    vbox->setMargin( 0 );
    vbox->setContentsMargins( 0, 0, 0, 8 );

    QLabel* pixLabel = new QLabel();
    pixLabel->setPixmap( gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/splash.png" ) ) );

    QString message;
    message = QString( "%1 - %2<br/> [%3] <br/> %4 <br/> <a href=\"http://%5\">%6</a>" )
            .arg( tools::translate( "Application", "SWORD" ) )
            .arg( tools::AppProjectVersion() )
            .arg( QString::fromStdString( tools::GetFullVersion( "missing" ) ) )
            .arg( tools::translate( "Application", "%1 MASA Group" ).arg( QDateTime::currentDateTime().date().year() ) )
            .arg( tools::translate( "Application", "www.masagroup.net" ) )
            .arg( tools::translate( "Application", "www.masagroup.net" ) );

    if( !license.isEmpty() )
        message += tr( "<br>License will expire on " ) + license;
    RichLabel* label = new RichLabel( "aboutLabel", message );
    label->setAlignment( Qt::TextSingleLine | Qt::AlignCenter );
    label->setTextInteractionFlags( Qt::TextSelectableByMouse );

    vbox->addWidget( pixLabel, 1, Qt::AlignCenter );
    vbox->addWidget( label, 0, Qt::AlignCenter );
    setMaximumSize( 64, 64 );
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

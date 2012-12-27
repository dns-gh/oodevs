// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "SessionTray.h"
#include "TrayMenu.h"
#include "clients_kernel/Tools.h"
#include "tools/GeneralConfig.h"
#include <QtGui/qsystemtrayicon.h>

// -----------------------------------------------------------------------------
// Name: SessionTray constructor
// Created: RDS 2008-09-26
// -----------------------------------------------------------------------------
SessionTray::SessionTray( QWidget *parent )
    : trayIcon_( *new QSystemTrayIcon( QIcon( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ), parent ) )
{
    trayIcon_.show();
    QObject::connect( &trayIcon_, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ), parent, SLOT( Maximize( QSystemTrayIcon::ActivationReason ) ) );
    trayIcon_.setContextMenu( new TrayMenu( parent ) );
}

// -----------------------------------------------------------------------------
// Name: SessionTray destructor
// Created: RDS 2008-09-26
// -----------------------------------------------------------------------------
SessionTray::~SessionTray()
{
    trayIcon_.hide();
}

// -----------------------------------------------------------------------------
// Name: SessionTray::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void SessionTray::OnLanguageChanged()
{
    trayIcon_.setToolTip( tools::translate( "SessionTray", "Show window" ) );
}

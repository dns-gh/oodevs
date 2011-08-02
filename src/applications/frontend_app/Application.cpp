// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "Application.h"
#include "MainWindow.h"
#include "ENT/ENT_Tr.h"
#include "clients_gui/Tools.h"
#include <QtCore/qsettings.h>
#include <QtCore/qtextcodec.h>

namespace
{
    QString ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }
}

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv )
    : Application_ABC ( argc, argv )
{
    const QString locale = ReadLang();
    AddTranslator( locale, "qt" );
    AddTranslator( locale, "ENT" );
    AddTranslator( locale, "frontend" );
    AddTranslator( locale, "frontend_app" );
    AddTranslator( locale, "clients_kernel" );
    AddTranslator( locale, "clients_gui" );
    AddTranslator( locale, "clients_gui_sword" );
    ENT_Tr::InitTranslations();
    Q3MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
    connect( this, SIGNAL( lastWindowClosed() ), SLOT( quit() ) );
}

// -----------------------------------------------------------------------------
// Name: Application destructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
Application::~Application()
{
    // NOTHING
}


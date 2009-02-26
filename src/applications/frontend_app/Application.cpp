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
#include <qsettings.h>
#include <qtextcodec.h>

namespace
{
    QString ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", "SWORD Officer Training" );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }
}

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv )
    : QApplication ( argc, argv )
{
    const QString locale = ReadLang();
    AddTranslator( "qt", locale );
    AddTranslator( "ENT", locale );
    AddTranslator( "frontend", locale );
    AddTranslator( "frontend_app", locale );
    AddTranslator( "clients_kernel", locale );
    AddTranslator( "clients_gui", locale );
    AddTranslator( "tools", locale );
    ENT_Tr::InitTranslations();
    QMainWindow* mainWindow = new MainWindow();
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

// -----------------------------------------------------------------------------
// Name: Application::AddTranslator
// Created: SBO 2008-04-09
// -----------------------------------------------------------------------------
void Application::AddTranslator( const char* t, const QString& locale )
{
    std::auto_ptr< QTranslator > trans( new QTranslator( this ) );
    const QString file = QString( "%1_%2" ).arg( t ).arg( locale );
    if( trans->load( file, "." ) || trans->load( file, "resources/locales" ) )
       installTranslator( trans.release() );
}

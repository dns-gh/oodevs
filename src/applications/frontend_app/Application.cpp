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

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv, const QString& locale )
    : QApplication ( argc, argv )
{
    AddTranslator( "qt", locale );
    AddTranslator( "ENT", locale );
    AddTranslator( "frontend", locale );
    AddTranslator( "frontend_app", locale );
    AddTranslator( "clients_kernel", locale );
    AddTranslator( "clients_gui", locale );
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
    if( trans->load( t + locale, "." ) || trans->load( t + locale, "resources/locales" ) )
       installTranslator( trans.release() );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Application_ABC.h"
#include <QtCore/qtranslator.h>

// -----------------------------------------------------------------------------
// Name: Application_ABC constructor
// Created: HBD 2010-06-28
// -----------------------------------------------------------------------------
Application_ABC::Application_ABC( int& argc, char** argv )
    : QApplication( argc, argv )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Application_ABC::AddTranslator
// Created: HBD 2010-06-28
// -----------------------------------------------------------------------------
void Application_ABC::AddTranslator( const QString& locale, const char* t )
{
    std::auto_ptr< QTranslator > trans( new QTranslator( this ) );
    const QString file = QString( "%1_%2" ).arg( t ).arg( locale );
    if( trans->load( file, "." ) || trans->load( file, "resources/locales" ) )
        installTranslator( trans.release() );
}

// -----------------------------------------------------------------------------
// Name: Application_ABC destructor
// Created: HBD 2010-06-28
// -----------------------------------------------------------------------------
Application_ABC::~Application_ABC()
{
    // NOTHING
}

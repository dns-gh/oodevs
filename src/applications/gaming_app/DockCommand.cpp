// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "DockCommand.h"
#include "gaming/CommandHandler.h"
#include "gaming/Command.h"
#include <qmainwindow.h>
#include <qdockwindow.h>
#include <string>
#include <boost/function.hpp>

// -----------------------------------------------------------------------------
// Name: DockCommand constructor
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
DockCommand::DockCommand( CommandHandler& handler, QMainWindow& mainWindow )
    : handler_( handler )
    , mainWindow_( mainWindow )
{
    handler_.Register( "dock", *this );
}

// -----------------------------------------------------------------------------
// Name: DockCommand destructor
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
DockCommand::~DockCommand()
{
    handler_.Unregister( "dock", *this );
}

// -----------------------------------------------------------------------------
// Name: DockCommand::Receive
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
void DockCommand::Receive( const Command& command )
{
    if( command.ArgumentCount() == 0 )
        return;

    boost::function1< void, QDockWindow* > func = command.Argument( 1 ) == "show" ? std::mem_fun( &QDockWindow::show ) : std::mem_fun( &QDockWindow::hide );
    QStringList list;
    for( unsigned int i = 2; i <= command.ArgumentCount(); ++i )
        list.append( command.Argument( i ).c_str() );
    QPtrList< QDockWindow > docks = mainWindow_.dockWindows();
    for( QPtrList< QDockWindow >::iterator it = docks.begin(); it != docks.end(); ++it )
        if( !(*it)->inherits( "QToolBar" ) && ( list.empty() || list.contains( (*it)->name() ) ) )
            func( *it );
}

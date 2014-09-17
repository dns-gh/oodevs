// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LinkInterpreter_ABC.h"
#include "moc_LinkInterpreter_ABC.cpp"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC constructor
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
LinkInterpreter_ABC::LinkInterpreter_ABC( QObject* parent, ActionController& actions )
    : QObject( parent )
    , actions_( actions )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC destructor
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
LinkInterpreter_ABC::~LinkInterpreter_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::Interprete
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter_ABC::Interprete( const QString& link )
{
    return Interprete( QUrl( link ) );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::Interprete
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter_ABC::Interprete( const QUrl& url )
{
    const QString protocol = url.protocol();
    if( protocol == "cmd" )
        return ExecuteCommand( url.path() );
    if( protocol == "http" || protocol == "file" )
        return ExecuteCommand( url );
    if( protocol == "id" )
        return InterpreteId( url );
    return false;
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::ExecuteCommand
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter_ABC::ExecuteCommand( const QUrl& url )
{
    Q3Process openPage(0);
    openPage.addArgument( "cmd" );
    openPage.addArgument( "/c" );
    openPage.addArgument( "start" );
    openPage.addArgument( url.toString() ); // $$$$ ABR 2013-04-22: Bugged, use ShellExecuteW instead LinkInterpreter if it's a file:// url
    return openPage.start();
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::InterpreteId
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter_ABC::InterpreteId( const QUrl& url )
{
    const QString classId = url.host();
    const QString strId = url.fileName();
    const QString ref = url.ref();
    bool ok = false;
    const unsigned long id = strId.toULong( &ok );

    const Entity_ABC* entity = entites_[ classId.toStdString() ][ id ];
    return entity != 0 && InterpreteEntity( *entity, ref );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::InterpreteEntity
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
bool LinkInterpreter_ABC::InterpreteEntity( const kernel::Entity_ABC& entity, const QString& action )
{
    entity.Activate( actions_ );
    if( action == "select" )
        actions_.SetSelected( entity, false );
    return true;
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::AddEntity
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter_ABC::AddEntity( const std::string& category, const Entity_ABC& entity )
{
    entites_[ category ][ entity.GetId() ] = & entity;
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::RemoveEntity
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter_ABC::RemoveEntity( const std::string& category, const Entity_ABC& entity )
{
    entites_[ category ][ entity.GetId() ] = 0;
}

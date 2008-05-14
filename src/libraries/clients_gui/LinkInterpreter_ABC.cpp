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

#include <qprocess.h>

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
    if( protocol == "cmd" || protocol == "http" )
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
    QProcess openPage(0);
    openPage.addArgument( "cmd" );
    openPage.addArgument( "/c" );
    openPage.addArgument( "start" );
    openPage.addArgument( url );
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
    bool ok = false;
    const unsigned long id = strId.toULong( &ok );

    const Entity_ABC* entity = entites_[ classId ][ id ];
    if( entity )
        entity->Activate( actions_ );
    return entity != 0;
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::AddEntity
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter_ABC::AddEntity( const QString& category, const Entity_ABC& entity )
{
    entites_[ category ][ entity.GetId() ] = & entity;
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::RemoveEntity
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter_ABC::RemoveEntity( const QString& category, const Entity_ABC& entity )
{
    entites_[ category ][ entity.GetId() ] = 0;
}
 
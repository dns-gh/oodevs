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
    return Interprete( Q3Url( link ) );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::Interprete
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter_ABC::Interprete( const Q3Url& url )
{
    const QString protocol = url.protocol();
    if( protocol == "cmd" )
        return ExecuteCommand( url.path( true ) );
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
bool LinkInterpreter_ABC::ExecuteCommand( const Q3Url& url )
{
    Q3Process openPage(0);
    openPage.addArgument( "cmd" );
    openPage.addArgument( "/c" );
    openPage.addArgument( "start" );
    openPage.addArgument( url.toString( true, true ) );
    return openPage.start();
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::InterpreteId
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter_ABC::InterpreteId( const Q3Url& url )
{
    const QString classId = url.host();
    const QString strId = url.fileName();
    const QString ref = url.ref();
    bool ok = false;
    const unsigned long id = strId.toULong( &ok );

    const Entity_ABC* entity = entites_[ classId ][ id ];
    return entity != 0 && InterpreteEntity( *entity, ref );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter_ABC::InterpreteEntity
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
bool LinkInterpreter_ABC::InterpreteEntity( const kernel::Entity_ABC& entity, const QString& action )
{
    if( action == "select" )
        entity.Select( actions_ );
    else
        entity.Activate( actions_ );
    return true;
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

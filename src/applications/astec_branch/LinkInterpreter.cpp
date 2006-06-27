// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LinkInterpreter.h"
#include "InternalLinks.h"
#include "moc_LinkInterpreter.cpp"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "Controllers.h"
#include "ActionController.h"

#include <qprocess.h>

// -----------------------------------------------------------------------------
// Name: LinkInterpreter constructor
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
LinkInterpreter::LinkInterpreter( QObject* parent, Controllers& controllers, Model& model )
    : QObject( parent )
    , controllers_( controllers )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter destructor
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
LinkInterpreter::~LinkInterpreter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::Interprete
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter::Interprete( const QString& link )
{
    int pos = link.find( "://" );
    if( pos > 0 )
    {
        const QString protocole = link.left( pos );
        const QString address   = link.mid( pos + 3 );
        return Interprete( protocole, address );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::Interprete
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter::Interprete( const QString& protocole, const QString& address )
{
    if( protocole == "cmd" )
        return ExecuteCommand( address );
    if( protocole == "http" )
        return FollowLink( address );
    if( protocole == "id" )
        return InterpreteId( address );
    return false;
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::FollowLink
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter::FollowLink( const QString& address )
{
    return ExecuteCommand( "http://" + address );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::ExecuteCommand
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter::ExecuteCommand( const QString& address )
{
    QProcess openPage(0);
    openPage.addArgument( "cmd" );
    openPage.addArgument( "/c" );
    openPage.addArgument( "start" );
    openPage.addArgument( address );
    return openPage.start();
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::InterpreteId
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter::InterpreteId( const QString& address )
{
    int pos = address.find( '/' );
    if( pos > 0 )
    {
        const QString classId = address.left( pos );
        const QString id      = address.mid( pos + 1 );
        return InterpreteId( classId, id );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::Activate
// Created: AGE 2006-05-12
// -----------------------------------------------------------------------------
template< typename T >
bool LinkInterpreter::Activate( const Resolver_ABC< T >& resolver, unsigned long id )
{
    const T* activated = resolver.Find( id );
    if( activated )
        controllers_.actions_.Activate( *activated );
    return activated;
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::InterpreteId
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter::InterpreteId( const QString& classId, const QString& strId )
{
    bool ok = false;
    unsigned long id = strId.toULong( &ok );
    return ok && ( 
        ( classId == InternalLinks::agent_      && Activate< Agent >( model_.agents_, id ) )
     || ( classId == InternalLinks::object_     && Activate< Object >( model_.objects_, id ) )
     || ( classId == InternalLinks::population_ && Activate< Population >( model_.agents_, id ) ) );
    // $$$$ AGE 2006-06-27: manque les connaissances. Observer les KnowledgeGroups ?
}

 
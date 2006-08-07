// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "LinkInterpreter.h"
#include "InternalLinks.h"
#include "moc_LinkInterpreter.cpp"
#include "astec_gaming/Model.h"
#include "astec_gaming/AgentsModel.h"
#include "astec_gaming/ObjectsModel.h"
#include "astec_gaming/Controllers.h"
#include "astec_gaming/ActionController.h"

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
    return Interprete( QUrl( link ) );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::Interprete
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter::Interprete( const QUrl& url )
{
    const QString protocol = url.protocol();
    if( protocol == "cmd" || protocol == "http" )
        return ExecuteCommand( url );
    if( protocol == "id" )
        return InterpreteId( url );
    return false;
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::ExecuteCommand
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter::ExecuteCommand( const QUrl& url )
{
    QProcess openPage(0);
    openPage.addArgument( "cmd" );
    openPage.addArgument( "/c" );
    openPage.addArgument( "start" );
    openPage.addArgument( url );
    return openPage.start();
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::InterpreteId
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter::InterpreteId( const QUrl& url )
{
    const QString classId = url.host();
    const QString strId = url.fileName();

    bool ok = false;
    const unsigned long id = strId.toULong( &ok );
    return ok && ( 
        ( classId == InternalLinks::agent_      && Activate< Agent_ABC >( model_.agents_, id ) )
     || ( classId == InternalLinks::object_     && Activate< Object_ABC >( model_.objects_, id ) )
     || ( classId == InternalLinks::population_ && Activate< Population >( model_.agents_, id ) ) );
    // $$$$ AGE 2006-06-27: manque les connaissances. Observer les KnowledgeGroups ?
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


 
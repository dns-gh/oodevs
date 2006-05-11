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
#include "moc_LinkInterpreter.cpp"
#include <qprocess.h>

// -----------------------------------------------------------------------------
// Name: LinkInterpreter constructor
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
LinkInterpreter::LinkInterpreter( QObject* parent )
    : QObject( parent )
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
// Name: LinkInterpreter::InterpreteId
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
bool LinkInterpreter::InterpreteId( const QString& classId, const QString& id )
{
    return false; // $$$$ AGE 2006-05-11: 
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::Cleanup
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
QString LinkInterpreter::Cleanup( const QString& className )
{
    return QString( className ).remove( "class " );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::GetAddress
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
QString LinkInterpreter::GetAddress( void* add )
{
    return QString::number( reinterpret_cast< int >( add ) );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::Embed
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
QString LinkInterpreter::Embed( const QString& message, const QString& classId, const QString& id )
{
    return "<a href=\"id://" + classId + "/" + id + "\">" + message + "</a>";
}


 
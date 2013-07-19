// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "XQuery.h"
#include <tools/Path.h>

#pragma warning( disable: 4127 )
#pragma warning( disable: 4251 )

#include <QtXmlPatterns/QXmlQuery>
#include <QtCore/QFile>

QStringList kernel::xquery::Evaluate( const tools::Path& xmlFilePath, const QString& queryStr )
{
    QFile xmlFile( QString::fromStdString( xmlFilePath.ToUTF8() ) );
    if( !xmlFile.exists() )
        throw MASA_EXCEPTION( std::string( "Invalid xml file: " ) + xmlFile.fileName().toStdString() );
    if( !xmlFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
        throw MASA_EXCEPTION( std::string( "Can't open xml file: " ) + xmlFile.fileName().toStdString() );

    QStringList result;
    QXmlQuery query;

    query.setFocus( &xmlFile );
    query.setQuery( queryStr );
    if( !query.isValid())
        throw MASA_EXCEPTION( std::string( "Invalid query: " ) + queryStr.toStdString() );
    query.evaluateTo( &result );

    xmlFile.close();
    return ( result.size() == 1 && result[ 0 ].size() == 0 ) ? QStringList() : result;
}

// -----------------------------------------------------------------------------
// Name: XQuery::Evaluate
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
QStringList kernel::xquery::Evaluate( const QString& text, const QString& queryStr )
{
    QStringList result;
    QXmlQuery query;

    query.setFocus( text );
    query.setQuery( queryStr );
    if( !query.isValid())
        throw MASA_EXCEPTION( std::string( "Invalid query: " ) + queryStr.toStdString() );
    query.evaluateTo( &result );

    return ( result.size() == 1 && result[ 0 ].size() == 0 ) ? QStringList() : result;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ConnectionDatabase.h"
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

using namespace plugins;
using namespace plugins::crossbow;

namespace
{
    void ThrowError()
    {
        IErrorInfoPtr ipError;
        BSTR strError;
        ::GetErrorInfo( 0, &ipError );
        if ( ipError != NULL )
        {
            ipError->GetDescription( &strError );
            const std::string error = boost::lexical_cast< std::string >( _bstr_t( strError ) );
            MT_LOG_ERROR_MSG( error ); // $$$$ SBO 2008-05-15: should throw
        }
        else
            MT_LOG_ERROR_MSG( "Unable to retrieve details" );
    }
}

// -----------------------------------------------------------------------------
// Name: ConnectionDatabase constructor
// Created: JCR 2009-02-03
// -----------------------------------------------------------------------------
ConnectionDatabase::ConnectionDatabase( const std::string& url )
{    
    IWorkspaceFactoryPtr spWorkspaceFactory;
    spWorkspaceFactory.CreateInstance( CLSID_SdeWorkspaceFactory );

    if( spWorkspaceFactory == NULL )
        throw std::runtime_error( "Unable to create sde workspace factory." );

    IWorkspacePtr workspace;
    if( FAILED( spWorkspaceFactory->Open( CreateProperty( url ), 0, &workspace ) ) )
    {
        ThrowError();
        MT_LOG_ERROR_MSG( "Unable to open database connection." );
        throw std::runtime_error( "Unable to open database connection." );
    }
    else
    {
        Initialize( workspace );
        MT_LOG_INFO_MSG( "Connected to " + url + "." );
    }    
}

// -----------------------------------------------------------------------------
// Name: ConnectionDatabase destructor
// Created: JCR 2009-02-03
// -----------------------------------------------------------------------------
ConnectionDatabase::~ConnectionDatabase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectionDatabase::CreateProperty
// Created: JCR 2009-02-11
// -----------------------------------------------------------------------------
IPropertySetPtr ConnectionDatabase::CreateProperty( const std::string& url )
{
    // sde://user:pass@server[:port]/database.schema
    static boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)@([\\w\\-\\.]+)(:(\\d+)){0,1}\\/(\\w*).(\\w*)" );
    boost::cmatch matches;
    IPropertySetPtr spProperty;

    if ( boost::regex_match( url.c_str(), matches, expression ) )
    {            
        spProperty.CreateInstance( CLSID_PropertySet );
        database_ = matches[ 7 ];
        schema_ = matches[ 8 ];

        const std::string server( matches[ 4 ] );
        const std::string protocol( matches[ 1 ] );
        spProperty->SetProperty( CComBSTR( "server" ), CComVariant( server.c_str() ) );
        spProperty->SetProperty( CComBSTR( "instance" ), CComVariant( std::string( protocol + ":postgresql:" + server ).c_str() ) );
        spProperty->SetProperty( CComBSTR( "user" ), CComVariant( std::string( matches[ 2 ] ).c_str() ) );
        spProperty->SetProperty( CComBSTR( "password" ), CComVariant( std::string( matches[ 3 ] ).c_str() ) );
        if ( matches[ 6 ] != "" )
            spProperty->SetProperty( CComBSTR( "port" ), CComVariant( std::string( matches[ 6 ] ).c_str() ) );
        spProperty->SetProperty( CComBSTR( "database" ),  CComVariant( database_.c_str() ) );
        spProperty->SetProperty( CComBSTR( "version" ),  CComVariant( "sde.DEFAULT" ) );

        std::string msg = "server:" + server + ":" + matches[ 6 ] + ", " +
            "instance:" + protocol + ":postgresql:" + server + ", " +
            "user:" + matches[ 2 ] + ", " +
            "password:" + matches[ 3 ] + ", " +
            "database:" + database_ + "." + schema_ + ", " +
            "version:sde.DEFAULT";
        MT_LOG_INFO_MSG( msg );
    }
    return spProperty;
}

// -----------------------------------------------------------------------------
// Name: ConnectionDatabase::GetTableName
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
std::string ConnectionDatabase::GetTableName( const std::string& name ) const
{
    if ( database_ != "" && schema_ != "" )
        return database_ + "." + schema_ + "." + name;
    return name;
}

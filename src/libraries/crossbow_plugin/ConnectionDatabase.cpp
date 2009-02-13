// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "esri_plugin_pch.h"
#include "ConnectionDatabase.h"
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

using namespace plugins;
using namespace plugins::esri;

namespace
{
    void ThrowError()
    {
        IErrorInfoPtr ipError;
        BSTR strError;
        ::GetErrorInfo( 0, &ipError );
        ipError->GetDescription( &strError );
        const std::string error = boost::lexical_cast< std::string >( _bstr_t( strError ) );
        MT_LOG_ERROR_MSG( error ); // $$$$ SBO 2008-05-15: should throw
    }
}

// -----------------------------------------------------------------------------
// Name: ConnectionDatabase constructor
// Created: JCR 2009-02-03
// -----------------------------------------------------------------------------
ConnectionDatabase::ConnectionDatabase( const std::string& url )
{    
    try
    {
        IWorkspaceFactoryPtr spWorkspaceFactory;
        spWorkspaceFactory.CreateInstance( CLSID_SdeWorkspaceFactory );

        if( spWorkspaceFactory == NULL )
            throw std::runtime_error( "Unable to create sde workspace factory." );

        IWorkspacePtr workspace;
        if( SUCCEEDED( spWorkspaceFactory->Open( CreateProperty( url ), 0, &workspace ) ) )
            Initialize( workspace );
        else
            ThrowError();
    }
    catch( std::exception& e )
    {
        MT_LOG_INFO_MSG( e.what() );
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
    // sde://user:pass@server:port/database
    static boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)@(\\w+)(:(\\d+)){0,1}\\/(\\w*)" );
    boost::cmatch matches;
    IPropertySetPtr spProperty;

    if ( boost::regex_match( url.c_str(), matches, expression ) )
    {            
        spProperty.CreateInstance( CLSID_PropertySet );
        protocol_ = matches[ 1 ];
        database_ = matches[ 7 ];
        std::string server( matches[ 4 ] );
        spProperty->SetProperty( CComBSTR( "server" ), CComVariant( server.c_str() ) );
        spProperty->SetProperty( CComBSTR( "instance" ), CComVariant( std::string( protocol_ + ":postgresql:" + server ).c_str() ) );
        spProperty->SetProperty( CComBSTR( "user" ), CComVariant( std::string( matches[ 2 ] ).c_str() ) );
        spProperty->SetProperty( CComBSTR( "password" ), CComVariant( std::string( matches[ 3 ] ).c_str() ) );
        if ( matches[ 6 ] != "" )
            spProperty->SetProperty( CComBSTR( "port" ), CComVariant( std::string( matches[ 6 ] ).c_str() ) );
        spProperty->SetProperty( CComBSTR( "database" ),  CComVariant( database_.c_str() ) );
        spProperty->SetProperty( CComBSTR( "version" ),  CComVariant( "sde.DEFAULT" ) );

        std::string msg = "server:" + server + ", " +
            "instance:" + protocol_ + ":postgresql:" + server + ", " +
            "user:" + matches[ 2 ] + ", " +
            "password:" + matches[ 3 ] + ", " +
            "database:" + database_ + ", " +
            "version:sde.DEFAULT";
        MT_LOG_INFO_MSG( msg );
    }
    return spProperty;
}

// -----------------------------------------------------------------------------
// Name: ConnectionDatabase::Decorate
// Created: JCR 2009-02-11
// -----------------------------------------------------------------------------
std::string ConnectionDatabase::Decorate( const std::string& name )
{
    if ( database_ != "" && protocol_ != "" )
        return database_ + "." + protocol_ + "." + name;
    return name;
}

// -----------------------------------------------------------------------------
// Name: ConnectionDatabase::OpenTable
// Created: JCR 2009-02-11
// -----------------------------------------------------------------------------
Table_ABC* ConnectionDatabase::OpenTable( const std::string& name )
{
    return Database::OpenTable( Decorate( name ) );
}

// -----------------------------------------------------------------------------
// Name: ConnectionDatabase::OpenBufferedTable
// Created: JCR 2009-02-11
// -----------------------------------------------------------------------------
Table_ABC& ConnectionDatabase::OpenBufferedTable( const std::string& name, bool clear /*= true*/ )
{
    return Database::OpenBufferedTable( Decorate( name ), clear );
}

// -----------------------------------------------------------------------------
// Name: ConnectionDatabase::ClearTable
// Created: JCR 2009-02-11
// -----------------------------------------------------------------------------
void ConnectionDatabase::ClearTable( const std::string& name )
{
    Database::ClearTable( Decorate( name ) );
}
    
// -----------------------------------------------------------------------------
// Name: ConnectionDatabase::ReleaseTable
// Created: JCR 2009-02-11
// -----------------------------------------------------------------------------
void ConnectionDatabase::ReleaseTable( const std::string& name )
{
    Database::ReleaseTable( Decorate( name ) );
}

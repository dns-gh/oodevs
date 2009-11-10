// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "crossbow_plugin/ESRI.h"
#include "crossbow_plugin/QueryBuilder.h"
#include "crossbow_plugin/ConnectionDatabase.h"
#include "crossbow_plugin/Workspace.h"

#include "crossbow_plugin/Point.h"
#include "crossbow_plugin/Line.h"
#include "crossbow_plugin/Area.h"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace plugins;
using namespace plugins::crossbow;

// #define CROSSBOW_TEST_ESRI_CONNECTION
#ifdef CROSSBOW_TEST_ESRI_CONNECTION

namespace
{
    IPropertySetPtr CreateProperty()
    {        
        IPropertySetPtr spProperty;
        spProperty.CreateInstance( CLSID_PropertySet );
        spProperty->SetProperty( CComBSTR( "server" ), CComVariant( "localhost" ) );
        spProperty->SetProperty( CComBSTR( "instance" ), CComVariant( "sde:postgresql:localhost" ) );
        spProperty->SetProperty( CComBSTR( "user" ), CComVariant( "sword" ) );
        spProperty->SetProperty( CComBSTR( "password" ), CComVariant( "sword" ) );
        spProperty->SetProperty( CComBSTR( "port" ), CComVariant( "5432" ) );
        spProperty->SetProperty( CComBSTR( "database" ),  CComVariant( "sword_crossbow_db" ) );
        spProperty->SetProperty( CComBSTR( "version" ),  CComVariant( "sde.DEFAULT" ) );
        return spProperty;
    }

    class ScopedLicence
    {
    public:
        ScopedLicence() 
        {
            ::CoInitialize( NULL );
            IAoInitializePtr ipInit( CLSID_AoInitialize );
            esriLicenseStatus licenseStatus = esriLicenseFailure;
  
	        ipInit->IsProductCodeAvailable(esriLicenseProductCodeEngine, &licenseStatus);
	        if (licenseStatus == esriLicenseAvailable)
                ipInit->Initialize(esriLicenseProductCodeEngine, &licenseStatus);
        }
        ~ScopedLicence()
        {
            IAoInitializePtr ipInit( CLSID_AoInitialize );
            ipInit->Shutdown();
            ipInit.Release();
            ::CoUninitialize();
        }
    };

    void ThrowError()
    {
        IErrorInfoPtr ipError;
        BSTR strError;
        ::GetErrorInfo( 0, &ipError );
        ipError->GetDescription( &strError );
        const std::string error = boost::lexical_cast< std::string >( _bstr_t( strError ) );
        std::cerr << "Failed to open: " << error <<std::endl;
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EsriTest_Connection_Sample )
{
    ScopedLicence licence;
    {
        IWorkspaceFactoryPtr spWorkspaceFactory;
        spWorkspaceFactory.CreateInstance( CLSID_SdeWorkspaceFactory );

        if( spWorkspaceFactory == NULL )
            throw std::runtime_error( "Unable to create sde workspace factory." );

        IWorkspacePtr workspace;
        if ( FAILED( spWorkspaceFactory->Open( CreateProperty(), 0, &workspace ) ) )
        {
            ThrowError();
            BOOST_CHECK( "spWorkspaceFactory->Open failed" && false );
        }
        BOOST_CHECK( workspace != NULL );
    }
}


// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EsriTest_Connection )
{
    crossbow::Workspace scoped;
    {
        IWorkspaceFactoryPtr spWorkspaceFactory;
        spWorkspaceFactory.CreateInstance( CLSID_SdeWorkspaceFactory );

        if( spWorkspaceFactory == NULL )
            throw std::runtime_error( "Unable to create sde workspace factory." );

        IWorkspacePtr workspace;
        BOOST_CHECK( SUCCEEDED( spWorkspaceFactory->Open( CreateProperty(), 0, &workspace ) ) );
        BOOST_CHECK( workspace != NULL );
    }
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EsriTest_CrossbowConnection )
{
    crossbow::Workspace scoped;
    {
    	crossbow::ConnectionDatabase db( "sde://sword:sword@localhost:5432/sword_crossbow_db.sword" );
    }
    std::cout << "crossbow::Worskpace loaded." << std::endl;
}

#endif


// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing )
{        
    const boost::regex expression( "(sde|postgresql):\\/\\/(\\w+):(\\w+)@(\\w+):(\\d+)\\/(\\w*).(\\w*)" );
    boost::cmatch matches;
    
    std::string url( "sde://user:pass@localhost:4242/database.schema" );
    if ( boost::regex_match( url.c_str(), matches, expression ) )
    {
        BOOST_CHECK_EQUAL( matches[ 0 ], url );
        BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
        BOOST_CHECK_EQUAL( matches[ 3 ], "pass" );
        BOOST_CHECK_EQUAL( matches[ 4 ], "localhost" );
        BOOST_CHECK_EQUAL( matches[ 5 ], "4242" );
        BOOST_CHECK_EQUAL( matches[ 6 ], "database" );
        BOOST_CHECK_EQUAL( matches[ 7 ], "schema" );
    }
    else
        BOOST_CHECK( false && " unmatched." );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing_2 )
{
    const boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)" );
    boost::cmatch matches;
    
    std::string url( "sde://user:08/database" );
    if ( boost::regex_match( url.c_str(), matches, expression ) )
    {
        BOOST_CHECK_EQUAL( matches[ 0 ], url );
        BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
        BOOST_CHECK_EQUAL( matches[ 3 ], "08" );
        BOOST_CHECK_EQUAL( matches[ 4 ], "database" );
    }
    else
        BOOST_CHECK( true && " unmatched." );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing_Port )
{
    const boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)@(\\w+)(:(\\d+)){0,1}\\/(\\w*).(\\w*)" );
    boost::cmatch matches;
    
    std::string url( "sde://user:08@localhost/database.schema" );
    if ( boost::regex_match( url.c_str(), matches, expression ) )
    {
        BOOST_CHECK_EQUAL( matches[ 0 ], url );
        BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
        BOOST_CHECK_EQUAL( matches[ 3 ], "08" );
        BOOST_CHECK_EQUAL( matches[ 4 ], "localhost" );
        BOOST_CHECK_EQUAL( matches[ 5 ], "" );
        BOOST_CHECK_EQUAL( matches[ 6 ], "" );
        BOOST_CHECK_EQUAL( matches[ 7 ], "database" );
        BOOST_CHECK_EQUAL( matches[ 8 ], "schema" );
        BOOST_CHECK_EQUAL( matches.size(), 9u );
    }
    else
        BOOST_CHECK( false && " unmatched." );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing_IpAndPort )
{
    const boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)@([\\w\\-\\.]+)(:(\\d+)){0,1}\\/(\\w*).(\\w*)" );
    boost::cmatch matches;
    
    std::string url( "sde://user:08@172.0.65.2/database.schema" );
    if ( boost::regex_match( url.c_str(), matches, expression ) )
    {
        BOOST_CHECK_EQUAL( matches[ 0 ], url );
        BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
        BOOST_CHECK_EQUAL( matches[ 3 ], "08" );
        BOOST_CHECK_EQUAL( matches[ 4 ], "172.0.65.2" );
        BOOST_CHECK_EQUAL( matches[ 5 ], "" );
        BOOST_CHECK_EQUAL( matches[ 6 ], "" );
        BOOST_CHECK_EQUAL( matches[ 7 ], "database" );
        BOOST_CHECK_EQUAL( matches[ 8 ], "schema" );
        BOOST_CHECK_EQUAL( matches.size(), 9u );
    }
    else
        BOOST_CHECK( false && " unmatched." );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing_HostAndPort )
{
    const boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)@([\\w\\-\\.]+)(:(\\d+)){0,1}\\/(\\w*).(\\w*)" );
    boost::cmatch matches;
    
    std::string url( "sde://user:08@my-host-25/database.schema" );
    if ( boost::regex_match( url.c_str(), matches, expression ) )
    {
        BOOST_CHECK_EQUAL( matches[ 0 ], url );
        BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
        BOOST_CHECK_EQUAL( matches[ 3 ], "08" );
        BOOST_CHECK_EQUAL( matches[ 4 ], "my-host-25" );
        BOOST_CHECK_EQUAL( matches[ 5 ], "" );
        BOOST_CHECK_EQUAL( matches[ 6 ], "" );
        BOOST_CHECK_EQUAL( matches[ 7 ], "database" );
        BOOST_CHECK_EQUAL( matches[ 8 ], "schema" );
        BOOST_CHECK_EQUAL( matches.size(), 9u );
    }
    else
        BOOST_CHECK( false && " unmatched." );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing_PortOptional )
{
    const boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)@(\\w+)(:(\\d+)){0,1}\\/(\\w*).(\\w*)" );
    boost::cmatch matches;
    
    std::string url( "sde://user:08@localhost:4242/database.schema" );
    if ( boost::regex_match( url.c_str(), matches, expression ) )
    {
        BOOST_CHECK_EQUAL( matches[ 0 ], url );
        BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
        BOOST_CHECK_EQUAL( matches[ 3 ], "08" );
        BOOST_CHECK_EQUAL( matches[ 4 ], "localhost" );
        BOOST_CHECK_EQUAL( matches[ 5 ], ":4242" );
        BOOST_CHECK_EQUAL( matches[ 6 ], "4242" );
        BOOST_CHECK_EQUAL( matches[ 7 ], "database" );
        BOOST_CHECK_EQUAL( matches[ 8 ], "schema" );
        BOOST_CHECK_EQUAL( matches.size(), 9u );
    }
    else
        BOOST_CHECK( false && " unmatched." );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertId )
{
    InsertQueryBuilder builder( "table" );
    builder.SetId( "id" );
    BOOST_CHECK_EQUAL( "INSERT INTO table (id) VALUES (nextval('table_id_seq'))", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertString )
{
    InsertQueryBuilder builder( "table" );
    builder.SetField( "row1", "v1" );
    builder.SetField( "row2", "v2" );    
    BOOST_CHECK_EQUAL( "INSERT INTO table (row1,row2) VALUES ('v1','v2')", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertNumeric )
{
    InsertQueryBuilder builder( "table" );
    builder.SetField( "row1", 1 );
    builder.SetField( "row2", 2 );    
    BOOST_CHECK_EQUAL( "INSERT INTO table (row1,row2) VALUES (1,2)", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertPoint )
{
    InsertQueryBuilder builder( "table" );
    builder.SetGeometry( crossbow::Point() );
    BOOST_CHECK_EQUAL( "INSERT INTO table (shape) VALUES (st_point(0,0,0))", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_UpdatePoint )
{
    UpdateQueryBuilder builder( "table" );
    builder.SetGeometry( crossbow::Point() );
    BOOST_CHECK_EQUAL( "UPDATE table SET shape=st_point(0,0,0)", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertLine )
{
    InsertQueryBuilder builder( "table" );
    builder.SetGeometry( crossbow::Line() );
    BOOST_CHECK_EQUAL( "INSERT INTO table (shape) VALUES (st_linestring('linestring()',0))", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_InsertPolygon )
{
    InsertQueryBuilder builder( "table" );
    builder.SetGeometry( Area() );
    BOOST_CHECK_EQUAL( "INSERT INTO table (shape) VALUES (st_polygon('polygon(())',0))", builder.Create() );
}


BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_UpdateString )
{
    UpdateQueryBuilder builder( "table" );
    builder.SetField( "row1", "v1" );
    builder.SetField( "row2", "v2" );
    BOOST_CHECK_EQUAL( "UPDATE table SET row1='v1',row2='v2'", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_UpdateQuotedString )
{
    UpdateQueryBuilder builder( "table" );
    builder.SetField( "row1", "l'v1" );
    builder.SetField( "row2", "v''2" );
    BOOST_CHECK_EQUAL( "UPDATE table SET row1='l''v1',row2='v''''2'", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_UpdateNumeric )
{
    UpdateQueryBuilder builder( "table" );
    builder.SetField( "row1", 1 );
    builder.SetField( "row2", 2 );    
    BOOST_CHECK_EQUAL( "UPDATE table SET row1=1,row2=2", builder.Create() );
}

BOOST_AUTO_TEST_CASE( EsriTest_QueryBuilder_UpdateNumericClause )
{
    UpdateQueryBuilder builder( "table" );
    builder.SetField( "row1", 1 );
    builder.SetField( "row2", 2 );
    builder.SetClause( "row_id=4242" );
    BOOST_CHECK_EQUAL( "UPDATE table SET row1=1,row2=2 WHERE row_id=4242", builder.Create() );
}

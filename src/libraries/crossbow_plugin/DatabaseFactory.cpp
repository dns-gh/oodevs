// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "DatabaseFactory.h"
#include "dispatcher/Config.h"
#include "OGR_Database.h"
#include <gdal/ogrsf_frmts.h>

#pragma warning( push, 0 )
#pragma warning( disable: 4127 4244 4245 4996 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/regex.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

using namespace plugins;
using namespace plugins::crossbow;



// -----------------------------------------------------------------------------
// Name: DatabaseFactory constructor
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
DatabaseFactory::DatabaseFactory()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: DatabaseFactory destructor
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
DatabaseFactory::~DatabaseFactory()
{
    // NOTHING
}


namespace 
{
    bool IsFileDatabase( const bfs::path& p )
    {
        const std::string extension( p.extension() );
        return extension == "mdb" || extension == "gdb";
    }

    bool IsShapefileDatabase( const bfs::path& p )
    {
        return p.extension() == "shp";
    }

    bool IsSDEDatabase( const bfs::path& p )
    {
        // boost::regexp_match( )
        return p.root_path() == "sde:";
    }
    
    bool IsPostgreSQLDatabase( const bfs::path& p )
    {
        // boost::regexp_match( )
        return p.root_path() == "postgis:";
    }

    class ConnectionProperty
    {
    public:
        // -----------------------------------------------------------------------------
        // Name: ConnectionProperty constructor
        // @brief: @type is either 'sde' or 'postgres' depending of the source type
        //         @url parameter must have the follow format 
        //          sde://user:pass@server[:port]/database.schema
        // Created: JCR 2009-02-10
        // -----------------------------------------------------------------------------
        explicit ConnectionProperty( const std::string& url, const std::string& type ) 
        {
            boost::regex expression( "(" + type + "):\\/\\/(\\w+):(\\w+)@([\\w\\-\\.]+)(:(\\d+)){0,1}\\/(\\w*).(\\w*)" );
            boost::cmatch matches;
            if ( boost::regex_match( url.c_str(), matches, expression ) )
            {
                protocol_ = matches[ 1 ];
                user_ = matches[ 2 ];
                password_ = matches[ 3 ];
                server_ = matches[ 4 ];
                if ( matches[ 6 ] != "" )
                    port_ = matches[ 6 ];
                database_ = matches[ 7 ];
                schema_ = matches[ 8 ];

                std::string msg = "server:" + server_ + ":" + matches[ 6 ] + ", " +
                    "instance:" + protocol_ + ":postgresql:" + server_ + ", " +
                    "user:" + matches[ 2 ] + ", " +
                    "password:" + matches[ 3 ] + ", " +
                    "database:" + database_ + "." + schema_ + ", " +
                    "version:sde.DEFAULT";
                MT_LOG_INFO_MSG( msg );
            }
        }

        std::string toSDE() const 
        {
            // SDE:server,instance,database,username,password,layer,[version]
            if ( protocol_ != "sde" && user_ == schema_ )
                throw std::runtime_error( "invalid SDE protocol : " + protocol_ );
            return std::string( "SDE:" ) + 
                server_ +  ":" + port_ + "," + 
                protocol_ + ":postgresql:" + server_ + "," + 
                database_ + "," + user_ + "," + password_;
        }

        std::string toPostgreSQL() const 
        {
            // PG:"dbname='databasename' host='addr' port='5432' user='x' password='y'"
            if ( protocol_ != "postgres" )
                throw std::runtime_error( "invalid PG protocol : " + protocol_ );
            return std::string( "PG:\"" ) + 
                    "dbname='" + database_ + "." + schema_ + "'" +
                    "host='" + server_ + "'" +
                    "port='" + port_ + "'" +
                    "user='" + user_ + "'" +
                    "password='" + password_ + "'" + "\"";
        }
    public:
        std::string database_;
        std::string schema_;
    
    private:
        std::string protocol_;
        std::string server_;
        std::string port_;
        std::string user_;
        std::string password_;
    };

}

// -----------------------------------------------------------------------------
// Name: DatabaseFactory::Create
// Created: JCR 2009-02-10
// -----------------------------------------------------------------------------
std::auto_ptr< crossbow::Database_ABC > DatabaseFactory::Create( const dispatcher::Config& config, const std::string& name ) const
{
    bfs::path p( name, bfs::native );
    
    if ( IsFileDatabase( p ) )
        return CreatePgeo( config, name );
    if ( IsShapefileDatabase( p ) )
        return CreateShapefile( config, name );
    if ( IsSDEDatabase( p ) )
        return CreateSDE( name );
    if ( IsPostgreSQLDatabase( p ) )
        return CreatePostgreSQL( name );
    throw std::runtime_error( "Unknown geodatabase connection properties: " + name );
}

namespace 
{
    OGRSFDriver* GetDriver( const char* name )
    {
        OGRSFDriver* driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName( "ESRI Shapefile" );
        if( !driver )
            throw std::runtime_error( "Unknown driver : " + std::string( name ) );
        return driver;
    }

    std::auto_ptr< crossbow::Database_ABC > CreateDatabase( OGRDataSource* db, const std::string& name, const std::string& schema ) 
    {
        if( !db )
            throw std::runtime_error( "Unknown geodatabase connection properties: " + name );
        return std::auto_ptr< crossbow::Database_ABC >( new OGR_Database( db, name, schema ) );
    }
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< crossbow::Database_ABC > DatabaseFactory::CreateShapefile
// Created: JCR 2010-03-01
// -----------------------------------------------------------------------------
std::auto_ptr< crossbow::Database_ABC > DatabaseFactory::CreateShapefile( const dispatcher::Config& config, const std::string& name ) const
{
    OGRSFDriver* driver = GetDriver( "ESRI Shapefile" );
    OGRDataSource* datasource = OGRSFDriverRegistrar::Open( config.BuildExerciseChildFile( name ).c_str(), TRUE, &driver );
    return CreateDatabase( datasource, "", "" );
}
    
// -----------------------------------------------------------------------------
// Name: std::auto_ptr< crossbow::Database_ABC > DatabaseFactory::CreatePgeo
// Created: JCR 2010-03-01
// -----------------------------------------------------------------------------
std::auto_ptr< crossbow::Database_ABC > DatabaseFactory::CreatePgeo( const dispatcher::Config& config, const std::string& name ) const
{
    OGRSFDriver* driver = GetDriver( "PGeo" );
    OGRDataSource* datasource = OGRSFDriverRegistrar::Open( config.BuildExerciseChildFile( name ).c_str(), TRUE, &driver );
    return CreateDatabase( datasource, "", "" );
}
    
// -----------------------------------------------------------------------------
// Name: std::auto_ptr< crossbow::Database_ABC > DatabaseFactory::CreatePostgreSQL
// Created: JCR 2010-03-01
// -----------------------------------------------------------------------------
std::auto_ptr< crossbow::Database_ABC > DatabaseFactory::CreateSDE( const std::string& name ) const
{
    OGRSFDriver* driver = GetDriver( "SDE" );
    ConnectionProperty  properties( name, "sde" );
    const std::string connection( properties.toSDE() );
    std::cout << connection << std::endl;
    OGRDataSource* datasource = OGRSFDriverRegistrar::Open( connection.c_str(), TRUE, &driver );
    return CreateDatabase( datasource, properties.database_, properties.schema_ );
}
    
// -----------------------------------------------------------------------------
// Name: std::auto_ptr< crossbow::Database_ABC > DatabaseFactory::CreateSDE
// Created: JCR 2010-03-01
// -----------------------------------------------------------------------------
std::auto_ptr< crossbow::Database_ABC > DatabaseFactory::CreatePostgreSQL( const std::string& name ) const
{
    OGRSFDriver* driver = GetDriver( "PostgreSQL" );
    ConnectionProperty  properties( name, "postgres" );
    OGRDataSource* datasource = OGRSFDriverRegistrar::Open( properties.toPostgreSQL().c_str(), TRUE, &driver );
    return CreateDatabase( datasource, properties.database_, properties.schema_ );
}

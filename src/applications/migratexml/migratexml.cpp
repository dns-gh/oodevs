// *****************************************************************************
//
// This filename is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#include <tools/FileWrapper.h>
#include "tools/RealFileLoader.h"
#include "tools/RealFileLoaderObserver_ABC.h"
#include <tools/Helpers.h>
#include <tools/Exception.h>
#include <tools/Path.h>
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

namespace po = boost::program_options;

namespace
{

class StdObserver : public tools::RealFileLoaderObserver_ABC
{
    virtual bool NotifyInvalidXml( const tools::Path&, const xml::exception& )
    {
        // rethrown
        return false;
    } 

    virtual void NotifyNoXmlSchemaSpecified( const tools::Path& )
    {
        std::cerr << "error: no schema found" << std::endl;
    }

    virtual void NotifyFileMigrated( const tools::Path&, const std::string& fromVersion,
            const std::string& toVersion )
    {
        std::cout << "migrating from " << fromVersion << " to " << toVersion << std::endl;
    }

    virtual void GetXsdErrors( std::vector< std::string >& errors ) const
    {
        for( auto it = errors.cbegin(); it != errors.cend(); ++it )
            std::cerr << "error: schema error:" << *it << std::endl;
    }
};

// Applies XSL transformations listed in migrations files onto input and saves
// the result in output.
void MigrateXml( const tools::Path& migrations, const tools::Path& input,
        const tools::Path& output )
{
    if( !migrations.Exists() )
        throw std::invalid_argument( "failed to open " + migrations.ToUTF8() );
    if( !input.Exists() )
        throw std::invalid_argument( "failed to open " + input.ToUTF8() );
    tools::Xifstream xis( migrations );
    tools::RealFileLoader loader( xis );
    StdObserver observer;
    auto migrated = loader.LoadFile( input, observer );
    output.Parent().CreateDirectories();
    tools::Xofstream xos( output );
    xos << *migrated;
}

}  // namespace

int main( int, char** )
{
    int ret = 0;
    try
    {
        std::vector< std::wstring > wargs = po::split_winmain( GetCommandLineW() );
        std::vector< std::string > args;
        if( wargs.size() > 0 )
        {
            // Skip executable path in positional arguments
            for( auto it = wargs.begin() + 1; it != wargs.end(); ++it )
                args.push_back( tools::FromUnicodeToUtf8( *it ) );
        }

        bool verbose = false;
        std::string migration, input, output;
        po::variables_map vm;
        po::options_description desc("Allowed options");
        desc.add_options()
            ( "help", "display help and exit")
            ( "verbose", po::value( &verbose )->zero_tokens()->default_value( false ),
              "enable more verbose output" )
            ( "migrations", po::value( &migration ), "migrations definition file")
            ( "input", po::value( &input ), "input file")
            ( "output", po::value( &output ), "output file or directory");
        po::positional_options_description pargs;
        pargs.add( "migrations", 1 );
        pargs.add( "input", 1 );
        pargs.add( "output", 1 );
        po::store( po::command_line_parser( args ).options( desc )
            .positional( pargs ).run(), vm );
        po::notify(vm);

        if( vm.count( "help" ) || !vm.count( "migrations" ) || !vm.count( "input" )
            || !vm.count( "output" ) )
        {
            std::cout << "usage: migratexml MIGRATIONFILE INPUT OUTPUT\n\n";
            std::cout << "Migrate input XML file using supplied migrations.xml migration file\n";
            std::cout << desc << std::endl;
            exit( 0 );
        }

        const auto mig = tools::Path::FromUTF8( migration );
        const auto in = tools::Path::FromUTF8( input );
        const auto out = tools::Path::FromUTF8( output );
        MigrateXml( mig, in, out );
    }
    catch( const std::exception& e )
    {
        ret = 1;
        std::cerr << "error: " << tools::GetExceptionMsg( e ) << std::endl;
    }
    exit( ret );
}

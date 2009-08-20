// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "FileLoader.h"
#include "tools/ExerciseConfig.h"
#include <boost/filesystem.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FileLoader constructor
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
FileLoader::FileLoader( const tools::ExerciseConfig& config )
    : config_( config )
    , xis_( new xml::xifstream( config.GetPhysicalFile() ) )
{
    *xis_ >> xml::start( "physical" );
}

// -----------------------------------------------------------------------------
// Name: FileLoader destructor
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
FileLoader::~FileLoader()
{
    // NOTHING
}

namespace
{
    struct CheckedLoader
    {
        typedef boost::function< void ( xml::xistream& ) > T_Loader;

        CheckedLoader( const std::string& file, const tools::ExerciseConfig& config, T_Loader loader )
            : path_( config.BuildPhysicalChildFile( file ) )
            , config_( config )
            , loader_( loader )
        {
            xml::xifstream xis( path_ );
            xis >> xml::list( *this, &CheckedLoader::LoadFile );
        }

    private:
        void LoadFile( const std::string&, xml::xistream& xis )
        {
            std::auto_ptr< xml::grammar > grammar( new xml::null_grammar() );
            const std::string schema = xml::attribute< std::string >( xis, "xsi:noNamespaceSchemaLocation", "" );
            if( !schema.empty() )
                grammar.reset( new xml::external_grammar( config_.BuildResourceChildFile( schema ) ) );
            xml::xifstream input( path_, *grammar );
            loader_( input );
        }

        CheckedLoader& operator=( const CheckedLoader& );
        const std::string path_;
        const tools::ExerciseConfig& config_;
        T_Loader loader_;
    };
}

// -----------------------------------------------------------------------------
// Name: FileLoader::Load
// Created: SBO 2009-08-20
// -----------------------------------------------------------------------------
FileLoader& FileLoader::Load( const std::string& rootTag, T_Loader loader )
{
    std::string file;
    *xis_ >> xml::start( rootTag ) >> xml::attribute( "file", file ) >> xml::end();
    CheckedLoader( file, config_, loader );
    return *this;
}

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
        typedef boost::function< void ( xml::xisubstream ) > T_Loader;

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
            const std::string schema = xis.attribute< std::string >( "xsi:noNamespaceSchemaLocation", "" );
            if( schema.empty() )
                loader_( xml::xifstream( path_ ) );
            else
            {
                CheckedLoader::GetModelVersion( schema );
                loader_( xml::xifstream( path_, xml::external_grammar( config_.BuildResourceChildFile( schema ) ) ) );
            }
        }

        void GetModelVersion( const std::string& grammarPath )
        {
            xml::xifstream xis( config_.BuildResourceChildFile( grammarPath ) );
            xis >> xml::start( "xs:schema" )
                    >> xml::optional >> xml::attribute( "version", modelVersion_ )
                >> xml::end;
        }

        CheckedLoader& operator=( const CheckedLoader& );
        const std::string path_;
        const tools::ExerciseConfig& config_;
        std::string modelVersion_;
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
    *xis_ >> xml::start( rootTag ) >> xml::attribute( "file", file ) >> xml::end;
    CheckedLoader( file, config_, loader );
    return *this;
}

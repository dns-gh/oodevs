// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "DefaultLoader.h"
#include "RealFileLoader.h"
#include "SchemaVersionExtractor.h"
#include "GeneralConfig.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem/operations.hpp>

namespace bfs = boost::filesystem;
using namespace tools;

// -----------------------------------------------------------------------------
// Name: DefaultLoader constructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
DefaultLoader::DefaultLoader( RealFileLoaderObserver_ABC& observer )
    : observer_              ( observer )
    , schemaVersionExtractor_( new SchemaVersionExtractor() )
{
    xml::xifstream xis( tools::GeneralConfig::BuildResourceChildFile( "migrations.xml" ) );
    fileLoader_.reset( new RealFileLoader( xis, *schemaVersionExtractor_ ) );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
DefaultLoader::~DefaultLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::LoadFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void DefaultLoader::LoadFile( const std::string& fileName, T_Loader loader ) const
{
    loader( *fileLoader_->LoadFile( fileName, observer_ ) );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::LoadFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
std::auto_ptr< xml::xistream > DefaultLoader::LoadFile( const std::string& fileName ) const
{
    return fileLoader_->LoadFile( fileName, observer_ );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::LoadPhysicalFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
std::string DefaultLoader::LoadPhysicalFile( const std::string&, T_Loader ) const
{
    throw std::runtime_error( "Invalid call DefaultFileLoader::LoadPhysicalFile" );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::LoadOptionalPhysicalFile
// Created: ABR 2011-05-24
// -----------------------------------------------------------------------------
std::string DefaultLoader::LoadOptionalPhysicalFile( const std::string&, T_Loader ) const
{
    throw std::runtime_error( "Invalid call DefaultFileLoader::LoadOptionalPhysicalFile" );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::CheckFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void DefaultLoader::CheckFile( const std::string& fileName ) const
{
    fileLoader_->LoadFile( fileName, observer_ );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::CheckOptionalFile
// Created: ABR 2011-05-24
// -----------------------------------------------------------------------------
void DefaultLoader::CheckOptionalFile( const std::string& fileName ) const
{
    if( bfs::exists( fileName ) && !bfs::is_directory( fileName ) )
        fileLoader_->LoadFile( fileName, observer_ );
}

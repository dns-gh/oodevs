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
#include "FileWrapper.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: DefaultLoader constructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
DefaultLoader::DefaultLoader( RealFileLoaderObserver_ABC& observer )
    : observer_              ( observer )
    , schemaVersionExtractor_( new SchemaVersionExtractor() )
{

    Xifstream xis( tools::GeneralConfig::BuildResourceChildFile( "migrations.xml" ) );
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
void DefaultLoader::LoadFile( const Path& fileName, T_Loader loader ) const
{
    loader( *fileLoader_->LoadFile( fileName, observer_ ) );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::LoadOptionalFile
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
bool DefaultLoader::LoadOptionalFile( const Path& fileName, T_Loader loader ) const
{
    if( fileName.Exists() && !fileName.IsDirectory() )
    {
        LoadFile( fileName, loader );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::LoadFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
std::auto_ptr< xml::xistream > DefaultLoader::LoadFile( const Path& fileName ) const
{
    return fileLoader_->LoadFile( fileName, observer_ );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::LoadPhysicalFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
Path DefaultLoader::LoadPhysicalFile( const std::string&, T_Loader ) const
{
    throw MASA_EXCEPTION( "Invalid call DefaultFileLoader::LoadPhysicalFile" );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::LoadOptionalPhysicalFile
// Created: ABR 2011-05-24
// -----------------------------------------------------------------------------
Path DefaultLoader::LoadOptionalPhysicalFile( const std::string&, T_Loader ) const
{
    throw MASA_EXCEPTION( "Invalid call DefaultFileLoader::LoadOptionalPhysicalFile" );
}

Path DefaultLoader::GetPhysicalChildFile( const std::string& ) const
{
    throw MASA_EXCEPTION( "Invalid call DefaultFileLoader::GetPhysicalChildFile" );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::CheckFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void DefaultLoader::CheckFile( const Path& fileName ) const
{
    fileLoader_->LoadFile( fileName, observer_ );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::CheckOptionalFile
// Created: ABR 2011-05-24
// -----------------------------------------------------------------------------
void DefaultLoader::CheckOptionalFile( const Path& fileName ) const
{
    if( fileName.Exists() && !fileName.IsDirectory() )
        fileLoader_->LoadFile( fileName, observer_ );
}

// -----------------------------------------------------------------------------
// Name: DefaultLoader::GetObserver
// Created: MMC 2012-06-01
// -----------------------------------------------------------------------------
const RealFileLoaderObserver_ABC& DefaultLoader::GetObserver() const
{
    return observer_;
}


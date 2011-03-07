// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_FileLoader.h"
#include "ADN_FileLoaderObserver.h"
#include "tools/GeneralConfig.h"
#include "tools/SchemaVersionExtractor.h"
#include "tools/RealFileLoader.h"

using namespace tools;

//-----------------------------------------------------------------------------
// Name: ADN_FileLoader constructor
// Created: NLD 2011-03-07
//-----------------------------------------------------------------------------
ADN_FileLoader::ADN_FileLoader( tools::GeneralConfig& config, tools::RealFileLoaderObserver_ABC& observer )
    : config_                ( config )
    , observer_              ( observer )
    , schemaVersionExtractor_( new SchemaVersionExtractor() )
    , fileLoader_            ()
{
    xml::xifstream migrationsXis( tools::GeneralConfig::BuildResourceChildFile( "migrations.xml" ) );
    fileLoader_.reset( new RealFileLoader( migrationsXis, *schemaVersionExtractor_ ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_FileLoader destructor
// Created: NLD 2011-03-07
//-----------------------------------------------------------------------------
ADN_FileLoader::~ADN_FileLoader()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoader::LoadPhysicalFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void ADN_FileLoader::LoadFile( const std::string& fileName, T_Loader loader ) const
{
    std::auto_ptr< xml::xistream > xis = fileLoader_->LoadFile( fileName, observer_ );
    loader( *xis );
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoader::LoadPhysicalFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
std::string ADN_FileLoader::LoadPhysicalFile( const std::string& /*rootTag*/, T_Loader /*loader*/ ) const
{
    throw std::runtime_error( "ADN_FileLoader::LoadPhysicalFile not implemented - should not be used" );
}

// -----------------------------------------------------------------------------
// Name: ADN_FileLoader::CheckFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void ADN_FileLoader::CheckFile( const std::string& fileName ) const
{
    fileLoader_->LoadFile( fileName, observer_ );
}

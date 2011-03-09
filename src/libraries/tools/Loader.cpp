// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "Loader.h"
#include "RealFileLoader.h"
#include "SchemaVersionExtractor.h"
#include "ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: Loader constructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
Loader::Loader( const ExerciseConfig& config, RealFileLoaderObserver_ABC& observer )
    : config_                ( config )
    , observer_              ( observer )
    , schemaVersionExtractor_( new SchemaVersionExtractor() )
    , fileLoader_            ()
{
    xml::xifstream migrationsXis( config_.BuildResourceChildFile( "migrations.xml" ) );
    fileLoader_.reset( new RealFileLoader( migrationsXis, *schemaVersionExtractor_ ) );
}

// -----------------------------------------------------------------------------
// Name: Loader destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
Loader::~Loader()
{
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadPhysicalFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void Loader::LoadFile( const std::string& fileName, T_Loader loader ) const
{
    std::auto_ptr< xml::xistream > xis = fileLoader_->LoadFile( fileName, observer_ );
    loader( *xis );
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
std::auto_ptr< xml::xistream > Loader::LoadFile( const std::string& fileName ) const
{
    return fileLoader_->LoadFile( fileName, observer_ );
};

// -----------------------------------------------------------------------------
// Name: Loader::LoadPhysicalFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
std::string Loader::LoadPhysicalFile( const std::string& rootTag, T_Loader loader ) const
{
    if( !physicalFileXis_.get() )
        physicalFileXis_ = fileLoader_->LoadFile( config_.GetPhysicalFile(), observer_ );

    std::string childFileName;
    *physicalFileXis_ >> xml::start( "physical" )
                         >> xml::start( rootTag )
                             >> xml::attribute( "file", childFileName )
                         >> xml::end
                      >>  xml::end;
    childFileName = config_.BuildPhysicalChildFile( childFileName );
    LoadFile( childFileName, loader );
    return childFileName;
}

// -----------------------------------------------------------------------------
// Name: Loader::CheckFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
void Loader::CheckFile( const std::string& fileName ) const
{
    std::auto_ptr< xml::xistream > xis = fileLoader_->LoadFile( fileName, observer_ );
}

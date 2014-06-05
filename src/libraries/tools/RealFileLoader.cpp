// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "RealFileLoader.h"
#include "GeneralConfig.h"
#include "RealFileLoaderObserver_ABC.h"
#include "FileMigration.h"
#include "FileMatcherFactory.h"
#include "FileMatcher_ABC.h"
#include "FileWrapper.h"
#include "SchemaVersionExtractor_ABC.h"
#include "XmlStreamOperators.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/ref.hpp>
#include <boost/smart_ptr/make_shared.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: RealFileLoader constructor
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
RealFileLoader::RealFileLoader( xml::xistream& xis, const SchemaVersionExtractor_ABC& versionExtractor )
    : fileMatcherFactory_( new FileMatcherFactory() )
    , versionExtractor_  ( versionExtractor )
{
    xis >> xml::start( "migrations" )
            >> xml::start( "default" )
                >> xml::list( "match-if", *this, &RealFileLoader::ReadFileMatcher )
            >> xml::end
            >> xml::start( "added-files" )
                >> xml::list( "added-file", *this, &RealFileLoader::ReadAddedFile )
            >> xml::end
            >> xml::list( "migration", *this, &RealFileLoader::ReadMigration )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader destructor
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
RealFileLoader::~RealFileLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::ReadFileMatcher
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
void RealFileLoader::ReadFileMatcher( xml::xistream& xis )
{
    defaultSchemasAssignment_.push_back( fileMatcherFactory_->CreateFileMatcher( xis ) );
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::ReadAddedFile
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
void RealFileLoader::ReadAddedFile( xml::xistream& xis )
{
    const Path fileName = Path::FromUTF8( xis.attribute< std::string >( "filename" ) );
    const Path assignedFile = tools::GeneralConfig::BuildResourceChildFile( Path::FromUTF8( xis.attribute< std::string >( "assign-file" ) ) );
    addedFiles_.push_back( std::make_pair( fileName, assignedFile ) );
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::ReadMigration
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
void RealFileLoader::ReadMigration( xml::xistream& xis )
{
    boost::shared_ptr< FileMigration_ABC > newMigration = boost::make_shared< FileMigration >( boost::ref( xis ) );
    if( !migrations_.empty() && migrations_.back()->GetToVersion() != newMigration->GetFromVersion() )
        throw MASA_EXCEPTION( "Invalid migration chain: no migration between version " + migrations_.back()->GetToVersion().ToUTF8() + " and version " + newMigration->GetFromVersion().ToUTF8() );

    migrations_.push_back( newMigration );
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::AssignDefaultSchema
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
bool RealFileLoader::AssignDefaultSchema( const Path& inputFileName, xml::xistream& xis, Path& newSchema ) const
{
    BOOST_FOREACH( boost::shared_ptr< FileMatcher_ABC > fileMatcher, defaultSchemasAssignment_ )
        if( fileMatcher->MatchAndReturnNewSchema( inputFileName, xis, newSchema ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::UpgradeToLastVersion
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
std::unique_ptr< xml::xistream > RealFileLoader::UpgradeToLastVersion( const Path& inputFileName, std::unique_ptr< xml::xistream > xis, const Path& initialSchema, const Path& initialVersion, RealFileLoaderObserver_ABC& observer ) const
{
    Path schema = initialSchema;
    bool applyMigration = false;
    for( auto it = migrations_.begin(); it != migrations_.end(); ++it )
    {
        const FileMigration_ABC& migration = **it;
        const std::string fromVersion = migration.GetFromVersion().ToUTF8();
        const std::string toVersion = migration.GetToVersion().ToUTF8();

        if( !applyMigration && initialVersion.ToUTF8() == fromVersion )
            applyMigration = true;
        if( applyMigration )
        {
            xis = migration.UpgradeFile( std::move( xis ), schema );
            observer.NotifyFileMigrated( inputFileName, fromVersion, toVersion );
        }
        std::string schemaName = schema.ToUTF8();
        boost::algorithm::replace_first( schemaName, "/" + fromVersion, "/" + toVersion );
        schema = Path::FromUTF8( schemaName );
    }
    return xis;
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::LoadFile
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
std::unique_ptr< xml::xistream > RealFileLoader::LoadFile( const Path& initialInputFileName, RealFileLoaderObserver_ABC& observer ) const
{
    std::string data;
    Path inputFileName = initialInputFileName;
    if( !tools::ReadFile( inputFileName, data ) )
    {
        const std::string genericInputFileName = initialInputFileName.ToUTF8();
        BOOST_FOREACH( const T_AddedFile& addedFile, addedFiles_ )
        {
            const std::string& match = addedFile.first.ToUTF8();
            if( boost::algorithm::ends_with( genericInputFileName, match ) )
            {
                inputFileName = addedFile.second;
                break;
            }
        }
        if( !tools::ReadFile( inputFileName, data ) )
            throw MASA_EXCEPTION( "failed to open " + inputFileName.ToUTF8() );
    }

    // Get schema and version
    Path schema;
    std::unique_ptr< xml::xistream > xis( new xml::xistringstream( data ));
    *xis >> xml::list( [&]( const std::string&, const std::string&, xml::xistream& x )
        {
            schema = x.attribute< tools::Path >( "xsi:noNamespaceSchemaLocation", "" );
        } );

    Path version = versionExtractor_.ExtractVersion( schema );
    if( version.IsEmpty() )
    {
        Path newSchema;
        if( AssignDefaultSchema( inputFileName, *xis, newSchema ) )
        {
            schema = newSchema;
            version = versionExtractor_.ExtractVersion( schema );
        }
        if( schema.IsEmpty() )
            observer.NotifyNoXmlSchemaSpecified( inputFileName );
    }
    else
    {
        // Check XML against its schema, only if it was initially set, and
        // contains a version number
        try
        {
            const std::string schemaPath = GeneralConfig::BuildResourceChildFile( schema ).ToUTF8();
            xml::xistringstream( data, xml::external_grammar( schemaPath ));
        }
        catch( const xml::exception& e )
        {
            if( !observer.NotifyInvalidXml( inputFileName, e ) )
                throw;
        }
    }
    return UpgradeToLastVersion( inputFileName, std::move( xis ), schema, version, observer );
}

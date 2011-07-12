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
#include "SchemaVersionExtractor_ABC.h"
#include <tools/XmlCrc32Signature.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/iterator/reverse_iterator.hpp>

using namespace tools;
namespace bfs = boost::filesystem;

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

// =============================================================================
// Tools
// =============================================================================

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
    const std::string fileName = xis.attribute< std::string >( "filename" );
    const std::string assignedFile = tools::GeneralConfig::BuildResourceChildFile( xis.attribute< std::string >( "assign-file" ) );
    addedFiles_.push_back( std::make_pair( bfs::path( fileName ).string(), assignedFile ) );
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::ReadMigration
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
void RealFileLoader::ReadMigration( xml::xistream& xis )
{
    boost::shared_ptr< FileMigration_ABC > newMigration( new FileMigration( xis ) );
    if( !migrations_.empty() && migrations_.back()->GetToVersion() != newMigration->GetFromVersion() )
        throw std::runtime_error( "Invalid migration chain: no migration between version " + migrations_.back()->GetToVersion() + " and version " + newMigration->GetFromVersion() );

    migrations_.push_back( newMigration );
}

// =============================================================================
// Operations
// =============================================================================
namespace
{
    void ExtractRootNode( const std::string& nodeName, xml::xistream& , std::string& rootNode )
    {
        rootNode = nodeName;
    }

    void ExtractSchemaName( xml::xistream& xis, std::string& schema )
    {
        schema = xis.attribute< std::string >( "xsi:noNamespaceSchemaLocation", "" );
    }
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::AssignDefaultSchema
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
bool RealFileLoader::AssignDefaultSchema( const std::string& inputFileName, xml::xistream& xis, std::string& newSchema ) const
{
    BOOST_FOREACH( boost::shared_ptr< FileMatcher_ABC > fileMatcher, defaultSchemasAssignment_ )
    {
        if( fileMatcher->MatchAndReturnNewSchema( inputFileName, xis, newSchema ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::UpgradeToLastVersion
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
std::auto_ptr< xml::xistream > RealFileLoader::UpgradeToLastVersion( const std::string& inputFileName, std::auto_ptr< xml::xistream > xis, const std::string& initialSchema, const std::string& initialVersion, RealFileLoaderObserver_ABC& observer ) const
{
    std::string schema( initialSchema );
    bool applyMigration = false;
    for( T_Migrations::const_iterator it = migrations_.begin(); it != migrations_.end(); ++it )
    {
        const FileMigration_ABC& migration = **it;
        if( !applyMigration && initialVersion == migration.GetFromVersion() )
            applyMigration = true;

        if( applyMigration )
        {
            xis = migration.UpgradeFile( xis, schema );
            observer.NotifyFileMigrated( inputFileName, migration.GetFromVersion(), migration.GetToVersion() );
        }
        std::stringstream from;
        from << "/" << migration.GetFromVersion(); // $$$$ LDCDon't replace 4.3.0 because it has 3.0 inside. Otherwise: 4.3.0 -> 4.4.2.0 or somesuch...
        std::stringstream to;
        to << "/" << migration.GetToVersion();
        boost::algorithm::replace_first( schema, from.str(), to.str() );
    }
    return xis;
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::LoadFile
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
const std::string& RealFileLoader::CheckIfAddedFile( const std::string& initialInputFileName ) const
{
    const std::string genericInputFileName = bfs::path( initialInputFileName ).string();
    if( !bfs::exists( initialInputFileName ) )
    {
        BOOST_FOREACH( const T_AddedFile& addedFile, addedFiles_ )
        {
            const std::string& match = addedFile.first;
            if(  genericInputFileName.size() >= match.size()
                && genericInputFileName.compare( genericInputFileName.size() - match.size(), match.size(), match ) == 0 )
                    return addedFile.second;
        }
    }
    return initialInputFileName;
}

namespace
{
    void CheckSignature( const std::string& inputFile, RealFileLoaderObserver_ABC& observer )
    {
        tools::EXmlCrc32SignatureError error = tools::CheckXmlCrc32Signature( inputFile );
        if( error && !observer.NotifySignatureError( inputFile, error ) )
            throw std::runtime_error( boost::str( boost::format( "Check before upgrade failed : File %s SignatureException %d " ) % inputFile % error ) );
    }
    void AddSignature( const std::string& inputFile )
    {
        tools::EXmlCrc32SignatureError error = WriteXmlCrc32Signature( inputFile );
        if( error )
            throw std::runtime_error( boost::str( boost::format( "Signing after upgrade failed : File %s SignatureException %d " ) % inputFile % error ) );
    }
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::LoadFile
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
std::auto_ptr< xml::xistream > RealFileLoader::LoadFile( const std::string& initialInputFileName, RealFileLoaderObserver_ABC& observer ) const
{
    // Return default file if it's been explicitly added
    const std::string& inputFileName = CheckIfAddedFile( initialInputFileName );

    // Get schema and version
    std::auto_ptr< xml::xistream > xis( new xml::xifstream( inputFileName ) );
    std::string schema;
    *xis >> xml::list( boost::bind( &ExtractSchemaName, _3, boost::ref( schema ) ) );

    std::string version = versionExtractor_.ExtractVersion( schema );
    if( version.empty() )
    {
        std::string newSchema;
        if( AssignDefaultSchema( inputFileName, *xis, newSchema ) )
        {
            schema = newSchema;
            version = versionExtractor_.ExtractVersion( schema );
        }
        if( schema.empty() )
            observer.NotifyNoXmlSchemaSpecified( inputFileName );
    }
    // Check XML against its schema, only if it was initially set, and contains a version number
    else
    {
        try
        {
            xml::xifstream( inputFileName, xml::external_grammar( GeneralConfig::BuildResourceChildFile( schema ) ) );
        }
        catch( xml::exception& e )
        {
            if( !observer.NotifyInvalidXml( inputFileName, e ) )
                throw;
        }
    }

    CheckSignature( inputFileName, observer );
    xis = UpgradeToLastVersion( inputFileName, xis, schema, version, observer );
    AddSignature( inputFileName );
    return xis;
}

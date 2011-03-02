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
#include "SchemaVersionExtractor_ABC.h"
#include <tools/XmlCrc32Signature.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: RealFileLoader constructor
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
RealFileLoader::RealFileLoader( xml::xistream& xis, const SchemaVersionExtractor_ABC& versionExtractor )
    : versionExtractor_( versionExtractor )
{
    xis >> xml::start( "migrations" )
            >> xml::start( "legacy" )
                >> xml::list( "assign", *this, &RealFileLoader::ReadDefaultSchemaAssignment )
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
// Name: RealFileLoader::ReadDefaultSchemaAssignment
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
void RealFileLoader::ReadDefaultSchemaAssignment( xml::xistream& xis )
{
    const std::string rootNode = xis.attribute< std::string >( "root-node" );
    const std::string schema   = xis.attribute< std::string >( "assign-schema" );
    defaultSchemasAssignment_.insert( std::make_pair( rootNode, schema ) );
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

    void CheckSignature( const std::string& inputFile, RealFileLoaderObserver_ABC& observer )
    {
        tools::EXmlCrc32SignatureError error = tools::CheckXmlCrc32Signature( inputFile );
        if( error && !observer.NotifySignatureError( inputFile, error ) )
            throw std::runtime_error( boost::str( boost::format( "File %s SignatureException %d " ) % inputFile % error ) );
    }
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::AssignDefaultSchema
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
const std::string& RealFileLoader::AssignDefaultSchema( xml::xistream& xis, const std::string& currentSchema ) const
{
    std::string rootNode;
    xis >> xml::list( boost::bind( &ExtractRootNode, _2, _3, boost::ref( rootNode ) ) );

    T_DefaultSchemasAssignment::const_iterator it = defaultSchemasAssignment_.find( rootNode );
    if( it != defaultSchemasAssignment_.end() )
        return it->second;;
    return currentSchema;
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
        boost::algorithm::replace_first( schema, migration.GetFromVersion(), migration.GetToVersion() );
    }
    return xis;
}

// -----------------------------------------------------------------------------
// Name: RealFileLoader::LoadFile
// Created: NLD 2011-02-14
// -----------------------------------------------------------------------------
std::auto_ptr< xml::xistream > RealFileLoader::LoadFile( const std::string& inputFileName, RealFileLoaderObserver_ABC& observer ) const
{
    std::auto_ptr< xml::xistream > xis( new xml::xifstream( inputFileName ) );
    std::string schema;
    *xis >> xml::list( boost::bind( &ExtractSchemaName, _3, boost::ref( schema ) ) );

    std::string version = versionExtractor_.ExtractVersion( schema );
    if( version.empty() )
    {
        schema = AssignDefaultSchema( *xis, schema );
        version = versionExtractor_.ExtractVersion( schema );
    }

    if( schema.empty() )
        observer.NotifyNoXmlSchemaSpecified( inputFileName );
    else
    {
        try
        {
            xml::xifstream( inputFileName, xml::external_grammar( GeneralConfig::BuildResourceChildFile( schema ) ) );
        }
        catch( xml::exception& e )
        {
            if( !observer.NotifyInvalidXml( inputFileName, e ) )
                throw e;
        }
    }
    CheckSignature( inputFileName, observer );
    return UpgradeToLastVersion( inputFileName, xis, schema, version, observer );
}

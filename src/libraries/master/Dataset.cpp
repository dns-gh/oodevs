// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "master_pch.h"
#include "Dataset.h"
#include "Config.h"
#include "Network_Def.h"

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

using namespace master;

// -----------------------------------------------------------------------------
// Name: Dataset constructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
Dataset::Dataset( const DataManager& /*dataManager*/, const Config& config, const std::string& name )
    : name_          ( name )
    , physicalModels_()
{
    const bfs::path physicalsDir( config.GetPhysicalsDir( name ), bfs::native );

    for( bfs::directory_iterator it( physicalsDir ); it != bfs::directory_iterator(); ++it )
    {
        bfs::path current = *it;
        const std::string leaf( current.leaf() );
        if( bfs::is_directory( current ) && bfs::exists( bfs::path( config.GetPhysicalFile( name, leaf ), bfs::native ) ) )
            physicalModels_.Create( leaf, *this, config, leaf );
    }
    if( bfs::exists( bfs::path( config.GetPhysicalFile( name, "" ), bfs::native ) ) )
        physicalModels_.Create( "", *this, config, "" );

    MT_LOG_INFO_MSG( "Dataset loaded : '" << name_ << "'" );
}
    
// -----------------------------------------------------------------------------
// Name: Dataset destructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
Dataset::~Dataset()
{
    // NOTHING
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ModelsContainer< std::string, PhysicalModel > Dataset::GetPhysicalModels
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
const ModelsContainer< std::string, PhysicalModel >& Dataset::GetPhysicalModels() const
{
    return physicalModels_;    
}

// -----------------------------------------------------------------------------
// Name: Dataset::GetName
// Created: NLD 2007-01-31
// -----------------------------------------------------------------------------
const std::string& Dataset::GetName() const
{
    return name_;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Dataset::SendCreation
// Created: NLD 2007-01-31
// -----------------------------------------------------------------------------
void Dataset::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgOutMasterDatasetCreation asn;
    Send( asn() );
    asn.Send( publisher );

    physicalModels_.Apply( std::mem_fun_ref( &PhysicalModel::SendCreation ), publisher );
}

// -----------------------------------------------------------------------------
// Name: Dataset::Send
// Created: NLD 2007-01-31
// -----------------------------------------------------------------------------
void Dataset::Send( ASN1T_Dataset& asn ) const
{
    asn.name = name_.c_str();
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "dispatcher/Config.h"
#include "FolkManager.h"
#include <algorithm>

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: FolkManager constructor
// Created: JCR 2007-08-28
// -----------------------------------------------------------------------------
FolkManager::FolkManager( const dispatcher::Config& /*config*/ )
    : container_size_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FolkManager destructor
// Created: JCR 2007-08-28
// -----------------------------------------------------------------------------
FolkManager::~FolkManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FolkManager::Update
// Created: JCR 2007-08-28
// -----------------------------------------------------------------------------
void FolkManager::Send( const ASN1T_MsgFolkCreation& asn )
{
    container_size_ = asn.container_size;
    Update( activities_, asn.activities );
    Update( profiles_, asn.profiles );
}

// -----------------------------------------------------------------------------
// Name: FolkManager::Update
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
template< typename MsgFolkCreation_trait >
void FolkManager::Update( T_Trait& trait, const MsgFolkCreation_trait& asn )
{
    trait.clear();
    trait.resize( asn.n );
    for( unsigned i = 0; i < asn.n; ++i )
        trait[i] = (const char*)asn.elem[i].data;
}

// -----------------------------------------------------------------------------
// Name: FolkManager::Filter
/* Read asn message containing population information and store appropriate 
   indiviudals
*/
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
unsigned FolkManager::Filter( const ASN1T_MsgFolkGraphEdgeUpdate& asn, /*out*/ std::vector<unsigned>& container ) const
{
    unsigned population = 0;
    int c = -1;
    
    for ( unsigned i = 0; i < asn.population_occupation.n; ++i )
    {
        const int individuals = asn.population_occupation.elem[i];
        if ( i % ( activities_.size() * profiles_.size() ) == 0 )
            ++c;
        population += individuals;
        container[c] += individuals;
    }
    return population;
}

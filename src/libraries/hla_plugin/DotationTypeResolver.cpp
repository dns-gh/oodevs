// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "DotationTypeResolver.h"
#include "rpr/EntityType.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include "clients_kernel/DotationType.h"
#include <tools/Resolver_ABC.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DotationTypeResolver constructor
// Created: VPR 2011-09-26
// -----------------------------------------------------------------------------
DotationTypeResolver::DotationTypeResolver( const rpr::EntityTypeResolver_ABC& entityTypeResolver, const tools::Resolver_ABC< kernel::DotationType, unsigned long >& dotationTypeResolver,
                                            const tools::Resolver_ABC< kernel::DotationType, std::string >& dotationNameResolver )
    : entityTypeResolver_  ( entityTypeResolver )
    , dotationTypeResolver_( dotationTypeResolver )
    , dotationNameResolver_( dotationNameResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationTypeResolver destructor
// Created: VPR 2011-09-26
// -----------------------------------------------------------------------------
DotationTypeResolver::~DotationTypeResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationTypeResolver::Resolve
// Created: VPR 2011-09-26
// -----------------------------------------------------------------------------
rpr::EntityType DotationTypeResolver::Resolve( unsigned int munitionIdentifier ) const
{
    const kernel::DotationType& dotationType = dotationTypeResolver_.Get( munitionIdentifier );
    rpr::EntityType result;
    entityTypeResolver_.Find( dotationType.GetName(), result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DotationTypeResolver::Resolve
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
unsigned int DotationTypeResolver::Resolve( const rpr::EntityType& munitionType ) const
{
    std::string munitionName;
    entityTypeResolver_.Resolve( munitionType, munitionName );
    return dotationNameResolver_.Get( munitionName ).GetId();
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "MunitionTypeResolver.h"
#include "rpr/EntityType.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/DotationType.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: MunitionTypeResolver constructor
// Created: VPR 2011-09-26
// -----------------------------------------------------------------------------
MunitionTypeResolver::MunitionTypeResolver( const rpr::EntityTypeResolver_ABC& entityTypeResolver, const tools::Resolver_ABC< kernel::DotationType, unsigned long >& dotationTypeResolver,
                                            const tools::Resolver_ABC< kernel::DotationType, std::string > dotationNameResolver )
    : entityTypeResolver_  ( entityTypeResolver )
    , dotationTypeResolver_( dotationTypeResolver )
    , dotationNameResolver_( dotationNameResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MunitionTypeResolver destructor
// Created: VPR 2011-09-26
// -----------------------------------------------------------------------------
MunitionTypeResolver::~MunitionTypeResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MunitionTypeResolver::Resolve
// Created: VPR 2011-09-26
// -----------------------------------------------------------------------------
rpr::EntityType MunitionTypeResolver::Resolve( unsigned int munitionIdentifier ) const
{
    const kernel::DotationType& dotationType = dotationTypeResolver_.Get( munitionIdentifier );
    return entityTypeResolver_.Find( dotationType.GetName() );
}

// -----------------------------------------------------------------------------
// Name: MunitionTypeResolver::Resolve
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
unsigned int MunitionTypeResolver::Resolve( const rpr::EntityType& munitionType ) const
{
    const std::string munitionName = entityTypeResolver_.Resolve( munitionType );
    return dotationNameResolver_.Get( munitionName ).GetId();
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Formation.h"
#include "clients_kernel/HierarchyLevel_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Formation::Formation( const ASN1T_MsgFormationCreation& message, Controller& controller, const Resolver_ABC< HierarchyLevel_ABC >& resolver  )
    : EntityImplementation< Formation_ABC >( controller, message.oid, message.nom )
    , level_( resolver.Get( message.niveau ) )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( level_.GetName() ).arg( message.oid );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
const HierarchyLevel_ABC& Formation::GetLevel() const
{
    return level_;
}

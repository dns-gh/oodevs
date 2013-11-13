// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LimitsModel.h"
#include "TacticalLine_ABC.h"
#include "TacticalLineFactory.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: LimitsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LimitsModel::LimitsModel( TacticalLineFactory& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimitsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LimitsModel::~LimitsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Purge
// Created: SBO 2007-03-20
// -----------------------------------------------------------------------------
void LimitsModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Create
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::Create( const sword::LimitCreation& message )
{
    kernel::TacticalLine_ABC* line = factory_.Create( message );
    Register( message.id().id(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Create
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::Create( const sword::PhaseLineCreation& message )
{
    kernel::TacticalLine_ABC* line = factory_.Create( message );
    Register( message.id().id(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::DeleteLimit
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::DeleteLimit( unsigned long id )
{
    kernel::TacticalLine_ABC* line = Find( id );
    Remove( id );
    delete line;
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::DeleteLima
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::DeleteLima( unsigned long id )
{
    kernel::TacticalLine_ABC* line = Find( id );
    Remove( id );
    delete line;
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::DestroyAgent
// Created: LDC 2013-11-12
// -----------------------------------------------------------------------------
void LimitsModel::DestroyAgent( unsigned long id )
{
    std::vector< unsigned long > idToRemove;
    auto functor = [ id, &idToRemove ] ( const kernel::TacticalLine_ABC& element )
    {
        const kernel::Hierarchies* hierarchy = element.Retrieve< kernel::TacticalHierarchies >();
        if( hierarchy && hierarchy->GetSuperior() && hierarchy->GetSuperior()->GetId() == id )
            idToRemove.push_back( element.GetId() );
    };
    Apply( functor );
    for( auto it = idToRemove.begin(); it != idToRemove.end(); ++it )
        DeleteLimit( *it );
}

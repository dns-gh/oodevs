// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "EntityResolver_ABC.h"
#include "UrbanObject_ABC.h"
#include "Agent_ABC.h"
#include "Automat_ABC.h"
#include "Inhabitant_ABC.h"
#include "KnowledgeGroup_ABC.h"
#include "Object_ABC.h"
#include "Population_ABC.h"
#include "Formation_ABC.h"
#include "Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntityResolver_ABC constructor
// Created: MCO 2013-04-30
// -----------------------------------------------------------------------------
EntityResolver_ABC::EntityResolver_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityResolver_ABC destructor
// Created: MCO 2013-04-30
// -----------------------------------------------------------------------------
EntityResolver_ABC::~EntityResolver_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityResolver_ABC::FindEntity
// Created: MCO 2013-04-30
// -----------------------------------------------------------------------------
kernel::Entity_ABC* EntityResolver_ABC::FindEntity( unsigned int id ) const
{
    if( id == 0 )
        return 0;
    kernel::Entity_ABC* entity = FindAgent( id );
    if( !entity )
        entity = FindAutomat( id );
    if( !entity )
        entity = FindInhabitant( id );
    if( !entity )
        entity = FindKnowledgeGroup( id );
    if( !entity )
        entity = FindObject( id );
    if( !entity )
        entity = FindPopulation( id );
    if( !entity )
        entity = FindFormation( id );
    if( !entity )
        entity = FindTeam( id );
    if( !entity )
        entity = FindUrbanObject( id );
    return entity;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AutomatHierarchies.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatHierarchies::AutomatHierarchies( Controller& controller, Entity_ABC& holder, const Resolver_ABC< KnowledgeGroup_ABC >& groupResolver, PropertiesDictionary& dictionary )
    : EntityHierarchies< CommunicationHierarchies >( controller, holder, 0 )
    , controller_   ( controller )
    , groupResolver_( groupResolver ) 
{
    CreateDictionary( dictionary ); 
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatHierarchies::~AutomatHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::DoUpdate
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AutomatHierarchies::DoUpdate( const ASN1T_MsgAutomatCreation& message )
{
    SetSuperior( &groupResolver_.Get( message.oid_groupe_connaissance) );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::DoUpdate
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AutomatHierarchies::DoUpdate( const ASN1T_MsgAutomatChangeKnowledgeGroupAck& message )
{
    if( ! message.error_code )
        ChangeSuperior( & groupResolver_.Get( message.oid_groupe_connaissance) );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::AddSuperiorToDictionary
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void AutomatHierarchies::AddSuperiorToDictionary( PropertiesDictionary& dico, Entity_ABC* const& superior ) const
{
    dico.Register( *(const CommunicationHierarchies*)this, tools::translate( "AutomatHierarchies", "Hierarchies/Superior" ), superior );
}

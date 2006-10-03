// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "CommunicationHierarchies.h"
#include "Entity_ABC.h"
#include "Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies constructor
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
CommunicationHierarchies::CommunicationHierarchies( Controller& controller )
    : EntityHierarchies( controller )
    , controller_( controller )
{
    // NOTHING
}
 
// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies destructor
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
CommunicationHierarchies::~CommunicationHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies::IsSubordinateOf
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
bool CommunicationHierarchies::IsSubordinateOf( const Entity_ABC& entity ) const
{
    const Entity_ABC* superior = GetSuperior();
    if( superior == & entity )
        return true;
    if( superior == 0 )
        return false;
    const CommunicationHierarchies* hierarchies = superior->Retrieve< CommunicationHierarchies >();
    return hierarchies && hierarchies->IsSubordinateOf( entity );
}
    
// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies::AddSubordinate
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void CommunicationHierarchies::AddSubordinate( Entity_ABC& entity )
{
    Register( entity.GetId(), entity );
//    controller_.Update( *(CommunicationHierarchies*)this );
}
    
// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies::RemoveSubordinate
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void CommunicationHierarchies::RemoveSubordinate( const Entity_ABC& entity )
{
    Remove( entity.GetId() );
//    controller_.Update( *(CommunicationHierarchies*)this );
}

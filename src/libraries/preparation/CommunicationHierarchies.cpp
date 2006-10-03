// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "CommunicationHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controller.h"

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
CommunicationHierarchies::CommunicationHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::CommunicationHierarchies( controller )
    , controller_( controller )
    , holder_( holder )
    , superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies destructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
CommunicationHierarchies::~CommunicationHierarchies()
{
    if( superior_ )
        if( kernel::CommunicationHierarchies* hierarchies = superior_->Retrieve< kernel::CommunicationHierarchies >() )
            hierarchies->RemoveSubordinate( holder_ );
    controller_.Delete( *(kernel::CommunicationHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies::DoUpdate
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void CommunicationHierarchies::DoUpdate( const kernel::InstanciationComplete& )
{
    RegisterToSuperior();
    controller_.Create( *(kernel::CommunicationHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies::GetSuperior
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* CommunicationHierarchies::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies::GetEntity
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& CommunicationHierarchies::GetEntity() const
{
    return holder_;
}

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies::ChangeSuperior
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void CommunicationHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    RemoveFromSuperior();
    superior_ = &superior;
    RegisterToSuperior();
    controller_.Update( *(kernel::CommunicationHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies::RegisterToSuperior
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void CommunicationHierarchies::RegisterToSuperior()
{
    if( superior_ )
        if( kernel::CommunicationHierarchies* hierarchies = superior_->Retrieve< kernel::CommunicationHierarchies >() )
        {
            hierarchies->AddSubordinate( holder_ );
            controller_.Update( *hierarchies );
        }
}
    
// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies::RemoveFromSuperior
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void CommunicationHierarchies::RemoveFromSuperior()
{
    if( superior_ )
        if( kernel::CommunicationHierarchies* hierarchies = superior_->Retrieve< kernel::CommunicationHierarchies >() )
        {
            hierarchies->RemoveSubordinate( holder_ );
            controller_.Update( *hierarchies );
            superior_ = 0;
        }
}

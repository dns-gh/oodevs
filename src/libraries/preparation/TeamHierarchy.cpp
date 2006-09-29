// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamHierarchy.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/Team_ABC.h"

// -----------------------------------------------------------------------------
// Name: TeamHierarchy constructor
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
TeamHierarchy::TeamHierarchy( kernel::Controllers& controllers, const kernel::Team_ABC& team, const kernel::Entity_ABC& holder )
    : controllers_( controllers )
    , team_( controllers, &team )
    , holder_( holder )
{
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: TeamHierarchy destructor
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
TeamHierarchy::~TeamHierarchy()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchy::GetTeam
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
const kernel::Team_ABC& TeamHierarchy::GetTeam() const
{
    return *team_;
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchy::NotifyUpdated
// Created: SBO 2006-09-29
// -----------------------------------------------------------------------------
void TeamHierarchy::NotifyUpdated( const kernel::Hierarchies& hierarchy )
{
    if( &holder_ != &hierarchy.GetEntity() ) // $$$$ SBO 2006-09-29: 
        return;
    const kernel::Hierarchies* root = &hierarchy;
    while( root && root->GetSuperior() )
        root = root->GetSuperior()->Retrieve< kernel::Hierarchies >();
    if( !root )
        return;
    if( const kernel::Team_ABC* team = dynamic_cast< const kernel::Team_ABC* >( &root->GetEntity() ) )
        team_ = team;
}

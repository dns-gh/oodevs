// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "PeopleAffinities.h"
#include "Model.h"
#include "TeamsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/Team_ABC.h"
#include "tools/Iterator.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PeopleAffinities constructor
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
PeopleAffinities::PeopleAffinities( kernel::Controllers& controllers, Model& model )
    : Affinities()
    , controllers_( controllers )
    , model_      ( model )
{
    InitializeAffinities();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities constructor
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
PeopleAffinities::PeopleAffinities( xml::xistream& xis, kernel::Controllers& controllers, Model& model )
    : Affinities( xis )
    , controllers_( controllers )
    , model_      ( model )
{
    if( affinities_.empty() )
        InitializeAffinities();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities destructor
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
PeopleAffinities::~PeopleAffinities()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities::NotifyCreated
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void PeopleAffinities::NotifyCreated( const kernel::Team_ABC& team )
{
    if( affinities_.find( team.GetId() ) == affinities_.end() )
        affinities_[ team.GetId() ] = 0.f;
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities::NotifyDeleted
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void PeopleAffinities::NotifyDeleted( const kernel::Team_ABC& team )
{
    affinities_.erase( team.GetId() );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities::Clear
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
void PeopleAffinities::Clear()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinities::InitializeAffinities
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
void PeopleAffinities::InitializeAffinities()
{
    tools::Iterator< const kernel::Team_ABC& > it = model_.teams_.CreateIterator();
    while( it.HasMoreElements() )
        if( affinities_.find( it.NextElement().GetId() ) == affinities_.end() )
            affinities_[ it.NextElement().GetId() ] = 0.f;
}
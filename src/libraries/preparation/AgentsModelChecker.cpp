// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentsModelChecker.h"
#include "AgentsModel.h"
#include "Exceptions.h"
#include "Tools.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentsModelChecker constructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
AgentsModelChecker::AgentsModelChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsModelChecker destructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
AgentsModelChecker::~AgentsModelChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsModelChecker::Check
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void AgentsModelChecker::Check( const AgentsModel& model ) const
{
    CheckCommandPosts( model );
}

// -----------------------------------------------------------------------------
// Name: AgentsModelChecker::CheckCommandPosts
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void AgentsModelChecker::CheckCommandPosts( const AgentsModel& model ) const
{
    Iterator< const Automat_ABC& > it = static_cast< const Resolver< Automat_ABC >& >( model ).CreateIterator();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& automat = it.NextElement();
        const kernel::TacticalHierarchies& hierarchies = automat.Get< kernel::TacticalHierarchies >();
        Iterator< const Entity_ABC& > itChild = hierarchies.CreateSubordinateIterator();
        unsigned int commandPostCount = 0;
        while( itChild.HasMoreElements() )
        {
            const Agent_ABC* agent = dynamic_cast< const Agent_ABC* >( & itChild.NextElement() );
            if( agent && agent->IsCommandPost() )
                ++commandPostCount;
        }
        if( commandPostCount == 0 )
            throw InvalidModelException( tools::translate( "AgentsModelChecker", "Units model" )
                                       , tools::translate( "AgentsModelChecker", "Automat '%1' [%2] has no command post." ).arg( automat.GetName() ).arg( automat.GetId() ) );
        else if( commandPostCount > 1 )
            throw InvalidModelException( tools::translate( "AgentsModelChecker", "Units model" )
                                       , tools::translate( "AgentsModelChecker", "Automat '%1' [%2] has '%3' command posts." ).arg( automat.GetName() ).arg( automat.GetId() ).arg( commandPostCount ) );
    }
}

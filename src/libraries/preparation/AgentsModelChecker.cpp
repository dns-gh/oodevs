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
#include "clients_kernel/Tools.h"
#include "ModelChecker_ABC.h"
#include "CommandPostAttributes.h"
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
bool AgentsModelChecker::Check( const AgentsModel& model, ModelChecker_ABC& checker ) const
{
    return CheckCommandPosts( model, checker );
}

namespace
{
    bool IsCommandPost( const kernel::Entity_ABC& entity )
    {
        if( const CommandPostAttributes* pAttributes = entity.Retrieve< CommandPostAttributes >() )
            return pAttributes->IsCommandPost();
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModelChecker::CheckCommandPosts
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
bool AgentsModelChecker::CheckCommandPosts( const AgentsModel& model, ModelChecker_ABC& checker ) const
{
    tools::Iterator< const Automat_ABC& > it = static_cast< const tools::Resolver< Automat_ABC >& >( model ).CreateIterator();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& automat = it.NextElement();
        const kernel::TacticalHierarchies& hierarchies = automat.Get< kernel::TacticalHierarchies >();
        tools::Iterator< const Entity_ABC& > itChild = hierarchies.CreateSubordinateIterator();
        unsigned int commandPostCount = 0;
        while( itChild.HasMoreElements() )
        {
            if( IsCommandPost( itChild.NextElement() ) )
                ++commandPostCount;
        }
        if( commandPostCount == 0 )
            return checker.Reject( QString( "%1:\n%2" )
                                  .arg( tools::translate( "AgentsModelChecker", "Units model" ) )
                                  .arg( tools::translate( "AgentsModelChecker", "Automat '%1' [%2] has no command post." ).arg( automat.GetName() ).arg( automat.GetId() ) ) );
        else if( commandPostCount > 1 )
            return checker.Reject( QString( "%1:\n%2" )
                                  .arg( tools::translate( "AgentsModelChecker", "Units model" ) )
                                  .arg( tools::translate( "AgentsModelChecker", "Automat '%1' [%2] has '%3' command posts." ).arg( automat.GetName() ).arg( automat.GetId() ).arg( commandPostCount ) ) );
    }
    return checker.Validate();
}

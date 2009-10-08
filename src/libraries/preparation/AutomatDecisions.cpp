// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AutomatDecisions.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AutomatDecisions constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
AutomatDecisions::AutomatDecisions( Controller& controller, const Automat_ABC& agent )
    : controller_( controller )
    , agent_( agent )
    , bEmbraye_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
AutomatDecisions::AutomatDecisions( xml::xistream& xis, kernel::Controller& controller, const kernel::Automat_ABC& agent )
    : controller_( controller )
    , agent_( agent )
{
    std::string engaged;
    xis >> optional() >> attribute( "engaged", engaged );
    bEmbraye_ = engaged.empty() || engaged == "true";
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
AutomatDecisions::~AutomatDecisions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::GetAgent
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const Automat_ABC& AutomatDecisions::GetAgent() const
{
    return agent_;
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::CanBeOrdered
// Created: AGE 2007-04-05
// -----------------------------------------------------------------------------
bool AutomatDecisions::CanBeOrdered() const
{
    return bEmbraye_ && ! HasEngagedSuperior();
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::HasEngagedSuperior
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
bool AutomatDecisions::HasEngagedSuperior() const
{
    const Entity_ABC* superior = agent_.Get< TacticalHierarchies >().GetSuperior();
    const AutomatDecisions* superiorDecisions = superior ? superior->Retrieve< AutomatDecisions >() : 0;
    return superiorDecisions && superiorDecisions->bEmbraye_;
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::IsEmbraye
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
bool AutomatDecisions::IsEmbraye() const
{
    return bEmbraye_;
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::Engage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AutomatDecisions::Engage()
{
    bEmbraye_ = true;
    controller_.Update( *this );
    ForwardEngageStatus();
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::Disengage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void AutomatDecisions::Disengage()
{
    bEmbraye_ = false;
    controller_.Update( *this );
    ForwardEngageStatus();
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::ForwardEngageStatus
// Created: AGE 2007-04-05
// -----------------------------------------------------------------------------
void AutomatDecisions::ForwardEngageStatus()
{
    tools::Iterator< const Entity_ABC& > children = agent_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
        if( const AutomatDecisions* cd = children.NextElement().Retrieve< AutomatDecisions >() )
        {
            AutomatDecisions* childDecisions = const_cast< AutomatDecisions* >( cd );
            if( bEmbraye_ )
                childDecisions->Engage();
            else
                childDecisions->Disengage();
        }
}

// -----------------------------------------------------------------------------
// Name: AutomatDecisions::SerializeAttributes
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void AutomatDecisions::SerializeAttributes( xml::xostream& xos ) const
{
    xos << attribute( "engaged", bEmbraye_ ? "true" : "false" );
}

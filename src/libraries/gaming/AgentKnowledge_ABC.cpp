// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentKnowledge_ABC.h"
#include "clients_kernel/ActionController.h"

using namespace kernel;

const QString AgentKnowledge_ABC::typeName_ = "agentKnowledge";

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC constructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
AgentKnowledge_ABC::AgentKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC destructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
AgentKnowledge_ABC::~AgentKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC::GetTypeName
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
QString AgentKnowledge_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC::Select
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void AgentKnowledge_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC::ContextMenu
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void AgentKnowledge_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, where );
}   

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC::Activate
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void AgentKnowledge_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "PopulationKnowledge_ABC.h"
#include "ActionController.h"

using namespace kernel;

const QString PopulationKnowledge_ABC::typeName_ = "populationKnowledge";

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC constructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
PopulationKnowledge_ABC::PopulationKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC destructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
PopulationKnowledge_ABC::~PopulationKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC::GetTypeName
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
QString PopulationKnowledge_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC::Select
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void PopulationKnowledge_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Knowledge_ABC*)this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC::ContextMenu
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void PopulationKnowledge_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Knowledge_ABC*)this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC::Activate
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void PopulationKnowledge_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Knowledge_ABC*)this );
}

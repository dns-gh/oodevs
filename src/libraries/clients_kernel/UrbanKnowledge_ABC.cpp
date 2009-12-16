// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "UrbanKnowledge_ABC.h"
#include "ActionController.h"

using namespace kernel;

const QString UrbanKnowledge_ABC::typeName_ = "TerrainObjectKnowledge";

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC constructor
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
UrbanKnowledge_ABC::UrbanKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC destructor
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
UrbanKnowledge_ABC::~UrbanKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC::GetTypeName
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
QString UrbanKnowledge_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC::Select
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanKnowledge_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Knowledge_ABC*)this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC::ContextMenu
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanKnowledge_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Knowledge_ABC*)this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC::Activate
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanKnowledge_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Knowledge_ABC*)this );
}

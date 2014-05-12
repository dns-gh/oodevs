// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "TacticalLine_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string TacticalLine_ABC::typeName_ = "tacticalLine";

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC destructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
TacticalLine_ABC::~TacticalLine_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const std::string& TacticalLine_ABC::GetTypeName() const
{
    return typeName_;
}

void TacticalLine_ABC::Select( ActionController& actions ) const
{
    actions.Select( *this, *(const kernel::Entity_ABC*)this );
}

void TacticalLine_ABC::ContextMenu( ActionController& actions, const QPoint& point ) const
{
    actions.ContextMenu( *this, *(const kernel::Entity_ABC*)this, point );
}

void TacticalLine_ABC::Activate( ActionController& actions ) const
{
    actions.Activate( *this, *(const kernel::Entity_ABC*)this );
}

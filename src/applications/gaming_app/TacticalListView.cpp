// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TacticalListView.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : HierarchyListView< TacticalHierarchies >( pParent, controllers, factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalListView destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TacticalListView::~TacticalListView()
{
    // NOTHING
}

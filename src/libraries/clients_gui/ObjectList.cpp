// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectList.h"
#include "ObjectListView.h"
#include "EntitySearchBox.h"
#include "clients_kernel/Object_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectList constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
ObjectList::ObjectList( Controllers& controllers, ItemFactory_ABC& factory )
{
    new EntitySearchBox< Object_ABC >( this, controllers );
    new ObjectListView( this, controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: ObjectList destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
ObjectList::~ObjectList()
{
    // NOTHING
}

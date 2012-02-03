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

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectList constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
ObjectList::ObjectList( Controllers& controllers, ItemFactory_ABC& factory, const Profile_ABC& profile )
{
    new EntitySearchBox< Object_ABC >( this, controllers );
    new ObjectListView( this, controllers, factory, profile );
}

// -----------------------------------------------------------------------------
// Name: ObjectList destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
ObjectList::~ObjectList()
{
    // NOTHING
}

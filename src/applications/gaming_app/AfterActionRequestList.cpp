// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterActionRequestList.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/ItemFactory_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequestList::AfterActionRequestList( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory )
    : QVBox( parent )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequestList destructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequestList::~AfterActionRequestList()
{
    controllers_.Unregister( *this );
}

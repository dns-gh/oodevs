// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Page_ABC.h"
#include <qwidgetstack.h>

// -----------------------------------------------------------------------------
// Name: Page_ABC constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Page_ABC::Page_ABC( QWidgetStack* pages )
    : QVBox ( pages )
    , pages_( pages )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Page_ABC destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
Page_ABC::~Page_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Page_ABC::showEvent
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void Page_ABC::showEvent( QShowEvent* event )
{
    pages_->raiseWidget( this );
    Update();
    QVBox::showEvent( event );
}

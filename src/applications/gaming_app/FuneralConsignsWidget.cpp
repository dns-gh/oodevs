// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FuneralConsignsWidget.h"

// -----------------------------------------------------------------------------
// Name: FuneralConsignsWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
FuneralConsignsWidget::FuneralConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : LogisticConsignsWidget_ABC< FuneralConsignsWidget, LogFuneralConsign, LogFuneralConsigns >( parent, controllers, factory )
    , factory_( factory )
{
    AddConsignColumn( tools::translate( "FuneralConsignsWidget", "Supplier:" ) );
    AddConsignColumn( tools::translate( "FuneralConsignsWidget", "Consumer:" ) );
    AddConsignColumn( tools::translate( "FuneralConsignsWidget", "Handler:" ) );
    AddConsignColumn( tools::translate( "FuneralConsignsWidget", "Convoy:" ) );
    AddConsignColumn( tools::translate( "FuneralConsignsWidget", "State:" ) );
    AddConsignColumn( tools::translate( "FuneralConsignsWidget", "Current state end:" ) );
    AddConsignColumn( tools::translate( "FuneralConsignsWidget", "Current packaging:" ) );
    AddConsignColumn( tools::translate( "FuneralConsignsWidget", "Rank:" ) );
}

// -----------------------------------------------------------------------------
// Name: FuneralConsignsWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
FuneralConsignsWidget::~FuneralConsignsWidget()
{
    // NOTHING
}

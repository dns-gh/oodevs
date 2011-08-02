// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoCompositionDialog.h"
#include "EquipmentsListView.h"
#include "HumansListView.h"
#include "gaming/Tools.h"

// -----------------------------------------------------------------------------
// Name: InfoCompositionDialog constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
InfoCompositionDialog::InfoCompositionDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : InfoDialog< Equipments >( parent, controllers, tools::translate( "InfoCompositionDialog", "Composition" ) )
{
    new EquipmentsListView( new Q3HGroupBox( tools::translate( "InfoCompositionDialog", "Equipments" ), RootWidget() ), controllers, factory );
    new HumansListView    ( new Q3HGroupBox( tools::translate( "InfoCompositionDialog", "Humans" ), RootWidget() ), controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: InfoCompositionDialog destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
InfoCompositionDialog::~InfoCompositionDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoCompositionDialog::sizeHint
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
QSize InfoCompositionDialog::sizeHint() const
{
    return QSize( 500, 300 );
}

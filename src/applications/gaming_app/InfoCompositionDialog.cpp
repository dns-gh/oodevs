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
#include "LendingsListView.h"
#include "BorrowingsListView.h"
#include "clients_kernel/Tools.h"
#include "gaming/Equipments.h"
#include "gaming/Lendings.h"

// -----------------------------------------------------------------------------
// Name: InfoCompositionDialog constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
InfoCompositionDialog::InfoCompositionDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& /*factory*/ )
    : InfoDialog_Base( controllers, parent, tools::translate( "InfoCompositionDialog", "Composition" ) )
{
    new EquipmentsListView( new Q3HGroupBox( tools::translate( "InfoCompositionDialog", "Equipments" ), RootWidget() ), controllers );
    new HumansListView    ( new Q3HGroupBox( tools::translate( "InfoCompositionDialog", "Humans" ), RootWidget() ), controllers );
    new LendingsListView( new Q3HGroupBox( tools::translate( "InfoCompositionDialog", "Lent equipment(s)" ), RootWidget() ), controllers );
    new BorrowingsListView( new Q3HGroupBox( tools::translate( "InfoCompositionDialog", "Borrowed equipment(s)" ), RootWidget() ), controllers );
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
// Name: InfoCompositionDialog::ShouldDisplay
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool InfoCompositionDialog::ShouldDisplay( const kernel::Entity_ABC& element ) const
{
    return element.Retrieve< Lendings >() || element.Retrieve< Equipments >();
}

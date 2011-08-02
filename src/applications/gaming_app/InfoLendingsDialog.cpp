// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoLendingsDialog.h"
#include "LendingsListView.h"
#include "BorrowingsListView.h"
#include "gaming/Tools.h"

// -----------------------------------------------------------------------------
// Name: InfoLendingsDialog constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
InfoLendingsDialog::InfoLendingsDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : InfoDialog< Lendings >( parent, controllers, tools::translate( "InfoLendingsDialog", "Lendings" ) )
{
    new LendingsListView( new Q3HGroupBox( tools::translate( "InfoLendingsDialog", "Lent equipment(s)" ), RootWidget() ), controllers, factory );
    new BorrowingsListView( new Q3HGroupBox( tools::translate( "InfoLendingsDialog", "Borrowed equipment(s)" ), RootWidget() ), controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: InfoLendingsDialog destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
InfoLendingsDialog::~InfoLendingsDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoLendingsDialog::sizeHint
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
QSize InfoLendingsDialog::sizeHint() const
{
    return QSize( 300, 250 );
}

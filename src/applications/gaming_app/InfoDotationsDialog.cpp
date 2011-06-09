// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoDotationsDialog.h"
#include "DotationsListView.h"
#include "gaming/Tools.h"

// -----------------------------------------------------------------------------
// Name: InfoDotationsDialog constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
InfoDotationsDialog::InfoDotationsDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : InfoDialog< kernel::Dotations_ABC >( parent, controllers, tools::translate( "InfoDotationsDialog", "Resources" ) )
{
    new DotationsListView( new QHGroupBox( tools::translate( "InfoDotationsDialog", "Resources" ), RootWidget() ), controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: InfoDotationsDialog destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
InfoDotationsDialog::~InfoDotationsDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoDotationsDialog::sizeHint
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
QSize InfoDotationsDialog::sizeHint() const
{
    return QSize( 300, 200 );
}

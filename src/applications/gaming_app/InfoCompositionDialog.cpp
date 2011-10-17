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
#include "gaming/Tools.h"
#include "gaming/Equipments.h"
#include "gaming/Lendings.h"

// -----------------------------------------------------------------------------
// Name: InfoCompositionDialog constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
InfoCompositionDialog::InfoCompositionDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : InfoDialog_Base( parent, tools::translate( "InfoCompositionDialog", "Composition" ) )
    , controllers_( controllers )
{
    controllers_.Register( *this );

    new EquipmentsListView( new Q3HGroupBox( tools::translate( "InfoCompositionDialog", "Equipments" ), RootWidget() ), controllers, factory );
    new HumansListView    ( new Q3HGroupBox( tools::translate( "InfoCompositionDialog", "Humans" ), RootWidget() ), controllers, factory );
    new LendingsListView( new Q3HGroupBox( tools::translate( "InfoCompositionDialog", "Lent equipment(s)" ), RootWidget() ), controllers, factory );
    new BorrowingsListView( new Q3HGroupBox( tools::translate( "InfoCompositionDialog", "Borrowed equipment(s)" ), RootWidget() ), controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: InfoCompositionDialog destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
InfoCompositionDialog::~InfoCompositionDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoCompositionDialog::sizeHint
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
QSize InfoCompositionDialog::sizeHint() const
{
    return QSize( 500, 300 );
}

// -----------------------------------------------------------------------------
// Name: InfoCompositionDialog::ShouldDisplay
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool InfoCompositionDialog::ShouldDisplay( const kernel::Entity_ABC& element ) const
{
    return element.Retrieve< Lendings >() || element.Retrieve< Equipments >();
}

// -----------------------------------------------------------------------------
// Name: InfoCompositionDialog::NotifySelected
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void InfoCompositionDialog::NotifySelected( const kernel::Entity_ABC* element )
{
    SetEnabled( element && ShouldDisplay( *element ) );
}
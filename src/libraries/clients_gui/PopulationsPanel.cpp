// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PopulationsPanel.h"

#include "SubObjectName.h"
#include "PopulationTypesListView.h"
#include "RichSpinBox.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationsPanel constructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationsPanel::PopulationsPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::PopulationType >& types )
    : InfoPanel_ABC( parent, panel, tools::translate( "gui::PopulationsPanel", "Crowds" ), "PopulationsPanel" )
    , controllers_( controllers )
{
    gui::SubObjectName subObject( "populationPanel" );
    Q3VBox* vbox = new Q3VBox( this );
    vbox->setMargin( 5 );
    vbox->setSpacing( 5 );
    {
        Q3HBox* box = new Q3HBox( vbox );
        box->setSpacing( 5 );
        new QLabel( tools::translate( "gui::PopulationsPanel", "Healthy number:" ), box );
        number_ = new RichSpinBox( "number", box, 1, 1000000, 10 );
        number_->setValue( 1000 );
        list_ = new PopulationTypesListView( "populationTypeList", vbox, controllers_, types, *number_ );
    }
    setWidget( vbox );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel destructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationsPanel::~PopulationsPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel::NotifyUpdated
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationsPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    Show();
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel::BeforeSelection
// Created: LDC 2010-10-25
// -----------------------------------------------------------------------------
void PopulationsPanel::BeforeSelection()
{
    list_->setDragEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel::AfterSelection
// Created: LDC 2010-10-25
// -----------------------------------------------------------------------------
void PopulationsPanel::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel::Select
// Created: LDC 2010-10-25
// -----------------------------------------------------------------------------
void PopulationsPanel::Select( const kernel::Formation_ABC& /*element*/ )
{
    list_->setDragEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: PopulationsPanel::Select
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
void PopulationsPanel::Select( const kernel::Team_ABC& element )
{
    list_->setDragEnabled( element.GetId() != 0 );
}

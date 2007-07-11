// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PopulationsPanel.h"
#include "clients_kernel/Controllers.h"
#include "PopulationTypesListView.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationsPanel constructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationsPanel::PopulationsPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const kernel::Resolver_ABC< kernel::PopulationType >& types, gui::ItemFactory_ABC& factory )
    : gui::InfoPanel_ABC( parent, panel, tr( "Populations" ) )
    , controllers_( controllers )
{
    list_ = new PopulationTypesListView( this, controllers_, types, factory );
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

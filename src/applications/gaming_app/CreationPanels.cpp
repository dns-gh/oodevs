// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "CreationPanels.h"
#include "preparation/StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/UnitsPanel.h"
#include "gaming/Simulation.h"
#include "ObjectCreationPanel.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CreationPanels constructor
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
CreationPanels::CreationPanels( QWidget* parent, Controllers& controllers, const StaticModel& staticModel, ItemFactory_ABC& factory, Publisher_ABC& publisher, ParametersLayer& paramLayer, GlTools_ABC& tools, SymbolIcons& icons, ColorStrategy_ABC& colorStrategy )
    : Panels( parent )
    , controllers_( controllers )
    , shown_( true )
{
    AddPanel( units_   = new gui::UnitsPanel    ( this, *this, controllers, staticModel.types_, factory, icons, colorStrategy ) );
    AddPanel( objects_ = new ObjectCreationPanel( this, *this, controllers, publisher, staticModel, paramLayer, tools ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels destructor
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
CreationPanels::~CreationPanels()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::Draw
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void CreationPanels::Draw( Viewport_ABC& viewport ) const
{
    objects_->Draw( viewport );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::NotifyUpdated
// Created: AGE 2007-10-18
// -----------------------------------------------------------------------------
void CreationPanels::NotifyUpdated( const Simulation& simu )
{
    if( simu.IsReplayer() )
    {
        Remove( units_ );
        Remove( objects_ );
    } else 
    {
        Add( units_ );
        Add( objects_ );
    }
}

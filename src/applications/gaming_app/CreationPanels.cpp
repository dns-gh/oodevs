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
#include "gaming/StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/DrawerPanel.h"
#include "clients_gui/IntelligencesPanel.h"
#include "clients_gui/PopulationsPanel.h"
#include "clients_gui/UnitsPanel.h"
#include "gaming/DrawingsModel.h"
#include "protocol/SimulationSenders.h"
#include "FireCreationPanel.h"
#include "ObjectCreationPanel.h"
#include "WeatherPanel.h"

using namespace kernel;
using namespace gui;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: CreationPanels constructor
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
CreationPanels::CreationPanels( QWidget* parent, Controllers& controllers, const ::StaticModel& staticModel, ItemFactory_ABC& factory, ActionsModel& actionsModel,
                                const Time_ABC& simulation, ParametersLayer& paramLayer, ::WeatherLayer& weatherLayer, GlTools_ABC& tools, SymbolIcons& icons,
                                ColorStrategy_ABC& colorStrategy, DrawingsModel& drawings, const tools::ExerciseConfig& config )
    : Panels      ( parent )
    , controllers_( controllers )
{
    AddPanel( units_ = new UnitsPanel( this, *this, controllers, staticModel.types_, factory, icons, colorStrategy ) );
    AddPanel( objects_ = new ObjectCreationPanel( this, *this, controllers, actionsModel, staticModel, simulation, paramLayer, tools ) );
    AddPanel( intel_ = new IntelligencesPanel( this, *this, controllers, staticModel.levels_, icons, &config ) );
    AddPanel( drawings_ = new DrawerPanel( this, *this, paramLayer, controllers, drawings ) );
    AddPanel( fires_ = new FireCreationPanel( this, *this, controllers, actionsModel, simulation, staticModel, paramLayer, tools ) );
    AddPanel( weather_ = new ::WeatherPanel( this, *this, controllers, actionsModel, staticModel, simulation, weatherLayer ) );
    AddPanel( crowds_ = new PopulationsPanel( this, *this, controllers, staticModel.types_, factory ) );
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
    fires_->Draw( viewport );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::NotifyUpdated
// Created: JSR 2011-01-12
// -----------------------------------------------------------------------------
void CreationPanels::NotifyUpdated( const ModelLoaded& )
{
    Add( units_ );
    Add( objects_ );
    Add( intel_ );
    Add( crowds_ );
    Add( weather_ );
    Add( drawings_ );
    Add( fires_ );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::NotifyUpdated
// Created: JSR 2011-01-12
// -----------------------------------------------------------------------------
void CreationPanels::NotifyUpdated( const ModelUnLoaded& )
{
    Remove( units_ );
    Remove( objects_ );
    Remove( intel_ );
    Remove( crowds_ );
    Remove( weather_ );
    Remove( drawings_ );
    Remove( fires_ );
}

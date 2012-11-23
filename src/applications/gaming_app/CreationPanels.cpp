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
#include "clients_kernel/Profile_ABC.h"
#include "clients_gui/DrawerPanel.h"
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
    Q3VBox* box = new Q3VBox( parent );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    AddPanel( units_ = new UnitsPanel( this, *this, controllers, staticModel.types_, factory, icons, colorStrategy, false ) );
    AddPanel( crowds_ = new PopulationsPanel( this, *this, controllers, staticModel.types_, factory ) );
    AddPanel( objects_ = new ObjectCreationPanel( box, *this, controllers, actionsModel, staticModel, simulation, paramLayer, tools ) );
    AddPanel( fires_ = new FireCreationPanel( this, *this, controllers, actionsModel, simulation, staticModel, paramLayer, tools ) );
    AddPanel( weather_ = new ::WeatherPanel( this, *this, controllers, actionsModel, staticModel, simulation, weatherLayer ) );
    AddPanel( drawings_ = new DrawerPanel( this, *this, paramLayer, controllers, drawings, config ) );

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
    AddPanels();
    Add( drawings_ );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::NotifyUpdated
// Created: JSR 2011-01-12
// -----------------------------------------------------------------------------
void CreationPanels::NotifyUpdated( const ModelUnLoaded& )
{
    RemovePanels();
    Remove( drawings_ );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::NotifyUpdated
// Created: LGY 2012-11-22
// -----------------------------------------------------------------------------
void CreationPanels::NotifyUpdated( const kernel::Profile_ABC& profile )
{
    if( !profile.IsSupervision() )
        RemovePanels();
    else
        AddPanels();
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::AddPanels
// Created: LGY 2012-11-22
// -----------------------------------------------------------------------------
void CreationPanels::AddPanels()
{
    Add( units_ );
    Add( objects_ );
    Add( crowds_ );
    Add( weather_ );
    Add( fires_ );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::RemovePanels
// Created: LGY 2012-11-22
// -----------------------------------------------------------------------------
void CreationPanels::RemovePanels()
{
    Remove( units_ );
    Remove( objects_ );
    Remove( crowds_ );
    Remove( weather_ );
    Remove( fires_ );
}

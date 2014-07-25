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
#include "gaming/DrawingsModel.h"
#include "gaming/Model.h"
#include "gaming/StaticModel.h"
#include "gaming/TeamsModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/DrawerPanel.h"
#include "clients_gui/PopulationsPanel.h"
#include "clients_gui/UnitsPanel.h"
#include "tools/ExerciseConfig.h"
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
CreationPanels::CreationPanels( QWidget* parent, Controllers& controllers, const ::StaticModel& staticModel, const Model& model,
                                const Time_ABC& simulation, ParametersLayer& paramLayer, ::WeatherLayer& weatherLayer, GlTools_ABC& tools, SymbolIcons& icons,
                                ColorStrategy_ABC& colorStrategy, const tools::ExerciseConfig& config )
    : gui::RichDockWidget( controllers, parent, "creation-panel" )
{
    gui::SubObjectName subObject( "CreationPanels" );
    setWindowTitle( tools::translate( "CreationPanels", "Creation" ) );
    panels_ = new gui::Panels( "panels", this );
    setWidget( panels_ );

    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    panels_->AddPanel( units_ = new UnitsPanel( panels_, *panels_, controllers, staticModel.types_, icons, colorStrategy ) );
    panels_->AddPanel( crowds_ = new PopulationsPanel( panels_, *panels_, controllers, staticModel.types_ ) );
    panels_->AddPanel( objects_ = new ObjectCreationPanel( panels_, *panels_, controllers, model.actions_, staticModel, simulation, model.teams_.GetNoSideTeam(), paramLayer, tools, config ) );
    panels_->AddPanel( fires_ = new FireCreationPanel( panels_, *panels_, controllers, model.actions_, simulation, staticModel, paramLayer, tools ) );
    panels_->AddPanel( weather_ = new ::WeatherPanel( panels_, *panels_, controllers, model.actions_, staticModel, simulation, weatherLayer ) );
    panels_->AddPanel( drawings_ = new DrawerPanel( panels_, *panels_, paramLayer, controllers, model.drawings_, staticModel.drawings_, config ) );
    controllers_.Update( *this );
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
void CreationPanels::Draw( gui::Viewport_ABC& viewport ) const
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
    panels_->Add( drawings_ );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::NotifyUpdated
// Created: JSR 2011-01-12
// -----------------------------------------------------------------------------
void CreationPanels::NotifyUpdated( const ModelUnLoaded& )
{
    RemovePanels();
    panels_->Remove( drawings_ );
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
// Name: CreationPanels::NotifyModeChanged
// Created: NPT 2013-02-28
// -----------------------------------------------------------------------------
void CreationPanels::NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode )
{
    RichDockWidget::NotifyModeChanged( newMode, useDefault, firstChangeToSavedMode );
    panels_->SelectAndLockPanel( GetCurrentMode() == eModes_Replay? 5 : 0, GetCurrentMode() == eModes_Replay );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::AddPanels
// Created: LGY 2012-11-22
// -----------------------------------------------------------------------------
void CreationPanels::AddPanels()
{
    panels_->Add( units_ );
    panels_->Add( objects_ );
    panels_->Add( crowds_ );
    panels_->Add( weather_ );
    panels_->Add( fires_ );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::RemovePanels
// Created: LGY 2012-11-22
// -----------------------------------------------------------------------------
void CreationPanels::RemovePanels()
{
    panels_->Remove( units_ );
    panels_->Remove( objects_ );
    panels_->Remove( crowds_ );
    panels_->Remove( weather_ );
    panels_->Remove( fires_ );
}

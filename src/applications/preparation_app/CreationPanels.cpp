// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "CreationPanels.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_gui/DrawerPanel.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/PopulationsPanel.h"
#include "clients_gui/UnitsPanel.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "preparation/TeamsModel.h"
#include "GhostsPanel.h"
#include "TemplatesPanel.h"
#include "InhabitantCreationPanel.h"
#include "ObjectCreationPanel.h"
#include "WeatherPanel.h"
#include "tools/ExerciseConfig.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CreationPanels constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
CreationPanels::CreationPanels( QWidget* parent,
                                kernel::Controllers& controllers,
                                const ::StaticModel& staticModel,
                                const Model& model,
                                const tools::ExerciseConfig& config,
                                gui::SymbolIcons& icons,
                                gui::ColorStrategy_ABC& colorStrategy,
                                const std::shared_ptr< gui::ParametersLayer >& paramLayer,
                                const std::shared_ptr< gui::WeatherLayer >& weatherLayer,
                                gui::GlProxy& glProxy,
                                ColorController& colorController )
    : Panels( "CreationPanels", parent )
{
    gui::SubObjectName subObject( "CreationPanels" );
    AddPanel( new gui::UnitsPanel ( this, *this, controllers, staticModel.types_, icons, colorStrategy ) );
    AddPanel( new gui::PopulationsPanel( this, *this, controllers, ( tools::Resolver< PopulationType >&)( staticModel.types_ ) ) );
    inhabitantCreationPanel_ = new InhabitantCreationPanel( this, *this, controllers, staticModel.types_, *model.agents_, paramLayer, glProxy );
    AddPanel( inhabitantCreationPanel_ );
    objectCreationPanel_ = new ObjectCreationPanel( this, *this, controllers, staticModel, *model.objects_, *model.urban_, *model.weather_, model.teams_->GetNoSideTeam(), paramLayer, glProxy, config );
    AddPanel( objectCreationPanel_ );
    ghostPanel_ = new GhostsPanel( this, *this, controllers, model.GetSymbolsFactory(), icons, colorStrategy );
    AddPanel( ghostPanel_ );
    AddPanel( new TemplatesPanel( this, *this, controllers, *model.agents_, *model.formations_, *model.ghosts_, staticModel.types_, colorController ) );
    AddPanel( new gui::DrawerPanel( this, *this, paramLayer, controllers, *model.drawings_, staticModel.drawings_, config ) );
    weatherPanel_ = new WeatherPanel( this, *this, controllers, staticModel.coordinateConverter_, weatherLayer );
    AddPanel( weatherPanel_ );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels destructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
CreationPanels::~CreationPanels()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::GetObjectCreationPanel
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
ObjectCreationPanel& CreationPanels::GetObjectCreationPanel() const
{
    return *objectCreationPanel_;
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::GetInhabitantCreationPanel
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
InhabitantCreationPanel& CreationPanels::GetInhabitantCreationPanel() const
{
    return *inhabitantCreationPanel_;
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::Load
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
void CreationPanels::Load()
{
    ghostPanel_->Load();
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::Purge
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
void CreationPanels::Purge()
{
    weatherPanel_->Purge();
}

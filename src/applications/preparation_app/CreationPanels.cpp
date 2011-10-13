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
#include "clients_gui/IntelligencesPanel.h"
#include "clients_gui/PopulationsPanel.h"
#include "clients_gui/UnitsPanel.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
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
CreationPanels::CreationPanels( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel, const Model& model,
                                const tools::ExerciseConfig& config, gui::ItemFactory_ABC& factory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy,
                                gui::ParametersLayer& paramLayer, gui::WeatherLayer& weatherLayer, gui::GlProxy& glProxy )
    : Panels( parent )
{
    AddPanel( new gui::UnitsPanel ( this, *this, controllers, staticModel.types_, factory, icons, colorStrategy ) );
    AddPanel( new gui::PopulationsPanel( this, *this, controllers, ( tools::Resolver< PopulationType >&)( staticModel.types_ ), factory ) );
    inhabitantCreationPanel_ = new InhabitantCreationPanel( this, *this, controllers, staticModel.types_, model.agents_, paramLayer, glProxy );
    AddPanel( inhabitantCreationPanel_ );
    objectCreationPanel_ = new ObjectCreationPanel( this, *this, controllers, staticModel, model.objects_, model.urban_, paramLayer, glProxy, config );
    AddPanel( objectCreationPanel_ );
    AddPanel( new TemplatesPanel( this, *this, controllers, model.agents_, model.formations_, staticModel.types_ ) );
    AddPanel( new gui::DrawerPanel( this, *this, paramLayer, controllers, model.drawings_, config ) );
    AddPanel( new WeatherPanel( this, *this, controllers, staticModel.coordinateConverter_, weatherLayer ) );
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

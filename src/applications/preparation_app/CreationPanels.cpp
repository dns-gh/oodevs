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
#include "preparation/StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_gui/IntelligencesPanel.h"
#include "clients_gui/PopulationsPanel.h"
#include "clients_gui/UnitsPanel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CreationPanels constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
CreationPanels::CreationPanels( QWidget* parent, Controllers& controllers, const StaticModel& staticModel, gui::ItemFactory_ABC& factory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy )
    : Panels( parent )
{
    AddPanel( new gui::UnitsPanel ( this, *this, controllers, staticModel.types_, factory, icons, colorStrategy ) );
    AddPanel( new gui::PopulationsPanel( this, *this, controllers, ( tools::Resolver< PopulationType >&)( staticModel.types_ ), factory ) );
    intelligencesPanel_ = new gui::IntelligencesPanel( this, *this, controllers, staticModel.levels_, icons );
    AddPanel( intelligencesPanel_ );
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
// Name: CreationPanels::Load
// Created: ABR 2011-05-25
// -----------------------------------------------------------------------------
void CreationPanels::Load( const tools::ExerciseConfig& config )
{
    intelligencesPanel_->Load( config );
}

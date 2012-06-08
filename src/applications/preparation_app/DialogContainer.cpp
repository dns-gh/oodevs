// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "DialogContainer.h"

#include "AgentAffinitiesDialog.h"
#include "ChangeDiplomacyDialog.h"
#include "ColorEditor.h"
#include "ExerciseDialog.h"
#include "FileLoaderObserver.h"
#include "FilterDialog.h"
#include "FilterDialogs.h"
#include "FormationHierarchyEditor.h"
#include "GhostSymbolDialog.h"
#include "LogisticStockEditor.h"
#include "LogisticLinksEditor.h"
#include "LongNameEditor.h"
#include "ModelConsistencyDialog.h"
#include "OrbatPanel.h"
#include "PeopleAffinitiesDialog.h"
#include "PerformanceDialog.h"
#include "ProfileDialog.h"
#include "ProfileWizardDialog.h"
#include "ScoreDialog.h"
#include "SuccessFactorDialog.h"
#include "SymbolEditor.h"
#include "UnitStateDialog.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_kernel/ObjectTypes.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "tools/DefaultLoader.h"
#include "tools/ExerciseConfig.h"

// -----------------------------------------------------------------------------
// Name: DialogContainer constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
DialogContainer::DialogContainer( QWidget* parent, kernel::Controllers& controllers, Model& model, const ::StaticModel& staticModel, const kernel::Profile_ABC& profile,
                                  gui::ColorStrategy_ABC& colorStrategy, gui::ColorEditor_ABC& colorEditor, const gui::EntitySymbols& symbols,
                                  const tools::ExerciseConfig& config, gui::SymbolIcons& icons, gui::LightingProxy& lighting, const gui::Painter_ABC& painter,
                                  gui::ItemFactory_ABC& factory, gui::ParametersLayer& paramLayer, const kernel::GlTools_ABC& tools )
    : QObject( parent )
{
    new ChangeDiplomacyDialog( parent, controllers, profile );
    new UnitStateDialog( parent, controllers, staticModel );
    new AgentAffinitiesDialog( parent, controllers );
    new PeopleAffinitiesDialog( parent, controllers );
    new ColorEditor( parent, controllers, colorStrategy, colorEditor );
    new SymbolEditor( parent, controllers, symbols, config );
    new GhostSymbolDialog( parent, controllers, model.GetSymbolsFactory(), icons, colorStrategy );
    new LogisticLinksEditor( parent, controllers );
    new LogisticStockEditor( parent, controllers, staticModel );
    new LongNameEditor( parent, controllers, staticModel );
    new FormationHierarchyEditor( parent, controllers, staticModel.levels_ );

    prefDialog_ = new gui::PreferencesDialog( parent, controllers, lighting, staticModel.coordinateSystems_, painter );
    prefDialog_->AddPage( tr( "Orbat" ), *new OrbatPanel( prefDialog_, controllers ) );
    profileDialog_ = new ProfileDialog( parent, controllers, factory, symbols, model, staticModel.extensions_ );
    profileWizardDialog_ = new ProfileWizardDialog( parent, model );
    scoreDialog_ = new ScoreDialog( parent, controllers, factory, model.scores_, paramLayer, staticModel, config, tools );
    successFactorDialog_ = new SuccessFactorDialog( parent, controllers, model.successFactors_, factory, staticModel.successFactorActionTypes_, model.scores_ );
    exerciseDialog_ = new ExerciseDialog( parent, controllers, model.exercise_, config );
    consistencyDialog_ = new ModelConsistencyDialog( parent, model, staticModel, controllers, static_cast< const tools::DefaultLoader& >( config.GetLoader() ).GetObserver() );
    performanceDialog_ = new PerformanceDialog( parent, model, staticModel );
    filtersDialog_ = new FilterDialogs( parent, config, model, staticModel.coordinateConverter_ );
}

// -----------------------------------------------------------------------------
// Name: DialogContainer destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
DialogContainer::~DialogContainer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::Load
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void DialogContainer::Load()
{
    scoreDialog_->Load();
    filtersDialog_->Load();
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::Purge
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
void DialogContainer::Purge()
{
    filtersDialog_->Purge();
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetPrefDialog
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
gui::PreferencesDialog& DialogContainer::GetPrefDialog() const
{
    return *prefDialog_;
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetProfileDialog
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
ProfileDialog& DialogContainer::GetProfileDialog() const
{
    return *profileDialog_;
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetProfileWizardDialog
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
ProfileWizardDialog& DialogContainer::GetProfileWizardDialog() const
{
    return *profileWizardDialog_;
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetScoreDialog
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
ScoreDialog& DialogContainer::GetScoreDialog() const
{
    return *scoreDialog_;
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetSuccessFactorDialog
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
SuccessFactorDialog& DialogContainer::GetSuccessFactorDialog() const
{
    return *successFactorDialog_;
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetExerciseDialog
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
ExerciseDialog& DialogContainer::GetExerciseDialog() const
{
    return *exerciseDialog_;
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetConsistencyDialog
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
gui::ConsistencyDialog_ABC& DialogContainer::GetConsistencyDialog() const
{
    return *consistencyDialog_;
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetPerformanceDialog
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
PerformanceDialog& DialogContainer::GetPerformanceDialog() const
{
    return *performanceDialog_;
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetFiltersDialog
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
FilterDialogs& DialogContainer::GetFiltersDialog() const
{
    return *filtersDialog_;
}

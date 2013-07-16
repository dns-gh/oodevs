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
#include "FilterDialogs.h"
#include "SymbolDialog.h"
#include "LogisticStockEditor.h"
#include "LogisticLinksEditor.h"
#include "LongNameEditor.h"
#include "ModelConsistencyDialog.h"
#include "OrbatPanel.h"
#include "PeopleAffinitiesDialog.h"
#include "PerformanceDialog.h"
#include "ProfileDialog.h"
#include "ProfileWizardDialog.h"
#include "RemoveBlocksDialog.h"
#include "ScoreDialog.h"
#include "SuccessFactorDialog.h"
#include "SymbolEditor.h"
#include "TerrainExportDialog.h"
#include "UnitStateDialog.h"
#include "clients_gui/AddRasterDialog.h"
#include "clients_gui/PreferencesDialog.h"
#include "clients_gui/SoundManager.h"
#include "clients_kernel/ObjectTypes.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "tools/DefaultLoader.h"
#include "tools/ExerciseConfig.h"
#include "tools/RealFileLoaderObserver_ABC.h"

#include <vector>
#include <string>

// -----------------------------------------------------------------------------
// Name: DialogContainer constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
DialogContainer::DialogContainer( QWidget* parent, kernel::Controllers& controllers, Model& model, const ::StaticModel& staticModel, const kernel::Profile_ABC& profile,
                                  gui::ColorStrategy_ABC& colorStrategy, gui::ColorEditor_ABC& colorEditor, const gui::EntitySymbols& symbols,
                                  const tools::ExerciseConfig& config, gui::SymbolIcons& icons, gui::LightingProxy& lighting, const gui::Painter_ABC& painter,
                                  gui::ParametersLayer& paramLayer, gui::GlTools_ABC& tools, gui::GlSelector& selector, gui::Elevation2dLayer& elevation2dLayer )
    : QObject( parent )
{
    gui::SubObjectName subObject( "DialogContainer" );
    new ChangeDiplomacyDialog( parent, controllers, profile );
    new UnitStateDialog( parent, controllers, staticModel );
    new AgentAffinitiesDialog( parent, controllers );
    new PeopleAffinitiesDialog( parent, controllers );
    new ColorEditor( parent, controllers, colorStrategy, colorEditor );
    new SymbolEditor( parent, controllers, symbols, config );
    new SymbolDialog( parent, controllers, model.GetSymbolsFactory(), icons, colorStrategy );
    new LogisticLinksEditor( parent, controllers );
    new LogisticStockEditor( parent, controllers, staticModel );
    new LongNameEditor( parent, controllers, staticModel );

    std::vector< std::string > sounds;
    gui::SoundManager soundManager( sounds );
    prefDialog_ = new gui::PreferencesDialog( parent, controllers, lighting, staticModel.coordinateSystems_, painter, selector, elevation2dLayer, soundManager );
    prefDialog_->AddPage( tr( "Orbat" ), *new OrbatPanel( prefDialog_, controllers ) );
    profileDialog_ = new ProfileDialog( parent, controllers, symbols, model, staticModel.extensions_ );
    profileWizardDialog_ = new ProfileWizardDialog( parent, model );
    scoreDialog_ = new ScoreDialog( "scoreDialog", parent, controllers, model.scores_, paramLayer, staticModel, config, tools );
    successFactorDialog_ = new SuccessFactorDialog( parent, controllers, model.successFactors_, staticModel.successFactorActionTypes_, model.scores_ );
    exerciseDialog_ = new ExerciseDialog( parent, controllers, model.exercise_, config );
    consistencyDialog_ = new ModelConsistencyDialog( parent, model, staticModel, controllers, const_cast< tools::RealFileLoaderObserver_ABC& >( static_cast< const tools::DefaultLoader& >( config.GetLoader() ).GetObserver() ) );
    performanceDialog_ = new PerformanceDialog( parent, model, staticModel );
    filtersDialog_ = new FilterDialogs( parent, config, model, staticModel.coordinateConverter_ );
    addRasterDialog_ = new gui::AddRasterDialog( parent );
    removeBlocksDialog_ = new RemoveBlocksDialog( parent, controllers, model.urban_ );
    terrainExportDialog_ = new TerrainExportDialog( parent, config, model.urban_ );
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

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetAddRasterDialog
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
gui::AddRasterDialog& DialogContainer::GetAddRasterDialog() const
{
    return *addRasterDialog_;
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetRemoveBlocksDialog
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
RemoveBlocksDialog& DialogContainer::GetRemoveBlocksDialog() const
{
    return *removeBlocksDialog_;
}

// -----------------------------------------------------------------------------
// Name: DialogContainer::GetTerrainExportDialog
// Created: ABR 2012-06-13
// -----------------------------------------------------------------------------
TerrainExportDialog& DialogContainer::GetTerrainExportDialog() const
{
    return *terrainExportDialog_;
}

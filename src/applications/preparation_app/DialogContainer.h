// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __DialogContainer_h_
#define __DialogContainer_h_

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class SymbolFactory;
}

namespace gui
{
    class AddRasterDialog;
    class ColorStrategy_ABC;
    class ColorEditor_ABC;
    class ConsistencyDialog_ABC;
    class EntitySymbols;
    class GlTools_ABC;
    class GlSelector;
    class LightingProxy;
    class Painter_ABC;
    class ParametersLayer;
    class PreferencesDialog;
    class SymbolIcons;
    class Elevation2dLayer;
    class GraphicPreferences;
    class DisplayExtractor;
    class LinkInterpreter;
    class UnitStateDialog;
}

namespace tools
{
    class ExerciseConfig;
}

class ExerciseDialog;
class FilterDialogs;
class Model;
class PerformanceDialog;
class ProfileDialog;
class ProfileWizardDialog;
class RemoveBlocksDialog;
class ScoreDialog;
class StaticModel;
class SuccessFactorDialog;
class TerrainExportDialog;

// =============================================================================
/** @class  DialogContainer
    @brief  DialogContainer
*/
// Created: AGE 2006-04-20
// =============================================================================
class DialogContainer : public QObject
{
public:
    //! @name Constructors/Destructor
    //@{
             DialogContainer( QWidget* parent, kernel::Controllers& controllers, Model& model, const StaticModel& staticModel, const kernel::Profile_ABC& profile,
                              gui::ColorStrategy_ABC& colorStrategy, gui::ColorEditor_ABC& colorEditor, const gui::EntitySymbols& symbols,
                              const tools::ExerciseConfig& config, gui::SymbolIcons& icons, gui::LightingProxy& lighting, const gui::Painter_ABC& painter,
                              gui::ParametersLayer& paramLayer, gui::GlTools_ABC& tools, gui::GlSelector& selector, gui::Elevation2dLayer& elevation2dLayer, gui::GraphicPreferences& preferences );
    virtual ~DialogContainer();
    //@}

    //! @name Operations
    //@{
    void Load();
    void Purge();
    //@}

    //! @name Accessors
    //@{
    gui::PreferencesDialog& GetPrefDialog() const;
    ProfileDialog& GetProfileDialog() const;
    ProfileWizardDialog& GetProfileWizardDialog() const;
    ScoreDialog& GetScoreDialog() const;
    SuccessFactorDialog& GetSuccessFactorDialog() const;
    ExerciseDialog& GetExerciseDialog() const;
    gui::ConsistencyDialog_ABC& GetConsistencyDialog() const;
    PerformanceDialog& GetPerformanceDialog() const;
    FilterDialogs& GetFiltersDialog() const;
    gui::AddRasterDialog& GetAddRasterDialog() const;
    RemoveBlocksDialog& GetRemoveBlocksDialog() const;
    TerrainExportDialog& GetTerrainExportDialog() const;
    //@}

private:
    //! @name Member data
    //@{
    gui::PreferencesDialog*     prefDialog_;
    ProfileDialog*              profileDialog_;
    ProfileWizardDialog*        profileWizardDialog_;
    ScoreDialog*                scoreDialog_;
    SuccessFactorDialog*        successFactorDialog_;
    ExerciseDialog*             exerciseDialog_;
    gui::ConsistencyDialog_ABC* consistencyDialog_;
    PerformanceDialog*          performanceDialog_;
    FilterDialogs*              filtersDialog_;
    gui::AddRasterDialog*       addRasterDialog_;
    RemoveBlocksDialog*         removeBlocksDialog_;
    TerrainExportDialog*        terrainExportDialog_;
    std::unique_ptr< gui::DisplayExtractor > displayExtractor_;
    std::unique_ptr< gui::LinkInterpreter > linkInterpreter_;
    gui::UnitStateDialog*       unitStateDialog_;
    //@}
};

#endif // __DialogContainer_h_

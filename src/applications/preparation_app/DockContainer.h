// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __DockContainer_h_
#define __DockContainer_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class AutomatsLayer;
    class FormationLayer;
    class EntitySymbols;
    class SearchTreeView_ABC;
    class SymbolIcons;
    class ColorStrategy_ABC;
    class ParametersLayer;
    class WeatherLayer;
    class GlProxy;
    class TerrainProfilerLayer;
    class TerrainProfiler;
}

namespace tools
{
    class ExerciseConfig;
}

class ColorController;
class ModelBuilder;
class Model;
class StaticModel;
class ObjectCreationPanel;
class InhabitantCreationPanel;
class CreationPanels;
class LivingAreaPanel;
class UsagesDockWidget;

// =============================================================================
/** @class  DockContainer
    @brief  Dock manager
*/
// Created: LGY 2012-01-04
// =============================================================================
class DockContainer : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DockContainer( QMainWindow* parent, kernel::Controllers& controllers, gui::AutomatsLayer& automats,
                            gui::FormationLayer& formation, gui::EntitySymbols& icons, ModelBuilder& modelBuilder,
                            Model& model, StaticModel& staticModel, const tools::ExerciseConfig& config, gui::SymbolIcons& symbols,
                            gui::ColorStrategy_ABC& colorStrategy, gui::ParametersLayer& paramLayer, gui::WeatherLayer& weatherLayer,
                            gui::GlProxy& glProxy, ColorController& colorController, gui::TerrainProfilerLayer& terrainProfilerLayer,
                            const kernel::Profile_ABC& profile );
    virtual ~DockContainer();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Load();
    void BlockCreationOnListViews( bool enable );
    //@}

    //! @name Getters
    //@{
    ObjectCreationPanel& GetObjectCreationPanel() const;
    InhabitantCreationPanel& GetInhabitantCreationPanel() const;
    LivingAreaPanel& GetLivingAreaPanel() const;
    gui::TerrainProfiler& GetTerrainProfiler() const;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< gui::SearchTreeView_ABC* > treeViews_;
    CreationPanels*                         pCreationPanel_;
    LivingAreaPanel*                        pLivingAreaPanel_;
    gui::TerrainProfiler*                   terrainProfiler_;
    UsagesDockWidget*                       pUsagesPanel_;
    //@}
};

#endif // __DockContainer_h_

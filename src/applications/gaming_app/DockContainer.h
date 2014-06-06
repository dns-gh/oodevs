// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __gaming_DockContainer_h_
#define __gaming_DockContainer_h_

#include <boost/noncopyable.hpp>

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder;
    }
}

namespace gui
{
    class ColorStrategy_ABC;
    class DisplayExtractor;
    class EntitySymbols;
    class GlProxy;
    class Logger;
    class MiniViews;
    class ParametersLayer;
    class RichItemFactory;
    class SymbolIcons;
    class TerrainProfiler;
    class TerrainProfilerLayer;
}

namespace kernel
{
    class Controllers;
}

class ActionsScheduler;
class AfterAction;
class AutomatsLayer;
class Config;
class CreationPanels;
class DrawingsBuilder;
class EventDockWidget;
class FormationLayer;
class IndicatorExportDialog;
class IndicatorPlotFactory;
class ItineraryEditionDockWidget;
class Model;
class Network;
class OrbatDockWidget;
class ProfilingPanel;
class ProfileFilter;
class Simulation;
class StaticModel;
class TimelineDockWidget;
class SimulationController;
class UnitStateDialog;
class WeatherLayer;

// =============================================================================
/** @class  DockContainer
    @brief  DockContainer
*/
// Created: ABR 2013-02-13
// =============================================================================
class DockContainer : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    DockContainer( QMainWindow* parent,
                   kernel::Controllers& controllers,
                   const StaticModel& staticModel,
                   Model& model,
                   Network& network,
                   const Simulation& simulation,
                   const Config& config,
                   ProfileFilter& profile,
                   gui::ParametersLayer& paramLayer,
                   gui::TerrainProfilerLayer& profilerLayer,
                   AutomatsLayer& automatsLayer,
                   FormationLayer& formationLayer,
                   ::WeatherLayer& weatherLayer,
                   gui::GlProxy& proxy,
                   gui::RichItemFactory& factory,
                   gui::ColorStrategy_ABC& colorStrategy,
                   gui::SymbolIcons& symbolIcons,
                   const gui::EntitySymbols& entitySymbols,
                   IndicatorExportDialog& indicatorExportDialog,
                   SimulationController& simulationController,
                   DrawingsBuilder& drawingsBuilder,
                   gui::DisplayExtractor& extractor,
                   UnitStateDialog& unitStateDialog );
    virtual ~DockContainer();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Load();
    //@}

    //! @name Accessors
    //@{
    CreationPanels& GetCreationPanel() const;
    gui::Logger& GetLoggerPanel() const;
    ProfilingPanel& GetProfilingPanel() const;
    gui::TerrainProfiler& GetTerrainProfiler() const;
    gui::MiniViews& GetMiniView() const;
    EventDockWidget& GetEventDockWidget() const;
    ItineraryEditionDockWidget& GetItineraryDockWidget() const;
    //@}

private:
    //! @name Panels
    //@{
    CreationPanels* creationPanel_;
    OrbatDockWidget* orbatDockWidget_;
    gui::Logger* loggerPanel_;
    AfterAction* afterAction_;
    ProfilingPanel* profilingPanel_;
    gui::TerrainProfiler* terrainProfiler_;
    gui::MiniViews* miniView_;
    TimelineDockWidget* timelineDockWidget_;
    EventDockWidget* eventDockWidget_;
    ItineraryEditionDockWidget* itineraryDockWidget_;
    //@}

    //! @name Member data
    //@{
    std::unique_ptr< actions::gui::InterfaceBuilder > interfaceBuilder_;
    std::unique_ptr< ActionsScheduler > scheduler_;
    std::unique_ptr< IndicatorPlotFactory > plotFactory_;
    //@}
};

#endif // __gaming_DockContainer_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "DockContainer.h"

#include "AfterAction.h"
#include "AutomatsLayer.h"
#include "ChatDock.h"
#include "ClockDock.h"
#include "GamingConfig.h"
#include "CreationPanels.h"
#include "EventDockWidget.h"
#include "ExtensionsPanel.h"
#include "FormationLayer.h"
#include "IndicatorPlotFactory.h"
#include "InfoDock.h"
#include "InfoPanels.h"
#include "InhabitantPanel.h"
#include "ItineraryEditionDockWidget.h"
#include "InhabitantPanel.h"
#include "LinkInterpreter.h"
#include "NotesPanel.h"
#include "OrbatDockWidget.h"
#include "PlanificationModePanel.h"
#include "ProfilesPanel.h"
#include "ProfilingPanel.h"
#include "Properties.h"
#include "ResourceLinksDialog.h"
#include "ScorePanel.h"
#include "TimelinePanel.h"
#include "TimelineDockWidget.h"
#include "TimelineWebView.h"

#include "actions/ActionsModel.h"
#include "actions_gui/InterfaceBuilder.h"
#include "ENT/ENT_Enums.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/EventPresenter.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/Logger.h"
#include "clients_gui/MiniViews.h"
#include "clients_gui/TerrainProfiler.h"
#include "clients_gui/RichItemFactory.h"
#include "gaming/ActionsScheduler.h"
#include "gaming/AgentServerMsgMgr.h"
#include "gaming/LimitsModel.h"
#include "gaming/Model.h"
#include "gaming/Network.h"
#include "gaming/PathfindModel.h"
#include "gaming/ProfileFilter.h"
#include "gaming/Simulation.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: DockContainer constructor
// Created: ABR 2013-02-13
// -----------------------------------------------------------------------------
DockContainer::DockContainer( QMainWindow* parent,
                              kernel::Controllers& controllers,
                              const StaticModel& staticModel,
                              Model& model,
                              Network& network,
                              const Simulation& simulation,
                              const GamingConfig& config,
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
                              UnitStateDialog& unitStateDialog )
    : timelineDockWidget_( 0 )
{
    const bool hasLegacyTimeline = config.HasTimeline();
    // Tools
    interfaceBuilder_.reset(
        new actions::gui::InterfaceBuilder(
            controllers, config, paramLayer, staticModel,
            &model.agentKnowledgeConverter_, &model.objectKnowledgeConverter_,
            &simulation, &model.limits_, &model.pathfinds_ ) );
    scheduler_.reset( new ActionsScheduler( parent, controllers, simulation, model.actions_, network.GetMessageMgr(), simulationController, hasLegacyTimeline ) );
    plotFactory_.reset( new IndicatorPlotFactory( parent, controllers, network.GetMessageMgr(), indicatorExportDialog, simulation ) );

    // -----------------------------------------------------------------------------
    // Left
    // -----------------------------------------------------------------------------
    // ResourceNetwork panel
    {
        gui::RichDockWidget* pResourceWnd = new ResourceLinksDialog( parent, model, controllers, model.actions_, staticModel );
        pResourceWnd->SetModes( eModes_Default );
        pResourceWnd->SetReadOnlyModes( eModes_Replay );

        parent->addDockWidget( Qt::LeftDockWidgetArea, pResourceWnd );
    }
    // Extensions panel
    {
        ExtensionsPanel* pExtensionsPanel = new ExtensionsPanel( parent, controllers, model, staticModel, simulation, profile );
        pExtensionsPanel->SetModes( eModes_Default );
        pExtensionsPanel->SetReadOnlyModes( eModes_Replay );
        parent->addDockWidget( Qt::LeftDockWidgetArea, pExtensionsPanel );
    }
    // Clock
    {
        gui::RichDockWidget* clockWnd = new ClockDock( parent, controllers, simulation, *scheduler_ );
        clockWnd->SetModes( eModes_Default, eModes_None, true );
        parent->addDockWidget( Qt::LeftDockWidgetArea, clockWnd );
        clockWnd->setVisible( false );
        clockWnd->setFloating( true );
    }
    // Properties
    {
        Properties* properties = new Properties( parent, controllers, proxy );
        properties->SetModes( eModes_Default );
        parent->addDockWidget( Qt::LeftDockWidgetArea, properties );
    }
    // Event panel
    {
        eventDockWidget_ = new EventDockWidget( parent, controllers, model, config, simulation, *interfaceBuilder_, profile, proxy, entitySymbols );
        eventDockWidget_->SetModes( eModes_Default );
        eventDockWidget_->SetMenuVisibility( false );
        parent->addDockWidget( Qt::LeftDockWidgetArea, eventDockWidget_ );
    }
    // Orbat panel
    {
        orbatDockWidget_ = new OrbatDockWidget( controllers, parent, "orbat", profile, automatsLayer, formationLayer,
            model.actions_, staticModel, simulation, entitySymbols, drawingsBuilder, paramLayer );
        orbatDockWidget_->SetModes( eModes_Default, eModes_None, true );
        parent->addDockWidget( Qt::LeftDockWidgetArea, orbatDockWidget_ );
    }
    // AfterAction
    {
        afterAction_ = new AfterAction( parent, controllers, config, model.aar_, *plotFactory_, *interfaceBuilder_ );
        afterAction_->SetModes( eModes_Default | eModes_Gaming );
        parent->addDockWidget( Qt::LeftDockWidgetArea, afterAction_ );
    }
    // Itinerary edition
    {
        itineraryDockWidget_ = new ItineraryEditionDockWidget( parent, controllers );
        itineraryDockWidget_->SetModes( eModes_AllGaming & ~eModes_Itinerary, eModes_Itinerary );
        parent->addDockWidget( Qt::LeftDockWidgetArea, itineraryDockWidget_ );
    }
    // -----------------------------------------------------------------------------
    // Right
    // -----------------------------------------------------------------------------
    // Creation panel
    {
        creationPanel_ = new CreationPanels( parent, controllers, staticModel, model, simulation, paramLayer, weatherLayer, proxy, symbolIcons, colorStrategy, config );
        creationPanel_->SetModes( eModes_Default );
        parent->addDockWidget( Qt::RightDockWidgetArea, creationPanel_ );
    }
    // Profiles panel
    {
        ProfilesPanel* profilesPanel = new ProfilesPanel( parent, controllers, network, profile, model.teams_ );
        profilesPanel->SetModes( eModes_Default | eModes_Replay );
        parent->addDockWidget( Qt::RightDockWidgetArea, profilesPanel );
    }
    // Inhabitant panel
    {
        InhabitantPanel* inhabitantPanel = new InhabitantPanel( parent, controllers, model );
        inhabitantPanel->SetModes( eModes_Default );
        parent->addDockWidget( Qt::RightDockWidgetArea, inhabitantPanel );
    }
    // Score panel
    {
        ScorePanel* scorePanel = new ScorePanel( parent, controllers, extractor, *plotFactory_, indicatorExportDialog, model.scores_ );
        scorePanel->SetModes( eModes_Default );
        parent->addDockWidget( Qt::RightDockWidgetArea, scorePanel );
    }
    // Notes panel
    {
        NotesPanel* notePanel = new NotesPanel( parent, controllers, model.notes_, network.GetMessageMgr() );
        notePanel->SetModes( eModes_Default );
        parent->addDockWidget( Qt::RightDockWidgetArea, notePanel );
    }
    // Profiler
    {
        profilingPanel_ = new ProfilingPanel( parent, controllers, network, simulation );
        profilingPanel_->SetModes( eModes_Default );
        profilingPanel_->SetMenuVisibility( false );
        parent->addDockWidget( Qt::RightDockWidgetArea, profilingPanel_ );
    }
    // Info panel
    {
        InfoPanels* pInfoPanel = new InfoPanels( parent, controllers, factory, extractor );
        pInfoPanel->SetModes( eModes_Default );
        parent->addDockWidget( Qt::RightDockWidgetArea, pInfoPanel );
    }
    // Mini views
    {
        miniView_ = new gui::MiniViews( parent, controllers, profile );
        miniView_->SetModes( eModes_Default );
        parent->addDockWidget( Qt::RightDockWidgetArea, miniView_ );
    }
    // Terrain profile
    {
        terrainProfiler_ = new gui::TerrainProfiler( parent, controllers, staticModel.detection_, profilerLayer );
        terrainProfiler_->SetModes( eModes_Default );
        terrainProfiler_->SetMenuVisibility( false );
        parent->addDockWidget( Qt::RightDockWidgetArea, terrainProfiler_ );
    }

    // -----------------------------------------------------------------------------
    // Top
    // -----------------------------------------------------------------------------
    // Planification mode panel
    {
        PlanificationModePanel* pPlanificationModePanel = new PlanificationModePanel( parent, controllers );
        pPlanificationModePanel->SetModes( eModes_Gaming | eModes_Replay | eModes_Default, eModes_Planning );
        parent->addDockWidget( Qt::TopDockWidgetArea, pPlanificationModePanel );
    }

    // Timelines
    {
        // New Timeline
        timelineDockWidget_ = new TimelineDockWidget( parent, controllers, config, model );
        timelineDockWidget_->SetModes( eModes_Default );
        parent->addDockWidget( Qt::TopDockWidgetArea, timelineDockWidget_ );
        eventDockWidget_->SetTimelineHandler( timelineDockWidget_->GetWebView() );
        QObject::connect( timelineDockWidget_->GetWebView().get(), SIGNAL( StartCreation( E_EventTypes, const QDateTime& ) ), &eventDockWidget_->GetPresenter(), SLOT( StartCreation( E_EventTypes, const QDateTime& ) ) );
    }
    if( hasLegacyTimeline )
    {
        // Old Timeline
        TimelinePanel* timelinePanel = new TimelinePanel( parent, controllers, model, *scheduler_, config, profile, extractor );
        timelinePanel->SetModes( eModes_Default );
        parent->addDockWidget( Qt::TopDockWidgetArea, timelinePanel );
    }

    // -----------------------------------------------------------------------------
    // Bottom
    // -----------------------------------------------------------------------------
    // Chat
    {
        gui::RichDockWidget* chatDock = new ChatDock( parent, controllers, network.GetMessageMgr(), network.GetCommands() );
        chatDock->SetModes( eModes_Default );
        parent->addDockWidget( Qt::BottomDockWidgetArea, chatDock );
    }
    // Logger
    {
        loggerPanel_ = new gui::Logger( parent, controllers, simulation, "./Debug/Gaming.log" );
        loggerPanel_->SetModes( eModes_None, eModes_Default );
        parent->addDockWidget( Qt::BottomDockWidgetArea, loggerPanel_ );
    }
    // Info
    {
        gui::RichDockWidget* infoWnd = new InfoDock( parent, controllers, profile, entitySymbols, factory, extractor,
                                                     model, simulation, simulationController, unitStateDialog );
        infoWnd->SetModes( eModes_Default, eModes_None, true );
        parent->addDockWidget( Qt::BottomDockWidgetArea, infoWnd );
    }
}

// -----------------------------------------------------------------------------
// Name: DockContainer destructor
// Created: ABR 2013-02-13
// -----------------------------------------------------------------------------
DockContainer::~DockContainer()
{
    delete afterAction_;
    delete eventDockWidget_;
    if( timelineDockWidget_ )
        delete timelineDockWidget_;
}

// -----------------------------------------------------------------------------
// Name: DockContainer::Purge
// Created: ABR 2013-02-13
// -----------------------------------------------------------------------------
void DockContainer::Purge()
{
    orbatDockWidget_->Purge();
    if( timelineDockWidget_ )
        timelineDockWidget_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: DockContainer::Load
// Created: ABR 2013-02-13
// -----------------------------------------------------------------------------
void DockContainer::Load()
{
    orbatDockWidget_->Load();
    if( timelineDockWidget_ )
        timelineDockWidget_->Connect();
}

// -----------------------------------------------------------------------------
// Name: DockContainer::GetCreationPanel
// Created: ABR 2013-02-13
// -----------------------------------------------------------------------------
CreationPanels& DockContainer::GetCreationPanel() const
{
    return *creationPanel_;
}

// -----------------------------------------------------------------------------
// Name: DockContainer::GetLoggerPanel
// Created: ABR 2013-02-13
// -----------------------------------------------------------------------------
gui::Logger& DockContainer::GetLoggerPanel() const
{
    return *loggerPanel_;
}

// -----------------------------------------------------------------------------
// Name: DockContainer::GetProfilingPanel
// Created: ABR 2013-02-13
// -----------------------------------------------------------------------------
ProfilingPanel& DockContainer::GetProfilingPanel() const
{
    return *profilingPanel_;
}

// -----------------------------------------------------------------------------
// Name: DockContainer::GetTerrainProfiler
// Created: ABR 2013-02-13
// -----------------------------------------------------------------------------
gui::TerrainProfiler& DockContainer::GetTerrainProfiler() const
{
    return *terrainProfiler_;
}

// -----------------------------------------------------------------------------
// Name: DockContainer::GetMiniView
// Created: ABR 2013-02-14
// -----------------------------------------------------------------------------
gui::MiniViews& DockContainer::GetMiniView() const
{
    return *miniView_;
}

// -----------------------------------------------------------------------------
// Name: DockContainer::GetEventDockWidget
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
EventDockWidget& DockContainer::GetEventDockWidget() const
{
    return *eventDockWidget_;
}

// -----------------------------------------------------------------------------
// Name: DockContainer::GetItineraryDockWidget
// Created: SLI 2014-04-10
// -----------------------------------------------------------------------------
ItineraryEditionDockWidget& DockContainer::GetItineraryDockWidget() const
{
    return *itineraryDockWidget_;
}

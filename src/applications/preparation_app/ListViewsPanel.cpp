// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ListViewsPanel.h"
#include "moc_ListViewsPanel.cpp"
#include "TacticalListView.h"
#include "PreparationProfile.h"
#include "CommunicationListView.h"
#include "ObjectListView.h"
#include "PopulationListView.h"
#include "LogisticListView.h"
#include "InhabitantListView.h"
#include "UrbanListView.h"
#include "clients_gui/SearchListView.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/AggregateToolbar.h"
#include "clients_kernel/Tools.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "preparation/FormationModel.h"

// -----------------------------------------------------------------------------
// Name: ListViewsPanel constructor
// Created: LGY 2012-06-26
// -----------------------------------------------------------------------------
ListViewsPanel::ListViewsPanel( kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons,
                                ModelBuilder& modelBuilder, Model& model, gui::GlProxy& glProxy, std::vector< gui::SearchListView_ABC* >& listViews,
                                gui::SymbolIcons& symbols, StaticModel& staticModel, const gui::AggregateToolbar& aggregateToolbar )
    : QTabWidget()
{
    {
        QTabWidget* pFirstAgentsTabWidget = new QTabWidget();
        CreateUnitTabWidget( pFirstAgentsTabWidget, this, controllers, factory, icons, modelBuilder, model, staticModel, glProxy, listViews, aggregateToolbar, true );

        pSecondAgentsTabWidget_ = new QTabWidget();
        pSecondAgentsTabWidget_->hide();
        CreateUnitTabWidget( pSecondAgentsTabWidget_, this, controllers, factory, icons, modelBuilder, model, staticModel, glProxy, listViews, aggregateToolbar, false );

        QSplitter* splitter = new QSplitter();
        splitter->addWidget( pFirstAgentsTabWidget );
        splitter->addWidget( pSecondAgentsTabWidget_ );
        splitter->setCollapsible( pFirstAgentsTabWidget, false );
        splitter->setCollapsible( pSecondAgentsTabWidget_, true );

        QWidget* widget = new QWidget();
        QVBoxLayout* box = new QVBoxLayout( widget );
        box->setSpacing( 0 );
        box->setMargin( 0 );
        box->addWidget( splitter, 1 );

        addTab( widget, tools::translate( "DockContainer", "Units" ) );
    }
    // Objects
    {
        gui::SearchListView_ABC* searchListView = new gui::SearchListView< ObjectListView >( this, controllers, factory, modelBuilder );
        Configure( searchListView, listViews, aggregateToolbar, ePreparationMode_Terrain );
        addTab( searchListView, tools::translate( "DockContainer","Objects" ) );
    }
    // Urban
    {
        gui::SearchListView_ABC* searchListView = new gui::SearchListView< UrbanListView >( this, controllers, factory, modelBuilder, symbols, staticModel );
        Configure( searchListView, listViews, aggregateToolbar );
        addTab( searchListView, tools::translate( "DockContainer","Urban" ) );
    }
    // Crowds
    {
        gui::SearchListView_ABC* searchListView = new gui::SearchListView< PopulationListView >( this, controllers, factory, modelBuilder );
        Configure( searchListView, listViews, aggregateToolbar, ePreparationMode_Terrain );
        addTab( searchListView, tools::translate( "DockContainer","Crowds" ) );
    }
    // Populations
    {
        gui::SearchListView_ABC* searchListView = new gui::SearchListView< InhabitantListView >( this, controllers, factory, modelBuilder );
        Configure( searchListView, listViews, aggregateToolbar, ePreparationMode_Terrain );
        addTab( searchListView, tools::translate( "DockContainer","Populations" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ListViewsPanel destructor
// Created: LGY 2012-06-26
// -----------------------------------------------------------------------------
ListViewsPanel::~ListViewsPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListViewsPanel::CreateUnitTabWidget
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void ListViewsPanel::CreateUnitTabWidget( QTabWidget* parent, QTabWidget* tabWidget,
                                          kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons,
                                          ModelBuilder& modelBuilder, Model& model, StaticModel& staticModel, gui::GlProxy& glProxy,
                                          std::vector< gui::SearchListView_ABC* >& listViews, const gui::AggregateToolbar& aggregateToolbar,
                                          bool first )
{
    std::vector< gui::HierarchyListView_ABC* >& lists = first ? firstUnitViews_ : secondUnitViews_;
    // Tactical
    {
        gui::SearchListView_ABC* searchListView = new gui::SearchListView< TacticalListView >( tabWidget, controllers, factory, icons, model, modelBuilder, staticModel.types_, glProxy );
        lists.push_back( static_cast< gui::HierarchyListView_ABC* >( searchListView->GetRichListView() ) );
        connect( lists.back(), SIGNAL( ListViewFocusIn( gui::HierarchyListView_ABC* ) ), SLOT( FocusIn( gui::HierarchyListView_ABC* ) ) );
        Configure( searchListView, listViews, aggregateToolbar, ePreparationMode_Terrain );
        parent->addTab( searchListView, tools::translate( "DockContainer","Tactical" ) );
    }
    // Communication
    {
        gui::SearchListView_ABC* searchListView = new gui::SearchListView< CommunicationListView >( tabWidget, controllers, factory, icons, modelBuilder );
        lists.push_back( static_cast< gui::HierarchyListView_ABC* >( searchListView->GetRichListView() ) );
        connect( lists.back(), SIGNAL( ListViewFocusIn( gui::HierarchyListView_ABC* ) ), SLOT( FocusIn( gui::HierarchyListView_ABC* ) ) );
        Configure( searchListView, listViews, aggregateToolbar, ePreparationMode_Terrain );
        parent->addTab( searchListView, tools::translate( "DockContainer","Communication" ) );
    }
    // Logistic
    {
        gui::SearchListView_ABC* searchListView = new gui::SearchListView< LogisticListView >( tabWidget, controllers, factory, PreparationProfile::GetProfile(), icons, modelBuilder );
        lists.push_back( static_cast< gui::HierarchyListView_ABC* >( searchListView->GetRichListView() ) );
        connect( lists.back(), SIGNAL( ListViewFocusIn( gui::HierarchyListView_ABC* ) ), SLOT( FocusIn( gui::HierarchyListView_ABC* ) ) );
        Configure( searchListView, listViews, aggregateToolbar, ePreparationMode_Terrain );
        parent->addTab( searchListView, tools::translate( "DockContainer", "Logistic" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ListViewsPanel::Configure
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void ListViewsPanel::Configure( gui::SearchListView_ABC* searchListView, std::vector< gui::SearchListView_ABC* >& listViews,
                                const gui::AggregateToolbar& aggregateToolbar, int readOnlyMode )
{
    if( readOnlyMode != -1 )
        searchListView->GetRichListView()->SetReadOnlyModes( readOnlyMode );
    connect( &aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchListView->GetRichListView(), SLOT( LockDragAndDrop( bool ) ) );
    listViews.push_back( searchListView );
}

// -----------------------------------------------------------------------------
// Name: ListViewsPanel::SetVisible
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void ListViewsPanel::SetVisible( bool value )
{
    pSecondAgentsTabWidget_->setVisible( value );
}

// -----------------------------------------------------------------------------
// Name: ListViewsPanel::FocusIn
// Created: JSR 2012-07-06
// -----------------------------------------------------------------------------
void ListViewsPanel::FocusIn( gui::HierarchyListView_ABC* list )
{
    std::vector< gui::HierarchyListView_ABC* >::iterator it;
    if( std::find( firstUnitViews_.begin(), firstUnitViews_.end(), list ) != firstUnitViews_.end() )
    {
        for( it = firstUnitViews_.begin(); it != firstUnitViews_.end(); ++it )
            ( *it )->ActivateSelection( true );
        for( it = secondUnitViews_.begin(); it != secondUnitViews_.end(); ++it )
            ( *it )->ActivateSelection( false );
    }
    else if( std::find( secondUnitViews_.begin(), secondUnitViews_.end(), list ) != secondUnitViews_.end() )
    {
        for( it = firstUnitViews_.begin(); it != firstUnitViews_.end(); ++it )
            ( *it )->ActivateSelection( false );
        for( it = secondUnitViews_.begin(); it != secondUnitViews_.end(); ++it )
            ( *it )->ActivateSelection( true );
    }
}

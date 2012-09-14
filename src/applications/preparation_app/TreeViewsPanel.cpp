// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "TreeViewsPanel.h"
#include "moc_TreeViewsPanel.cpp"
#include "ModelBuilder.h"
#include "TacticalTreeView.h"
#include "PreparationProfile.h"
#include "CommunicationTreeView.h"
 #include "clients_gui/ObjectTreeView.h"
// #include "PopulationTreeView.h"
// #include "LogisticTreeView.h"
// #include "InhabitantTreeView.h"
// #include "UrbanTreeView.h"
#include "clients_gui/EntityTreeView.h"
#include "clients_gui/SearchTreeView.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/AggregateToolbar.h"
#include "clients_kernel/Tools.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "preparation/FormationModel.h"

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel constructor
// Created: LGY 2012-06-26
// -----------------------------------------------------------------------------
TreeViewsPanel::TreeViewsPanel( kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons,
                                ModelBuilder& modelBuilder, Model& model, std::vector< gui::SearchTreeView_ABC* >& treeViews,
                                gui::SymbolIcons& /*symbols*/, StaticModel& staticModel, const gui::AggregateToolbar& aggregateToolbar )
    : QTabWidget()
{
    {
        QTabWidget* pFirstAgentsTabWidget = new QTabWidget();
        CreateUnitTabWidget( pFirstAgentsTabWidget, this, controllers, factory, icons, modelBuilder, model, staticModel, treeViews, aggregateToolbar, true );

        pSecondAgentsTabWidget_ = new QTabWidget();
        pSecondAgentsTabWidget_->hide();
        CreateUnitTabWidget( pSecondAgentsTabWidget_, this, controllers, factory, icons, modelBuilder, model, staticModel, treeViews, aggregateToolbar, false );

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
        gui::SearchTreeView_ABC* searchTreeView = new gui::SearchTreeView< gui::ObjectTreeView >( this, controllers, PreparationProfile::GetProfile(), modelBuilder );
        Configure( searchTreeView, treeViews, aggregateToolbar, ePreparationMode_Terrain );
        addTab( searchTreeView, tools::translate( "DockContainer","Objects" ) );
    }
    /*
    // Urban
    {
        gui::SearchTreeView_ABC* searchTreeView = new gui::SearchTreeView< UrbanTreeView >( this, controllers, factory, modelBuilder, symbols, staticModel, model.urban_ );
        Configure( searchTreeView, treeViews, aggregateToolbar );
        addTab( searchTreeView, tools::translate( "DockContainer","Urban" ) );
    }
    */
    // Crowds
    {
        gui::SearchTreeView_ABC* searchTreeView = new gui::SearchTreeView< gui::PopulationTreeView >( this, controllers, PreparationProfile::GetProfile(), modelBuilder );
        Configure( searchTreeView, treeViews, aggregateToolbar, ePreparationMode_Terrain );
        addTab( searchTreeView, tools::translate( "DockContainer","Crowds" ) );
    }
    // Populations
    {
        gui::SearchTreeView_ABC* searchTreeView = new gui::SearchTreeView< gui::InhabitantTreeView >( this, controllers, PreparationProfile::GetProfile(), modelBuilder );
        Configure( searchTreeView, treeViews, aggregateToolbar, ePreparationMode_Terrain );
        addTab( searchTreeView, tools::translate( "DockContainer","Populations" ) );
    }

}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel destructor
// Created: LGY 2012-06-26
// -----------------------------------------------------------------------------
TreeViewsPanel::~TreeViewsPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::CreateUnitTabWidget
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void TreeViewsPanel::CreateUnitTabWidget( QTabWidget* parent, QTabWidget* tabWidget, kernel::Controllers& controllers,
                                          gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons, ModelBuilder& modelBuilder,
                                          Model& model, StaticModel& staticModel, std::vector< gui::SearchTreeView_ABC* >& treeViews,
                                          const gui::AggregateToolbar& aggregateToolbar, bool first )
{
    std::vector< gui::HierarchyTreeView_ABC* >& trees = first ? firstUnitViews_ : secondUnitViews_;
    // Tactical
    {
        gui::SearchTreeView_ABC* searchTreeView = new gui::SearchTreeView< TacticalTreeView >( tabWidget, controllers, PreparationProfile::GetProfile(), modelBuilder, icons, model, staticModel.types_, factory );
        trees.push_back( static_cast< gui::HierarchyTreeView_ABC* >( searchTreeView->GetRichTreeView() ) );
        connect( trees.back(), SIGNAL( TreeViewFocusIn( gui::HierarchyTreeView_ABC* ) ), SLOT( FocusIn( gui::HierarchyTreeView_ABC* ) ) );
        Configure( searchTreeView, treeViews, aggregateToolbar, ePreparationMode_Terrain );
        parent->addTab( searchTreeView, tools::translate( "DockContainer","Tactical" ) );
    }

    // Communication
    {
        gui::SearchTreeView_ABC* searchTreeView = new gui::SearchTreeView< CommunicationTreeView >( tabWidget, controllers, PreparationProfile::GetProfile(), modelBuilder, icons );
        trees.push_back( static_cast< gui::HierarchyTreeView_ABC* >( searchTreeView->GetRichTreeView() ) );
        connect( trees.back(), SIGNAL( TreeViewFocusIn( gui::HierarchyTreeView_ABC* ) ), SLOT( FocusIn( gui::HierarchyTreeView_ABC* ) ) );
        Configure( searchTreeView, treeViews, aggregateToolbar, ePreparationMode_Terrain );
        parent->addTab( searchTreeView, tools::translate( "DockContainer","Communication" ) );
    }
/*    // Logistic
    {
        gui::SearchTreeView_ABC* searchTreeView = new gui::SearchTreeView< LogisticTreeView >( tabWidget, controllers, factory, PreparationProfile::GetProfile(), icons, modelBuilder );
        trees.push_back( static_cast< gui::HierarchyTreeView_ABC* >( searchTreeView->GetRichTreeView() ) );
        connect( trees.back(), SIGNAL( TreeViewFocusIn( gui::HierarchyTreeView_ABC* ) ), SLOT( FocusIn( gui::HierarchyTreeView_ABC* ) ) );
        Configure( searchTreeView, treeViews, aggregateToolbar, ePreparationMode_Terrain );
        parent->addTab( searchTreeView, tools::translate( "DockContainer", "Logistic" ) );
    }
*/
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::Configure
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void TreeViewsPanel::Configure( gui::SearchTreeView_ABC* searchTreeView, std::vector< gui::SearchTreeView_ABC* >& treeViews,
                                const gui::AggregateToolbar& aggregateToolbar, int readOnlyMode )
{
    if( readOnlyMode != -1 )
        searchTreeView->GetRichTreeView()->SetReadOnlyModes( readOnlyMode );
    connect( &aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchTreeView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
    treeViews.push_back( searchTreeView );
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::SetVisible
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void TreeViewsPanel::SetVisible( bool value )
{
    pSecondAgentsTabWidget_->setVisible( value );
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::FocusIn
// Created: JSR 2012-07-06
// -----------------------------------------------------------------------------
void TreeViewsPanel::FocusIn( gui::HierarchyTreeView_ABC* tree )
{
    std::vector< gui::HierarchyTreeView_ABC* >::iterator it;
    if( std::find( firstUnitViews_.begin(), firstUnitViews_.end(), tree ) != firstUnitViews_.end() )
    {
        for( it = firstUnitViews_.begin(); it != firstUnitViews_.end(); ++it )
            ( *it )->ActivateSelection( true );
        for( it = secondUnitViews_.begin(); it != secondUnitViews_.end(); ++it )
            ( *it )->ActivateSelection( false );
    }
    else if( std::find( secondUnitViews_.begin(), secondUnitViews_.end(), tree ) != secondUnitViews_.end() )
    {
        for( it = firstUnitViews_.begin(); it != firstUnitViews_.end(); ++it )
            ( *it )->ActivateSelection( false );
        for( it = secondUnitViews_.begin(); it != secondUnitViews_.end(); ++it )
            ( *it )->ActivateSelection( true );
    }
}

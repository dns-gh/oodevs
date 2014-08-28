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
#include "CommunicationTreeView.h"
#include "InhabitantTreeView.h"
#include "LogisticTreeView.h"
#include "ModelBuilder.h"
#include "ObjectTreeView.h"
#include "PopulationTreeView.h"
#include "PreparationProfile.h"
#include "TacticalTreeView.h"
#include "UrbanTreeView.h"
#include "clients_gui/AggregateToolbar.h"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_gui/DrawingsTreeView.h"
#include "clients_gui/EntityTreeView.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_gui/RenameInterface.h"
#include "clients_gui/RichView.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Pathfind_ABC.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "preparation/FormationModel.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"


namespace
{
    template< typename View >
    void Configure( View* richView,
                    std::vector< gui::RichView_ABC* >& treeViews,
                    const gui::AggregateToolbar& aggregateToolbar,
                    gui::RenameInterface& renameInterface,
                    int readOnlyMode = -1 )
    {
        auto view = richView->GetView();
        if( !view )
            return;
        if( readOnlyMode != -1 )
            view->SetReadOnlyModes( readOnlyMode );
        QObject::connect( &aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), view, SLOT( LockDragAndDrop( bool ) ) );
        treeViews.push_back( richView );
        renameInterface.AddEntityTreeView( view );
    }

    void CreateUnitTabWidget( gui::RichWidget< QTabWidget >* parent,
                              gui::RichWidget< QTabWidget >* tabWidget,
                              kernel::Controllers& controllers,
                              gui::EntitySymbols& icons,
                              ModelBuilder& modelBuilder,
                              Model& model,
                              StaticModel& staticModel,
                              std::vector< gui::RichView_ABC* >& treeViews,
                              const gui::AggregateToolbar& aggregateToolbar,
                              gui::ChangeSuperiorDialog& superiorDialog,
                              gui::RenameInterface& renameInterface,
                              std::vector< gui::HierarchyTreeView_ABC* >& views )
    {
        gui::SubObjectName subObject( "UnitTabWidget" );
        // Tactical
        {
            auto richView = new gui::RichView< TacticalTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                                   "TacticalTreeView",
                                                                   tabWidget,
                                                                   controllers,
                                                                   PreparationProfile::GetProfile(),
                                                                   modelBuilder,
                                                                   icons,
                                                                   model,
                                                                   staticModel.types_,
                                                                   superiorDialog );
            views.push_back( static_cast< gui::HierarchyTreeView_ABC* >( richView->GetView() ) );
            QObject::connect( views.back(), SIGNAL( TreeViewFocusIn( gui::HierarchyTreeView_ABC* ) ),
                              tabWidget,    SLOT( FocusIn( gui::HierarchyTreeView_ABC* ) ) );
            Configure( richView, treeViews, aggregateToolbar, renameInterface, eModes_Terrain );
            parent->addTab( richView, tools::translate( "DockContainer","Tactical" ) );
        }
        // Communication
        {
            auto richView = new gui::RichView< CommunicationTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                                        "CommunicationTreeView",
                                                                        tabWidget,
                                                                        controllers,
                                                                        PreparationProfile::GetProfile(),
                                                                        modelBuilder,
                                                                        icons,
                                                                        superiorDialog );
            views.push_back( static_cast< gui::HierarchyTreeView_ABC* >( richView->GetView() ) );
            QObject::connect( views.back(), SIGNAL( TreeViewFocusIn( gui::HierarchyTreeView_ABC* ) ),
                              tabWidget,    SLOT( FocusIn( gui::HierarchyTreeView_ABC* ) ) );
            Configure( richView, treeViews, aggregateToolbar, renameInterface, eModes_Terrain );
            parent->addTab( richView, tools::translate( "DockContainer","Communication" ) );
        }
        // Logistic
        {
            auto richView = new gui::RichView< LogisticTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                                   "LogisticTreeView",
                                                                   tabWidget,
                                                                   controllers,
                                                                   PreparationProfile::GetProfile(),
                                                                   modelBuilder,
                                                                   icons,
                                                                   superiorDialog );
            views.push_back( static_cast< gui::HierarchyTreeView_ABC* >( richView->GetView() ) );
            QObject::connect( views.back(), SIGNAL( TreeViewFocusIn( gui::HierarchyTreeView_ABC* ) ), tabWidget, SLOT( FocusIn( gui::HierarchyTreeView_ABC* ) ) );
            Configure( richView, treeViews, aggregateToolbar, renameInterface, eModes_Terrain );
            parent->addTab( richView, tools::translate( "DockContainer", "Logistic" ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel constructor
// Created: LGY 2012-06-26
// -----------------------------------------------------------------------------
TreeViewsPanel::TreeViewsPanel( kernel::Controllers& controllers,
                                gui::EntitySymbols& icons,
                                ModelBuilder& modelBuilder,
                                Model& model,
                                std::vector< gui::RichView_ABC* >& treeViews,
                                gui::SymbolIcons& symbols,
                                StaticModel& staticModel,
                                const gui::AggregateToolbar& aggregateToolbar,
                                gui::ParametersLayer& paramLayer )
    : gui::RichWidget< QTabWidget >( "TreeViewsPanel" )
    , changeSuperiorDialog_( new gui::ChangeSuperiorDialog( controllers, icons, this ) )
    , renameInterface_( new gui::RenameInterface( controllers, this ) )
{
    // Tactical
    gui::SubObjectName subObject( "TreeViewsPanel" );
    {
        gui::RichWidget< QTabWidget >* pFirstAgentsTabWidget = new gui::RichWidget< QTabWidget >( "FirstAgentsTabWidget" );
        {
            gui::SubObjectName subObject( "pFirstAgentsTabWidget" );
            CreateUnitTabWidget( pFirstAgentsTabWidget, this, controllers, icons, modelBuilder, model, staticModel, treeViews,
                                 aggregateToolbar, *changeSuperiorDialog_, *renameInterface_, firstUnitViews_ );
        }
        pSecondAgentsTabWidget_ = new gui::RichWidget< QTabWidget >( "SecondAgentsTabWidget" );
        {
            gui::SubObjectName subObject( "pSecondAgentsTabWidget_" );
            pSecondAgentsTabWidget_->hide();
            CreateUnitTabWidget( pSecondAgentsTabWidget_, this, controllers, icons, modelBuilder, model, staticModel, treeViews,
                                 aggregateToolbar, *changeSuperiorDialog_, *renameInterface_, secondUnitViews_ );
        }

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
        auto richView = new gui::RichView< ObjectTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                             "ObjectTreeView",
                                                             this,
                                                             controllers,
                                                             PreparationProfile::GetProfile(),
                                                             modelBuilder );
        Configure( richView, treeViews, aggregateToolbar, *renameInterface_, eModes_Terrain );
        addTab( richView, tools::translate( "DockContainer","Objects" ) );
    }
    // Urban
    {
        auto richView = new gui::RichView< UrbanTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit | gui::RichView_ABC::eOptions_FilterBox | gui::RichView_ABC::eOptions_ClearButton,
                                                            "UrbanTreeView",
                                                            this,
                                                            controllers,
                                                            PreparationProfile::GetProfile(),
                                                            modelBuilder,
                                                            symbols,
                                                            staticModel );
        Configure( richView, treeViews, aggregateToolbar, *renameInterface_ );
        addTab( richView, tools::translate( "DockContainer","Urban" ) );
    }
    // Crowds
    {
        auto richView = new gui::RichView< PopulationTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                                 "PopulationTreeView",
                                                                 this, controllers,
                                                                 PreparationProfile::GetProfile(),
                                                                 modelBuilder );
        Configure( richView, treeViews, aggregateToolbar, *renameInterface_, eModes_Terrain );
        addTab( richView, tools::translate( "DockContainer","Crowds" ) );
    }
    // Populations
    {
        auto richView = new gui::RichView< InhabitantTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                                 "InhabitantTreeView",
                                                                 this,
                                                                 controllers,
                                                                 PreparationProfile::GetProfile(),
                                                                 modelBuilder );
        Configure( richView, treeViews, aggregateToolbar, *renameInterface_, eModes_Terrain );
        addTab( richView, tools::translate( "DockContainer","Populations" ) );
    }
    // Drawings
    {
        auto richView = new gui::RichView< gui::DrawingsTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                                    "DrawingsTreeView",
                                                                    this,
                                                                    controllers,
                                                                    PreparationProfile::GetProfile(),
                                                                    modelBuilder,
                                                                    paramLayer );
        Configure( richView, treeViews, aggregateToolbar, *renameInterface_, eModes_Terrain );
        addTab( richView, tools::translate( "DockContainer","Drawings" ) );
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

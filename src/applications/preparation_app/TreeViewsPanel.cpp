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
#include "clients_gui/DrawingsTreeView.h"
#include "clients_gui/EntityTreeView.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_gui/RichView.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
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
                    int readOnlyMode = -1 )
    {
        auto view = richView->GetView();
        if( !view )
            return;
        if( readOnlyMode != -1 )
            view->SetReadOnlyModes( readOnlyMode );
        QObject::connect( &aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), view, SLOT( LockDragAndDrop( bool ) ) );
        treeViews.push_back( richView );
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
    , controllers_      ( controllers )
    , contextMenuEntity_( controllers )
    , modelBuilder_     ( modelBuilder )
{
    gui::SubObjectName subObject( "TreeViewsPanel" );
    {
        gui::RichWidget< QTabWidget >* pFirstAgentsTabWidget = new gui::RichWidget< QTabWidget >( "FirstAgentsTabWidget" );
        {
            gui::SubObjectName subObject( "pFirstAgentsTabWidget" );
            CreateUnitTabWidget( pFirstAgentsTabWidget, this, controllers, icons, modelBuilder, model, staticModel, treeViews, aggregateToolbar, true );
        }
        pSecondAgentsTabWidget_ = new gui::RichWidget< QTabWidget >( "SecondAgentsTabWidget" );
        {
            gui::SubObjectName subObject( "pSecondAgentsTabWidget_" );
            pSecondAgentsTabWidget_->hide();
            CreateUnitTabWidget( pSecondAgentsTabWidget_, this, controllers, icons, modelBuilder, model, staticModel, treeViews, aggregateToolbar, false );
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
        Configure( richView, treeViews, aggregateToolbar, eModes_Terrain );
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
        Configure( richView, treeViews, aggregateToolbar );
        addTab( richView, tools::translate( "DockContainer","Urban" ) );
    }
    // Crowds
    {
        auto richView = new gui::RichView< PopulationTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                                 "PopulationTreeView",
                                                                 this, controllers,
                                                                 PreparationProfile::GetProfile(),
                                                                 modelBuilder );
        Configure( richView, treeViews, aggregateToolbar, eModes_Terrain );
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
        Configure( richView, treeViews, aggregateToolbar, eModes_Terrain );
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
        Configure( richView, treeViews, aggregateToolbar, eModes_Terrain );
        addTab( richView, tools::translate( "DockContainer","Drawings" ) );
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel destructor
// Created: LGY 2012-06-26
// -----------------------------------------------------------------------------
TreeViewsPanel::~TreeViewsPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::CreateUnitTabWidget
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void TreeViewsPanel::CreateUnitTabWidget( gui::RichWidget< QTabWidget >* parent,
                                          gui::RichWidget< QTabWidget >* tabWidget,
                                          kernel::Controllers& controllers,
                                          gui::EntitySymbols& icons,
                                          ModelBuilder& modelBuilder,
                                          Model& model,
                                          StaticModel& staticModel,
                                          std::vector< gui::RichView_ABC* >& treeViews,
                                          const gui::AggregateToolbar& aggregateToolbar,
                                          bool first )
{
    gui::SubObjectName subObject( "UnitTabWidget" );
    std::vector< gui::HierarchyTreeView_ABC* >& trees = first ? firstUnitViews_ : secondUnitViews_;
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
                                                               staticModel.types_ );
        trees.push_back( static_cast< gui::HierarchyTreeView_ABC* >( richView->GetView() ) );
        connect( trees.back(), SIGNAL( TreeViewFocusIn( gui::HierarchyTreeView_ABC* ) ), SLOT( FocusIn( gui::HierarchyTreeView_ABC* ) ) );
        Configure( richView, treeViews, aggregateToolbar, eModes_Terrain );
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
                                                                    icons );
        trees.push_back( static_cast< gui::HierarchyTreeView_ABC* >( richView->GetView() ) );
        connect( trees.back(), SIGNAL( TreeViewFocusIn( gui::HierarchyTreeView_ABC* ) ), SLOT( FocusIn( gui::HierarchyTreeView_ABC* ) ) );
        Configure( richView, treeViews, aggregateToolbar, eModes_Terrain );
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
                                                               icons );
        trees.push_back( static_cast< gui::HierarchyTreeView_ABC* >( richView->GetView() ) );
        connect( trees.back(), SIGNAL( TreeViewFocusIn( gui::HierarchyTreeView_ABC* ) ), SLOT( FocusIn( gui::HierarchyTreeView_ABC* ) ) );
        Configure( richView, treeViews, aggregateToolbar, eModes_Terrain );
        parent->addTab( richView, tools::translate( "DockContainer", "Logistic" ) );
    }
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


// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::Rename
// Created: LGY 2014-05-26
// -----------------------------------------------------------------------------
void TreeViewsPanel::Rename( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    contextMenuEntity_ = &entity;
    menu.InsertItem( "Command", tr( "Rename" ), this, SLOT( OnRename() ), false, 4 );
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::NotifyContextMenu
// Created: LGY 2014-05-26
// -----------------------------------------------------------------------------
void TreeViewsPanel::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    Rename( agent, menu );
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::NotifyContextMenu
// Created: LGY 2014-05-26
// -----------------------------------------------------------------------------
void TreeViewsPanel::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    Rename( automat, menu );
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::NotifyContextMenu
// Created: LGY 2014-05-26
// -----------------------------------------------------------------------------
void TreeViewsPanel::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    Rename( formation, menu );
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::NotifyContextMenu
// Created: LGY 2014-05-26
// -----------------------------------------------------------------------------
void TreeViewsPanel::NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu )
{
    Rename( team, menu );
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::NotifyContextMenu
// Created: LGY 2014-05-26
// -----------------------------------------------------------------------------
void TreeViewsPanel::NotifyContextMenu( const kernel::Ghost_ABC& ghost, kernel::ContextMenu& menu )
{
    Rename( ghost, menu );
}

namespace
{
    bool Rename( kernel::Entity_ABC& entity, std::vector< gui::HierarchyTreeView_ABC* >& views )
    {
        for( auto it = views.begin(); it != views.end(); ++it )
            if( (*it)->isVisible() && (*it)->IsActivated() && (*it)->Exist( entity ) )
            {
                (*it)->Rename( entity );
                return true;
            }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TreeViewsPanel::OnRename
// Created: LGY 2014-05-26
// -----------------------------------------------------------------------------
void TreeViewsPanel::OnRename()
{
    if( contextMenuEntity_ )
    {
        kernel::Entity_ABC& entity = *contextMenuEntity_.ConstCast();
        if( !::Rename( entity, firstUnitViews_ ) )
            if( !::Rename( entity, secondUnitViews_ ) )
                gui::longname::ShowRenameDialog( this, entity, modelBuilder_ );
    }
}

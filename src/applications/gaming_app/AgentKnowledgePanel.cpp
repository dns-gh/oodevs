// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentKnowledgePanel.h"
#include "moc_AgentKnowledgePanel.cpp"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "gaming/AgentKnowledges.h"
#include "gaming/PerceptionMap.h"
#include "gaming/Tools.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/GroupDisplayer.h"
#include "clients_gui/LabelDisplayer.h"
#include "clients_gui/ListDisplayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel constructor
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
AgentKnowledgePanel::AgentKnowledgePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tools::translate( "AgentKnowledgePanel", "Unit knowledge" ) )
    , controllers_ ( controllers )
    , selected_    ( controllers )
    , subSelected_ ( controllers )
    , selectionCandidate_( controllers )
    , display_     ( 0 )
{
    pKnowledgeListView_ = new ListDisplayer< AgentKnowledgePanel >( this, *this, factory );
    pKnowledgeListView_->AddColumn( tools::translate( "AgentKnowledgePanel", "Known units" ) );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tools::translate( "AgentKnowledgePanel", "Details" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Identifier:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Associated agent:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Location:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Heading:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Speed:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Operational state:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Perception level:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Max perception level:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Side:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Level:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Nature:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Surrender:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Prisoner:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Refugees picked up:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Command post:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Relevance:" ) )
                .AddLabel( tools::translate( "AgentKnowledgePanel",  "Critical intelligence:" ) );

    pPerceptionListView_ = new ListDisplayer< AgentKnowledgePanel >( this, *this, factory );
    pPerceptionListView_->AddColumn( tools::translate( "AgentKnowledgePanel", "Unit" ) ).
                          AddColumn( tools::translate( "AgentKnowledgePanel", "Perception level" ) );

    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );
    connect( pKnowledgeListView_, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter( QListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter( QListViewItem* ) ) );

    connect( pPerceptionListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );
    connect( pPerceptionListView_, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter( QListViewItem* ) ) );
    connect( pPerceptionListView_, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter( QListViewItem* ) ) );
    controllers_.Register( *this );
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel destructor
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
AgentKnowledgePanel::~AgentKnowledgePanel()
{
    controllers_.Unregister( *this );
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::showEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::showEvent( QShowEvent* )
{
    if( selected_ )
        NotifyUpdated( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::NotifyUpdated( const AgentKnowledges& knowledges )
{
    if( ! IsVisible() || selected_ != &knowledges )
        return;

    pKnowledgeListView_->DeleteTail(
        pKnowledgeListView_->DisplayList( knowledges.CreateIterator() )
        );
    ValuedListItem* item = FindItem( subSelected_, pKnowledgeListView_->firstChild() );
    if( item )
        pKnowledgeListView_->setSelected( item, true );
    else
    {
        subSelected_ = 0;
        pKnowledgeListView_->setSelected( pKnowledgeListView_->firstChild(), true );
        OnSelectionChanged( pKnowledgeListView_->firstChild() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Display( const AgentKnowledge_ABC& k, Displayer_ABC& displayer, ValuedListItem* item )
{
    item->SetValue( &k );
    displayer.Display( tools::translate( "AgentKnowledgePanel", "Known units" ), k.GetEntity() );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::BeforeSelection
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::BeforeSelection()
{
    selectionCandidate_ = 0;
    KnowledgeGroupSelectionObserver::BeforeSelection();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Select
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Select( const AgentKnowledge_ABC& k )
{
    selectionCandidate_ = &k;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::AfterSelection
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::AfterSelection()
{
    if( selectionCandidate_ )
    {
        const Entity_ABC* owner = & selectionCandidate_->GetOwner();
        const KnowledgeGroup_ABC* kg = static_cast< const KnowledgeGroup_ABC* >( owner );
        Select( kg );
        ValuedListItem* item = FindItem( (const AgentKnowledge_ABC*)selectionCandidate_, pKnowledgeListView_->firstChild() );
        if( item )
        {
            pKnowledgeListView_->setSelected( item, true );
            pKnowledgeListView_->ensureItemVisible( item );
        }
    }
    else
        KnowledgeGroupSelectionObserver::AfterSelection();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Select
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Select( const KnowledgeGroup_ABC* element )
{
    const AgentKnowledges* k     = element ? element->Retrieve< AgentKnowledges >() : 0;
    if( ! k || k != selected_ )
    {
        selected_ = k;
        if( selected_ ) {
            subSelected_ = 0;
            Show();
            NotifyUpdated( *selected_ );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnSelectionChanged
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnSelectionChanged( QListViewItem* i )
{
    ValuedListItem* item = (ValuedListItem*)( i );
    if( ! item || ! item->IsA< const AgentKnowledge_ABC >() )
        subSelected_ = 0;
    else
        subSelected_ = item->GetValue< const AgentKnowledge_ABC >();

    if( subSelected_ )
    {
        NotifyUpdated( *subSelected_ );
        const PerceptionMap* perceptions = subSelected_->Retrieve< PerceptionMap >();
        if( perceptions )
            NotifyUpdated( *perceptions  );
    }
    else
        display_->Hide();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnContextMenuRequested
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnContextMenuRequested( QListViewItem* i, const QPoint& pos )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->ContextMenu( controllers_.actions_, pos );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnRequestCenter
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnRequestCenter( QListViewItem* i )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->Activate( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::NotifyUpdated( const AgentKnowledge_ABC& k )
{
    if( ! IsVisible() || subSelected_ != & k )
        return;

    k.Display( display_->Group( tools::translate( "AgentKnowledgePanel", "Details" ) ) );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::NotifyUpdated( const PerceptionMap& perceptions )
{
    if( ! IsVisible() || ! subSelected_ || subSelected_->Retrieve< PerceptionMap >() != & perceptions )
        return;

    pPerceptionListView_->DeleteTail(
        pPerceptionListView_->DisplayList( perceptions.perceptions_.begin(), perceptions.perceptions_.end() )
        );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Display( const Perception& perception, Displayer_ABC& displayer, ValuedListItem* item )
{
    item->SetValue( perception.detected_ );
    displayer.Display( tools::translate( "AgentKnowledgePanel", "Unit" ), perception.detected_->GetName() ); // to prevent link
    displayer.Display( tools::translate( "AgentKnowledgePanel", "Perception level" ), perception.level_ );
}

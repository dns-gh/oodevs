// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentKnowledgePanel.h"
#include "moc_AgentKnowledgePanel.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "gaming/AgentKnowledges.h"
#include "gaming/PerceptionMap.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/ListDisplayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel constructor
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
AgentKnowledgePanel::AgentKnowledgePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tools::translate( "AgentKnowledge", "Unit knowledge" ) )
    , controllers_ ( controllers )
    , selected_    ( controllers )
    , subSelected_ ( controllers )
    , selectionCandidate_( controllers )
    , display_     ( 0 )
{
    QVBoxLayout* layout = new QVBoxLayout( this );
    pKnowledgeListView_ = new ListDisplayer< AgentKnowledgePanel >( this, *this, factory );
    pKnowledgeListView_->AddColumn( tools::translate( "AgentKnowledge", "Known units" ) );
    layout->addWidget( pKnowledgeListView_ );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tools::translate( "AgentKnowledge", "Details" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Identifier:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Associated agent:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Location:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Heading:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Speed:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Operational state:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Perception level:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Max perception level:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Side:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Level:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Nature:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Surrender:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Prisoner:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Refugees picked up:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Command post:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Relevance:" ) )
                .AddLabel( tools::findTranslation( "AgentKnowledge",  "Critical intelligence:" ) );
    
    pPerceptionListView_ = new ListDisplayer< AgentKnowledgePanel >( this, *this, factory );
    pPerceptionListView_->AddColumn( tools::translate( "AgentKnowledge", "Unit" ) ).
                          AddColumn( tools::translate( "AgentKnowledge", "Perception level" ) );
    layout->addWidget( pPerceptionListView_ );

    connect( pKnowledgeListView_, SIGNAL( selectionChanged( Q3ListViewItem* ) ), this, SLOT( OnSelectionChanged( Q3ListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( Q3ListViewItem*, const QPoint& ) ) );
    connect( pKnowledgeListView_, SIGNAL( doubleClicked       ( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter( Q3ListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( spacePressed        ( Q3ListViewItem* ) ),                     this, SLOT( OnRequestCenter( Q3ListViewItem* ) ) );

    connect( pPerceptionListView_, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( Q3ListViewItem*, const QPoint& ) ) );
    connect( pPerceptionListView_, SIGNAL( doubleClicked       ( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter( Q3ListViewItem* ) ) );
    connect( pPerceptionListView_, SIGNAL( spacePressed        ( Q3ListViewItem* ) ),                     this, SLOT( OnRequestCenter( Q3ListViewItem* ) ) );
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
    displayer.Display( tools::translate( "AgentKnowledge", "Known units" ), k.GetEntity() );
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
void AgentKnowledgePanel::OnSelectionChanged( Q3ListViewItem* i )
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
void AgentKnowledgePanel::OnContextMenuRequested( Q3ListViewItem* i, const QPoint& pos )
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
void AgentKnowledgePanel::OnRequestCenter( Q3ListViewItem* i )
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

    k.Display( display_->Group( tools::translate( "AgentKnowledge", "Details" ) ) );
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
    displayer.Display( tools::translate( "AgentKnowledge", "Unit" ), perception.detected_->GetName() ); // to prevent link
    displayer.Display( tools::translate( "AgentKnowledge", "Perception level" ), perception.level_ );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UrbanKnowledgePanel.h"
#include "moc_UrbanKnowledgePanel.cpp"

#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/UrbanKnowledge_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/ListDisplayer.h"
#include "gaming/UrbanKnowledges.h"
#include "gaming/UrbanPerceptions.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel constructor
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
UrbanKnowledgePanel::UrbanKnowledgePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tr( "Urban knowledges" ) )
    , controllers_ ( controllers )
    , selected_    ( controllers )
    , subSelected_ ( controllers )
{
    pKnowledgeListView_ = new ListDisplayer< UrbanKnowledgePanel >( this, *this, factory );
    pKnowledgeListView_->AddColumn( tools::translate( "Urban", "Known blocks" ) );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tr( "Details" ) )
                .AddLabel( tr( "Identifier:" ) )
                .AddLabel( tr( "Associated block:" ) )
                .AddLabel( tr( "Perceived:" ) )
                .AddLabel( tr( "Progress:" ) )
                .AddLabel( tr( "Maximum Progress:" ) );

    pPerceptionListView_ = new ListDisplayer< UrbanKnowledgePanel >( this, *this, factory );
    pPerceptionListView_->AddColumn( tr( "Agent" ) );

    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel destructor
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
UrbanKnowledgePanel::~UrbanKnowledgePanel()
{
    controllers_.Unregister( *this );
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel::showEvent
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledgePanel::showEvent( QShowEvent* )
{
    if( selected_ )
        NotifyUpdated( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel::NotifyUpdated
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledgePanel::NotifyUpdated( const UrbanKnowledges& element )
{
    if( ! IsVisible() || selected_ != &element )
        return;

    pKnowledgeListView_->DeleteTail(
      pKnowledgeListView_->DisplayList( element.CreateIterator() )
    );

    ValuedListItem* item = FindItem( subSelected_, pKnowledgeListView_->firstChild() );
    if( item )
    {
        pKnowledgeListView_->setSelected( item, true );
        NotifyUpdated( *subSelected_ );
    }
    else
    {
        subSelected_ = 0;
        pKnowledgeListView_->setSelected( pKnowledgeListView_->firstChild(), true );
        OnSelectionChanged( pKnowledgeListView_->firstChild() );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel::NotifyUpdated
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledgePanel::NotifyUpdated( const UrbanPerceptions& element )
{
     if( ! IsVisible() || ! subSelected_ || subSelected_->Retrieve< UrbanPerceptions >() != &element )
         return;
     pPerceptionListView_->DeleteTail(
         pPerceptionListView_->DisplayList( element.detectingAutomats_.begin(), element.detectingAutomats_.end() )
     );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel::NotifyUpdated
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledgePanel::NotifyUpdated( const kernel::UrbanKnowledge_ABC& element )
{
    if( ! IsVisible() || &element != subSelected_ )
        return;
    display_->Hide();
    element.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel::Display
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledgePanel::Display( const kernel::UrbanKnowledge_ABC& k, Displayer_ABC& displayer, ValuedListItem* item )
{
    item->SetValue( &k );
    k.DisplayInList( displayer );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel::Display
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledgePanel::Display( const Automat_ABC* agent, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( agent );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel::Select
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledgePanel::Select( const Team_ABC* team )
{
    const UrbanKnowledges* k = team ? team->Retrieve< UrbanKnowledges >() : 0;
    if( ! k || k != selected_ )
    {
        selected_ = k;
        if( selected_ )
        {
            subSelected_ = 0;
            Show();
            NotifyUpdated( *selected_ );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel::Select
// Created: LDC 2010-04-15
// -----------------------------------------------------------------------------
void UrbanKnowledgePanel::Select( const kernel::KnowledgeGroup_ABC* group )
{
    const Hierarchies* hierarchies = group ? group->Retrieve< kernel::CommunicationHierarchies >() : 0;
    if( hierarchies )
        Select( static_cast< const Team_ABC* >( & hierarchies->GetTop() ) );
    else
        Select( ( const Team_ABC* )0 );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel::OnSelectionChanged
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledgePanel::OnSelectionChanged( QListViewItem* i )
{
    ValuedListItem* item = (ValuedListItem*)( i );
    if( !item || !item->IsA< const UrbanKnowledge_ABC >() )
        subSelected_ = 0;
    else
        subSelected_ = item->GetValue< const UrbanKnowledge_ABC >();

    if( subSelected_ )
    {
        NotifyUpdated( *subSelected_ );
        const UrbanPerceptions* perceptions = subSelected_->Retrieve< UrbanPerceptions >();
        if( perceptions )
            NotifyUpdated( *perceptions );
    }
    else
        display_->Hide();
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel::OnContextMenuRequested
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledgePanel::OnContextMenuRequested( QListViewItem* i, const QPoint& pos )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->ContextMenu( controllers_.actions_, pos );
    }
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationKnowledgePanel.h"
#include "moc_PopulationKnowledgePanel.cpp"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "gaming/PopulationKnowledges.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "gaming/PopulationKnowledge.h"
#include "gaming/PopulationConcentrationKnowledge.h"
#include "gaming/PopulationFlowKnowledge.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/GroupDisplayer.h"
#include "clients_gui/LabelDisplayer.h"
#include "clients_gui/ListDisplayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel constructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
PopulationKnowledgePanel::PopulationKnowledgePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tr( "Population knowledges" ) )
    , controllers_ ( controllers )
    , selected_    ( controllers )
    , subSelected_ ( controllers )
    , selectedPart_( controllers )
{
    knowledgeList_ = new ListDisplayer< PopulationKnowledgePanel >( this, *this, factory );
    knowledgeList_->AddColumn( tr( "Known populations" ) );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tr( "Details" ) )
                .AddLabel( tr( "Identifier:" ) )
                .AddLabel( tr( "Associated population:" ) )
                .AddLabel( tr( "Side:" ) );

    display_->AddGroup( tr( "Concentration" ) )
                .AddLabel( tr( "Alive people:" ) )
                .AddLabel( tr( "Dead people:" ) )
                .AddLabel( tr( "Mood:" ) )
                .AddLabel( tr( "Perceived:" ) )
                .AddLabel( tr( "Relevance:" ) );

    display_->AddGroup( tr( "Flow" ) )
                .AddLabel( tr( "Heading:" ) )
                .AddLabel( tr( "Speed:" ) )
                .AddLabel( tr( "Alive people:" ) )
                .AddLabel( tr( "Dead people:" ) )
                .AddLabel( tr( "Mood:" ) )
                .AddLabel( tr( "Perceived:" ) );

    connect( knowledgeList_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( knowledgeList_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );

    controllers_.Register( *this );
}

// $$$$ AGE 2006-02-27: ajouter
//    pFlowPartBox_ = new QGroupBox( 1, Qt::Horizontal, tr( "Portions de flux" ), this );
//    pFlowPartTable_ = new QTable( 0, 2, pFlowPartBox_ );
//    pFlowPartTable_->horizontalHeader()->setLabel( 0, tr( "#" ) );
//    pFlowPartTable_->horizontalHeader()->setLabel( 1, tr( "Pertinence" ) );
//    pFlowPartTable_->setColumnWidth   ( 0, 20 );
//    pFlowPartTable_->setColumnReadOnly( 0, true );
//    pFlowPartTable_->setColumnReadOnly( 1, true );
//    pFlowPartTable_->setLeftMargin( 0 );
//    pFlowPartTable_->setShowGrid( false );
//    pFlowPartTable_->setMaximumHeight( 100 );
//    pFlowPartBox_->hide();

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel destructor
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
PopulationKnowledgePanel::~PopulationKnowledgePanel()
{
    controllers_.Unregister( *this );
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::showEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::showEvent( QShowEvent* )
{
    if( selected_ )
        NotifyUpdated( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnContextMenuRequested
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnContextMenuRequested( QListViewItem* i, const QPoint& pos )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->ContextMenu( controllers_.actions_, pos );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnSelectionChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnSelectionChanged( QListViewItem* i )
{
    ValuedListItem* item = (ValuedListItem*)( i );
    display_->Group( tr( "Flow" ) ).Hide();
    display_->Group( tr( "Concentration" ) ).Hide();
    if( item && item->IsA< const PopulationKnowledge_ABC >() )
    {
        subSelected_ = item->GetValue< const PopulationKnowledge_ABC >();
        subSelected_->Display( *display_ );
    }
    else if( item && item->IsA< const PopulationConcentrationKnowledge >() )
    {
        selectedPart_ = item->GetValue< const PopulationConcentrationKnowledge >();
        selectedPart_->Display( *display_ );
    }
    else if( item && item->IsA< const PopulationFlowKnowledge >() )
    {
        selectedPart_ = item->GetValue< const PopulationFlowKnowledge >();
        selectedPart_->Display( *display_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::NotifyUpdated( const PopulationKnowledges& element )
{
    if( ! IsVisible() || selected_ != &element )
        return;
    knowledgeList_->DeleteTail( knowledgeList_->DisplayList( element.CreateIterator() ) );
    selected_ = &element;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::Display
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::Display( const PopulationKnowledge_ABC& knowledge, Displayer_ABC& displayer, ValuedListItem* item )
{
    item->SetValue( &knowledge );
    knowledge.DisplayInList( displayer );
    const PopulationKnowledge& k = dynamic_cast< const PopulationKnowledge& >( knowledge );  // $$$$ AGE 2008-06-19:
    ValuedListItem* subItem = knowledgeList_->DisplayList( k.Resolver< PopulationConcentrationKnowledge >::CreateIterator(), item );
    subItem = knowledgeList_->DisplayList( k.Resolver< PopulationFlowKnowledge >::CreateIterator(), item, subItem );
    knowledgeList_->DeleteTail( subItem );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::Display
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::Display( const PopulationPartKnowledge_ABC& knowledge, Displayer_ABC& displayer, ValuedListItem* )
{
    knowledge.DisplayInList( displayer );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::NotifyUpdated( const PopulationKnowledge_ABC& element )
{
    if( ! IsVisible() || subSelected_ != &element )
        return;
    element.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::NotifyUpdated( const PopulationFlowKnowledge& element )
{
    if( ! IsVisible() || selectedPart_ != &element )
        return;
    display_->Group( tr( "Concentration" ) ).Hide();
    element.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::NotifyUpdated( const PopulationConcentrationKnowledge& element )
{
    if( ! IsVisible() || selectedPart_ != &element )
        return;
    display_->Group( tr( "Flow" ) ).Hide();
    element.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::Select( const KnowledgeGroup_ABC* element )
{
    const PopulationKnowledges* k = element ? element->Retrieve< PopulationKnowledges >() : 0;
    if( ! k || k != selected_ )
    {
        selected_ = k;
        if( selected_ ) {
            Show();
            NotifyUpdated( *selected_ );
        }
        else
            Hide();
    }
}

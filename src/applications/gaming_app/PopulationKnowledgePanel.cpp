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
#include "gaming/Tools.h"
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
    : InfoPanel_ABC( parent, panel, tools::translate( "PopulationKnowledgePanel", "Crowd knowledges" ) )
    , controllers_ ( controllers )
    , selected_    ( controllers )
    , subSelected_ ( controllers )
    , selectedPart_( controllers )
{
    knowledgeList_ = new ListDisplayer< PopulationKnowledgePanel >( this, *this, factory );
    knowledgeList_->AddColumn( tools::translate( "PopulationKnowledgePanel", "Known crowds" ) );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tools::translate( "PopulationKnowledgePanel", "Details" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Identifier:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Associated crowd:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Side:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Critical intelligence:" ) );

    display_->AddGroup( tools::translate( "PopulationKnowledgePanel", "Concentration" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Alive people:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Dead people:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Mood:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Perceived:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Relevance:" ) );

    display_->AddGroup( tools::translate( "PopulationKnowledgePanel", "Flow" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Heading:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Speed:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Alive people:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Dead people:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Mood:" ) )
                .AddLabel( tools::translate( "PopulationKnowledgePanel", "Perceived:" ) );

    connect( knowledgeList_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( knowledgeList_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );

    controllers_.Register( *this );
}

// $$$$ AGE 2006-02-27: ajouter
//    pFlowPartBox_ = new QGroupBox( 1, Qt::Horizontal, tools::translate( "PopulationKnowledgePanel", "Portions de flux" ), this );
//    pFlowPartTable_ = new QTable( 0, 2, pFlowPartBox_ );
//    pFlowPartTable_->horizontalHeader()->setLabel( 0, tools::translate( "PopulationKnowledgePanel", "#" ) );
//    pFlowPartTable_->horizontalHeader()->setLabel( 1, tools::translate( "PopulationKnowledgePanel", "Pertinence" ) );
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
    display_->Group( tools::translate( "PopulationKnowledgePanel", "Flow" ) ).Hide();
    display_->Group( tools::translate( "PopulationKnowledgePanel", "Concentration" ) ).Hide();
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
    display_->Group( tools::translate( "PopulationKnowledgePanel", "Concentration" ) ).Hide();
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
    display_->Group( tools::translate( "PopulationKnowledgePanel", "Flow" ) ).Hide();
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

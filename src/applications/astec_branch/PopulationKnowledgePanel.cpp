// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationKnowledgePanel.h"
#include "moc_PopulationKnowledgePanel.cpp"

#include "DisplayBuilder.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include "ListDisplayer.h"
#include "Controllers.h"
#include "KnowledgeGroup.h"
#include "PopulationKnowledges.h"
#include "PopulationKnowledge.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlowKnowledge.h"

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel constructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
PopulationKnowledgePanel::PopulationKnowledgePanel( InfoPanels* pParent, Controllers& controllers )
    : InfoPanel_ABC( pParent, "C. popu" )
    , controllers_ ( controllers )
    , owner_       ( 0 )
    , selected_    ( 0 )
    , subSelected_ ( 0 )
    , selectedPart_( 0 )
{
    knowledgeList_ = new ListDisplayer< PopulationKnowledgePanel >( this, *this );
    knowledgeList_->AddColumn( "Populations connues" );

    pOwnTeamCheckBox_ = new QCheckBox( tr( "Afficher propre camp" ), this );
    pOwnTeamCheckBox_->setChecked( true );

    display_ = new DisplayBuilder( this );
    display_->AddGroup( "Détails" )
                .AddLabel( "Id:" )
                .AddLabel( "Population associée:" )
                .AddLabel( "Camp:" );

    display_->AddGroup( "Concentration" )
                .AddLabel( "Id:" )
                .AddLabel( "Concentration associée:" )
                .AddLabel( "Position:" )
                .AddLabel( "Humains vivants:" )
                .AddLabel( "Humains morts:" )
                .AddLabel( "Attitude:" )
                .AddLabel( "Percue:" )
                .AddLabel( "Pertinence:" );

    display_->AddGroup( "Flux" )
                .AddLabel( "Id:" )
                .AddLabel( "Flux associée:" )
                .AddLabel( "Direction:" )
                .AddLabel( "Vitesse:" )
                .AddLabel( "Humains vivants:" )
                .AddLabel( "Humains morts:" )
                .AddLabel( "Attitude:" )
                .AddLabel( "Percue:" )
                .AddLabel( "Portions connues:" );

    connect( knowledgeList_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( knowledgeList_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );

    connect( pOwnTeamCheckBox_,   SIGNAL( clicked() )                    , this, SLOT( ToggleDisplayOwnTeam() ) );

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
    controllers_.Remove( *this );
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
// Name: PopulationKnowledgePanel::ToggleDisplayOwnTeam
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::ToggleDisplayOwnTeam()
{
    showEvent( 0 );
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
    display_->Group( "Flux" ).Hide();
    display_->Group( "Concentration" ).Hide();
    if( item && item->IsA< const PopulationKnowledge* >() ) {
        subSelected_ = item->GetValue< const PopulationKnowledge* >();
        subSelected_->Display( *display_ );
    }
    else if( item && item->IsA< const PopulationPartKnowledge_ABC* >() ) {
        selectedPart_ = item->GetValue< const PopulationPartKnowledge_ABC* >();
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

    knowledgeList_->DeleteTail( 
        knowledgeList_->DisplayList( element.CreateIterator() )
        );
    ValuedListItem* item = FindItem( subSelected_, knowledgeList_->firstChild() );
    if( item )
        knowledgeList_->setSelected( item, true );
    else
        subSelected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::Display
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::Display( const PopulationKnowledge& knowledge, Displayer_ABC& displayer, ValuedListItem* item )
{
    if( pOwnTeamCheckBox_->isChecked() || ! owner_ || ! knowledge.IsInTeam( *owner_ ) )
    {
        item->SetValue( &knowledge );
        knowledge.DisplayInList( displayer );
        knowledgeList_->DeleteTail( 
            knowledgeList_->DisplayList( knowledge.CreateIterator(), item  )
            );
    }
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
void PopulationKnowledgePanel::NotifyUpdated( const PopulationKnowledge& element )
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
    display_->Group( "Concentration" ).Hide();
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
    display_->Group( "Flux" ).Hide();
    element.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::Select( const KnowledgeGroup* element )
{
    const PopulationKnowledges* k = element ? element->Retrieve< PopulationKnowledges >() : 0;
    owner_ = element ? & element->GetTeam() : 0;
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

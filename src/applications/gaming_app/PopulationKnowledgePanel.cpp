// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationKnowledgePanel.h"
#include "moc_PopulationKnowledgePanel.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/PopulationKnowledges.h"
#include "gaming/PopulationKnowledge.h"
#include "gaming/PopulationConcentrationKnowledge.h"
#include "gaming/PopulationFlowKnowledge.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/GroupDisplayer.h"
#include "clients_gui/LabelDisplayer.h"
#include "clients_gui/ListDisplayer.h"

using namespace kernel;
using namespace gui;

Q_DECLARE_METATYPE( const kernel::PopulationKnowledge_ABC* )
Q_DECLARE_METATYPE( const PopulationFlowKnowledge* )
Q_DECLARE_METATYPE( const PopulationConcentrationKnowledge* )
#define KnowledgeRole ( Qt::UserRole + 1 )

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
    QWidget* view = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout( view );

    knowledgeList_ = new QTreeView();
    knowledgeList_->setContextMenuPolicy( Qt::CustomContextMenu );
    knowledgeList_->setRootIsDecorated( false );
    knowledgeList_->setEditTriggers( 0 );
    knowledgeList_->setModel( &knowledgeModel_ );
    knowledgeModel_.setColumnCount( 1 );
    knowledgeModel_.setHorizontalHeaderLabels( QStringList( tools::translate( "PopulationKnowledgePanel", "Known crowds" ) ) );
    
    layout->addWidget( knowledgeList_ );

    display_ = new DisplayBuilder( view, factory );
    display_->AddGroup( tools::translate( "PopulationKnowledgePanel", "Details" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Identifier:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Associated crowd:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Side:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Critical intelligence:" ) );

    display_->AddGroup( tools::translate( "PopulationKnowledgePanel", "Concentration" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Alive people:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Dead people:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Mood:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Perceived:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Relevance:" ) );

    display_->AddGroup( tools::translate( "PopulationKnowledgePanel", "Flow" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Heading:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Speed:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Alive people:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Dead people:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Mood:" ) )
                .AddLabel( tools::findTranslation( "Crowd", "Perceived:" ) );

    connect( knowledgeList_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelectionChanged() ) );
    connect( knowledgeList_, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenuRequested( const QPoint & ) ) );

    setWidget( view );

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
void PopulationKnowledgePanel::OnContextMenuRequested( const QPoint & pos )
{
    const QModelIndex index = knowledgeList_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = knowledgeModel_.itemFromIndex( index );
    if( item && item->data( KnowledgeRole ).isValid() )
        item->data( KnowledgeRole ).value< const PopulationKnowledge_ABC* >()->ContextMenu( controllers_.actions_, knowledgeList_->viewport()->mapToGlobal( pos ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgePanel::OnSelectionChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationKnowledgePanel::OnSelectionChanged()
{
    const QModelIndex index = knowledgeList_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    display_->Group( tools::translate( "PopulationKnowledgePanel", "Flow" ) ).Hide();
    display_->Group( tools::translate( "PopulationKnowledgePanel", "Concentration" ) ).Hide();
    QStandardItem* item = knowledgeModel_.itemFromIndex( index );

    if( item && item->data( KnowledgeRole ).isValid() )
    {
        if( item->data( KnowledgeRole ).canConvert< const PopulationKnowledge_ABC* >() )
        {
            subSelected_ = item->data( KnowledgeRole ).value< const PopulationKnowledge_ABC* >();
            subSelected_->Activate( controllers_.actions_ );
            subSelected_->Display( *display_ );
        }
        else if( item->data( KnowledgeRole ).canConvert< const PopulationConcentrationKnowledge* >() )
        {
            selectedPart_ = item->data( KnowledgeRole ).value< const PopulationConcentrationKnowledge* >();
            selectedPart_->Display( *display_ );
        }
        else if( item->data( KnowledgeRole ).canConvert< const PopulationFlowKnowledge* >() )
        {
            selectedPart_ = item->data( KnowledgeRole ).value< const PopulationFlowKnowledge* >();
            selectedPart_->Display( *display_ );
        }
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

    //population knowledge
    knowledgeModel_.removeRows( 0, knowledgeModel_.rowCount() );
    tools::Iterator< const kernel::PopulationKnowledge_ABC& > iterator = element.CreateIterator();
    while( iterator.HasMoreElements() )
    {
        //population knowledge infos
        const kernel::PopulationKnowledge_ABC& knowledge = iterator.NextElement();
        QStandardItem* nameItem = new QStandardItem( knowledge.GetEntity()->GetName() + " - "+ QString::number( knowledge.GetEntity()->GetId() ) );
        nameItem->setData( QVariant::fromValue( &knowledge ), KnowledgeRole );
        knowledgeModel_.appendRow( nameItem );
        const PopulationKnowledge& k = dynamic_cast< const PopulationKnowledge& >( knowledge ); 

        //concentration knowledge
        tools::Iterator< const PopulationConcentrationKnowledge& > iteratorConcentration = k.Resolver< PopulationConcentrationKnowledge >::CreateIterator();
        while( iteratorConcentration.HasMoreElements() && k.Resolver< PopulationConcentrationKnowledge >::Count() )
        {
            const PopulationConcentrationKnowledge& concentration = iteratorConcentration.NextElement();
            QStandardItem* concentrationItem = new QStandardItem(  tools::translate( "Crowd", "Concentration - " ) + QString::number( concentration.GetNId() ) );
            concentrationItem->setData( QVariant::fromValue( &concentration ), KnowledgeRole );
            nameItem->appendRow( concentrationItem );
        }

        //flow knowledge
        tools::Iterator< const PopulationFlowKnowledge& > iteratorFlow = k.Resolver< PopulationFlowKnowledge >::CreateIterator();
        while( iteratorFlow.HasMoreElements() && k.Resolver< PopulationFlowKnowledge >::Count() )
        {
            const PopulationFlowKnowledge& flow = iteratorFlow.NextElement();
            QStandardItem* flowItem = new QStandardItem( tools::translate( "Crowd", "Flow - " ) + QString::number( flow.GetNId() ) );
            flowItem->setData( QVariant::fromValue( &flow ), KnowledgeRole );
            nameItem->appendRow( flowItem );
        }
    }
    selected_ = &element;
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

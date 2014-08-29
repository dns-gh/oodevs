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
                .AddLabel( "Identifier", tools::findTranslation( "Crowd", "Identifier:" ) )
                .AddLabel( "Associated", tools::findTranslation( "Crowd", "Associated crowd:" ) )
                .AddLabel( "Side", tools::findTranslation( "Crowd", "Side:" ) )
                .AddLabel( "Critical", tools::findTranslation( "Crowd", "Critical intelligence:" ) );

    display_->AddGroup( tools::translate( "PopulationKnowledgePanel", "Concentration" ) )
                .AddLabel( "Alive", tools::findTranslation( "Crowd", "Alive people:" ) )
                .AddLabel( "Dead", tools::findTranslation( "Crowd", "Dead people:" ) )
                .AddLabel( "Mood", tools::findTranslation( "Crowd", "Mood:" ) )
                .AddLabel( "Perceived", tools::findTranslation( "Crowd", "Perceived:" ) )
                .AddLabel( "Relevance", tools::findTranslation( "Crowd", "Relevance:" ) );

    display_->AddGroup( tools::translate( "PopulationKnowledgePanel", "Flow" ) )
                .AddLabel( "Heading", tools::findTranslation( "Crowd", "Heading:" ) )
                .AddLabel( "Speed", tools::findTranslation( "Crowd", "Speed:" ) )
                .AddLabel( "Alive", tools::findTranslation( "Crowd", "Alive people:" ) )
                .AddLabel( "Dead", tools::findTranslation( "Crowd", "Dead people:" ) )
                .AddLabel( "Mood", tools::findTranslation( "Crowd", "Mood:" ) )
                .AddLabel( "Perceived", tools::findTranslation( "Crowd", "Perceived:" ) );

    connect( knowledgeList_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelectionChanged() ) );
    connect( knowledgeList_, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenuRequested( const QPoint & ) ) );

    setWidget( view );

    controllers_.Register( *this );
}

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
        if( item->data( KnowledgeRole ).canConvert< const PopulationKnowledge_ABC* >() )
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
    ResizeModelOnNewContent( &knowledgeModel_, knowledgeList_->selectionModel(), element.Count(), *display_ );
    int i = 0;
    tools::Iterator< const kernel::PopulationKnowledge_ABC& > iterator = element.CreateIterator();
    while( iterator.HasMoreElements() )
    {
        //population knowledge infos
        const kernel::PopulationKnowledge_ABC& knowledge = iterator.NextElement();
        knowledgeModel_.item( i )->setText( knowledge.GetEntity()? knowledge.GetEntity()->GetName() + " - "+ QString::number( knowledge.GetEntity()->GetId() ) : QString::number( knowledge.GetEntityId() ) );
        knowledgeModel_.item( i )->setData( QVariant::fromValue( &knowledge ), KnowledgeRole );

        const PopulationKnowledge& k = static_cast< const PopulationKnowledge& >( knowledge );
        const tools::Resolver< PopulationConcentrationKnowledge >& concentrations = static_cast< const tools::Resolver< PopulationConcentrationKnowledge >& >( k );
        const tools::Resolver< PopulationFlowKnowledge >& flows = static_cast< const tools::Resolver< PopulationFlowKnowledge >& >( k );

        //concentration and flow count
        int populationSize = concentrations.Count() + flows.Count();
        int itemchildrenSize = knowledgeModel_.item( i )->rowCount();

        bool wasBlocked = knowledgeList_->selectionModel()->blockSignals( true );
        if( itemchildrenSize > populationSize )
            knowledgeModel_.item( i )->removeRows( populationSize, itemchildrenSize - populationSize );
        else if( itemchildrenSize < populationSize )
            for( int size = 0; size < populationSize - itemchildrenSize; ++size )
                knowledgeModel_.item( i )->appendRow( new QStandardItem() );
        knowledgeList_->selectionModel()->blockSignals( wasBlocked );

        int j = 0;
        //concentration knowledge
        tools::Iterator< const PopulationConcentrationKnowledge& > iteratorConcentration = concentrations.CreateIterator();
        while( iteratorConcentration.HasMoreElements()  )
        {
            const PopulationConcentrationKnowledge& concentration = iteratorConcentration.NextElement();
            knowledgeModel_.item( i )->child( j )->setText(  tools::translate( "Crowd", "Concentration - " ) + QString::number( concentration.GetNId() ) );
            knowledgeModel_.item( i )->child( j )->setData( QVariant::fromValue( &concentration ), KnowledgeRole );
            ++j;
        }

        //flow knowledge
        tools::Iterator< const PopulationFlowKnowledge& > iteratorFlow = flows.CreateIterator();
        while( iteratorFlow.HasMoreElements() )
        {
            const PopulationFlowKnowledge& flow = iteratorFlow.NextElement();
            knowledgeModel_.item( i )->child( j )->setText( tools::translate( "Crowd", "Flow - " ) + QString::number( flow.GetNId() ) );
            knowledgeModel_.item( i )->child( j )->setData( QVariant::fromValue( &flow ), KnowledgeRole );
            ++j;
        }
        ++i;
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

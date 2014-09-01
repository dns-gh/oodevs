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
#include "clients_gui/InternalLinks.h"

Q_DECLARE_METATYPE( const kernel::Automat_ABC* )
Q_DECLARE_METATYPE( const kernel::AgentKnowledge_ABC* )
#define EntityRole ( Qt::UserRole + 1 )
#define KnowledgeRole ( Qt::UserRole + 2 )

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
    QWidget* view = new QWidget();
    QVBoxLayout* layoutView = new QVBoxLayout( view );

    pKnowledgeListView_ = new QTreeView();
    pKnowledgeListView_->setContextMenuPolicy( Qt::CustomContextMenu );
    pKnowledgeListView_->setRootIsDecorated( false );
    pKnowledgeListView_->setEditTriggers( 0 );
    knowledgeModel_.setColumnCount( 1 );
    pKnowledgeListView_->setModel( &knowledgeModel_ );
    knowledgeModel_.setHorizontalHeaderLabels( QStringList( tools::translate( "AgentKnowledge", "Known units" ) ) );

    layoutView->addWidget( pKnowledgeListView_ );

    display_ = new DisplayBuilder( view, factory );
    display_->AddGroup( tools::translate( "AgentKnowledge", "Details" ) )
                .AddLabel( "Identifier", tools::findTranslation( "AgentKnowledge",  "Identifier:" ) )
                .AddLabel( "AssociatedAgent", tools::findTranslation( "AgentKnowledge",  "Associated agent:" ) )
                .AddLabel( "Location", tools::findTranslation( "AgentKnowledge",  "Location:" ) )
                .AddLabel( "Heading", tools::findTranslation( "AgentKnowledge",  "Heading:" ) )
                .AddLabel( "Speed", tools::findTranslation( "AgentKnowledge",  "Speed:" ) )
                .AddLabel( "Operational state", tools::findTranslation( "AgentKnowledge",  "Operational state:" ) )
                .AddLabel( "PerceptionLevel", tools::findTranslation( "AgentKnowledge",  "Perception level:" ) )
                .AddLabel( "MaxPerceptionLevel", tools::findTranslation( "AgentKnowledge",  "Max perception level:" ) )
                .AddLabel( "Side", tools::findTranslation( "AgentKnowledge",  "Side:" ) )
                .AddLabel( "Level", tools::findTranslation( "AgentKnowledge",  "Level:" ) )
                .AddLabel( "Nature", tools::findTranslation( "AgentKnowledge",  "Nature:" ) )
                .AddLabel( "Surrender", tools::findTranslation( "AgentKnowledge",  "Surrender:" ) )
                .AddLabel( "Prisoner", tools::findTranslation( "AgentKnowledge",  "Prisoner:" ) )
                .AddLabel( "RefugeesPickedUp", tools::findTranslation( "AgentKnowledge",  "Refugees picked up:" ) )
                .AddLabel( "CommandPost", tools::findTranslation( "AgentKnowledge",  "Command post:" ) )
                .AddLabel( "Relevance", tools::findTranslation( "AgentKnowledge",  "Relevance:" ) )
                .AddLabel( "CriticalIntelligence", tools::findTranslation( "AgentKnowledge",  "Critical intelligence:" ) );

    //perception tree view configuration
    pPerceptionListView_ = new QTreeView();
    pPerceptionListView_->setContextMenuPolicy( Qt::CustomContextMenu );
    pPerceptionListView_->setRootIsDecorated( false );
    pPerceptionListView_->setEditTriggers( 0 );
    perceptionModel_.setColumnCount( 2 );
    pPerceptionListView_->setModel( &perceptionModel_ );
    pPerceptionListView_->header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    QStringList list;
    list.append( tools::translate( "AgentKnowledge", "Unit" ) );
    list.append( tools::translate( "AgentKnowledge", "Perception level" ) );
    perceptionModel_.setHorizontalHeaderLabels( list );

    layoutView->addWidget( pPerceptionListView_ );

    connect( pKnowledgeListView_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelectionChanged() ) );
    connect( pKnowledgeListView_, SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( OnKnowledgeContextMenuEvent( const QPoint& ) ) );
    connect( pKnowledgeListView_, SIGNAL( doubleClicked( const QModelIndex& ) ), SLOT( OnKnowledgeRequestCenter() ) );

    connect( pPerceptionListView_, SIGNAL( doubleClicked( const QModelIndex& ) ), SLOT( OnPerceptionRequestCenter() ) );
    connect( pPerceptionListView_, SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( OnPerceptionContextMenuEvent( const QPoint& ) ) );

    setWidget( view );

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

    ResizeModelOnNewContent( &knowledgeModel_, pKnowledgeListView_->selectionModel(), knowledges.Count(), *display_ );
    int i = 0;
    tools::Iterator< const AgentKnowledge_ABC& > iterator = knowledges.CreateIterator();
    while( iterator.HasMoreElements() )
    {
        const AgentKnowledge_ABC& knowledge = iterator.NextElement();
        knowledgeModel_.item( i )->setText( knowledge.GetName() );
        knowledgeModel_.item( i )->setData( QVariant::fromValue( &knowledge ), KnowledgeRole );
        ++i;
    }
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
// Name: AgentKnowledgePanel::OnSelectionChanged
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnSelectionChanged()
{
    const QModelIndex index = pKnowledgeListView_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = knowledgeModel_.itemFromIndex( index );
    if( item && item->data( KnowledgeRole ).isValid() )
        subSelected_ = item->data( KnowledgeRole ).value< const kernel::AgentKnowledge_ABC* >();

    if( subSelected_ )
    {
        NotifyUpdated( *subSelected_ );
        const PerceptionMap* perceptions = subSelected_->Retrieve< PerceptionMap >();
        if( perceptions )
            NotifyUpdated( *perceptions  );
        else
            perceptionModel_.removeRows( 0, perceptionModel_.rowCount() );
    }
    else
        display_->Hide();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnPerceptionContextMenuEvent
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnPerceptionContextMenuEvent( const QPoint & pos )
{
    const QModelIndex index = pPerceptionListView_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = perceptionModel_.itemFromIndex( index );
    if( item && item->data( EntityRole ).isValid() )
        item->data( EntityRole ).value< const Automat_ABC* >()->ContextMenu( controllers_.actions_, pPerceptionListView_->viewport()->mapToGlobal( pos ) );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnKnowledgeContextMenuEvent
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnKnowledgeContextMenuEvent( const QPoint & pos )
{
    const QModelIndex index = pKnowledgeListView_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = knowledgeModel_.itemFromIndex( index );
    if( item && item->data( KnowledgeRole ).isValid() )
        item->data( KnowledgeRole ).value< const AgentKnowledge_ABC* >()->ContextMenu( controllers_.actions_, pKnowledgeListView_->viewport()->mapToGlobal( pos ) );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnPerceptionRequestCenter
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnPerceptionRequestCenter()
{
    const QModelIndex index = pPerceptionListView_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = perceptionModel_.itemFromIndex( index );
    if( item && item->data( EntityRole ).isValid() )
        item->data( EntityRole ).value< const Automat_ABC* >()->Activate( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnKnowledgeRequestCenter
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnKnowledgeRequestCenter()
{
    const QModelIndex index = pKnowledgeListView_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = knowledgeModel_.itemFromIndex( index );
    if( item && item->data( KnowledgeRole ).isValid() )
        item->data( KnowledgeRole ).value< const AgentKnowledge_ABC* >()->Activate( controllers_.actions_ );
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
    int perceptionSize = static_cast< int >( perceptions.perceptions_.size() );
    ResizeModelOnNewContent( &perceptionModel_, pPerceptionListView_->selectionModel(), perceptionSize );
    for( int i = 0; i < perceptionSize; ++i )
    {
        perceptionModel_.item( i, 0 )->setText( perceptions.perceptions_[ i ].detected_->GetName() );
        perceptionModel_.item( i, 0 )->setData( QVariant::fromValue( perceptions.perceptions_[ i ].detected_ ), EntityRole );
        perceptionModel_.item( i, 1 )->setText( tools::ToString( perceptions.perceptions_[ i ].level_ ) );
    }
}
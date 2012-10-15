// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UrbanKnowledgePanel.h"
#include "moc_UrbanKnowledgePanel.cpp"

#include "clients_kernel/Automat_ABC.h"
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

Q_DECLARE_METATYPE( const kernel::UrbanKnowledge_ABC* )
#define KnowledgeRole ( Qt::UserRole + 1 )

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
    QWidget* view = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout( view );

    pKnowledgeListView_ = new QTreeView();
    pKnowledgeListView_->setContextMenuPolicy( Qt::CustomContextMenu );
    pKnowledgeListView_->setRootIsDecorated( false );
    pKnowledgeListView_->setEditTriggers( 0 );
    pKnowledgeListView_->setModel( &knowledgeModel_ );
    knowledgeModel_.setHorizontalHeaderLabels( QStringList( tools::translate( "Urban", "Known blocks" ) ) );
    layout->addWidget( pKnowledgeListView_ );

    display_ = new DisplayBuilder( view, factory );
    display_->AddGroup( tr( "Details" ) )
                .AddLabel( tr( "Identifier:" ) )
                .AddLabel( tr( "Associated block:" ) )
                .AddLabel( tr( "Perceived:" ) )
                .AddLabel( tr( "Progress:" ) )
                .AddLabel( tr( "Maximum Progress:" ) );

    pPerceptionListView_ = new QTreeView();
    pPerceptionListView_->setRootIsDecorated( false );
    pPerceptionListView_->setEditTriggers( 0 );
    pPerceptionListView_->setModel( &perceptionModel_ );
    perceptionModel_.setHorizontalHeaderLabels( QStringList( tools::translate( "ObjectKnowledgePanel", "Agent" ) ) );

    layout->addWidget( pPerceptionListView_ );

    connect( pKnowledgeListView_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelectionChanged() ) );
    connect( pKnowledgeListView_, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenuRequested( const QPoint & ) ) );

    setWidget( view );

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

    knowledgeModel_.removeRows( 0, knowledgeModel_.rowCount() );
    tools::Iterator< const kernel::UrbanKnowledge_ABC& > iterator = element.CreateIterator();
    while( iterator.HasMoreElements() )
    {
        //population knowledge infos
        const kernel::UrbanKnowledge_ABC& knowledge = iterator.NextElement();
        QStandardItem* nameItem = new QStandardItem( knowledge.GetEntity()->GetName() );
        nameItem->setData( QVariant::fromValue( &knowledge ), KnowledgeRole );
        knowledgeModel_.appendRow( nameItem );
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
     perceptionModel_.removeRows( 0, perceptionModel_.rowCount() );
     for( UrbanPerceptions::CIT_Agents it = element.detectingAutomats_.begin(); it != element.detectingAutomats_.end(); ++it )
         perceptionModel_.appendRow( new QStandardItem( ( *it )->GetName() ) );
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
void UrbanKnowledgePanel::OnSelectionChanged()
{
    const QModelIndex index = pKnowledgeListView_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = knowledgeModel_.itemFromIndex( index );
    if( item && item->data( KnowledgeRole ).isValid() )
    {
        subSelected_ = item->data( KnowledgeRole ).value< const UrbanKnowledge_ABC* >();
        subSelected_->GetEntity()->Activate( controllers_.actions_ );
    }
    else
        subSelected_ = 0;

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
void UrbanKnowledgePanel::OnContextMenuRequested( const QPoint& pos )
{
    const QModelIndex index = pKnowledgeListView_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = knowledgeModel_.itemFromIndex( index );
    if( item && item->data( KnowledgeRole ).isValid() )
        item->data( KnowledgeRole ).value< const UrbanKnowledge_ABC* >()->ContextMenu( controllers_.actions_, pKnowledgeListView_->viewport()->mapToGlobal( pos ) );
}

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

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/UrbanKnowledge_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/Tools.h"
#include "gaming/UrbanKnowledges.h"
#include "gaming/UrbanPerceptions.h"

Q_DECLARE_METATYPE( const kernel::Automat_ABC* )
Q_DECLARE_METATYPE( const kernel::UrbanKnowledge_ABC* )

#define EntityRole ( Qt::UserRole )

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
                .AddLabel( "identifier", tr( "Identifier:" ) )
                .AddLabel( "associatedBlock", tr( "Associated block:" ) )
                .AddLabel( "Perceived", tr( "Perceived:" ) )
                .AddLabel( "Progress", tr( "Progress:" ) )
                .AddLabel( "MaxProgress", tr( "Maximum Progress:" ) );

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

    ResizeModelOnNewContent( &knowledgeModel_, pKnowledgeListView_->selectionModel(), element.Count(), *display_ );
    int i = 0;
    tools::Iterator< const kernel::UrbanKnowledge_ABC& > iterator = element.CreateIterator();
    while( iterator.HasMoreElements() )
    {
        const kernel::UrbanKnowledge_ABC& knowledge = iterator.NextElement();
        knowledgeModel_.item( i )->setText( knowledge.GetEntity()->GetName() );
        knowledgeModel_.item( i )->setData( QVariant::fromValue( &knowledge ), EntityRole );
        ++i;
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

     int knowledgeSize = static_cast< int >( element.detectingAutomats_.size() );
     ResizeModelOnNewContent( &perceptionModel_, pPerceptionListView_->selectionModel(), knowledgeSize );
     for( int i = 0; i < knowledgeSize; ++i )
     {
         perceptionModel_.item( i )->setText( element.detectingAutomats_[ i ]->GetName() );
         perceptionModel_.item( i, 0 )->setData( QVariant::fromValue( element.detectingAutomats_[ i ] ), EntityRole );
     }
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
    if( item && item->data( EntityRole ).isValid() )
    {
        subSelected_ = item->data( EntityRole ).value< const UrbanKnowledge_ABC* >();
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
    if( item && item->data( EntityRole ).isValid() )
        item->data( EntityRole ).value< const UrbanKnowledge_ABC* >()->ContextMenu( controllers_.actions_, pKnowledgeListView_->viewport()->mapToGlobal( pos ), this );
}

void UrbanKnowledgePanel::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    ResizeModelOnNewContent( &knowledgeModel_, pKnowledgeListView_->selectionModel(), 0, *display_ );
    ResizeModelOnNewContent( &perceptionModel_, pPerceptionListView_->selectionModel(), 0 );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgePanel::NotifyUpdated
// Created: ABR 2014-09-11
// -----------------------------------------------------------------------------
void UrbanKnowledgePanel::NotifyUpdated( const kernel::Automat_ABC& automat )
{
    if( !isVisible() )
        return;
    tools::UpdateEntityNameInModel< Automat_ABC >( perceptionModel_, automat, EntityRole );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectKnowledgePanel.h"
#include "moc_ObjectKnowledgePanel.cpp"

#include "clients_gui/DisplayBuilder.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/ObjectKnowledges.h"
#include "gaming/ObjectPerceptions.h"

Q_DECLARE_METATYPE( const kernel::ObjectKnowledge_ABC* )
#define KnowledgeRole ( Qt::UserRole + 1 )

using namespace kernel;
using namespace gui;

// $$$$ LDC 2011-04-04 FIXME ObjectKnowledgePanel.inl has been created to hold the translations specific to this file
// in order to avoid lupdate from duplicating the "Object" context used in gaming. This file is only useful for translator.pro

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel constructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
ObjectKnowledgePanel::ObjectKnowledgePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tools::translate( "ObjectKnowledgePanel", "Object knowledges" ) )
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
    knowledgeModel_.setHorizontalHeaderLabels( QStringList( tools::translate( "ObjectKnowledgePanel", "Known objects" ) ) );

    layout->addWidget( pKnowledgeListView_ );

    display_ = new DisplayBuilder( view, factory );
    display_->AddGroup( tools::findTranslation( "Object", "Information" ) )
                .AddLabel( tools::findTranslation( "Object", "Identifier:" ) )
                .AddLabel( tools::findTranslation( "Object", "Associated object:" ) )
                .AddLabel( tools::findTranslation( "Object", "Location:" ) )
                .AddLabel( tools::findTranslation( "Object", "Type:" ) )
                .AddLabel( tools::findTranslation( "Object", "Construction:" ) )
                .AddLabel( tools::findTranslation( "Object", "Construction resource:" ) )
                .AddLabel( tools::findTranslation( "Object", "Mining:" ) )
                .AddLabel( tools::findTranslation( "Object", "Development resource:" ) )
                .AddLabel( tools::findTranslation( "Object", "Bypass:" ) )
                .AddLabel( tools::findTranslation( "Object", "Obstacle type:" ) )
                .AddLabel( tools::findTranslation( "Object", "Reserved obstacle activated:" ) )
                .AddLabel( tools::findTranslation( "Object", "Activation time:" ) )
                .AddLabel( tools::findTranslation( "Object", "Activity time:" ) )
                .AddLabel( tools::findTranslation( "Object", "Perceived:" ) )
                .AddLabel( tools::findTranslation( "Object", "Relevance:" ) )
                .AddLabel( tools::findTranslation( "Object", "Delay time:" ) );

    display_->AddGroup( tools::findTranslation( "Object", "Crossing site" ) )
                .AddLabel( tools::findTranslation( "Object", "Width:" ) )
                .AddLabel( tools::findTranslation( "Object", "Depth:" ) )
                .AddLabel( tools::findTranslation( "Object", "Stream speed:" ) )
                .AddLabel( tools::findTranslation( "Object", "Build river banks:" ) );

    display_->AddGroup( tools::findTranslation( "Object", "Camp" ) )
                .AddLabel( tools::findTranslation( "Object", "Logistic superior:" ) );

    display_->AddGroup( tools::findTranslation( "NBC", "NBC" ) )
                .AddLabel( tools::findTranslation( "NBC", "Danger:" ) )
                .AddLabel( tools::findTranslation( "NBC", "NBC agent(s):" ) );

    display_->AddGroup( tools::findTranslation( "Object", "Logistic route" ) )
                .AddLabel( tools::findTranslation( "Object", "Equipped:" ) )
                .AddLabel( tools::findTranslation( "Object", "Flow:" ) )
                .AddLabel( tools::findTranslation( "Object", "Width:" ) )
                .AddLabel( tools::findTranslation( "Object", "Length:" ) )
                .AddLabel( tools::findTranslation( "Object", "Maximum weight:" ) );

    display_->AddGroup( tools::findTranslation( "Object", "Mine parameters" ) )
                .AddLabel( tools::findTranslation( "Object", "Activity time:" ) )
                .AddLabel( tools::findTranslation( "Object", "Density:" ) );

    display_->AddGroup( tools::findTranslation( "Object", "Fire" ) )
                .AddLabel( tools::findTranslation( "Object", "Fire class:" ) )
                .AddLabel( tools::findTranslation( "Object", "Max combustion energy:" ) );

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
// Name: ObjectKnowledgePanel destructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
ObjectKnowledgePanel::~ObjectKnowledgePanel()
{
    controllers_.Unregister( *this );
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::showEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::showEvent( QShowEvent* )
{
    if( selected_ )
        NotifyUpdated( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const ObjectKnowledges& element )
{
    if( !selected_ || selected_ != &element )
        return;

    ResizeModelOnNewContent( &knowledgeModel_, pKnowledgeListView_->selectionModel(), element.Count() );
    int i = 0;
    tools::Iterator< const kernel::ObjectKnowledge_ABC& > iterator = element.CreateIterator();
    while( iterator.HasMoreElements() )
    {
        const kernel::ObjectKnowledge_ABC& knowledge = iterator.NextElement();
        knowledgeModel_.item( i )->setText( knowledge.GetEntity()? knowledge.GetEntity()->GetName() : QString::number( knowledge.GetEntityId() ) );
        knowledgeModel_.item( i )->setData( QVariant::fromValue( &knowledge ), KnowledgeRole );
        ++i;
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const kernel::ObjectKnowledge_ABC& element )
{
    if( ! IsVisible() || &element != subSelected_ )
        return;
    display_->Hide();
    element.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const ObjectPerceptions& element )
{
    if( ! IsVisible() || ! subSelected_ || subSelected_->Retrieve< ObjectPerceptions >() != &element )
        return;

    int knowledgeSize = static_cast< int >( element.detectingAutomats_.size() );
    ResizeModelOnNewContent( &perceptionModel_, pPerceptionListView_->selectionModel(), knowledgeSize );
    for( int i = 0; i < knowledgeSize; ++i )
        perceptionModel_.item( i )->setText( element.detectingAutomats_[ i ]->GetName() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::DisplayExtension
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
template< typename T >
void ObjectKnowledgePanel::DisplayExtension( const T& extension )
{
    if( IsVisible() && subSelected_ && subSelected_->Retrieve< T >() == &extension )
        extension.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const ConstructionAttribute_ABC& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const BypassAttribute_ABC& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const ObstacleAttribute_ABC& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const LogisticAttribute_ABC& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const CrossingSiteAttribute_ABC& element )
{
   DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const SupplyRouteAttribute_ABC& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const NBCAttribute_ABC& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const TimeLimitedAttribute_ABC& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const MineAttribute_ABC& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::Select( const Team_ABC* team )
{
    const ObjectKnowledges* k = team ? team->Retrieve< ObjectKnowledges >() : 0;
    if( ! k || k != selected_ )
        Select( k );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::Select
// Created: LDC 2010-04-15
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::Select( const ObjectKnowledges* k )
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

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::Select
// Created: LDC 2010-04-15
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::Select( const kernel::KnowledgeGroup_ABC* group )
{
    const ObjectKnowledges* k = group ? group->Retrieve< ObjectKnowledges >() : 0;
    if( !k && group )
    {
        const Hierarchies* hierarchies = group->Retrieve< kernel::CommunicationHierarchies >();
        if( hierarchies )
            Select( static_cast< const Team_ABC* >( & hierarchies->GetTop() ) );
    }
    else if( k != selected_ )
        Select( k );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::UpdateExtension
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
template< typename T >
void ObjectKnowledgePanel::UpdateExtension( const ObjectKnowledge_ABC& k )
{
    const T* extension = k.Retrieve< T >();
    if( extension )
        NotifyUpdated( *extension );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::OnSelectionChanged
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::OnSelectionChanged()
{
    const QModelIndex index = pKnowledgeListView_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = knowledgeModel_.itemFromIndex( index );
    if( item && item->data( KnowledgeRole ).isValid() )
    {
        subSelected_ = item->data( KnowledgeRole ).value< const ObjectKnowledge_ABC* >();
        if( subSelected_ )
        {
            subSelected_->Activate( controllers_.actions_ );
            NotifyUpdated( *subSelected_ );
            UpdateExtension< ConstructionAttribute_ABC >( *subSelected_ );
            UpdateExtension< BypassAttribute_ABC >( *subSelected_ );
            UpdateExtension< ObstacleAttribute_ABC >( *subSelected_ );
            UpdateExtension< LogisticAttribute_ABC >( *subSelected_ );
            UpdateExtension< CrossingSiteAttribute_ABC >( *subSelected_ );
            UpdateExtension< SupplyRouteAttribute_ABC >( *subSelected_ );
            UpdateExtension< NBCAttribute_ABC >( *subSelected_ );
            UpdateExtension< TimeLimitedAttribute_ABC >( *subSelected_ );
            UpdateExtension< MineAttribute_ABC >( *subSelected_ );

            const ObjectPerceptions* perceptions = subSelected_->Retrieve< ObjectPerceptions >();
            if( perceptions )
                NotifyUpdated( *perceptions );
        }
    }
}

// $$$$ AGE 2006-03-13: Factor these
// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::OnContextMenuRequested
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::OnContextMenuRequested( const QPoint & pos )
{
    const QModelIndex index = pKnowledgeListView_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = knowledgeModel_.itemFromIndex( index );
    if( item && item->data( KnowledgeRole ).isValid() )
        item->data( KnowledgeRole ).value< const ObjectKnowledge_ABC* >()->ContextMenu( controllers_.actions_, pKnowledgeListView_->viewport()->mapToGlobal( pos ) );
}

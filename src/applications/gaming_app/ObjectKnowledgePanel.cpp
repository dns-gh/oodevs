// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectKnowledgePanel.h"
#include "moc_ObjectKnowledgePanel.cpp"

#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/ListDisplayer.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/ObjectKnowledges.h"
#include "gaming/ObjectPerceptions.h"

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
    pKnowledgeListView_ = new ListDisplayer< ObjectKnowledgePanel >( this, *this, factory );
    pKnowledgeListView_->AddColumn(  tools::translate( "ObjectKnowledgePanel", "Known objects" ) );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tools::translate( "Object", "Information" ) )
                .AddLabel( tools::translate( "Object", "Identifier:" ) )
                .AddLabel( tools::translate( "Object", "Associated object:" ) )
                .AddLabel( tools::translate( "Object", "Location:" ) )
                .AddLabel( tools::translate( "Object", "Type:" ) )
                .AddLabel( tools::translate( "Object", "Construction:" ) )
                .AddLabel( tools::translate( "Object", "Construction dotation:" ) )
                .AddLabel( tools::translate( "Object", "Mining:" ) )
                .AddLabel( tools::translate( "Object", "Development dotation:" ) )
                .AddLabel( tools::translate( "Object", "Bypass:" ) )
                .AddLabel( tools::translate( "Object", "Obstacle type:" ) )
                .AddLabel( tools::translate( "Object", "Reserved obstacle activated:" ) )
                .AddLabel( tools::translate( "Object", "Perceived:" ) )
                .AddLabel( tools::translate( "Object", "Relevance:" ) );

    display_->AddGroup( tools::translate( "Object", "Crossing site" ) )
                .AddLabel( tools::translate( "Object", "Width:" ) )
                .AddLabel( tools::translate( "Object", "Depth:" ) )
                .AddLabel( tools::translate( "Object", "Stream speed:" ) )
                .AddLabel( tools::translate( "Object", "Build river banks:" ) );

    display_->AddGroup( tools::translate( "Object", "Camp" ) )
                .AddLabel( tools::translate( "Object", "TC2:" ) );

    display_->AddGroup( tools::translate( "NBC", "NBC" ) )
                .AddLabel( tools::translate( "NBC", "Danger:" ) )
                .AddLabel( tools::translate( "NBC", "NBC agent(s):" ) );

    display_->AddGroup( tools::translate( "Object", "Logistic route" ) )
                .AddLabel( tools::translate( "Object", "Equipped:" ) )
                .AddLabel( tools::translate( "Object", "Flow:" ) )
                .AddLabel( tools::translate( "Object", "Width:" ) )
                .AddLabel( tools::translate( "Object", "Length:" ) )
                .AddLabel( tools::translate( "Object", "Maximum weight:" ) );

    display_->AddGroup( tools::translate( "Object", "Mine parameters" ) )
                .AddLabel( tools::translate( "Object", "Activity time:" ) )
                .AddLabel( tools::translate( "Object", "Density:" ) );

    display_->AddGroup( tools::translate( "Object", "Fire" ) )
                .AddLabel( tools::translate( "Object", "Fire class:" ) )
                .AddLabel( tools::translate( "Object", "Fire temperature:" ) );

    pPerceptionListView_ = new ListDisplayer< ObjectKnowledgePanel >( this, *this, factory );
    pPerceptionListView_->AddColumn( tools::translate( "ObjectKnowledgePanel", "Agent" ) );

    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );

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
    if ( selected_ && selected_ == &element )
        pKnowledgeListView_->DeleteTail( pKnowledgeListView_->DisplayList( element.CreateIterator() ));
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::Display
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::Display( const kernel::ObjectKnowledge_ABC& k, Displayer_ABC& displayer, ValuedListItem* item )
{
    item->SetValue( &k );
    k.DisplayInList( displayer );
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
    pPerceptionListView_->DeleteTail(
        pPerceptionListView_->DisplayList( element.detectingAutomats_.begin(), element.detectingAutomats_.end() )
    );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::Display
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::Display( const Automat_ABC* agent, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( agent );
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
void ObjectKnowledgePanel::NotifyUpdated( const ActivityTimeAttribute_ABC& element )
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
void ObjectKnowledgePanel::OnSelectionChanged( QListViewItem* i )
{
    ValuedListItem* item = (ValuedListItem*)( i );
    if( item->IsA< const ObjectKnowledge_ABC >() )
    {
        subSelected_ = item->GetValue< const ObjectKnowledge_ABC >();
        if( subSelected_ )
        {
            NotifyUpdated( *subSelected_ );
            UpdateExtension< ConstructionAttribute_ABC >( *subSelected_ );
            UpdateExtension< BypassAttribute_ABC >( *subSelected_ );
            UpdateExtension< ObstacleAttribute_ABC >( *subSelected_ );
            UpdateExtension< LogisticAttribute_ABC >( *subSelected_ );
            UpdateExtension< CrossingSiteAttribute_ABC >( *subSelected_ );
            UpdateExtension< SupplyRouteAttribute_ABC >( *subSelected_ );
            UpdateExtension< NBCAttribute_ABC >( *subSelected_ );
            UpdateExtension< ActivityTimeAttribute_ABC >( *subSelected_ );
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
void ObjectKnowledgePanel::OnContextMenuRequested( QListViewItem* i, const QPoint& pos )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->ContextMenu( controllers_.actions_, pos );
    }
}

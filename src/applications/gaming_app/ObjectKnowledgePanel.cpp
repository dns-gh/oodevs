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
    , selectedForTeam_( controllers )
    , selectedForKg_( controllers )
    , subSelected_ ( controllers )
{
    QVBoxLayout* layout = new QVBoxLayout( this );
    pKnowledgeListView_ = new ListDisplayer< ObjectKnowledgePanel >( this, *this, factory );
    pKnowledgeListView_->AddColumn( tools::translate( "ObjectKnowledgePanel", "Known objects" ) );
    layout->addWidget( pKnowledgeListView_ );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tools::findTranslation( "Object", "Information" ) )
                .AddLabel( tools::findTranslation( "Object", "Identifier:" ) )
                .AddLabel( tools::findTranslation( "Object", "Associated object:" ) )
                .AddLabel( tools::findTranslation( "Object", "Location:" ) )
                .AddLabel( tools::findTranslation( "Object", "Type:" ) )
                .AddLabel( tools::findTranslation( "Object", "Construction:" ) )
                .AddLabel( tools::findTranslation( "Object", "Construction resource:" ) )
                .AddLabel( tools::findTranslation( "Object", "Mining:" ) )
                .AddLabel( tools::findTranslation( "Object", "Development resource:" ) )
                .AddLabel( tools::findTranslation( "Object", "Breaching:" ) )
                .AddLabel( tools::findTranslation( "Object", "Obstacle type:" ) )
                .AddLabel( tools::findTranslation( "Object", "Obstacle activated:" ) )
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

    pPerceptionListView_ = new ListDisplayer< ObjectKnowledgePanel >( this, *this, factory );
    pPerceptionListView_->AddColumn( tools::translate( "ObjectKnowledgePanel", "Agent" ) );
    layout->addWidget( pPerceptionListView_ );

    connect( pKnowledgeListView_, SIGNAL( selectionChanged( Q3ListViewItem* ) ), this, SLOT( OnSelectionChanged( Q3ListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( Q3ListViewItem*, const QPoint& ) ) );

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
    if( selectedForKg_ )
        NotifyUpdated( *selectedForKg_ );
    else if( selectedForTeam_ )
        NotifyUpdated( *selectedForTeam_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const ObjectKnowledges& element )
{
    if( selectedForTeam_ == &element || selectedForKg_ == &element )
    {
        tools::Resolver< kernel::ObjectKnowledge_ABC > resolver;
        if( selectedForTeam_ )
        {
            auto it = selectedForTeam_->CreateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::ObjectKnowledge_ABC& kn = it.NextElement();
                if( resolver.Find( kn.GetId() ) == 0 )
                    resolver.Register( kn.GetId(), const_cast< kernel::ObjectKnowledge_ABC& >( kn ) );
            }
        }
        if( selectedForKg_ )
        {
            auto it = selectedForKg_->CreateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::ObjectKnowledge_ABC& kn = it.NextElement();
                if( resolver.Find( kn.GetId() ) == 0 )
                    resolver.Register( kn.GetId(), const_cast< kernel::ObjectKnowledge_ABC& >( kn ) );
            }
        }
        pKnowledgeListView_->DeleteTail( pKnowledgeListView_->DisplayList( resolver.CreateIterator() ) );
    }
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
    selectedForKg_ = 0;
    const ObjectKnowledges* k = team ? team->Retrieve< ObjectKnowledges >() : 0;
    if( ! k || k != selectedForTeam_ )
    {
        selectedForTeam_ = k;
    }
    DoShow();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::DoShow
// Created: LDC 2010-04-15
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::DoShow()
{
    if( selectedForTeam_ || selectedForKg_ )
    {
        subSelected_ = 0;
        Show();
        NotifyUpdated( selectedForTeam_ ? *selectedForTeam_ : *selectedForKg_ );
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
    if( !group )
    {
        selectedForKg_ = 0;
        selectedForTeam_ = 0;
        DoShow();
        return;
    }

    selectedForKg_ = group->Retrieve< ObjectKnowledges >();
    selectedForTeam_ = 0;
    if( !group->IsJammed() )
    {
        const Hierarchies* hierarchies = group->Retrieve< kernel::CommunicationHierarchies >();
        if( hierarchies )
             if( const Team_ABC* team = static_cast< const Team_ABC* >( & hierarchies->GetTop() ) )
                 selectedForTeam_ = team->Retrieve< ObjectKnowledges >();
    }
    DoShow();
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
void ObjectKnowledgePanel::OnSelectionChanged( Q3ListViewItem* i )
{
    ValuedListItem* item = (ValuedListItem*)( i );
    if( item->IsA< const ObjectKnowledge_ABC >() )
    {
        subSelected_ = item->GetValueNoCheck< const ObjectKnowledge_ABC >();
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
void ObjectKnowledgePanel::OnContextMenuRequested( Q3ListViewItem* i, const QPoint& pos )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->ContextMenu( controllers_.actions_, pos );
    }
}

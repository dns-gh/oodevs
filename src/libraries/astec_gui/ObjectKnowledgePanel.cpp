// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ObjectKnowledgePanel.h"
#include "moc_ObjectKnowledgePanel.cpp"

#include "ListDisplayer.h"
#include "DisplayBuilder.h"
#include "astec_kernel/Controllers.h"
#include "astec_gaming/ObjectKnowledges.h"
#include "astec_gaming/ObjectKnowledge.h"
#include "astec_gaming/CampAttributes.h"
#include "astec_gaming/CrossingSiteAttributes.h"
#include "astec_gaming/LogisticRouteAttributes.h"
#include "astec_gaming/NBCAttributes.h"
#include "astec_gaming/RotaAttributes.h"
#include "astec_kernel/Team_ABC.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel constructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
ObjectKnowledgePanel::ObjectKnowledgePanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( pParent, tr( "Connaissances objet" ) )
    , controllers_ ( controllers )
    , owner_       ( controllers )
    , selected_    ( controllers )
    , subSelected_ ( controllers )
{
    pKnowledgeListView_ = new ListDisplayer< ObjectKnowledgePanel >( this, *this, factory );
    pKnowledgeListView_->AddColumn( "Objets connus" );

    pOwnTeamCheckBox_ = new QCheckBox( tr( "Afficher propre camp" ), this );
    pOwnTeamCheckBox_->setChecked( true );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( "Détails" )
                .AddLabel( "Id:" )
                .AddLabel( "Objet associé:" )
                .AddLabel( "Position:" )
                .AddLabel( "Type:" )
                .AddLabel( "Construction:" )
                .AddLabel( "Valeur:" )
                .AddLabel( "Contournement:" )
                .AddLabel( "En préparation:" )
                .AddLabel( "Perçu:" )
                .AddLabel( "Pertinence:" );

    display_->AddGroup( "Site de franchissement" )
                .AddLabel( "Largeur:" )
                .AddLabel( "Profondeur:" )
                .AddLabel( "Vitesse courant:" )
                .AddLabel( "Berges à aménager:" );

    display_->AddGroup( "Camp" )
                .AddLabel( "TC2:" );

    display_->AddGroup( "Nuage/Zone NBC" )
                .AddLabel( "Agent_ABC NBC:" );

    display_->AddGroup( "ROTA" )
                .AddLabel( "Danger:" )
                .AddLabel( "Agents NBC:" );

    display_->AddGroup( "Itinéraire logistique" )
                .AddLabel( "Equipé:" )
                .AddLabel( "Débit:" )
                .AddLabel( "Largeur:" )
                .AddLabel( "Longueur:" )
                .AddLabel( "Poids supporté:" );

    pPerceptionListView_ = new ListDisplayer< ObjectKnowledgePanel >( this, *this, factory );
    pPerceptionListView_->AddColumn( "Agent_ABC" );

    connect( pOwnTeamCheckBox_,   SIGNAL( clicked() ),                          this, SLOT( ToggleDisplayOwnTeam() ) );
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
    controllers_.Remove( *this );
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
    pKnowledgeListView_->DeleteTail(
        pKnowledgeListView_->DisplayList( element.CreateIterator() )
        );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::Display
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::Display( const ObjectKnowledge& k, Displayer_ABC& displayer, ValuedListItem* item )
{
    if( pOwnTeamCheckBox_->isChecked() || ! owner_ || ! k.KnowledgeIsInTeam( *owner_ ) )
    {
        item->SetValue( &k );
        k.DisplayInList( displayer );
    }
    else
        delete item;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const ObjectKnowledge& element )
{
    if( ! IsVisible() || &element != subSelected_ )
        return;
    display_->Hide();
    element.Display( *display_ );
    pPerceptionListView_->DeleteTail(
        pPerceptionListView_->DisplayList( element.detectingAutomats_.begin(), element.detectingAutomats_.end() )
    );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::Display
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::Display( const Agent_ABC* agent, Displayer_ABC& displayer, ValuedListItem* )
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
void ObjectKnowledgePanel::NotifyUpdated( const CampAttributes& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const CrossingSiteAttributes& element )
{
   DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const LogisticRouteAttributes& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const NBCAttributes& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const RotaAttributes& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::Select
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::Select( const Team_ABC* team )
{
    owner_ = team;
    const ObjectKnowledges* k = team ? team->Retrieve< ObjectKnowledges >() : 0;
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
// Name: ObjectKnowledgePanel::UpdateExtension
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
template< typename T >
void ObjectKnowledgePanel::UpdateExtension( const ObjectKnowledge& k )
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
    if( item->IsA< const ObjectKnowledge* >() )
    {
        subSelected_ = item->GetValue< const ObjectKnowledge* >();
        if( subSelected_ )
        {
            NotifyUpdated( *subSelected_ );
            UpdateExtension< CampAttributes >( *subSelected_ );
            UpdateExtension< CrossingSiteAttributes >( *subSelected_ );
            UpdateExtension< LogisticRouteAttributes >( *subSelected_ );
            UpdateExtension< NBCAttributes >( *subSelected_ );
            UpdateExtension< RotaAttributes >( *subSelected_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::ToggleDisplayOwnTeam
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::ToggleDisplayOwnTeam()
{
    showEvent( 0 );
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

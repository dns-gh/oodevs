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

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include "gaming/ObjectKnowledges.h"
#include "gaming/ObjectKnowledge_ABC.h"
#include "gaming/ObjectPerceptions.h"
#include "clients_gui/ListDisplayer.h"
#include "clients_gui/DisplayBuilder.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel constructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
ObjectKnowledgePanel::ObjectKnowledgePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tr( "Connaissances objet" ) )
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
                .AddLabel( "Vitesse du courant:" )
                .AddLabel( "Berges à aménager:" );

    display_->AddGroup( "Camp" )
                .AddLabel( "TC2:" );

    display_->AddGroup( "Nuage/Zone NBC" )
                .AddLabel( "Agent NBC:" );

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
    pPerceptionListView_->AddColumn( "Agent" );

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
void ObjectKnowledgePanel::Display( const ObjectKnowledge_ABC& k, Displayer_ABC& displayer, ValuedListItem* item )
{
    // $$$$ AGE 2006-10-16: 
//    if( pOwnTeamCheckBox_->isChecked() || ! owner_ || ! k.KnowledgeIsInTeam( *owner_ ) )
    {
        item->SetValue( &k );
        k.DisplayInList( displayer );
    }
//    else
//        delete item;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const ObjectKnowledge_ABC& element )
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
void ObjectKnowledgePanel::NotifyUpdated( const CampAttributes_ABC& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const CrossingSiteAttributes_ABC& element )
{
   DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const LogisticRouteAttributes_ABC& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const NBCAttributes_ABC& element )
{
    DisplayExtension( element );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::NotifyUpdated( const RotaAttributes_ABC& element )
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
    if( item->IsA< const ObjectKnowledge_ABC* >() )
    {
        subSelected_ = item->GetValue< const ObjectKnowledge_ABC* >();
        if( subSelected_ )
        {
            NotifyUpdated( *subSelected_ );
            UpdateExtension< CampAttributes_ABC >( *subSelected_ );
            UpdateExtension< CrossingSiteAttributes_ABC >( *subSelected_ );
            UpdateExtension< LogisticRouteAttributes_ABC >( *subSelected_ );
            UpdateExtension< NBCAttributes_ABC >( *subSelected_ );
            UpdateExtension< RotaAttributes_ABC >( *subSelected_ );
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

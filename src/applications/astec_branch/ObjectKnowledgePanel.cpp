// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectKnowledgePanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 18:34 $
// $Revision: 9 $
// $Workfile: ObjectKnowledgePanel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectKnowledgePanel.h"
#include "moc_ObjectKnowledgePanel.cpp"

#include "ListDisplayer.h"
#include "DisplayBuilder.h"
#include "Controller.h"
#include "ActionController.h"
#include "ObjectKnowledges.h"
#include "ObjectKnowledge.h"
#include "CampAttributes.h"
#include "CrossingSiteAttributes.h"
#include "LogisticRouteAttributes.h"
#include "NBCAttributes.h"
#include "RotaAttributes.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel constructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
ObjectKnowledgePanel::ObjectKnowledgePanel( InfoPanel* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC( pParent, "C. objet" ) // $$$$ AGE 2006-02-24: tr(
    , selected_( 0 )
    , subSelected_( 0 )
{
    pKnowledgeListView_ = new ListDisplayer< ObjectKnowledgePanel >( this, *this );
    pKnowledgeListView_->AddColumn( "Objets connus" );

    display_ = new DisplayBuilder( this );
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

//    pPerceptionListView_ = new ListDisplayer< ObjectKnowledgePanel >( this, *this );
//    pPerceptionListView_->AddColumn( "Agent" );

    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );

    controller.Register( *this );
    actionController.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel destructor
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
ObjectKnowledgePanel::~ObjectKnowledgePanel()
{
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
void ObjectKnowledgePanel::Display( const ObjectKnowledge& k, Displayer_ABC& displayer, ValuedListItem* )
{
    k.DisplayInList( displayer );
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
void ObjectKnowledgePanel::Select( const Team* team )
{
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
            if( subSelected_->Retrieve< CampAttributes >() )
                NotifyUpdated( subSelected_->Get< CampAttributes >() );
            if( subSelected_->Retrieve< CrossingSiteAttributes >() )
                NotifyUpdated( subSelected_->Get< CrossingSiteAttributes >() );
            if( subSelected_->Retrieve< LogisticRouteAttributes >() )
                NotifyUpdated( subSelected_->Get< LogisticRouteAttributes >() );
            if( subSelected_->Retrieve< NBCAttributes >() )
                NotifyUpdated( subSelected_->Get< NBCAttributes >() );
            if( subSelected_->Retrieve< RotaAttributes >() )
                NotifyUpdated( subSelected_->Get< RotaAttributes >() );
        }                              
    }                                  
}                                      
                                       
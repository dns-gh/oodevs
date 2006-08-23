// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectPanel.h"

#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Units.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/GroupDisplayer.h"
#include "clients_gui/LabelDisplayer.h"
#include "gaming/CampAttributes.h"
#include "gaming/CrossingSiteAttributes.h"
#include "gaming/LogisticRouteAttributes.h"
#include "gaming/NBCAttributes.h"
#include "gaming/RotaAttributes.h"

#include <qgrid.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectPanel constructor
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
ObjectPanel::ObjectPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tr( "Etat objet" ) )
    , controllers_ ( controllers )
    , selected_    ( controllers )
{
    display_ = new DisplayBuilder( this, factory );
    
    // $$$$ AGE 2006-08-23: tous ces trucs doivent etre identiques au labels utilisés
    // $$$$ AGE 2006-08-23: par le modèle correspondant et pire : traduits de la même maniere.
    // $$$$ AGE 2006-08-23: Faire des fichiers avec un tas de statics référencés par les 2 ?
    display_->AddGroup( tr( "Informations" ) )
                .AddLabel( tr( "Id:" ) )
                .AddLabel( tr( "Nom:" ) )
                .AddLabel( tr( "Type:" ) )
                .AddLabel( tr( "Position:" ) )
                .AddSpinBox( tr( "Construction:" ), 0, 100, 1 )
                .AddSpinBox( tr( "Valorisation:" ), 0, 100, 1 )
                .AddSpinBox( tr( "Contournement:" ), 0, 100, 1 )
                .AddCheckBox( tr( "En préparation:" ) );

    display_->Group( tr( "Informations" ) )
                .AddLabel( tr( "Dotation construction:" ) )
                .AddLabel( tr( "Dotation valorisation:" ) );

    display_->AddGroup( tr( "Site de franchissement" ) )
                .AddLabel( tr( "Largeur:" ) )
                .AddLabel( tr( "Profondeur:" ) )
                .AddLabel( tr( "Vitesse du courant:" ) )
                .AddLabel( tr( "Berges à aménager:" ) );

    display_->AddGroup( tr( "Camp" ) )
                .AddLabel( tr( "TC2:" ) );

    display_->AddGroup( tr( "Nuage/Zone NBC" ) )
                .AddLabel( tr( "Agent NBC:" ) );

    display_->AddGroup( tr( "ROTA" ) )
                .AddLabel( tr( "Danger:" ) )
                .AddLabel( tr( "Agents NBC:" ) );

    display_->AddGroup( tr( "Itinéraire logistique" ) )
                .AddLabel( tr( "Equipé:" ) )
                .AddLabel( tr( "Débit:" ) )
                .AddLabel( tr( "Largeur:" ) )
                .AddLabel( tr( "Longueur:" ) )
                .AddLabel( tr( "Poids supporté:" ) );

    QWidget* pSpacer = new QWidget( this );
    pSpacer->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );

    // $$$$ AGE 23/08/2006 :
//    QHBox* pHBox = new QHBox( this );
//    pApplyButton_  = new QPushButton( tr( "Appliquer" ), pHBox );
//    pCancelButton_ = new QPushButton( tr( "Annuler" )  , pHBox );

//    connect( pApplyButton_,  SIGNAL( clicked() ), this, SLOT( OnApply() ) );
//    connect( pCancelButton_, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel destructor
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
ObjectPanel::~ObjectPanel()
{
    controllers_.Remove( *this );
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::showEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void ObjectPanel::showEvent( QShowEvent* )
{
    const Object_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::UpdateExtension
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
template< typename T >
void ObjectPanel::UpdateExtension( const Object_ABC& object )
{
    const T* extension = object.Retrieve< T >();
    if( extension )
        NotifyUpdated( *extension );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifySelected
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ObjectPanel::NotifySelected( const Object_ABC* object )
{
    if( selected_ != object || ! object )
    {
        selected_ = object;
        if( selected_ )
        {
            // $$$$ AGE 2006-02-17: refactor that
            Show();
            display_->Hide();
            NotifyUpdated( *selected_ );
            UpdateExtension< CampAttributes >( *selected_ );
            UpdateExtension< CrossingSiteAttributes >( *selected_ );
            UpdateExtension< LogisticRouteAttributes >( *selected_ );
            UpdateExtension< NBCAttributes >( *selected_ );
            UpdateExtension< RotaAttributes >( *selected_ );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const Object_ABC& object )
{
    if( selected_  != &object || ! IsVisible() )
        return;

    object.Display( *display_ );
}

// $$$$ AGE 2006-02-17: Factor
// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyDeleted
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyDeleted( const Object_ABC& object )
{
    if( selected_ == &object )
        NotifySelected( 0 );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::ShouldUpdate
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
// $$$$ AGE 2006-02-17: Factor
template< typename Extension >
bool ObjectPanel::ShouldUpdate( const Extension& extension )
{
    return IsVisible()
        && selected_ 
        && ( selected_->Retrieve< Extension >() == & extension );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const CampAttributes& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    attributes.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const CrossingSiteAttributes& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    attributes.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const LogisticRouteAttributes& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;
    
    attributes.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const NBCAttributes& attributes )
{
    if( ! ShouldUpdate( attributes) )
        return;

    attributes.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const RotaAttributes& attributes )
{
     if( ! ShouldUpdate( attributes ) )
        return;

    attributes.Display( *display_ );
}

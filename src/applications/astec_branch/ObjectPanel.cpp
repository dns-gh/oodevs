// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-11 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectPanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 17:12 $
// $Revision: 7 $
// $Workfile: ObjectPanel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectPanel.h"

#include "Object_ABC.h"
#include "ObjectType.h"
#include "CampAttributes.h"
#include "CrossingSiteAttributes.h"
#include "LogisticRouteAttributes.h"
#include "NBCAttributes.h"
#include "RotaAttributes.h"
#include "Controller.h"
#include "ActionController.h"
#include "Agent.h"
#include "DotationType.h"

#include "DisplayBuilder.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include "Units.h"

#include <qgrid.h>

// -----------------------------------------------------------------------------
// Name: ObjectPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
ObjectPanel::ObjectPanel( InfoPanels* info, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC ( info, tr( "Objet" ) )
    , selected_( 0 )
{
    display_ = new DisplayBuilder( this );
    display_->AddGroup( "Informations" )
                .AddLabel( "Id:" )
                .AddLabel( "Nom:" )
                .AddLabel( "Type:" )
                .AddLabel( "Position:" )
                .AddSpinBox( "Construction:", 0, 100, 1 )
                .AddSpinBox( "Valorisation:", 0, 100, 1 )
                .AddSpinBox( "Contournement:", 0, 100, 1 )
                .AddCheckBox( "En préparation:" );

    display_->Group( "Informations" )
                .AddLabel( "Dotation construction:" )
                .AddLabel( "Dotation valorisation:" );

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

    // $$$$ SBO 2005-09-23: QSpacerItem is inserted as first element in layout...
    //layout()->addItem( new QSpacerItem( 100, 50, QSizePolicy::Minimum, QSizePolicy::Expanding ) );
    QWidget* pSpacer = new QWidget( this );
    pSpacer->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );

//    QHBox* pHBox = new QHBox( this );
//    pApplyButton_  = new QPushButton( tr( "Appliquer" ), pHBox );
//    pCancelButton_ = new QPushButton( tr( "Annuler" )  , pHBox );

//    connect( pApplyButton_,  SIGNAL( clicked() ), this, SLOT( OnApply() ) );
//    connect( pCancelButton_, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );

    controller.Register( *this );
    actionController.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel destructor
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
ObjectPanel::~ObjectPanel()
{
    // $$$$ AGE 2006-03-16: controller_.Remove
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
            if( selected_->Retrieve< CampAttributes >() )
                NotifyUpdated( selected_->Get< CampAttributes >() );

            if( selected_->Retrieve< CrossingSiteAttributes >() )
                NotifyUpdated( selected_->Get< CrossingSiteAttributes >() );

            if( selected_->Retrieve< LogisticRouteAttributes >() )
                NotifyUpdated( selected_->Get< LogisticRouteAttributes >() );

            if( selected_->Retrieve< NBCAttributes >() )
                NotifyUpdated( selected_->Get< NBCAttributes >() );

            if( selected_->Retrieve< RotaAttributes >() )
                NotifyUpdated( selected_->Get< RotaAttributes >() );
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

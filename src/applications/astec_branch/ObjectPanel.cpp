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
#include "InfoPanel.h"
#include "Agent.h"
#include "DotationType.h"

#include "DisplayBuilder.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include "Units.h"

#include "App.h"
#include "World.h"
#include <qgrid.h>

// -----------------------------------------------------------------------------
// Name: ObjectPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
ObjectPanel::ObjectPanel( InfoPanel* info, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC ( info, tr( "Objet" ) )
    , selected_( 0 )
{
    display_ = new DisplayBuilder( this );
    display_->AddGroup( "Informations" )
                .AddItem( "Id:" )
                .AddItem( "Nom:" )
                .AddItem( "Type:" )
                .AddItem( "Position:" );

    GroupDisplayer* group = (GroupDisplayer*)&display_->Group( "Informations" );
    new QLabel( "Construction:", group );
    pPercentBuiltEdit_  = new QSpinBox( 0, 100, 1, group ); pPercentBuiltEdit_->setSuffix( "%" );
    new QLabel( "Valeur:", group );
    pPercentValueEdit_  = new QSpinBox( 0, 100, 1, group ); pPercentValueEdit_->setSuffix( "%" );
    new QLabel( "Contournement:", group );
    pPercentAroundEdit_ = new QSpinBox( 0, 100, 1, group ); pPercentAroundEdit_->setSuffix( "%" );
    new QLabel( "En préparation:", group );
    pIsUnderPrepCheckBox_ = new QCheckBox( group );

    display_->Group( "Informations" )
                .AddItem( "Dotation construction:" )
                .AddItem( "Dotation valorisation:" );

    display_->AddGroup( "Site de franchissement" )
                .AddItem( "Largeur:" )
                .AddItem( "Profondeur:" )
                .AddItem( "Vitesse courant:" )
                .AddItem( "Berges à aménager:" );

    display_->AddGroup( "Camp" )
                .AddItem( "TC2:" );

    display_->AddGroup( "Nuage/Zone NBC" )
                .AddItem( "Agent NBC:" );

    display_->AddGroup( "ROTA" )
                .AddItem( "Danger:" )
                .AddItem( "Agents NBC:" );

    display_->AddGroup( "Itinéraire Logistique" )
                .AddItem( "Equipé:" )
                .AddItem( "Débit:" )
                .AddItem( "Largeur:" )
                .AddItem( "Longueur:" )
                .AddItem( "Poids supporté:" );

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
    delete display_;
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
            NotifyUpdated( *selected_ );
            if( selected_->Retrieve< CampAttributes >() )
                NotifyUpdated( selected_->Get< CampAttributes >() );
            else
                display_->Group( "Camp" ).hide();

            if( selected_->Retrieve< CrossingSiteAttributes >() )
                NotifyUpdated( selected_->Get< CrossingSiteAttributes >() );
            else
                display_->Group( "Site de franchissement" ).hide();

            if( selected_->Retrieve< LogisticRouteAttributes >() )
                NotifyUpdated( selected_->Get< LogisticRouteAttributes >() );
            else
                display_->Group( "Itinéraire Logistique" ).hide();

            if( selected_->Retrieve< NBCAttributes >() )
                NotifyUpdated( selected_->Get< NBCAttributes >() );
            else
                display_->Group( "Nuage/Zone NBC" ).hide();

            if( selected_->Retrieve< RotaAttributes >() )
                NotifyUpdated( selected_->Get< RotaAttributes >() );
            else
                display_->Group( "ROTA" ).hide();
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
    if( selected_  != &object || ! isVisible() )
        return;

    std::string strPos;
    App::GetApp().GetWorld().SimToMosMgrsCoord( object.center_, strPos );

    display_->Group( "Informations" )
                .Display( "Id:", object.nId_ )
                .Display( "Nom:", object.strName_ )
                .Display( "Type:", object.type_.GetName() )
                .Display( "Position:", strPos )
                .Item( "Dotation construction:" )
                    .Start( object.nDotationConstruction_ )
                    .Add( " " ).Add( object.construction_ ).End(); // $$$$ AGE 2006-02-22: End devrait renvoyer le parent
    display_->Group( "Informations" )
                .Item( "Dotation valorisation:" )
                    .Start( object.nDotationValorization_ )
                    .Add( object.valorization_ ).End();
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
    return isVisible()
        && selected_ 
        && ( selected_->Retrieve< Extension >() == & extension );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const CampAttributes& a )
{
    if( ! ShouldUpdate( a ) )
        return;

    display_->Group( "Camp" ).Display( "TC2:", a.tc2_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const CrossingSiteAttributes& a )
{
    if( ! ShouldUpdate( a ) )
        return;

    display_->Group( "Site de franchissement" )
                .Display( "Largeur:", a.width_ * Units::meters )
                .Display( "Profondeur:", a.depth_ * Units::meters )
                .Display( "Vitesse courant:", a.speed_ * Units::metersPerSecond )
                .Display( "Berges à aménager:", a.needsConstruction_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const LogisticRouteAttributes& a )
{
    if( ! ShouldUpdate( a ) )
        return;

    display_->Group( "Itinéraire Logistique" )
                .Display( "Equipé:", a.bLogRouteEquipped_ )
                .Display( "Débit:", a.nLogRouteFlow_ * Units::vehiclesPerHour )
                .Display( "Largeur:", a.nLogRouteWidth_ * Units::meters )
                .Display( "Longueur:", a.nLogRouteLength_ * Units::meters )
                .Display( "Poids supporté:", a.nLogRouteMaxWeight_ * Units::tons );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const NBCAttributes& a )
{
    if( ! ShouldUpdate( a ) )
        return;

    display_->Group( "Nuage/Zone NBC" )
        .Display( "Agent NBC:", a.nbcId_ ); // $$$$ AGE 2006-02-17: resolve
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const RotaAttributes& a )
{
    display_->Group( "ROTA" )
                .Display( "Danger:", a.danger_ )
                .Display( "Agents NBC:", a.agents_ );
}

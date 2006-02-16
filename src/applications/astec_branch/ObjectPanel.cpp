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
#include "CampAttributes.h"
#include "CrossingSiteAttributes.h"
#include "LogisticRouteAttributes.h"
#include "NBCAttributes.h"
#include "RotaAttributes.h"
#include "Controller.h"
#include "ActionController.h"

#include "Display.h"
#include "DisplayGroup.h"
#include "DisplayItem.h"
#include <qgrid.h>

// -----------------------------------------------------------------------------
// Name: ObjectPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
ObjectPanel::ObjectPanel( QWidget* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC ( pParent )
{
    display_ = new Display( this );
    display_->AddGroup( "Informations" )
                .AddItem( "Id:" )
                .AddItem( "Nom:" )
                .AddItem( "Type:" )
                .AddItem( "Position:" );

    DisplayGroup* group = &display_->Group( "Informations" );
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

//    controller.Register( *this );
//    actionController.Register( *this );
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

}

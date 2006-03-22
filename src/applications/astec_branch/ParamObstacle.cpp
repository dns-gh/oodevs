// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamObstacle.cpp $
// $Author: Ape $
// $Modtime: 29/11/04 10:55 $
// $Revision: 6 $
// $Workfile: ParamObstacle.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamObstacle.h"

#include "App.h"
#include "World.h"
#include "ActionContext.h"
#include "Tools.h"
#include "ParamLocation.h"

// -----------------------------------------------------------------------------
// Name: ParamObstacle constructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::ParamObstacle( ASN1T_MissionGenObject& asnObject, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional, bool bOutsideData )
    : QGroupBox     ( 2, Qt::Horizontal, strLabel.c_str(), pParent )
    , Param_ABC ( bOptional )
    , asnObject_    ( asnObject )
    , strMenuText_  ( strMenuText )
{
    new QLabel( tr( "Type:" ), this );
    /*
    pTypeCombo_ = new QComboBox( this );
    for( int n1 = 0; n1 < eNbrObjectType; ++n1 )
        pTypeCombo_->insertItem( ENT_Tr::ConvertFromObjectType( (E_ObjectType)n1 ).c_str(), n1 );
    */

    pTypeCombo_ = new MT_ValuedComboBox< ASN1T_EnumObjectType >( this );
    // sorted obstacle type list
    pTypeCombo_->setSorting( true );
    for( uint n = 0; n < eNbrObjectType; ++n )
        pTypeCombo_->AddItem( ENT_Tr::ConvertFromObjectType( (E_ObjectType)n ).c_str(), (ASN1T_EnumObjectType)n );


    new QLabel( tr( "Urgence:" ), this );
    pUrgencyCombo_ = new QComboBox( this );
    for( int n2 = 0; n2 < eNbrMissionGenUrgence; ++n2 )
        pUrgencyCombo_->insertItem( ENT_Tr::ConvertFromMissionGenUrgence( ( E_MissionGenUrgence )n2 ).c_str(), n2 );

    new QLabel( tr( "Sous type:" ), this );
    pPreliminaryCombo_ = new QComboBox( this );
    for( int n3 = 0; n3 < eNbrMissionGenSousTypeObstacle; ++n3 )
        pPreliminaryCombo_->insertItem( ENT_Tr::ConvertFromMissionGenSousTypeObstacle( ( E_MissionGenSousTypeObstacle )n3 ).c_str(), n3 );

    new QLabel( tr( "Priorité:" ), this );
    pPriorityCombo_ = new QComboBox( this );
    for( int n4 = 0; n4 < eNbrMissionGenPriorite; ++n4 )
        pPriorityCombo_->insertItem( ENT_Tr::ConvertFromMissionGenPriorite( ( E_MissionGenPriorite )n4 ).c_str(), n4 );

    bool bOutsideDataLocalisation = false;
    if( bOutsideData )
    {
        pTypeCombo_       ->SetCurrentItem( asnObject_.type_obstacle );
        pUrgencyCombo_    ->setCurrentItem( asnObject_.urgence       );
        pPreliminaryCombo_->setCurrentItem( asnObject_.preliminaire  );
        pPriorityCombo_   ->setCurrentItem( asnObject_.priorite      );
        bOutsideDataLocalisation = true;
    }
    else // Make sure the localization type is initialized.
        asnObject.pos_obstacle.type = EnumTypeLocalisation::point;

    new QLabel( tr( "Localisation:" ), this );
    if ( asnObject_.pos_obstacle.vecteur_point.n != 0 )
        bOutsideDataLocalisation = true;
    pLocation_ = new ParamLocation( asnObject_.pos_obstacle, "", strMenuText, this, bOptional, bOutsideDataLocalisation );
}


// -----------------------------------------------------------------------------
// Name: ParamObstacle destructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacle::~ParamObstacle()
{
}


// -----------------------------------------------------------------------------
// Name: ParamObstacle::FillRemotePopupMenu
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void ParamObstacle::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    pLocation_->FillRemotePopupMenu( popupMenu, context );
}


// -----------------------------------------------------------------------------
// Name: ParamObstacle::CheckValidity
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
bool ParamObstacle::CheckValidity()
{
    return pLocation_->CheckValidity();
}


// -----------------------------------------------------------------------------
// Name: ParamObstacle::WriteMsg
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void ParamObstacle::WriteMsg( std::stringstream& strMsg )
{
    //asnObject_.type_obstacle =  (ASN1T_EnumObjectType)pTypeCombo_->currentItem();
    asnObject_.type_obstacle =  (ASN1T_EnumObjectType)pTypeCombo_->GetValue();
    asnObject_.urgence =        (ASN1T_EnumMissionGenUrgence)pUrgencyCombo_->currentItem();
    asnObject_.preliminaire =   (ASN1T_EnumMissionGenSousTypeObstacle)pPreliminaryCombo_->currentItem();
    asnObject_.priorite =       (ASN1T_EnumMissionGenPriorite)pPriorityCombo_->currentItem();

    strMsg << this->title().latin1() << ": localisation [";

    pLocation_->WriteMsg( strMsg );

    strMsg << "]";
}

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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamObstacle.cpp $
// $Author: Ape $
// $Modtime: 29/11/04 10:55 $
// $Revision: 6 $
// $Workfile: MOS_ParamObstacle.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamObstacle.h"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_ActionContext.h"
#include "MOS_Tools.h"
#include "MOS_ParamLocation.h"

// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacle constructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
MOS_ParamObstacle::MOS_ParamObstacle( ASN1T_MissionGenObject& asnObject, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional, bool bOutsideData )
    : QGroupBox     ( 2, Qt::Horizontal, strLabel.c_str(), pParent )
    , MOS_Param_ABC ( bOptional )
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

    if( bOutsideData )
    {
        pTypeCombo_       ->SetCurrentItem( asnObject_.type_obstacle );
        pUrgencyCombo_    ->setCurrentItem( asnObject_.urgence       );
        pPreliminaryCombo_->setCurrentItem( asnObject_.preliminaire  );
        pPriorityCombo_   ->setCurrentItem( asnObject_.priorite      );
    }
    else // Make sure the localization type is initialized.
        asnObject.pos_obstacle.type = EnumTypeLocalisation::point;

    new QLabel( tr( "Localisation:" ), this );
    bool bOutsideDataLocalisation = false;
    if ( asnObject_.pos_obstacle.vecteur_point.n != 0 )
        bOutsideDataLocalisation = true;
    pLocation_ = new MOS_ParamLocation( asnObject_.pos_obstacle, "", strMenuText, this, bOptional, bOutsideDataLocalisation );
    //$$$$ pas de tr sur "Localisation:";
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacle destructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
MOS_ParamObstacle::~MOS_ParamObstacle()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacle::FillRemotePopupMenu
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void MOS_ParamObstacle::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    pLocation_->FillRemotePopupMenu( popupMenu, context );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacle::CheckValidity
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
bool MOS_ParamObstacle::CheckValidity()
{
    return pLocation_->CheckValidity();
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacle::WriteMsg
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void MOS_ParamObstacle::WriteMsg( std::stringstream& strMsg )
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

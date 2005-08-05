// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Obstacle.cpp $
// $Author: Nld $
// $Modtime: 21/03/05 16:54 $
// $Revision: 15 $
// $Workfile: MOS_MTH_Obstacle.cpp $
//
// *****************************************************************************


#include "MOS_Light_Pch.h"
#include "MOS_MTH_Obstacle.h"

#ifndef MOS_USE_INLINE
#   include "MOS_MTH_Obstacle.inl"
#endif

#include "MOS_MTH_Localisation.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_MTH_Localisation_ListView.h"
#include "MOS_PointListViewItem.h"

#include <qvgroupbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qcombobox.h>

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle constructor
/** @param  asnObjet 
    @param  pParent 
    @param  strName 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
MOS_MTH_Obstacle::MOS_MTH_Obstacle( ASN1T_MissionGenObject& asnObjet, QWidget* pParent, const std::string& strName )
: QWidget( pParent )
, pAgent_( 0 )
, genObject_( asnObjet )
{
    QVGroupBox* pBox = new QVGroupBox( strName.c_str(), this );

    // Combo boxes
    QGrid* pGrid = new QGrid( 2, pBox );

    QLabel* pLabel = new QLabel( "Type obstacle", pGrid );
    pTypeObstacleGUI_ = new QComboBox( pGrid );

    for( uint i = 0; i < eNbrObjectType; ++i )
        pTypeObstacleGUI_->insertItem( ENT_Tr::ConvertFromObjectType( (E_ObjectType)i ).c_str(), i );

    pLabel = new QLabel( "Urgence", pGrid );
    pUrgenceGUI_ = new QComboBox( pGrid );
    pUrgenceGUI_->insertItem( "Urgence", EnumMissionGenUrgence::serie_minimum );
    pUrgenceGUI_->insertItem( "Complémentaire", EnumMissionGenUrgence::serie_complementaire );
    pUrgenceGUI_->setCurrentItem( EnumMissionGenUrgence::serie_complementaire );

    pLabel = new QLabel( "Sous type", pGrid );
    pPreliminaireGUI_ = new QComboBox( pGrid );
    pPreliminaireGUI_->insertItem( "Préliminaire", EnumMissionGenSousTypeObstacle::preliminaire );
    pPreliminaireGUI_->insertItem( "De manoeuvre", EnumMissionGenSousTypeObstacle::de_manoeuvre );
    pPreliminaireGUI_->setCurrentItem( EnumMissionGenSousTypeObstacle::de_manoeuvre );

    pLabel = new QLabel( "Priorité", pGrid );
    pPrioriteGUI_ = new QComboBox( pGrid );
    pPrioriteGUI_->insertItem( "Peu prioritaire", EnumMissionGenPriorite::peu_prioritaire );
    pPrioriteGUI_->insertItem( "Normal", EnumMissionGenPriorite::normal );
    pPrioriteGUI_->insertItem( "Prioritaire", EnumMissionGenPriorite::prioritaire );
    pPrioriteGUI_->setCurrentItem( EnumMissionGenPriorite::normal );

    pLocalisation_ = new MOS_MTH_Localisation( this, "Localisation" );
    pLocalisation_->Initialize();

    pLocalisation_->GetListView().DeleteAllPoints();

    pLocalisation_->SetLocalisationType( EnumTypeLocalisation::point );

    // $$$$ AGE 2005-02-07: 
    pLocalisation_->GetListView().CreatePoint( MT_Vector2D( 0, 0 ) );

    pTypeObstacleGUI_->setCurrentItem( EnumObjectType::abattis );
    pUrgenceGUI_->setCurrentItem( EnumMissionGenUrgence::serie_minimum );
    pPreliminaireGUI_->setCurrentItem( EnumMissionGenSousTypeObstacle::de_manoeuvre );
    pPrioriteGUI_->setCurrentItem( EnumMissionGenPriorite::normal );
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle destructor
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
MOS_MTH_Obstacle::~MOS_MTH_Obstacle()
{
    
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle::SetMission
/** @param  pMission 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void MOS_MTH_Obstacle::SetMission( MOS_Mission_ABC* /*pMission*/ )
{
    // nothing
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle::SetWidgetTab
/** @param  pWidget 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void MOS_MTH_Obstacle::SetWidgetTab( QWidget* /*pWidget*/ )
{
    // nothing
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle::SetAgent
/** @param  pAgent 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void MOS_MTH_Obstacle::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;
}


// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle::WriteMsg
/** @param  sParam 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void MOS_MTH_Obstacle::WriteMsg( std::string& sParam )
{
    static int nDegueu = 0;
//    genObject_.oid_obstacle_planifie = pItem->GetId();
    genObject_.oid_obstacle_planifie = ++nDegueu;
    genObject_.preliminaire = (ASN1T_EnumMissionGenSousTypeObstacle)pPreliminaireGUI_->currentItem();
    genObject_.type_obstacle = (ASN1T_EnumObjectType)pTypeObstacleGUI_->currentItem();
    genObject_.urgence = (ASN1T_EnumMissionGenUrgence)pUrgenceGUI_->currentItem();
    genObject_.priorite = (ASN1T_EnumMissionGenPriorite)pPrioriteGUI_->currentItem();
    pLocalisation_->WriteMsg( genObject_.pos_obstacle, sParam );
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle::FillRandomParameters
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void MOS_MTH_Obstacle::FillRandomParameters()
{
    // TO DO
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle::ReloadParameters
/** @param  archive 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void MOS_MTH_Obstacle::ReloadParameters( MT_InputArchive_ABC& /*archive*/ )
{
    // TO DO
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle::ResaveParameters
/** @param  archive 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void MOS_MTH_Obstacle::ResaveParameters( MT_OutputArchive_ABC& /*archive*/ )
{
    // TO DO
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle::NotifyAgentHasMoved
/** @param  agent 
    @param  vPos 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void MOS_MTH_Obstacle::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle::Initialize
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void MOS_MTH_Obstacle::Initialize()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle::Terminate
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void MOS_MTH_Obstacle::Terminate()
{
    if( genObject_.pos_obstacle.vecteur_point.n > 0 )
        delete [] genObject_.pos_obstacle.vecteur_point.elem;
}

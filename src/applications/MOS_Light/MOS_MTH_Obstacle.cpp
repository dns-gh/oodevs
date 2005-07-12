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
    pTypeObstacleGUI_->insertItem( "Bouchon de mines", EnumObjectType::bouchon_mines );
    pTypeObstacleGUI_->insertItem( "Zone minée linéaire", EnumObjectType::zone_minee_lineaire );
    pTypeObstacleGUI_->insertItem( "Zone minée par dispersion", EnumObjectType::zone_minee_par_dispersion );
    pTypeObstacleGUI_->insertItem( "Fossé antichar", EnumObjectType::fosse_anti_char );
    pTypeObstacleGUI_->insertItem( "Abattis", EnumObjectType::abattis );
    pTypeObstacleGUI_->insertItem( "Barricade", EnumObjectType::barricade );
    pTypeObstacleGUI_->insertItem( "Eboulement", EnumObjectType::eboulement );
    pTypeObstacleGUI_->insertItem( "Destruction de route", EnumObjectType::destruction_route );
    pTypeObstacleGUI_->insertItem( "Destruction de pont", EnumObjectType::destruction_pont );
    pTypeObstacleGUI_->insertItem( "Ponts flottants", EnumObjectType::pont_flottant );
    pTypeObstacleGUI_->insertItem( "Postes de tir", EnumObjectType::poste_tir );
    pTypeObstacleGUI_->insertItem( "Zones protégées", EnumObjectType::zone_protegee );
    pTypeObstacleGUI_->insertItem( "Zones implantation canon", EnumObjectType::zone_implantation_canon );
    pTypeObstacleGUI_->insertItem( "Zones implantation COBRA", EnumObjectType::zone_implantation_cobra );
    pTypeObstacleGUI_->insertItem( "Zones implantation LRM", EnumObjectType::zone_implantation_lrm );
    pTypeObstacleGUI_->insertItem( "Sites de franchissement", EnumObjectType::site_franchissement );
    pTypeObstacleGUI_->insertItem( "Nuage NBC", EnumObjectType::nuage_nbc );
    pTypeObstacleGUI_->insertItem( "Site de décontamination", EnumObjectType::site_decontamination );
    pTypeObstacleGUI_->insertItem( "Plot de ravitaillement", EnumObjectType::plot_ravitaillement );
    pTypeObstacleGUI_->insertItem( "Zone de brouillage_brod", EnumObjectType::zone_brouillage_brod );
    pTypeObstacleGUI_->insertItem( "Rota", EnumObjectType::rota );
    pTypeObstacleGUI_->insertItem( "Zone NBC", EnumObjectType::zone_nbc );
	pTypeObstacleGUI_->insertItem( "Zone de brouillage_bromure", EnumObjectType::zone_brouillage_bromure );
	pTypeObstacleGUI_->insertItem( "Aire de Poser", EnumObjectType::aire_poser );
	pTypeObstacleGUI_->insertItem( "Piste", EnumObjectType::piste );
    pTypeObstacleGUI_->insertItem( "PlateForme", EnumObjectType::plateforme );
    pTypeObstacleGUI_->insertItem( "Zone à mobilité améliorée", EnumObjectType::zone_mobilite_amelioree );
	pTypeObstacleGUI_->insertItem( "Zone de poser hélicoptere", EnumObjectType::zone_poser_helicoptere );
    pTypeObstacleGUI_->insertItem( "Aire logistique", EnumObjectType::aire_logistique );
    pTypeObstacleGUI_->insertItem( "Itineraire Logistique", EnumObjectType::itineraire_logistique );
    pTypeObstacleGUI_->insertItem( "Camp Prisonniers", EnumObjectType::camp_prisonniers );
    pTypeObstacleGUI_->insertItem( "Camp Refugies", EnumObjectType::camp_refugies );
    pTypeObstacleGUI_->insertItem( "Poste Controle", EnumObjectType::poste_controle );
    pTypeObstacleGUI_->insertItem( "Terrain largage", EnumObjectType::terrain_largage );
    pTypeObstacleGUI_->insertItem( "Zone interdite au mouvement", EnumObjectType::zone_interdite_mouvement );
    pTypeObstacleGUI_->insertItem( "Zone interdite au tir", EnumObjectType::zone_interdite_tir );
    pTypeObstacleGUI_->insertItem( "Zone implantation mortier", EnumObjectType::zone_implantation_mortier );
    pTypeObstacleGUI_->setCurrentItem( EnumObjectType::abattis );

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

//*****************************************************************************
//
// $Created: AGN 03-04-14 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Mission_OC.cpp $
// $Author: Nld $
// $Modtime: 9/05/05 12:08 $
// $Revision: 45 $
// $Workfile: MOS_Mission_OC.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_Mission_OC.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Mission_OC.inl"
#endif

#include "MOS_MTH_GDH.h"
#include "MOS_ASN_Messages.h"

#include "MOS_ValueList.h"
#include "MOS_Value.h"
#include "MOS_MTH_Itineraire1Pos.h"
#include "MOS_MTH_ItinerairePos.h"
#include "MOS_MTH_DirectionPos.h"
#include "MOS_MTH_ObjectKnowledgeList.h"
#include "MOS_MTH_Localisation.h"
#include "MOS_MTH_Obstacle.h"
#include "MOS_Agent.h"
#include "MOS_AgentMission.h"
#include "MOS_MTH_ListLocalisation_ListView.h"
#include "MOS_MTH_NatureAtlas.h"
#include "MOS_MTH_MaintenancePriorities.h"
#include "MOS_MTH_MedicalPriorities.h"

//-----------------------------------------------------------------------------
// Name: MOS_Mission_OC constructor
// Created: AGN 03-04-14
//-----------------------------------------------------------------------------
MOS_Mission_OC::MOS_Mission_OC( E_FragOrder nTypeMission, QWidget* pParent )
    : MOS_Mission_ABC( (E_MissionID)(-1), pParent )
    , nTypeOC_( nTypeMission )
{
    pASNMsgOrder_ = new MOS_ASN_MsgOrderConduite();
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_OC destructor
// Created: AGN 03-04-14
//-----------------------------------------------------------------------------
MOS_Mission_OC::~MOS_Mission_OC()
{
    delete pASNMsgOrder_;
}



//-----------------------------------------------------------------------------
// Name: MOS_Mission_OC::Initialize
// Created: AGN 03-04-14
//-----------------------------------------------------------------------------
void MOS_Mission_OC::Initialize()
{
    MOS_Mission_ABC::Initialize();

    switch( nTypeOC_ )
    {
        case eOrdreConduite_ChangerReglesEngagement              : InitializeChangeReglesEngagement(); break;
        case eOrdreConduite_Pion_AppliquerFeux                   : InitializePionAppliquerFeux    (); break;
        case eOrdreConduite_Pion_ChangerDePosition               : InitializePionChangerDePosition(); break;       
        case eOrdreConduite_AcquerirObjectif                    : InitializeAcquerirObjectif(); break;
        case eOrdreConduite_ChangerPositionDebarquement          : InitializeChangerPositionDebarquement(); break;
        case eOrdreConduite_Pion_RenforcerEnVSRAM               : InitializePionRenforcerEnVSRAM(); break;
        case eOrdreConduite_Pion_TransfererVSRAM                : InitializePionTransfererVSRAM(); break;
        case eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM        : InitializePionReprendreAuxOrdresVSRAM(); break;
        case eOrdreConduite_ModifierTempsBordeeMaintenance            : InitializeModifierTempsBordeeMaintenance(); break;
        case eOrdreConduite_ModifierTempsBordeeSante        : InitializeModifierTempsBordeeSante(); break;
        case eOrdreConduite_ModifierPrioritesReparations    : InitializeModifierPrioritesReparations(); break;
        case eOrdreConduite_ModifierPrioritesBlesses       : InitializeModifierPrioritesBlesses(); break;
        case eOrdreConduite_ModifierPrioritesTactiquesBlesses     : InitializeModifierPrioritesTactiquesBlesses(); break;
        case eOrdreConduite_ModifierPrioritesTactiquesReparations : InitializeModifierPrioritesTactiquesReparations(); break;

        case eOrdreConduite_Automate_RealiserVariantement        : InitializeAutomateRealiserVariantement(); break;
        case eOrdreConduite_Automate_DesequiperSiteFranchissement: InitializeAutomateDesesquiperSiteFranchissement(); break;
        case eOrdreConduite_Automate_ReagirFaceAEni              : InitializeAutomateReagirFaceAEni(); break;
        case eOrdreConduite_Automate_AffecterPionAObstacle       : InitializeAutomateAffecterPionAObstacle(); break;
        case eOrdreConduite_Automate_ReconnaitreZoneImplantation : InitializeAutomateReconnaitreZoneImplantation  (); break;
        case eOrdreConduite_Automate_TC2_GererMaterielAvantDeplacement  : InitializeAutomateTC2GererMaterielAvantDeplacement(); break;
        case eOrdreConduite_Automate_ActiverObstacle            : InitializeAutomateActiverObstacle();   break;
        case eOrdreConduite_Automate_TransfererRenforts         : InitializeAutomateTransfererRenforts(); break;


        default:
            assert( false );
    }

    for ( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
        (*itMissionTool)->Initialize();
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializePionAppliquerFeux
// Created: AGN 03-04-14
//-----------------------------------------------------------------------------
void MOS_Mission_OC::InitializePionAppliquerFeux()
{
    ASN1T_OrderConduite_Pion_AppliquerFeux& asnMission = *new ASN1T_OrderConduite_Pion_AppliquerFeux();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_pion_appliquer_feux;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_appliquer_feux = &asnMission;

    MOS_Value* pValue = 0;
    MOS_ValueButtonList<ASN1T_EnumTypeMunitionTirIndirect>*  pLockSelector = &CreateVarList( asnMission.munitions, "Munitions", 2, 3, false );
    pLockSelector->CreateValue( "Obus explosif" , EnumTypeMunitionTirIndirect::explosif,  true );
    pLockSelector->CreateValue( "Obus grenade"  , EnumTypeMunitionTirIndirect::grenade );
    pLockSelector->CreateValue( "Obus ACED"     , EnumTypeMunitionTirIndirect::aced );
    pLockSelector->CreateValue( "Obus fumigène" , EnumTypeMunitionTirIndirect::fumigene );
    pLockSelector->CreateValue( "Obus éclairant", EnumTypeMunitionTirIndirect::eclairant );
    pLockSelector->CreateValue( "Obus mine"     , EnumTypeMunitionTirIndirect::mine );

    MOS_ValueList* pParameters_ = &CreateValueList( "", 1, 2 );
    pValue = pParameters_->CreateValue( asnMission.nb_IT, "Nombre d'IT", MOSToolValueTitle | MOSToolValueText | MOSToolValueSlide | MOSToolValueModif );
    pValue->SetValueMaxWidget( 24 );

    CreatePoint( asnMission.cible, "Cible" );    
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeChangeReglesEngagement
// Created: NLD 2005-01-17
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeChangeReglesEngagement()
{
//    ASN1T_OrderConduite_ChangerReglesEngagement& asnMission = *new ASN1T_OrderConduite_ChangerReglesEngagement();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_changer_regles_engagement;
//    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_changer_regles_engagement = &asnMission;
   
    MOS_ValueButtonList<ASN1T_EnumReglesEngagement>*  pLockSelector = &CreateVarList( pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_changer_regles_engagement, "Roe", 2, 3, false );
    pLockSelector->CreateValue( "Tir interdit" , EnumReglesEngagement::tir_interdit,  true );
    pLockSelector->CreateValue( "Tir libre"    , EnumReglesEngagement::tir_libre );
    pLockSelector->CreateValue( "Tir sur riposte", EnumReglesEngagement::tir_sur_riposte );
}



// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeAcquerirObjectif
// Created: NLD 2003-09-15
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeAcquerirObjectif()
{
    ASN1T_OrderConduite_AcquerirObjectif& asnMission = *new ASN1T_OrderConduite_AcquerirObjectif();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_acquerir_objectif;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_acquerir_objectif = &asnMission;
    
    CreateLocation( asnMission, "Objectif" );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializePionChangerDePosition
// Created: NLD 2003-09-15
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializePionChangerDePosition()
{
    ASN1T_OrderConduite_Pion_ChangerDePosition& asnMission = *new ASN1T_OrderConduite_Pion_ChangerDePosition();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_pion_changer_de_position;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_changer_de_position = &asnMission;
    
    CreatePoint( asnMission, "Position" );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializePionRenforcerEnVSRAM
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializePionRenforcerEnVSRAM()
{
    ASN1T_OrderConduite_Pion_RenforcerEnVSRAM& asnMission = *new ASN1T_OrderConduite_Pion_RenforcerEnVSRAM();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_pion_renforcer_en_vs_ram;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_renforcer_en_vs_ram = &asnMission;

    CreateAgent( asnMission.pion_a_renforcer , "Pion à renforcer (TC2)" );
    MOS_ValueList* pParameters_ = &CreateValueList( "", 1, 2 );
    MOS_Value* pValue = pParameters_->CreateValue( asnMission.nbr_ambulances, "Nombre d'ambulances", MOSToolValueTitle | MOSToolValueText | MOSToolValueSlide | MOSToolValueModif );
    pValue->SetValueMinWidget( 0 );
    pValue->SetValueWidget   ( 1 );
    pValue->SetValueMaxWidget( 255 );
}
 
// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializePionTransfererVSRAM
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializePionTransfererVSRAM()
{
    ASN1T_OrderConduite_Pion_TransfererVSRAM& asnMission = *new ASN1T_OrderConduite_Pion_TransfererVSRAM();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_pion_transferer_vs_ram;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_transferer_vs_ram = &asnMission;

    CreateAgent( asnMission.pion_renforce , "Pion renforcé (TC2)" );
    CreateAgent( asnMission.pion_a_renforcer , "Pion à renforcer (TC2)" );    
    MOS_ValueList* pParameters_ = &CreateValueList( "", 1, 2 );
    MOS_Value* pValue = pParameters_->CreateValue( asnMission.nbr_ambulances, "Nombre d'ambulances", MOSToolValueTitle | MOSToolValueText | MOSToolValueSlide | MOSToolValueModif );
    pValue->SetValueMinWidget( 0 );
    pValue->SetValueWidget   ( 1 );
    pValue->SetValueMaxWidget( 255 );
}
 
// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializePionReprendreAuxOrdresVSRAM
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializePionReprendreAuxOrdresVSRAM()
{
    ASN1T_OrderConduite_Pion_ReprendreAuxOrdresVSRAM& asnMission = *new ASN1T_OrderConduite_Pion_ReprendreAuxOrdresVSRAM();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_pion_reprendre_aux_ordres_vs_ram;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_reprendre_aux_ordres_vs_ram = &asnMission;

    CreateAgent( asnMission.pion_renforce , "Pion renforcé (TC2)" );
    MOS_ValueList* pParameters_ = &CreateValueList( "", 1, 2 );
    MOS_Value* pValue = pParameters_->CreateValue( asnMission.nbr_ambulances, "Nombre d'ambulances", MOSToolValueTitle | MOSToolValueText | MOSToolValueSlide | MOSToolValueModif );
    pValue->SetValueMinWidget( 0 );
    pValue->SetValueWidget   ( 1 );
    pValue->SetValueMaxWidget( 255 );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeModifierTempsBordeeMaintenance
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeModifierTempsBordeeMaintenance()
{
//    ASN1T_OrderConduite_ModifierTempsBordee& asnMission = *new ASN1T_OrderConduite_ModifierTempsBordee();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_temps_bordee_maintenance;
//    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_temps_bordee = asnMission;

    MOS_ValueButtonList<ASN1T_EnumTempsBordee>*  pLockSelector = &CreateVarList( pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_temps_bordee_maintenance, "Temps de bordee", 2, 3, false );
    pLockSelector->CreateValue( "8 heures" , EnumTempsBordee::temps_8_heures,  true );
    pLockSelector->CreateValue( "12 heures" , EnumTempsBordee::temps_12_heures );
    pLockSelector->CreateValue( "16 heures" , EnumTempsBordee::temps_16_heures );
    pLockSelector->CreateValue( "20 heures" , EnumTempsBordee::temps_20_heures );
    pLockSelector->CreateValue( "24 heures" , EnumTempsBordee::temps_24_heures );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeModifierTempsBordeeSante
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeModifierTempsBordeeSante()
{
//    ASN1T_OrderConduite_ModifierTempsBordee& asnMission = *new ASN1T_OrderConduite_ModifierTempsBordee();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_temps_bordee_sante;
//    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_temps_bordee = asnMission;

    MOS_ValueButtonList<ASN1T_EnumTempsBordee>*  pLockSelector = &CreateVarList( pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_temps_bordee_sante, "Temps de bordee", 2, 3, false );
    pLockSelector->CreateValue( "8 heures" , EnumTempsBordee::temps_8_heures,  true );
    pLockSelector->CreateValue( "12 heures" , EnumTempsBordee::temps_12_heures );
    pLockSelector->CreateValue( "16 heures" , EnumTempsBordee::temps_16_heures );
    pLockSelector->CreateValue( "20 heures" , EnumTempsBordee::temps_20_heures );
    pLockSelector->CreateValue( "24 heures" , EnumTempsBordee::temps_24_heures );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeModifierPrioritesReparations
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeModifierPrioritesReparations()
{
    ASN1T_OrderConduite_ModifierPrioritesReparations& asnMission = *new ASN1T_OrderConduite_ModifierPrioritesReparations();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_reparations;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_reparations = &asnMission;

    CreateMaintenancePriorities( asnMission, "Priorities" );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeModifierPrioritesBlesses
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeModifierPrioritesBlesses()
{
    ASN1T_OrderConduite_ModifierPrioritesBlesses& asnMission = *new ASN1T_OrderConduite_ModifierPrioritesBlesses();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_blesses;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_blesses = &asnMission;

    CreateMedicalPriorities( asnMission, "Priorites" );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeModifierPrioritesTactiquesBlesses
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeModifierPrioritesTactiquesBlesses()
{
    ASN1T_OrderConduite_ModifierPrioritesTactiquesBlesses& asnMission = *new ASN1T_OrderConduite_ModifierPrioritesTactiquesBlesses();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_tactiques_blesses;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_tactiques_blesses = &asnMission;

    CreateAutomateList( (ASN1T_ListAgent&)asnMission, "Priorites" );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeModifierPrioritesTactiquesReparations
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeModifierPrioritesTactiquesReparations()
{
    ASN1T_OrderConduite_ModifierPrioritesTactiquesReparations& asnMission = *new ASN1T_OrderConduite_ModifierPrioritesTactiquesReparations();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_tactiques_reparations;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_tactiques_reparations = &asnMission;

    CreateAutomateList( (ASN1T_ListAgent&)asnMission, "Priorites" );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeAutomateRealiserVariantement
// Created: NLD 2004-04-26
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeAutomateRealiserVariantement()
{
    ASN1T_OrderConduite_Automate_RealiserVariantement& asnMission = *new ASN1T_OrderConduite_Automate_RealiserVariantement();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_realiser_variantement;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_realiser_variantement = &asnMission;

    CreateObjectKnowledge( asnMission.site_franchissement_original, "Site franchissement original" );
    CreateObjectKnowledge( asnMission.site_franchissement_variante, "Site franchissement varianté" );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeAutomateDesesquiperSiteFranchissement
// Created: NLD 2004-04-26
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeAutomateDesesquiperSiteFranchissement()
{
    ASN1T_OrderConduite_Automate_DesequiperSiteFranchissement& asnMission = *new ASN1T_OrderConduite_Automate_DesequiperSiteFranchissement();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_desequiper_site_franchissement;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_desequiper_site_franchissement = &asnMission;

    CreateObjectKnowledge( asnMission.site_franchissement, "Site franchissement" );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeAutomateReagirFaceAEni
// Created: NLD 2004-04-26
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeAutomateReagirFaceAEni()
{
    ASN1T_OrderConduite_Automate_ReagirFaceAEni& asnMission = *new ASN1T_OrderConduite_Automate_ReagirFaceAEni();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_reagir_face_a_eni;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_reagir_face_a_eni = &asnMission;

    CreateObjectKnowledge( asnMission.site_franchissement, "Site franchissement" );
    MOS_ValueButtonList< ASN1T_EnumActionReagirFaceAEni >* pAction = &CreateVarList( asnMission.action, "Action", 1, 3, false );
    pAction->CreateValue( "Aussitot Prêt"  , EnumActionReagirFaceAEni::detruire_moyens, true );
    pAction->CreateValue( "HSO"            , EnumActionReagirFaceAEni::defendre );
    pAction->CreateValue( "Au Commandement", EnumActionReagirFaceAEni::demonter );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeAutomateAffecterPionAObstacle
// Created: NLD 2004-04-26
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeAutomateAffecterPionAObstacle()
{
    ASN1T_OrderConduite_Automate_AffecterPionAObstacle& asnMission = *new ASN1T_OrderConduite_Automate_AffecterPionAObstacle();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_affecter_pion_a_obstacle;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_affecter_pion_a_obstacle = &asnMission;

    CreateAgent( asnMission.pion, "Agent" );   
    CreateGenObject( asnMission.obstacle, "Obstacle" );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeAutomateReconnaitreZoneImplantation
// Created: NLD 2004-05-11
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeAutomateReconnaitreZoneImplantation()
{
    ASN1T_OrderConduite_Automate_ReconnaitreZoneImplantation& asnMission = *new ASN1T_OrderConduite_Automate_ReconnaitreZoneImplantation();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_reconnaitre_zone_implantation;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_reconnaitre_zone_implantation = &asnMission;

    CreatePoint( asnMission, "Position" );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeAutomateTC2GererMaterielAvantDeplacement
// Created: NLD 2005-03-09
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeAutomateTC2GererMaterielAvantDeplacement()
{
//    ASN1T_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement& asnMission = *new ASN1T_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_tc2_gerer_materiel_avant_deplacement;
//    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_tc2_gerer_materiel_avant_deplacement = &asnMission;

    MOS_ValueButtonList< ASN1T_EnumTC2GererMaterielAvantDeplacement >* pAction = &CreateVarList( pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_tc2_gerer_materiel_avant_deplacement, "Mode gestion materiel" , 1, 3, false );
    pAction->CreateValue( "Deborder materiels vers BLD"     , EnumTC2GererMaterielAvantDeplacement::deborder_materiels_vers_bld, true );
    pAction->CreateValue( "Detruire materiels en réparation", EnumTC2GererMaterielAvantDeplacement::detruire_materiels_en_reparation  );
    pAction->CreateValue( "Réparer materiels sur place"     , EnumTC2GererMaterielAvantDeplacement::reparer_materiels_sur_place       );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeAutomateTransfererRenforts
// Created: NLD 2005-05-09
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeAutomateTransfererRenforts()
{
    ASN1T_OrderConduite_Automate_TransfererRenforts& asnMission = *new ASN1T_OrderConduite_Automate_TransfererRenforts();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_transferer_renforts;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_transferer_renforts = &asnMission;

    CreateAgent( asnMission.pion_a_transferer, "Pion à transférer" );   
    CreateAgent( asnMission.automate_a_renforcer, "Automate à renforcer" );
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeAutomateActiverObstacle
// Created: NLD 2005-03-09
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeAutomateActiverObstacle()
{
//    ASN1T_OrderConduite_Automate_ActiverObstacle& asnMission = *new ASN1T_OrderConduite_Automate_ActiverObstacle();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_activer_obstacle;
//    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_activer_obstacle = &asnMission;

    CreateObjectKnowledge( pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_activer_obstacle, "Obstacle" );
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_OC::InitializeChangerPositionDebarquement
// Created: NLD 2003-09-15
// -----------------------------------------------------------------------------
void MOS_Mission_OC::InitializeChangerPositionDebarquement()
{
    ASN1T_OrderConduite_ChangerPositionDebarquement& asnMission = *new ASN1T_OrderConduite_ChangerPositionDebarquement();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_changer_position_debarquement;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_changer_position_debarquement = &asnMission;
    
    CreatePoint( asnMission, "Position" );
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_OC::Terminate
// Created: AGN 03-04-14
//-----------------------------------------------------------------------------
void MOS_Mission_OC::Terminate()
{
    MOS_Mission_ABC::Terminate();
    switch( pASNMsgOrder_->GetAsnMsg().order_conduite.t )
    {
//        case T_MsgOrderConduite_order_conduite_order_conduite_changer_regles_engagement                 : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_changer_regles_engagement; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_pion_renforcer_en_vs_ram                  : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_renforcer_en_vs_ram; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_pion_transferer_vs_ram                    : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_transferer_vs_ram; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_pion_reprendre_aux_ordres_vs_ram          : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_reprendre_aux_ordres_vs_ram; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_changer_position_debarquement             : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_changer_position_debarquement; break;
//        case T_MsgOrderConduite_order_conduite_order_conduite_pion_modifier_temps_bordee                : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_modifier_temps_bordee; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_reparations            : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_reparations; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_blesses                : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_blesses; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_tactiques_reparations  : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_tactiques_reparations; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_tactiques_blesses      : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_tactiques_blesses; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_pion_appliquer_feux                       : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_appliquer_feux; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_pion_changer_de_position                  : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_changer_de_position; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_automate_realiser_variantement            : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_realiser_variantement; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_automate_desequiper_site_franchissement   : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_desequiper_site_franchissement; break; 
        case T_MsgOrderConduite_order_conduite_order_conduite_automate_reagir_face_a_eni                : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_reagir_face_a_eni; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_automate_affecter_pion_a_obstacle         : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_affecter_pion_a_obstacle; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_automate_reconnaitre_zone_implantation    : delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_reconnaitre_zone_implantation; break;
        case T_MsgOrderConduite_order_conduite_order_conduite_acquerir_objectif         :                 delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_acquerir_objectif ; break;
        default: 
            assert( false );
    }    
}



//-----------------------------------------------------------------------------
// Name: MOS_Mission_OC::SetAgent
// Created: AGN 03-04-14
//-----------------------------------------------------------------------------
void MOS_Mission_OC::SetAgent( MOS_Agent* pAgent )
{
    MOS_Mission_ABC::SetAgent( pAgent );
    if( pAgent )
        pASNMsgOrder_->GetAsnMsg().unit_id = pAgent->GetAgentID();    
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_OC::SendMsg
// Created: AGN 03-04-14
//-----------------------------------------------------------------------------
void MOS_Mission_OC::SendMsg( std::string& sParam ) 
{
    for ( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
        (*itMissionTool)->WriteMsg( sParam );    

    pASNMsgOrder_->GetAsnMsg().order_id = 43;

    pASNMsgOrder_->Send( 36999 );
}



//-----------------------------------------------------------------------------
// Name: MOS_Mission_OC::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_Mission_OC::FillRandomParameters()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_OC::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_Mission_OC::ReloadParameters( MT_InputArchive_ABC& /*archive*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_OC::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_Mission_OC::ResaveParameters( MT_OutputArchive_ABC& /*archive*/ )
{
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::GetName
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
std::string MOS_Mission_OC::GetName() const
{
//    return MOS_Tools::GetOrderDisplay( nTypeOC_ );
    return ENT_Tr::ConvertFromFragOrder( nTypeOC_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::IsAutomateMission
// Created: APE 2004-03-11
// -----------------------------------------------------------------------------
bool MOS_Mission_OC::IsAutomateMission() const
{
    return false;
}

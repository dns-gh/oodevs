// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE  $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MOS_FragmentaryOrderInterface_Gen_Skeleton.cpp $
// $Author: Ape $
// $Modtime: 25/10/04 10:45 $
// $Revision: 4 $
// $Workfile: AGR_MOS_FragmentaryOrderInterface_Gen_Skeleton.cpp $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface destructor
// Created: AGR
// -----------------------------------------------------------------------------
MOS_FragmentaryOrderInterface::~MOS_FragmentaryOrderInterface()
{
    switch( pASNMsgOrder_->GetAsnMsg().order_conduite.t )
    {
        case T_MsgOrderConduite_order_conduite_order_conduite_pion_appliquer_feux :
             delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_appliquer_feux;
             break;
        case T_MsgOrderConduite_order_conduite_order_conduite_pion_renforcer_en_vs_ram :
             delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_renforcer_en_vs_ram;
             break;
        case T_MsgOrderConduite_order_conduite_order_conduite_pion_transferer_vs_ram :
             delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_transferer_vs_ram;
             break;
        case T_MsgOrderConduite_order_conduite_order_conduite_pion_reprendre_aux_ordres_vs_ram :
             delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_reprendre_aux_ordres_vs_ram;
             break;
        case T_MsgOrderConduite_order_conduite_order_conduite_automate_realiser_variantement :
             delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_realiser_variantement;
             break;
        case T_MsgOrderConduite_order_conduite_order_conduite_automate_desequiper_site_franchissement :
             delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_desequiper_site_franchissement;
             break;
        case T_MsgOrderConduite_order_conduite_order_conduite_automate_reagir_face_a_eni :
             delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_reagir_face_a_eni;
             break;
        case T_MsgOrderConduite_order_conduite_order_conduite_automate_affecter_pion_a_obstacle :
             delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_affecter_pion_a_obstacle;
             break;
        case T_MsgOrderConduite_order_conduite_order_conduite_automate_transferer_renforts :
             delete pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_transferer_renforts;
             break;

    }
}


// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateInterface
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateInterface()
{
    switch( nMissionId_ )
    {
        case eOrdreConduite_MettreTenueNBC :
             CreateOrder_MettreTenueNBC();
             break;
        case eOrdreConduite_EnleverTenueNBC :
             CreateOrder_EnleverTenueNBC();
             break;
        case eOrdreConduite_PasserEnSilenceRadio :
             CreateOrder_PasserEnSilenceRadio();
             break;
        case eOrdreConduite_ArreterSilenceRadio :
             CreateOrder_ArreterSilenceRadio();
             break;
        case eOrdreConduite_PasserEnSilenceRadar :
             CreateOrder_PasserEnSilenceRadar();
             break;
        case eOrdreConduite_ArreterSilenceRadar :
             CreateOrder_ArreterSilenceRadar();
             break;
        case eOrdreConduite_RecupererTransporteurs :
             CreateOrder_RecupererTransporteurs();
             break;
        case eOrdreConduite_AttendreSePoster :
             CreateOrder_AttendreSePoster();
             break;
        case eOrdreConduite_Poursuivre :
             CreateOrder_Poursuivre();
             break;
        case eOrdreConduite_Interrompre :
             CreateOrder_Interrompre();
             break;
        case eOrdreConduite_ChangerReglesEngagement :
             CreateOrder_ChangerReglesEngagement();
             break;
        case eOrdreConduite_Deboucher :
             CreateOrder_Deboucher();
             break;
        case eOrdreConduite_Decrocher :
             CreateOrder_Decrocher();
             break;
        case eOrdreConduite_AcquerirObjectif :
             CreateOrder_AcquerirObjectif();
             break;
        case eOrdreConduite_Brouiller :
             CreateOrder_Brouiller();
             break;
        case eOrdreConduite_ChangerPositionDebarquement :
             CreateOrder_ChangerPositionDebarquement();
             break;
        case eOrdreConduite_Pion_ChangerDePosition :
             CreateOrder_Pion_ChangerDePosition();
             break;
        case eOrdreConduite_Pion_Contourner :
             CreateOrder_Pion_Contourner();
             break;
        case eOrdreConduite_Pion_EnvoyerVehicule :
             CreateOrder_Pion_EnvoyerVehicule();
             break;
        case eOrdreConduite_Pion_ActiverObstacle :
             CreateOrder_Pion_ActiverObstacle();
             break;
        case eOrdreConduite_Pion_AppliquerFeux :
             CreateOrder_Pion_AppliquerFeux();
             break;
        case eOrdreConduite_Pion_ExecuterTir :
             CreateOrder_Pion_ExecuterTir();
             break;
        case eOrdreConduite_Pion_Emettre :
             CreateOrder_Pion_Emettre();
             break;
        case eOrdreConduite_Pion_StopperEmission :
             CreateOrder_Pion_StopperEmission();
             break;
        case eOrdreConduite_Pion_RenforcerEnVSRAM :
             CreateOrder_Pion_RenforcerEnVSRAM();
             break;
        case eOrdreConduite_Pion_TransfererVSRAM :
             CreateOrder_Pion_TransfererVSRAM();
             break;
        case eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM :
             CreateOrder_Pion_ReprendreAuxOrdresVSRAM();
             break;
        case eOrdreConduite_ModifierTempsBordeeMaintenance :
             CreateOrder_ModifierTempsBordeeMaintenance();
             break;
        case eOrdreConduite_ModifierTempsBordeeSante :
             CreateOrder_ModifierTempsBordeeSante();
             break;
        case eOrdreConduite_ModifierPrioritesReparations :
             CreateOrder_ModifierPrioritesReparations();
             break;
        case eOrdreConduite_ModifierPrioritesBlesses :
             CreateOrder_ModifierPrioritesBlesses();
             break;
        case eOrdreConduite_ModifierPrioritesTactiquesReparations :
             CreateOrder_ModifierPrioritesTactiquesReparations();
             break;
        case eOrdreConduite_ModifierPrioritesTactiquesBlesses :
             CreateOrder_ModifierPrioritesTactiquesBlesses();
             break;
        case eOrdreConduite_Automate_RealiserVariantement :
             CreateOrder_Automate_RealiserVariantement();
             break;
        case eOrdreConduite_Automate_DesequiperSiteFranchissement :
             CreateOrder_Automate_DesequiperSiteFranchissement();
             break;
        case eOrdreConduite_Automate_ReagirFaceAEni :
             CreateOrder_Automate_ReagirFaceAEni();
             break;
        case eOrdreConduite_Automate_AffecterPionAObstacle :
             CreateOrder_Automate_AffecterPionAObstacle();
             break;
        case eOrdreConduite_Automate_ReconnaitreZoneImplantation :
             CreateOrder_Automate_ReconnaitreZoneImplantation();
             break;
        case eOrdreConduite_Automate_TC2_GererMaterielAvantDeplacement :
             CreateOrder_Automate_TC2_GererMaterielAvantDeplacement();
             break;
        case eOrdreConduite_Automate_ActiverObstacle :
             CreateOrder_Automate_ActiverObstacle();
             break;
        case eOrdreConduite_Automate_TransfererRenforts :
             CreateOrder_Automate_TransfererRenforts();
             break;

        default:
            assert( false );
    }

    this->CreateOkCancelButtons();
}


// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_MettreTenueNBC
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_MettreTenueNBC()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_EnleverTenueNBC
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_EnleverTenueNBC()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_PasserEnSilenceRadio
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_PasserEnSilenceRadio()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_ArreterSilenceRadio
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_ArreterSilenceRadio()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_PasserEnSilenceRadar
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_PasserEnSilenceRadar()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_ArreterSilenceRadar
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_ArreterSilenceRadar()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_RecupererTransporteurs
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_RecupererTransporteurs()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_AttendreSePoster
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_AttendreSePoster()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Poursuivre
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Poursuivre()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Interrompre
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Interrompre()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_ChangerReglesEngagement
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_ChangerReglesEngagement()
{
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_changer_regles_engagement;
#define asnMission pASNMsgOrder_->GetAsnMsg().order_conduite.u
    MOS_ParamComboBox< ASN1T_EnumReglesEngagement >* pSelector_order_conduite_changer_regles_engagement = &CreateVarList( asnMission.order_conduite_changer_regles_engagement, "Order conduite changer regles engagement" );
    pSelector_order_conduite_changer_regles_engagement->AddItem( "None", EnumReglesEngagement::none );
    pSelector_order_conduite_changer_regles_engagement->AddItem( "Libre", EnumReglesEngagement::tir_libre );
    pSelector_order_conduite_changer_regles_engagement->AddItem( "Sur riposte", EnumReglesEngagement::tir_sur_riposte );
    pSelector_order_conduite_changer_regles_engagement->AddItem( "Interdit", EnumReglesEngagement::tir_interdit );
#undef asnMission
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Deboucher
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Deboucher()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Decrocher
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Decrocher()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_AcquerirObjectif
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_AcquerirObjectif()
{
    ASN1T_OrderConduite_AcquerirObjectif& asnMission = *new ASN1T_OrderConduite_AcquerirObjectif();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_acquerir_objectif;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_acquerir_objectif = &asnMission;
    CreateLocation( asnMission, "Order conduite acquerir objectif" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Brouiller
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Brouiller()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_ChangerPositionDebarquement
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_ChangerPositionDebarquement()
{
    ASN1T_OrderConduite_ChangerPositionDebarquement& asnMission = *new ASN1T_OrderConduite_ChangerPositionDebarquement();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_changer_position_debarquement;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_changer_position_debarquement = &asnMission;
    CreatePoint( asnMission, "Order conduite changer position debarquement" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Pion_ChangerDePosition
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Pion_ChangerDePosition()
{
    ASN1T_OrderConduite_Pion_ChangerDePosition& asnMission = *new ASN1T_OrderConduite_Pion_ChangerDePosition();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_pion_changer_de_position;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_changer_de_position = &asnMission;
    CreatePoint( asnMission, "Order conduite pion changer de position" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Pion_Contourner
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Pion_Contourner()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Pion_EnvoyerVehicule
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Pion_EnvoyerVehicule()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Pion_ActiverObstacle
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Pion_ActiverObstacle()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Pion_AppliquerFeux
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Pion_AppliquerFeux()
{
    ASN1T_OrderConduite_Pion_AppliquerFeux& asnMission = *new ASN1T_OrderConduite_Pion_AppliquerFeux();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_pion_appliquer_feux;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_appliquer_feux = &asnMission;
    MOS_ParamComboBox< ASN1T_EnumTypeMunitionTirIndirect >* pSelector_munitions = &CreateVarList( asnMission.munitions, "Munitions" );
    pSelector_munitions->AddItem( "Explosif", EnumTypeMunitionTirIndirect::explosif );
    pSelector_munitions->AddItem( "Grenade", EnumTypeMunitionTirIndirect::grenade );
    pSelector_munitions->AddItem( "Aced", EnumTypeMunitionTirIndirect::aced );
    pSelector_munitions->AddItem( "Fumigene", EnumTypeMunitionTirIndirect::fumigene );
    pSelector_munitions->AddItem( "Eclairant", EnumTypeMunitionTirIndirect::eclairant );
    pSelector_munitions->AddItem( "Mine", EnumTypeMunitionTirIndirect::mine );
    CreateNumeric( asnMission.nb_IT, "Nb IT" );
    CreatePoint( asnMission.cible, "Cible" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Pion_ExecuterTir
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Pion_ExecuterTir()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Pion_Emettre
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Pion_Emettre()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Pion_StopperEmission
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Pion_StopperEmission()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Pion_RenforcerEnVSRAM
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Pion_RenforcerEnVSRAM()
{
    ASN1T_OrderConduite_Pion_RenforcerEnVSRAM& asnMission = *new ASN1T_OrderConduite_Pion_RenforcerEnVSRAM();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_pion_renforcer_en_vs_ram;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_renforcer_en_vs_ram = &asnMission;
    CreateAgent( asnMission.pion_a_renforcer, "Pion a renforcer" );
    CreateNumeric( asnMission.nbr_ambulances, "Nbr ambulances" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Pion_TransfererVSRAM
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Pion_TransfererVSRAM()
{
    ASN1T_OrderConduite_Pion_TransfererVSRAM& asnMission = *new ASN1T_OrderConduite_Pion_TransfererVSRAM();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_pion_transferer_vs_ram;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_transferer_vs_ram = &asnMission;
    CreateAgent( asnMission.pion_renforce, "Pion renforce" );
    CreateAgent( asnMission.pion_a_renforcer, "Pion a renforcer" );
    CreateNumeric( asnMission.nbr_ambulances, "Nbr ambulances" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Pion_ReprendreAuxOrdresVSRAM
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Pion_ReprendreAuxOrdresVSRAM()
{
    ASN1T_OrderConduite_Pion_ReprendreAuxOrdresVSRAM& asnMission = *new ASN1T_OrderConduite_Pion_ReprendreAuxOrdresVSRAM();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_pion_reprendre_aux_ordres_vs_ram;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_reprendre_aux_ordres_vs_ram = &asnMission;
    CreateAgent( asnMission.pion_renforce, "Pion renforce" );
    CreateNumeric( asnMission.nbr_ambulances, "Nbr ambulances" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_ModifierTempsBordeeMaintenance
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_ModifierTempsBordeeMaintenance()
{
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_temps_bordee_maintenance;
#define asnMission pASNMsgOrder_->GetAsnMsg().order_conduite.u
    MOS_ParamComboBox< ASN1T_EnumTempsBordee >* pSelector_order_conduite_modifier_temps_bordee_maintenance = &CreateVarList( asnMission.order_conduite_modifier_temps_bordee_maintenance, "Order conduite modifier temps bordee maintenance" );
    pSelector_order_conduite_modifier_temps_bordee_maintenance->AddItem( "8 heures", EnumTempsBordee::temps_8_heures );
    pSelector_order_conduite_modifier_temps_bordee_maintenance->AddItem( "12 heures", EnumTempsBordee::temps_12_heures );
    pSelector_order_conduite_modifier_temps_bordee_maintenance->AddItem( "16 heures", EnumTempsBordee::temps_16_heures );
    pSelector_order_conduite_modifier_temps_bordee_maintenance->AddItem( "20 heures", EnumTempsBordee::temps_20_heures );
    pSelector_order_conduite_modifier_temps_bordee_maintenance->AddItem( "24 heures", EnumTempsBordee::temps_24_heures );
#undef asnMission
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_ModifierTempsBordeeSante
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_ModifierTempsBordeeSante()
{
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_temps_bordee_sante;
#define asnMission pASNMsgOrder_->GetAsnMsg().order_conduite.u
    MOS_ParamComboBox< ASN1T_EnumTempsBordee >* pSelector_order_conduite_modifier_temps_bordee_sante = &CreateVarList( asnMission.order_conduite_modifier_temps_bordee_sante, "Order conduite modifier temps bordee sante" );
    pSelector_order_conduite_modifier_temps_bordee_sante->AddItem( "8 heures", EnumTempsBordee::temps_8_heures );
    pSelector_order_conduite_modifier_temps_bordee_sante->AddItem( "12 heures", EnumTempsBordee::temps_12_heures );
    pSelector_order_conduite_modifier_temps_bordee_sante->AddItem( "16 heures", EnumTempsBordee::temps_16_heures );
    pSelector_order_conduite_modifier_temps_bordee_sante->AddItem( "20 heures", EnumTempsBordee::temps_20_heures );
    pSelector_order_conduite_modifier_temps_bordee_sante->AddItem( "24 heures", EnumTempsBordee::temps_24_heures );
#undef asnMission
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_ModifierPrioritesReparations
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_ModifierPrioritesReparations()
{
    ASN1T_OrderConduite_ModifierPrioritesReparations& asnMission = *new ASN1T_OrderConduite_ModifierPrioritesReparations();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_reparations;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_reparations = &asnMission;
    CreateMaintenancePriorities( asnMission, "Order conduite modifier priorites reparations" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_ModifierPrioritesBlesses
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_ModifierPrioritesBlesses()
{
    ASN1T_OrderConduite_ModifierPrioritesBlesses& asnMission = *new ASN1T_OrderConduite_ModifierPrioritesBlesses();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_blesses;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_blesses = &asnMission;
    CreateMedicalPriorities( asnMission, "Order conduite modifier priorites blesses" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_ModifierPrioritesTactiquesReparations
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_ModifierPrioritesTactiquesReparations()
{
    ASN1T_OrderConduite_ModifierPrioritesTactiquesReparations& asnMission = *new ASN1T_OrderConduite_ModifierPrioritesTactiquesReparations();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_tactiques_reparations;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_tactiques_reparations = &asnMission;
    CreateAutomateList( asnMission, "Order conduite modifier priorites tactiques reparations" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_ModifierPrioritesTactiquesBlesses
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_ModifierPrioritesTactiquesBlesses()
{
    ASN1T_OrderConduite_ModifierPrioritesTactiquesBlesses& asnMission = *new ASN1T_OrderConduite_ModifierPrioritesTactiquesBlesses();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_modifier_priorites_tactiques_blesses;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_modifier_priorites_tactiques_blesses = &asnMission;
    CreateAutomateList( asnMission, "Order conduite modifier priorites tactiques blesses" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Automate_RealiserVariantement
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Automate_RealiserVariantement()
{
    ASN1T_OrderConduite_Automate_RealiserVariantement& asnMission = *new ASN1T_OrderConduite_Automate_RealiserVariantement();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_realiser_variantement;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_realiser_variantement = &asnMission;
    CreateObjectKnowledge( asnMission.site_franchissement_original, "Site franchissement original" );
    CreateObjectKnowledge( asnMission.site_franchissement_variante, "Site franchissement variante" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Automate_DesequiperSiteFranchissement
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Automate_DesequiperSiteFranchissement()
{
    ASN1T_OrderConduite_Automate_DesequiperSiteFranchissement& asnMission = *new ASN1T_OrderConduite_Automate_DesequiperSiteFranchissement();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_desequiper_site_franchissement;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_desequiper_site_franchissement = &asnMission;
    CreateObjectKnowledge( asnMission.site_franchissement, "Site franchissement" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Automate_ReagirFaceAEni
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Automate_ReagirFaceAEni()
{
    ASN1T_OrderConduite_Automate_ReagirFaceAEni& asnMission = *new ASN1T_OrderConduite_Automate_ReagirFaceAEni();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_reagir_face_a_eni;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_reagir_face_a_eni = &asnMission;
    CreateObjectKnowledge( asnMission.site_franchissement, "Site franchissement" );
    MOS_ParamComboBox< ASN1T_EnumActionReagirFaceAEni >* pSelector_action = &CreateVarList( asnMission.action, "Action" );
    pSelector_action->AddItem( "Moyens", EnumActionReagirFaceAEni::detruire_moyens );
    pSelector_action->AddItem( "Defendre", EnumActionReagirFaceAEni::defendre );
    pSelector_action->AddItem( "Demonter", EnumActionReagirFaceAEni::demonter );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Automate_AffecterPionAObstacle
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Automate_AffecterPionAObstacle()
{
    ASN1T_OrderConduite_Automate_AffecterPionAObstacle& asnMission = *new ASN1T_OrderConduite_Automate_AffecterPionAObstacle();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_affecter_pion_a_obstacle;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_affecter_pion_a_obstacle = &asnMission;
    CreateAgent( asnMission.pion, "Pion" );
    CreateGenObject( asnMission.obstacle, "Obstacle" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Automate_ReconnaitreZoneImplantation
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Automate_ReconnaitreZoneImplantation()
{
    ASN1T_OrderConduite_Automate_ReconnaitreZoneImplantation& asnMission = *new ASN1T_OrderConduite_Automate_ReconnaitreZoneImplantation();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_reconnaitre_zone_implantation;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_reconnaitre_zone_implantation = &asnMission;
    CreatePoint( asnMission, "Order conduite automate reconnaitre zone implantation" );
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Automate_TC2_GererMaterielAvantDeplacement
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Automate_TC2_GererMaterielAvantDeplacement()
{
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_tc2_gerer_materiel_avant_deplacement;
#define asnMission pASNMsgOrder_->GetAsnMsg().order_conduite.u
    MOS_ParamComboBox< ASN1T_EnumTC2GererMaterielAvantDeplacement >* pSelector_order_conduite_automate_tc2_gerer_materiel_avant_deplacement = &CreateVarList( asnMission.order_conduite_automate_tc2_gerer_materiel_avant_deplacement, "Order conduite automate tc2 gerer materiel avant deplacement" );
    pSelector_order_conduite_automate_tc2_gerer_materiel_avant_deplacement->AddItem( "Materiels vers bld", EnumTC2GererMaterielAvantDeplacement::deborder_materiels_vers_bld );
    pSelector_order_conduite_automate_tc2_gerer_materiel_avant_deplacement->AddItem( "Materiels en reparation", EnumTC2GererMaterielAvantDeplacement::detruire_materiels_en_reparation );
    pSelector_order_conduite_automate_tc2_gerer_materiel_avant_deplacement->AddItem( "Materiels sur place", EnumTC2GererMaterielAvantDeplacement::reparer_materiels_sur_place );
#undef asnMission
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Automate_ActiverObstacle
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Automate_ActiverObstacle()
{
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_activer_obstacle;
#define asnMission pASNMsgOrder_->GetAsnMsg().order_conduite.u
    CreateObjectKnowledge( pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_activer_obstacle, "Order conduite automate activer obstacle" );
#undef asnMission
}

// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::CreateOrder_Automate_TransfererRenforts
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::CreateOrder_Automate_TransfererRenforts()
{
    ASN1T_OrderConduite_Automate_TransfererRenforts& asnMission = *new ASN1T_OrderConduite_Automate_TransfererRenforts();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_transferer_renforts;
    pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_automate_transferer_renforts = &asnMission;
    CreateAgent( asnMission.pion_a_transferer, "Pion a transferer" );
    CreateAutomate( asnMission.automate_a_renforcer, "Automate a renforcer" );
}


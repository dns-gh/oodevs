// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_UnitMissionInterface.cpp $
// $Author: Nld $
// $Modtime: 10/05/05 17:02 $
// $Revision: 10 $
// $Workfile: MOS_UnitMissionInterface.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_UnitMissionInterface.h"
#include "moc_MOS_UnitMissionInterface.cpp"

#include "MOS_MissionPanel.h"
#include "MOS_ASN_Messages.h"
#include "MOS_ASN_Types.h"
#include "MOS_ParamPoint.h"
#include "MOS_ParamAgent.h"
#include "MOS_ParamAgentList.h"
#include "MOS_ParamPath.h"
#include "MOS_ParamLimits.h"
#include "MOS_ParamLimaList.h"
#include "MOS_ParamRadioBtnGroup.h"
#include "MOS_ParamGDH.h"
#include "MOS_ParamComboBox.h"
#include "MOS_ParamBool.h"
#include "MOS_ParamDirection.h"
#include "MOS_Agent.h"
#include "MOS_Tools.h"

// -----------------------------------------------------------------------------
// Name: MOS_UnitMissionInterface constructor
/** @param  mission 
    @param  pParent 
*/
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MOS_UnitMissionInterface::MOS_UnitMissionInterface( MOS_Agent& agent, uint nMissionId, MOS_MissionPanel& parentPanel )
    : MOS_MissionInterface_ABC( agent, parentPanel )
    , nMissionId_             ( nMissionId )
{
    pASNMsgOrder_ = new MOS_ASN_MsgPionOrder();
    pASNMsgOrder_->GetAsnMsg().order_id = (uint)(&agent_);
    pASNMsgOrder_->GetAsnMsg().oid_unite_executante = agent_.GetAgentID();

    QLabel* pLabel = new QLabel( ENT_Tr::ConvertFromUnitMission( E_UnitMission( nMissionId_ ) ).c_str(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    this->CreateInterface();
}


// -----------------------------------------------------------------------------
// Name: MOS_UnitMissionInterface::CreateDefaultParameters
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void MOS_UnitMissionInterface::CreateDefaultParameters()
{
    //$$$$ tr

    ASN1T_MsgPionOrder& order = pASNMsgOrder_->GetAsnMsg();
    paramVector_.push_back( new MOS_ParamLimits( order.oid_limite_gauche,
                                                 order.oid_limite_droite,
                                                 "Limites" ,
                                                 "Fixer limite 1",
                                                 "Fixer limite 2",
                                                 this ) );

    paramVector_.push_back( new MOS_ParamLimaList( order.oid_limas,
                                                   "Limas",
                                                   "Ajouter aux limas",
                                                   this ) );

    paramVector_.push_back( new MOS_ParamDirection( order.direction_dangereuse, "Direction dangeureuse", this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_UnitMissionInterface::OnOk
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void MOS_UnitMissionInterface::OnOk()
{
    if( ! this->CheckValidity() )
        return;

    std::stringstream strMsg;

    for( IT_ParamVector it = paramVector_.begin() ; it != paramVector_.end() ; ++it )
    {
        if( it != paramVector_.begin() )
            strMsg << "\n";
        (*it)->WriteMsg( strMsg );
    }

    std::stringstream strMsgTitle;
    strMsgTitle << "Mission pion " << ENT_Tr::ConvertFromUnitMission( E_UnitMission( nMissionId_ ) ) << " pour agent " << agent_.GetName();
    MT_LOG_INFO( strMsgTitle.str().c_str(), eSent, strMsg.str().c_str() );

    ASN1T_MsgPionOrder& order = pASNMsgOrder_->GetAsnMsg();
    order.m.oid_limite_gauchePresent = (order.oid_limite_gauche != MIL_NULL_LINE_ID) ? 1 : 0;
    order.m.oid_limite_droitePresent = (order.oid_limite_droite != MIL_NULL_LINE_ID) ? 1 : 0;
    pASNMsgOrder_->Send( 45 );

    parentPanel_.hide();
}


//-----------------------------------------------------------------------------
// Name: void MOS_UnitMissionInterface::CreateMission_Test_MoveTo
// Created: APE 2004-05-27
//-----------------------------------------------------------------------------
//void MOS_UnitMissionInterface::CreateMission_Test_MoveTo()
//{
//    ASN1T_Mission_Pion_Test_MoveTo& asnMission = *new ASN1T_Mission_Pion_Test_MoveTo();
//    pASNMsgOrder_->GetAsnMsg().mission.t                            = T_Mission_Pion_mission_pion_test_move_to;
//    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_move_to  = &asnMission;
//
//    CreatePath( asnMission.itineraire, "Itineraire" );
//
//    MOS_ParamComboBox< Mission_Pion_Test_MoveTo_type_itineraire::Root >& param = CreateVarList( asnMission.type_itineraire, "Type d'itinéraire" );
//    param.AddItem( "movement"    , Mission_Pion_Test_MoveTo_type_itineraire::movement       );
//    param.AddItem( "recon"       , Mission_Pion_Test_MoveTo_type_itineraire::recon          );
//    param.AddItem( "infiltration", Mission_Pion_Test_MoveTo_type_itineraire::infiltration   );
//    param.AddItem( "assault"     , Mission_Pion_Test_MoveTo_type_itineraire::assault        );
//    param.AddItem( "retreat"     , Mission_Pion_Test_MoveTo_type_itineraire::retreat        );
//    param.AddItem( "backup"      , Mission_Pion_Test_MoveTo_type_itineraire::backup         );
//
//    MOS_ParamComboBox< Mission_Pion_Test_MoveTo_verrouillage_vision::Root >& param2 = CreateVarList( asnMission.verrouillage_vision, "Verrouillage vision" );
//    param2.AddItem( "Rien",       Mission_Pion_Test_MoveTo_verrouillage_vision::rien      );
//    param2.AddItem( "Point",      Mission_Pion_Test_MoveTo_verrouillage_vision::point     );
//    param2.AddItem( "Direction",  Mission_Pion_Test_MoveTo_verrouillage_vision::direction );    
//
//    CreatePoint    ( asnMission.vision_point    , "Vision verrouillée sur point"     );
//    CreateDirection( asnMission.vision_direction, "Vision verrouillée sur direction" );
//
//    CreateBool( asnMission.debarque, "debarque" );
//}

//-----------------------------------------------------------------------------
// Name: void MOS_UnitMissionInterface::CreateMission_Test_CreateObject
// Created: APE 2004-05-27
//-----------------------------------------------------------------------------
//void MOS_UnitMissionInterface::CreateMission_Test_CreateObject()
//{
//    ASN1T_Mission_Pion_Test_CreateObject& asnMission = *new ASN1T_Mission_Pion_Test_CreateObject();
//    pASNMsgOrder_->GetAsnMsg().mission.t                            = T_Mission_Pion_mission_pion_test_create_object;
//    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_create_object = &asnMission;
//
//    CreateLocation( asnMission.forme, "Localisation" );
//
//    MOS_ParamComboBox< EnumObjectType::Root >& param = CreateVarList( asnMission.type, "Type" );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::bouchon_mines ).ascii()             , EnumObjectType::bouchon_mines );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::zone_minee_lineaire ).ascii()       , EnumObjectType::zone_minee_lineaire );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::zone_minee_par_dispersion ).ascii() , EnumObjectType::zone_minee_par_dispersion );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::fosse_anti_char ).ascii()           , EnumObjectType::fosse_anti_char );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::abattis ).ascii()                   , EnumObjectType::abattis );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::barricade ).ascii()                 , EnumObjectType::barricade );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::eboulement ).ascii()                , EnumObjectType::eboulement );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::destruction_route ).ascii()         , EnumObjectType::destruction_route );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::destruction_pont ).ascii()          , EnumObjectType::destruction_pont );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::pont_flottant ).ascii()             , EnumObjectType::pont_flottant );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::poste_tir ).ascii()                 , EnumObjectType::poste_tir );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::zone_protegee ).ascii()             , EnumObjectType::zone_protegee );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::zone_implantation_canon ).ascii()   , EnumObjectType::zone_implantation_canon );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::zone_implantation_cobra ).ascii()   , EnumObjectType::zone_implantation_cobra );
//    param.AddItem( MOS_Tools::ToString( EnumObjectType::zone_implantation_lrm ).ascii()     , EnumObjectType::zone_implantation_lrm );
//
//    CreateNumeric( asnMission.param, "Parametre" );
//}


//-----------------------------------------------------------------------------
// Name: void MOS_UnitMissionInterface::CreateMission_Test_DestroyObject
// Destroyd: APE 2004-05-27
//-----------------------------------------------------------------------------
//void MOS_UnitMissionInterface::CreateMission_Test_DestroyObject()
//{
//    ASN1T_Mission_Pion_Test_DestroyObject& asnMission = *new ASN1T_Mission_Pion_Test_DestroyObject();
//    pASNMsgOrder_->GetAsnMsg().mission.t                            = T_Mission_Pion_mission_pion_test_destroy_object;
//    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_destroy_object = &asnMission;
//
//    CreateObjectKnowledge( asnMission.objet, "Objet" );
//}


//-----------------------------------------------------------------------------
// Name: void MOS_UnitMissionInterface::CreateMission_Test_CreateBypass
// Created: APE 2004-05-27
//-----------------------------------------------------------------------------
//void MOS_UnitMissionInterface::CreateMission_Test_CreateBypass()
//{
//    ASN1T_Mission_Pion_Test_CreateBypass& asnMission = *new ASN1T_Mission_Pion_Test_CreateBypass();
//    pASNMsgOrder_->GetAsnMsg().mission.t                            = T_Mission_Pion_mission_pion_test_create_bypass;
//    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_create_bypass = &asnMission;
//
//    CreateObjectKnowledge( asnMission.objet, "Objet" );
//}


//-----------------------------------------------------------------------------
// Name: void MOS_UnitMissionInterface::CreateMission_Test_Fire
// Created: APE 2004-05-27
//-----------------------------------------------------------------------------
//void MOS_UnitMissionInterface::CreateMission_Test_Fire()
//{
//    ASN1T_Mission_Pion_Test_Fire& asnMission = *new ASN1T_Mission_Pion_Test_Fire();
//    pASNMsgOrder_->GetAsnMsg().mission.t                   = T_Mission_Pion_mission_pion_test_fire;
//    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_fire = &asnMission;
//
//    CreateBool( asnMission.tir_indirect, "Tir Indirect" );
//    CreatePoint( asnMission.point, "Cible pour tir indirect" );
//    CreateAgentKnowledgeList( asnMission.targets, "Cible pour tir direct" );
//
//    MOS_ParamComboBox< Mission_Pion_Test_Fire_munitions::Root >& param = CreateVarList( asnMission.munitions, "Munitions" );
//    param.AddItem( "Obus explosif",  Mission_Pion_Test_Fire_munitions::mun_obus_explosif );
//    param.AddItem( "Obus grenade",   Mission_Pion_Test_Fire_munitions::mun_obus_grenade );
//    param.AddItem( "Obus ACED",      Mission_Pion_Test_Fire_munitions::mun_obus_aced );
//    param.AddItem( "Obus fumigène",  Mission_Pion_Test_Fire_munitions::mun_obus_fumigene );
//    param.AddItem( "Obus éclairant", Mission_Pion_Test_Fire_munitions::mun_obus_eclairant );
//    param.AddItem( "Obus mine",      Mission_Pion_Test_Fire_munitions::mun_obus_mine );
//
//    CreateNumeric( asnMission.nb_obus, "Nombre d'obus" );
//}
//

//-----------------------------------------------------------------------------
// Name: void MOS_UnitMissionInterface::CreateMission_Test_ChangePosture
// Created: APE 2004-05-27
//-----------------------------------------------------------------------------
//void MOS_UnitMissionInterface::CreateMission_Test_ChangePosture()
//{
//    ASN1T_Mission_Pion_Test_ChangePosture& asnMission = *new ASN1T_Mission_Pion_Test_ChangePosture();
//    pASNMsgOrder_->GetAsnMsg().mission.t       = T_Mission_Pion_mission_pion_test_change_posture;
//    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_change_posture = &asnMission;
//
//    MOS_ParamComboBox< Mission_Pion_Test_ChangePosture_posture::Root >& param = CreateVarList( asnMission.posture, "Lock" );
//    param.AddItem( "Mouvement"              , Mission_Pion_Test_ChangePosture_posture::mouvement            );
//    param.AddItem( "Arrêt"                  , Mission_Pion_Test_ChangePosture_posture::arret                );
//    param.AddItem( "Poste réflexe"          , Mission_Pion_Test_ChangePosture_posture::poste_reflexe        );
//    param.AddItem( "Poste"                  , Mission_Pion_Test_ChangePosture_posture::poste                );
//    param.AddItem( "Poste amenagé"          , Mission_Pion_Test_ChangePosture_posture::poste_amenage        );
//    param.AddItem( "Poste preparé génie"    , Mission_Pion_Test_ChangePosture_posture::poste_prepare_genie  );
//    param.AddItem( "Retour mode automatique", Mission_Pion_Test_ChangePosture_posture::automatique          );    
//}


//-----------------------------------------------------------------------------
// Name: void MOS_UnitMissionInterface::CreateMission_Test_ChangePosture
// Created: APE 2004-05-27
//-----------------------------------------------------------------------------
//void MOS_UnitMissionInterface::CreateMission_Test_FindPosition()
//{
//    ASN1T_Mission_Pion_Test_FindPosition& asnMission = *new ASN1T_Mission_Pion_Test_FindPosition();
//    pASNMsgOrder_->GetAsnMsg().mission.t                            = T_Mission_Pion_mission_pion_test_find_position;
//    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_find_position = &asnMission;
//
//    CreatePoint( asnMission.point  , "Point" );
//
//    CreatePoint( asnMission.retreat, "Retraite" );
//    CreateAgentList( asnMission.unite , "Unités" );
//    CreateDirection( asnMission.dgo, "Danger" );
//
//    MOS_ParamComboBox< Mission_Pion_Test_FindPosition_type_objectif::Root >& param = CreateVarList( asnMission.type_objectif, "Type objectif" );
//    param.AddItem( "Agent",    Mission_Pion_Test_FindPosition_type_objectif::agent );
//    param.AddItem( "Point",    Mission_Pion_Test_FindPosition_type_objectif::point );
//    param.AddItem( "RepPoint", Mission_Pion_Test_FindPosition_type_objectif::rep_point );
//    param.AddItem( "Objet",    Mission_Pion_Test_FindPosition_type_objectif::objet );
//
//    MOS_ParamComboBox< Mission_Pion_Test_FindPosition_type_operation::Root >& param2 = CreateVarList( asnMission.type_operation, "Type operation" );
//    param2.AddItem( "see", Mission_Pion_Test_FindPosition_type_operation::see );
//    param2.AddItem( "shoot", Mission_Pion_Test_FindPosition_type_operation::shoot );
//    param2.AddItem( "add", Mission_Pion_Test_FindPosition_type_operation::add );
//    param2.AddItem( "rem", Mission_Pion_Test_FindPosition_type_operation::rem );
//    param2.AddItem( "safety", Mission_Pion_Test_FindPosition_type_operation::safety );
//    param2.AddItem( "border", Mission_Pion_Test_FindPosition_type_operation::border );
//    param2.AddItem( "appui", Mission_Pion_Test_FindPosition_type_operation::appui );
//    param2.AddItem( "support", Mission_Pion_Test_FindPosition_type_operation::support );
//    param2.AddItem( "ambush", Mission_Pion_Test_FindPosition_type_operation::ambush );
//    param2.AddItem( "random", Mission_Pion_Test_FindPosition_type_operation::random );
//
//    MOS_ParamComboBox< EnumUnitIdentificationLevel::Root >& param3 = CreateVarList( asnMission.niveau_identification, "Niveau identification" );
//    param3.AddItem( "Identifiee", EnumUnitIdentificationLevel::identifiee );
//    param3.AddItem( "Reconnue", EnumUnitIdentificationLevel::reconnue );
//    param3.AddItem( "Detectee", EnumUnitIdentificationLevel::detectee );
//
//    QGroupBox* pG = new QGroupBox( 2, Qt::Horizontal, "Type Chemin", this );
//    pG->setInsideSpacing( 1 );
//    pG->setInsideSpacing( 1 );
//    CreateBool( asnMission.avoid            , "Path find : avoid"            , pG );
//    CreateBool( asnMission.reco             , "Path find : reco"             , pG );
//    CreateBool( asnMission.no_obstacle_avoid, "Path find : no obstacle avoid", pG );
//    CreateBool( asnMission.debordement      , "Path find : debordement"      , pG );
//    CreateBool( asnMission.dec_points       , "Path find : dec points"       , pG );
//}


//-----------------------------------------------------------------------------
// Name: MOS_UnitMissionInterface::CreateMission_Test_PathFind
// Created: JDY 03-03-10
//-----------------------------------------------------------------------------
//void MOS_UnitMissionInterface::CreateMission_Test_PathFind()
//{
//    ASN1T_Mission_Pion_Test_PathFind& asnMission = *new ASN1T_Mission_Pion_Test_PathFind();
//    pASNMsgOrder_->GetAsnMsg().mission.t                            = T_Mission_Pion_mission_pion_test_path_find;
//    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_path_find      = &asnMission;
//
//    CreateNumeric( asnMission.nb_pathfind, "Nombre Paths" );
//}


//-----------------------------------------------------------------------------
// Name: MOS_UnitMissionInterface::CreateMission_Test_Reinforce
// Created: AGN 03-03-13
//-----------------------------------------------------------------------------
//void MOS_UnitMissionInterface::CreateMission_Test_Reinforce()
//{
//    ASN1T_Mission_Pion_Test_Reinforce& asnMission = *new ASN1T_Mission_Pion_Test_Reinforce();
//    pASNMsgOrder_->GetAsnMsg().mission.t                            = T_Mission_Pion_mission_pion_test_reinforce;
//    pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_reinforce      = &asnMission;
//
//    CreateAgent( asnMission.unite, "Unite à renforcer" );
//    CreateBool( asnMission.renforce, "Renforce/Annule Renfort" );
//}


#include "MOS_UnitMissionInterface_Gen.cpp"
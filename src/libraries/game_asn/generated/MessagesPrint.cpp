/* This file was generated by ASN1C V5.31 on 19-Jun-2007 */

#include "Messages.h"

void asn1Print_MessagesIncludes (ASN1ConstCharPtr name, ASN1T_MessagesIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

   sprintf (namebuf, "%s.limaLimiteExports", name);

   sprintf (namebuf, "%s.orderExports", name);

   sprintf (namebuf, "%s.controlExports", name);

   sprintf (namebuf, "%s.knowledgeGroupExports", name);

   sprintf (namebuf, "%s.formationExports", name);

   sprintf (namebuf, "%s.fireExports", name);

   sprintf (namebuf, "%s.unitExports", name);

   sprintf (namebuf, "%s.unitKnowledgeExports", name);

   sprintf (namebuf, "%s.objectExports", name);

   sprintf (namebuf, "%s.objectKnowledgeExports", name);

   sprintf (namebuf, "%s.populationExports", name);

   sprintf (namebuf, "%s.populationKnowledgeExports", name);

   sprintf (namebuf, "%s.logMaintenanceExports", name);

   sprintf (namebuf, "%s.logRavitaillementExports", name);

   sprintf (namebuf, "%s.logSanteExports", name);

   sprintf (namebuf, "%s.automateExports", name);

   sprintf (namebuf, "%s.armyExports", name);

   sprintf (namebuf, "%s.authExports", name);

}

void ASN1C_MessagesIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_MessagesIncludes (name, &msgData);
}

void asn1Print_MessagesExports (ASN1ConstCharPtr name)
{
}

void ASN1C_MessagesExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_MessagesExports (name);
}

void asn1Print_MsgsSimToClient_msg (ASN1ConstCharPtr name, ASN1T_MsgsSimToClient_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_limit_creation_request_ack", name);
         asn1Print_MsgLimitCreationRequestAck (namebuf, &pvalue->u.msg_limit_creation_request_ack);
         break;

      case 2:
         sprintf (namebuf, "%s.u.msg_limit_destruction_request_ack", name);
         asn1Print_MsgLimitDestructionRequestAck (namebuf, &pvalue->u.msg_limit_destruction_request_ack);
         break;

      case 3:
         sprintf (namebuf, "%s.u.msg_limit_update_request_ack", name);
         asn1Print_MsgLimitUpdateRequestAck (namebuf, &pvalue->u.msg_limit_update_request_ack);
         break;

      case 4:
         sprintf (namebuf, "%s.u.msg_lima_creation_request_ack", name);
         asn1Print_MsgLimaCreationRequestAck (namebuf, &pvalue->u.msg_lima_creation_request_ack);
         break;

      case 5:
         sprintf (namebuf, "%s.u.msg_lima_destruction_request_ack", name);
         asn1Print_MsgLimaDestructionRequestAck (namebuf, &pvalue->u.msg_lima_destruction_request_ack);
         break;

      case 6:
         sprintf (namebuf, "%s.u.msg_lima_update_request_ack", name);
         asn1Print_MsgLimaUpdateRequestAck (namebuf, &pvalue->u.msg_lima_update_request_ack);
         break;

      case 7:
         sprintf (namebuf, "%s.u.msg_pion_order_ack", name);
         asn1Print_MsgUnitOrderAck (namebuf, pvalue->u.msg_pion_order_ack);
         break;

      case 8:
         sprintf (namebuf, "%s.u.msg_automate_order_ack", name);
         asn1Print_MsgAutomatOrderAck (namebuf, pvalue->u.msg_automate_order_ack);
         break;

      case 9:
         sprintf (namebuf, "%s.u.msg_population_order_ack", name);
         asn1Print_MsgPopulationOrderAck (namebuf, pvalue->u.msg_population_order_ack);
         break;

      case 10:
         sprintf (namebuf, "%s.u.msg_frag_order_ack", name);
         asn1Print_MsgFragOrderAck (namebuf, pvalue->u.msg_frag_order_ack);
         break;

      case 11:
         sprintf (namebuf, "%s.u.msg_set_automate_mode_ack", name);
         asn1Print_MsgSetAutomatModeAck (namebuf, pvalue->u.msg_set_automate_mode_ack);
         break;

      case 12:
         sprintf (namebuf, "%s.u.msg_unit_creation_request_ack", name);
         asn1Print_MsgUnitCreationRequestAck (namebuf, &pvalue->u.msg_unit_creation_request_ack);
         break;

      case 13:
         sprintf (namebuf, "%s.u.msg_unit_magic_action_ack", name);
         asn1Print_MsgUnitMagicActionAck (namebuf, pvalue->u.msg_unit_magic_action_ack);
         break;

      case 14:
         sprintf (namebuf, "%s.u.msg_object_magic_action_ack", name);
         asn1Print_MsgObjectMagicActionAck (namebuf, pvalue->u.msg_object_magic_action_ack);
         break;

      case 15:
         sprintf (namebuf, "%s.u.msg_population_magic_action_ack", name);
         asn1Print_MsgPopulationMagicActionAck (namebuf, pvalue->u.msg_population_magic_action_ack);
         break;

      case 16:
         sprintf (namebuf, "%s.u.msg_change_diplomatie_ack", name);
         asn1Print_MsgChangeDiplomacyAck (namebuf, pvalue->u.msg_change_diplomatie_ack);
         break;

      case 17:
         sprintf (namebuf, "%s.u.msg_automate_change_groupe_connaissance_ack", name);
         asn1Print_MsgAutomatChangeKnowledgeGroupAck (namebuf, pvalue->u.msg_automate_change_groupe_connaissance_ack);
         break;

      case 18:
         sprintf (namebuf, "%s.u.msg_automate_change_liens_logistiques_ack", name);
         asn1Print_MsgAutomatChangeLogisticLinksAck (namebuf, pvalue->u.msg_automate_change_liens_logistiques_ack);
         break;

      case 19:
         sprintf (namebuf, "%s.u.msg_pion_change_superior_ack", name);
         asn1Print_MsgUnitChangeSuperiorAck (namebuf, pvalue->u.msg_pion_change_superior_ack);
         break;

      case 20:
         sprintf (namebuf, "%s.u.msg_log_ravitaillement_pousser_flux_ack", name);
         asn1Print_MsgLogSupplyPushFlowAck (namebuf, &pvalue->u.msg_log_ravitaillement_pousser_flux_ack);
         break;

      case 21:
         sprintf (namebuf, "%s.u.msg_log_ravitaillement_change_quotas_ack", name);
         asn1Print_MsgLogSupplyChangeQuotasAck (namebuf, &pvalue->u.msg_log_ravitaillement_change_quotas_ack);
         break;

      case 22:
         sprintf (namebuf, "%s.u.msg_ctrl_info", name);
         asn1Print_MsgControlInformation (namebuf, pvalue->u.msg_ctrl_info);
         break;

      case 23:
         sprintf (namebuf, "%s.u.msg_ctrl_begin_tick", name);
         asn1Print_MsgControlBeginTick (namebuf, &pvalue->u.msg_ctrl_begin_tick);
         break;

      case 24:
         sprintf (namebuf, "%s.u.msg_ctrl_end_tick", name);
         asn1Print_MsgControlEndTick (namebuf, pvalue->u.msg_ctrl_end_tick);
         break;

      case 25:
         sprintf (namebuf, "%s.u.msg_ctrl_stop_ack", name);
         asn1Print_MsgControlStopAck (namebuf, &pvalue->u.msg_ctrl_stop_ack);
         break;

      case 26:
         sprintf (namebuf, "%s.u.msg_ctrl_pause_ack", name);
         asn1Print_MsgControlPauseAck (namebuf, &pvalue->u.msg_ctrl_pause_ack);
         break;

      case 27:
         sprintf (namebuf, "%s.u.msg_ctrl_resume_ack", name);
         asn1Print_MsgControlResumeAck (namebuf, &pvalue->u.msg_ctrl_resume_ack);
         break;

      case 28:
         sprintf (namebuf, "%s.u.msg_ctrl_change_time_factor_ack", name);
         asn1Print_MsgControlChangeTimeFactorAck (namebuf, pvalue->u.msg_ctrl_change_time_factor_ack);
         break;

      case 29:
         sprintf (namebuf, "%s.u.msg_ctrl_meteo_globale_ack", name);
         break;

      case 30:
         sprintf (namebuf, "%s.u.msg_ctrl_meteo_locale_ack", name);
         break;

      case 31:
         sprintf (namebuf, "%s.u.msg_ctrl_checkpoint_save_begin", name);
         break;

      case 32:
         sprintf (namebuf, "%s.u.msg_ctrl_checkpoint_save_end", name);
         break;

      case 33:
         sprintf (namebuf, "%s.u.msg_ctrl_checkpoint_set_frequency_ack", name);
         break;

      case 34:
         sprintf (namebuf, "%s.u.msg_ctrl_checkpoint_save_now_ack", name);
         break;

      case 35:
         sprintf (namebuf, "%s.u.msg_ctrl_send_current_state_begin", name);
         break;

      case 36:
         sprintf (namebuf, "%s.u.msg_ctrl_send_current_state_end", name);
         break;

      case 37:
         sprintf (namebuf, "%s.u.msg_limit_creation", name);
         asn1Print_MsgLimitCreation (namebuf, pvalue->u.msg_limit_creation);
         break;

      case 38:
         sprintf (namebuf, "%s.u.msg_limit_update", name);
         asn1Print_MsgLimitUpdate (namebuf, pvalue->u.msg_limit_update);
         break;

      case 39:
         sprintf (namebuf, "%s.u.msg_limit_destruction", name);
         asn1Print_MsgLimitDestruction (namebuf, &pvalue->u.msg_limit_destruction);
         break;

      case 40:
         sprintf (namebuf, "%s.u.msg_lima_creation", name);
         asn1Print_MsgLimaCreation (namebuf, pvalue->u.msg_lima_creation);
         break;

      case 41:
         sprintf (namebuf, "%s.u.msg_lima_update", name);
         asn1Print_MsgLimaUpdate (namebuf, pvalue->u.msg_lima_update);
         break;

      case 42:
         sprintf (namebuf, "%s.u.msg_lima_destruction", name);
         asn1Print_MsgLimaDestruction (namebuf, &pvalue->u.msg_lima_destruction);
         break;

      case 43:
         sprintf (namebuf, "%s.u.msg_formation_creation", name);
         asn1Print_MsgFormationCreation (namebuf, pvalue->u.msg_formation_creation);
         break;

      case 44:
         sprintf (namebuf, "%s.u.msg_knowledge_group_creation", name);
         asn1Print_MsgKnowledgeGroupCreation (namebuf, pvalue->u.msg_knowledge_group_creation);
         break;

      case 45:
         sprintf (namebuf, "%s.u.msg_side_creation", name);
         asn1Print_MsgTeamCreation (namebuf, pvalue->u.msg_side_creation);
         break;

      case 46:
         sprintf (namebuf, "%s.u.msg_automate_creation", name);
         asn1Print_MsgAutomatCreation (namebuf, pvalue->u.msg_automate_creation);
         break;

      case 47:
         sprintf (namebuf, "%s.u.msg_automate_attributes", name);
         asn1Print_MsgAutomatAttributes (namebuf, pvalue->u.msg_automate_attributes);
         break;

      case 48:
         sprintf (namebuf, "%s.u.msg_pion_creation", name);
         asn1Print_MsgUnitCreation (namebuf, pvalue->u.msg_pion_creation);
         break;

      case 49:
         sprintf (namebuf, "%s.u.msg_unit_attributes", name);
         asn1Print_MsgUnitAttributes (namebuf, pvalue->u.msg_unit_attributes);
         break;

      case 50:
         sprintf (namebuf, "%s.u.msg_unit_pathfind", name);
         asn1Print_MsgUnitPathFind (namebuf, pvalue->u.msg_unit_pathfind);
         break;

      case 51:
         sprintf (namebuf, "%s.u.msg_pion_destruction", name);
         asn1Print_MsgUnitDestruction (namebuf, &pvalue->u.msg_pion_destruction);
         break;

      case 52:
         sprintf (namebuf, "%s.u.msg_change_diplomatie", name);
         asn1Print_MsgChangeDiplomacy (namebuf, pvalue->u.msg_change_diplomatie);
         break;

      case 53:
         sprintf (namebuf, "%s.u.msg_pion_change_superior", name);
         asn1Print_MsgUnitChangeSuperior (namebuf, pvalue->u.msg_pion_change_superior);
         break;

      case 54:
         sprintf (namebuf, "%s.u.msg_automate_change_liens_logistiques", name);
         asn1Print_MsgAutomatChangeLogisticLinks (namebuf, pvalue->u.msg_automate_change_liens_logistiques);
         break;

      case 55:
         sprintf (namebuf, "%s.u.msg_unit_knowledge_creation", name);
         asn1Print_MsgUnitKnowledgeCreation (namebuf, pvalue->u.msg_unit_knowledge_creation);
         break;

      case 56:
         sprintf (namebuf, "%s.u.msg_unit_knowledge_update", name);
         asn1Print_MsgUnitKnowledgeUpdate (namebuf, pvalue->u.msg_unit_knowledge_update);
         break;

      case 57:
         sprintf (namebuf, "%s.u.msg_unit_knowledge_destruction", name);
         asn1Print_MsgUnitKnowledgeDestruction (namebuf, pvalue->u.msg_unit_knowledge_destruction);
         break;

      case 58:
         sprintf (namebuf, "%s.u.msg_start_pion_fire", name);
         asn1Print_MsgStartUnitFire (namebuf, pvalue->u.msg_start_pion_fire);
         break;

      case 59:
         sprintf (namebuf, "%s.u.msg_stop_pion_fire", name);
         asn1Print_MsgStopUnitFire (namebuf, pvalue->u.msg_stop_pion_fire);
         break;

      case 60:
         sprintf (namebuf, "%s.u.msg_start_population_fire", name);
         asn1Print_MsgStartPopulationFire (namebuf, pvalue->u.msg_start_population_fire);
         break;

      case 61:
         sprintf (namebuf, "%s.u.msg_stop_population_fire", name);
         asn1Print_MsgStopPopulationFire (namebuf, pvalue->u.msg_stop_population_fire);
         break;

      case 62:
         sprintf (namebuf, "%s.u.msg_explosion", name);
         asn1Print_MsgExplosion (namebuf, pvalue->u.msg_explosion);
         break;

      case 63:
         sprintf (namebuf, "%s.u.msg_start_fire_effect", name);
         asn1Print_MsgStartFireEffect (namebuf, pvalue->u.msg_start_fire_effect);
         break;

      case 64:
         sprintf (namebuf, "%s.u.msg_stop_fire_effect", name);
         asn1Print_MsgStopFireEffect (namebuf, &pvalue->u.msg_stop_fire_effect);
         break;

      case 65:
         sprintf (namebuf, "%s.u.msg_cr", name);
         asn1Print_MsgReport (namebuf, pvalue->u.msg_cr);
         break;

      case 66:
         sprintf (namebuf, "%s.u.msg_trace", name);
         asn1Print_MsgTrace (namebuf, pvalue->u.msg_trace);
         break;

      case 67:
         sprintf (namebuf, "%s.u.msg_decisional_state", name);
         asn1Print_MsgDecisionalState (namebuf, pvalue->u.msg_decisional_state);
         break;

      case 68:
         sprintf (namebuf, "%s.u.msg_pion_order", name);
         asn1Print_MsgUnitOrder (namebuf, pvalue->u.msg_pion_order);
         break;

      case 69:
         sprintf (namebuf, "%s.u.msg_automate_order", name);
         asn1Print_MsgAutomatOrder (namebuf, pvalue->u.msg_automate_order);
         break;

      case 70:
         sprintf (namebuf, "%s.u.msg_population_order", name);
         asn1Print_MsgPopulationOrder (namebuf, pvalue->u.msg_population_order);
         break;

      case 71:
         sprintf (namebuf, "%s.u.msg_object_creation", name);
         asn1Print_MsgObjectCreation (namebuf, pvalue->u.msg_object_creation);
         break;

      case 72:
         sprintf (namebuf, "%s.u.msg_object_destruction", name);
         asn1Print_MsgObjectDestruction (namebuf, &pvalue->u.msg_object_destruction);
         break;

      case 73:
         sprintf (namebuf, "%s.u.msg_object_update", name);
         asn1Print_MsgObjectUpdate (namebuf, pvalue->u.msg_object_update);
         break;

      case 74:
         sprintf (namebuf, "%s.u.msg_object_knowledge_creation", name);
         asn1Print_MsgObjectKnowledgeCreation (namebuf, pvalue->u.msg_object_knowledge_creation);
         break;

      case 75:
         sprintf (namebuf, "%s.u.msg_object_knowledge_update", name);
         asn1Print_MsgObjectKnowledgeUpdate (namebuf, pvalue->u.msg_object_knowledge_update);
         break;

      case 76:
         sprintf (namebuf, "%s.u.msg_object_knowledge_destruction", name);
         asn1Print_MsgObjectKnowledgeDestruction (namebuf, pvalue->u.msg_object_knowledge_destruction);
         break;

      case 77:
         sprintf (namebuf, "%s.u.msg_log_sante_traitement_humain_creation", name);
         asn1Print_MsgLogMedicalHandlingCreation (namebuf, pvalue->u.msg_log_sante_traitement_humain_creation);
         break;

      case 78:
         sprintf (namebuf, "%s.u.msg_log_sante_traitement_humain_update", name);
         asn1Print_MsgLogMedicalHandlingUpdate (namebuf, pvalue->u.msg_log_sante_traitement_humain_update);
         break;

      case 79:
         sprintf (namebuf, "%s.u.msg_log_sante_traitement_humain_destruction", name);
         asn1Print_MsgLogMedicalHandlingDestruction (namebuf, pvalue->u.msg_log_sante_traitement_humain_destruction);
         break;

      case 80:
         sprintf (namebuf, "%s.u.msg_log_sante_etat", name);
         asn1Print_MsgLogMedicalState (namebuf, pvalue->u.msg_log_sante_etat);
         break;

      case 81:
         sprintf (namebuf, "%s.u.msg_log_maintenance_traitement_equipement_creation", name);
         asn1Print_MsgLogMaintenanceHandlingCreation (namebuf, pvalue->u.msg_log_maintenance_traitement_equipement_creation);
         break;

      case 82:
         sprintf (namebuf, "%s.u.msg_log_maintenance_traitement_equipement_update", name);
         asn1Print_MsgLogMaintenanceHandlingUpdate (namebuf, pvalue->u.msg_log_maintenance_traitement_equipement_update);
         break;

      case 83:
         sprintf (namebuf, "%s.u.msg_log_maintenance_traitement_equipement_destruction", name);
         asn1Print_MsgLogMaintenanceHandlingDestruction (namebuf, pvalue->u.msg_log_maintenance_traitement_equipement_destruction);
         break;

      case 84:
         sprintf (namebuf, "%s.u.msg_log_maintenance_etat", name);
         asn1Print_MsgLogMaintenanceState (namebuf, pvalue->u.msg_log_maintenance_etat);
         break;

      case 85:
         sprintf (namebuf, "%s.u.msg_log_ravitaillement_traitement_creation", name);
         asn1Print_MsgLogSupplyHandlingCreation (namebuf, pvalue->u.msg_log_ravitaillement_traitement_creation);
         break;

      case 86:
         sprintf (namebuf, "%s.u.msg_log_ravitaillement_traitement_update", name);
         asn1Print_MsgLogSupplyHandlingUpdate (namebuf, pvalue->u.msg_log_ravitaillement_traitement_update);
         break;

      case 87:
         sprintf (namebuf, "%s.u.msg_log_ravitaillement_traitement_destruction", name);
         asn1Print_MsgLogSupplyHandlingDestruction (namebuf, pvalue->u.msg_log_ravitaillement_traitement_destruction);
         break;

      case 88:
         sprintf (namebuf, "%s.u.msg_log_ravitaillement_etat", name);
         asn1Print_MsgLogSupplyState (namebuf, pvalue->u.msg_log_ravitaillement_etat);
         break;

      case 89:
         sprintf (namebuf, "%s.u.msg_log_ravitaillement_quotas", name);
         asn1Print_MsgLogSupplyQuotas (namebuf, pvalue->u.msg_log_ravitaillement_quotas);
         break;

      case 90:
         sprintf (namebuf, "%s.u.msg_population_creation", name);
         asn1Print_MsgPopulationCreation (namebuf, pvalue->u.msg_population_creation);
         break;

      case 91:
         sprintf (namebuf, "%s.u.msg_population_update", name);
         asn1Print_MsgPopulationUpdate (namebuf, pvalue->u.msg_population_update);
         break;

      case 92:
         sprintf (namebuf, "%s.u.msg_population_concentration_creation", name);
         asn1Print_MsgPopulationConcentrationCreation (namebuf, pvalue->u.msg_population_concentration_creation);
         break;

      case 93:
         sprintf (namebuf, "%s.u.msg_population_concentration_destruction", name);
         asn1Print_MsgPopulationConcentrationDestruction (namebuf, pvalue->u.msg_population_concentration_destruction);
         break;

      case 94:
         sprintf (namebuf, "%s.u.msg_population_concentration_update", name);
         asn1Print_MsgPopulationConcentrationUpdate (namebuf, pvalue->u.msg_population_concentration_update);
         break;

      case 95:
         sprintf (namebuf, "%s.u.msg_population_flux_creation", name);
         asn1Print_MsgPopulationFlowCreation (namebuf, pvalue->u.msg_population_flux_creation);
         break;

      case 96:
         sprintf (namebuf, "%s.u.msg_population_flux_destruction", name);
         asn1Print_MsgPopulationFlowDestruction (namebuf, pvalue->u.msg_population_flux_destruction);
         break;

      case 97:
         sprintf (namebuf, "%s.u.msg_population_flux_update", name);
         asn1Print_MsgPopulationFlowUpdate (namebuf, pvalue->u.msg_population_flux_update);
         break;

      case 98:
         sprintf (namebuf, "%s.u.msg_population_knowledge_creation", name);
         asn1Print_MsgPopulationKnowledgeCreation (namebuf, pvalue->u.msg_population_knowledge_creation);
         break;

      case 99:
         sprintf (namebuf, "%s.u.msg_population_knowledge_update", name);
         asn1Print_MsgPopulationKnowledgeUpdate (namebuf, pvalue->u.msg_population_knowledge_update);
         break;

      case 100:
         sprintf (namebuf, "%s.u.msg_population_knowledge_destruction", name);
         asn1Print_MsgPopulationKnowledgeDestruction (namebuf, pvalue->u.msg_population_knowledge_destruction);
         break;

      case 101:
         sprintf (namebuf, "%s.u.msg_population_concentration_knowledge_creation", name);
         asn1Print_MsgPopulationConcentrationKnowledgeCreation (namebuf, pvalue->u.msg_population_concentration_knowledge_creation);
         break;

      case 102:
         sprintf (namebuf, "%s.u.msg_population_concentration_knowledge_destruction", name);
         asn1Print_MsgPopulationConcentrationKnowledgeDestruction (namebuf, pvalue->u.msg_population_concentration_knowledge_destruction);
         break;

      case 103:
         sprintf (namebuf, "%s.u.msg_population_concentration_knowledge_update", name);
         asn1Print_MsgPopulationConcentrationKnowledgeUpdate (namebuf, pvalue->u.msg_population_concentration_knowledge_update);
         break;

      case 104:
         sprintf (namebuf, "%s.u.msg_population_flux_knowledge_creation", name);
         asn1Print_MsgPopulationFlowKnowledgeCreation (namebuf, pvalue->u.msg_population_flux_knowledge_creation);
         break;

      case 105:
         sprintf (namebuf, "%s.u.msg_population_flux_knowledge_destruction", name);
         asn1Print_MsgPopulationFlowKnowledgeDestruction (namebuf, pvalue->u.msg_population_flux_knowledge_destruction);
         break;

      case 106:
         sprintf (namebuf, "%s.u.msg_population_flux_knowledge_update", name);
         asn1Print_MsgPopulationFlowKnowledgeUpdate (namebuf, pvalue->u.msg_population_flux_knowledge_update);
         break;

      default:;
   }
}

void ASN1C_MsgsSimToClient_msg::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsSimToClient_msg (name, &msgData);
}

void asn1Print_MsgsSimToClient (ASN1ConstCharPtr name, ASN1T_MsgsSimToClient* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

   sprintf (namebuf, "%s.msg", name);
   asn1Print_MsgsSimToClient_msg (namebuf, &pvalue->msg);

}

void ASN1C_MsgsSimToClient::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsSimToClient (name, &msgData);
}

void asn1Print_MsgsClientToSim_msg (ASN1ConstCharPtr name, ASN1T_MsgsClientToSim_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_ctrl_stop", name);
         break;

      case 2:
         sprintf (namebuf, "%s.u.msg_ctrl_pause", name);
         break;

      case 3:
         sprintf (namebuf, "%s.u.msg_ctrl_resume", name);
         break;

      case 4:
         sprintf (namebuf, "%s.u.msg_ctrl_change_time_factor", name);
         asn1Print_MsgControlChangeTimeFactor (namebuf, &pvalue->u.msg_ctrl_change_time_factor);
         break;

      case 5:
         sprintf (namebuf, "%s.u.msg_ctrl_meteo_globale", name);
         asn1Print_MsgControlGlobalMeteo (namebuf, pvalue->u.msg_ctrl_meteo_globale);
         break;

      case 6:
         sprintf (namebuf, "%s.u.msg_ctrl_meteo_locale", name);
         asn1Print_MsgControlLocalMeteo (namebuf, pvalue->u.msg_ctrl_meteo_locale);
         break;

      case 7:
         sprintf (namebuf, "%s.u.msg_ctrl_checkpoint_save_now", name);
         asn1Print_MsgControlCheckPointSaveNow (namebuf, pvalue->u.msg_ctrl_checkpoint_save_now);
         break;

      case 8:
         sprintf (namebuf, "%s.u.msg_ctrl_checkpoint_set_frequency", name);
         asn1Print_MsgControlCheckPointSetFrequency (namebuf, &pvalue->u.msg_ctrl_checkpoint_set_frequency);
         break;

      case 9:
         sprintf (namebuf, "%s.u.msg_limit_creation_request", name);
         asn1Print_MsgLimitCreationRequest (namebuf, pvalue->u.msg_limit_creation_request);
         break;

      case 10:
         sprintf (namebuf, "%s.u.msg_limit_destruction_request", name);
         asn1Print_MsgLimitDestructionRequest (namebuf, &pvalue->u.msg_limit_destruction_request);
         break;

      case 11:
         sprintf (namebuf, "%s.u.msg_limit_update_request", name);
         asn1Print_MsgLimitUpdateRequest (namebuf, pvalue->u.msg_limit_update_request);
         break;

      case 12:
         sprintf (namebuf, "%s.u.msg_lima_creation_request", name);
         asn1Print_MsgLimaCreationRequest (namebuf, pvalue->u.msg_lima_creation_request);
         break;

      case 13:
         sprintf (namebuf, "%s.u.msg_lima_destruction_request", name);
         asn1Print_MsgLimaDestruction (namebuf, &pvalue->u.msg_lima_destruction_request);
         break;

      case 14:
         sprintf (namebuf, "%s.u.msg_lima_update_request", name);
         asn1Print_MsgLimaUpdateRequest (namebuf, pvalue->u.msg_lima_update_request);
         break;

      case 15:
         sprintf (namebuf, "%s.u.msg_pion_order", name);
         asn1Print_MsgUnitOrder (namebuf, pvalue->u.msg_pion_order);
         break;

      case 16:
         sprintf (namebuf, "%s.u.msg_automate_order", name);
         asn1Print_MsgAutomatOrder (namebuf, pvalue->u.msg_automate_order);
         break;

      case 17:
         sprintf (namebuf, "%s.u.msg_population_order", name);
         asn1Print_MsgPopulationOrder (namebuf, pvalue->u.msg_population_order);
         break;

      case 18:
         sprintf (namebuf, "%s.u.msg_frag_order", name);
         asn1Print_MsgFragOrder (namebuf, pvalue->u.msg_frag_order);
         break;

      case 19:
         sprintf (namebuf, "%s.u.msg_set_automate_mode", name);
         asn1Print_MsgSetAutomatMode (namebuf, pvalue->u.msg_set_automate_mode);
         break;

      case 20:
         sprintf (namebuf, "%s.u.msg_unit_creation_request", name);
         asn1Print_MsgUnitCreationRequest (namebuf, pvalue->u.msg_unit_creation_request);
         break;

      case 21:
         sprintf (namebuf, "%s.u.msg_unit_magic_action", name);
         asn1Print_MsgUnitMagicAction (namebuf, pvalue->u.msg_unit_magic_action);
         break;

      case 22:
         sprintf (namebuf, "%s.u.msg_object_magic_action", name);
         asn1Print_MsgObjectMagicAction (namebuf, pvalue->u.msg_object_magic_action);
         break;

      case 23:
         sprintf (namebuf, "%s.u.msg_population_magic_action", name);
         asn1Print_MsgPopulationMagicAction (namebuf, pvalue->u.msg_population_magic_action);
         break;

      case 24:
         sprintf (namebuf, "%s.u.msg_change_diplomatie", name);
         asn1Print_MsgChangeDiplomacy (namebuf, pvalue->u.msg_change_diplomatie);
         break;

      case 25:
         sprintf (namebuf, "%s.u.msg_automate_change_groupe_connaissance", name);
         asn1Print_MsgAutomatChangeKnowledgeGroup (namebuf, pvalue->u.msg_automate_change_groupe_connaissance);
         break;

      case 26:
         sprintf (namebuf, "%s.u.msg_automate_change_liens_logistiques", name);
         asn1Print_MsgAutomatChangeLogisticLinks (namebuf, pvalue->u.msg_automate_change_liens_logistiques);
         break;

      case 27:
         sprintf (namebuf, "%s.u.msg_pion_change_superior", name);
         asn1Print_MsgUnitChangeSuperior (namebuf, pvalue->u.msg_pion_change_superior);
         break;

      case 28:
         sprintf (namebuf, "%s.u.msg_log_ravitaillement_pousser_flux", name);
         asn1Print_MsgLogSupplyPushFlow (namebuf, pvalue->u.msg_log_ravitaillement_pousser_flux);
         break;

      case 29:
         sprintf (namebuf, "%s.u.msg_log_ravitaillement_change_quotas", name);
         asn1Print_MsgLogSupplyChangeQuotas (namebuf, pvalue->u.msg_log_ravitaillement_change_quotas);
         break;

      default:;
   }
}

void ASN1C_MsgsClientToSim_msg::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsClientToSim_msg (name, &msgData);
}

void asn1Print_MsgsClientToSim (ASN1ConstCharPtr name, ASN1T_MsgsClientToSim* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

   sprintf (namebuf, "%s.msg", name);
   asn1Print_MsgsClientToSim_msg (namebuf, &pvalue->msg);

}

void ASN1C_MsgsClientToSim::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsClientToSim (name, &msgData);
}

void asn1Print_MsgsSimToMiddle (ASN1ConstCharPtr name, ASN1T_MsgsSimToMiddle* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

}

void ASN1C_MsgsSimToMiddle::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsSimToMiddle (name, &msgData);
}

void asn1Print_MsgsMiddleToSim_msg (ASN1ConstCharPtr name, ASN1T_MsgsMiddleToSim_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_ctrl_client_announcement", name);
         break;

      default:;
   }
}

void ASN1C_MsgsMiddleToSim_msg::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsMiddleToSim_msg (name, &msgData);
}

void asn1Print_MsgsMiddleToSim (ASN1ConstCharPtr name, ASN1T_MsgsMiddleToSim* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

   sprintf (namebuf, "%s.msg", name);
   asn1Print_MsgsMiddleToSim_msg (namebuf, &pvalue->msg);

}

void ASN1C_MsgsMiddleToSim::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsMiddleToSim (name, &msgData);
}

void asn1Print_MsgsMiddleToClient_msg (ASN1ConstCharPtr name, ASN1T_MsgsMiddleToClient_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_ctrl_replay_info", name);
         asn1Print_MsgControlReplayInformation (namebuf, pvalue->u.msg_ctrl_replay_info);
         break;

      case 2:
         sprintf (namebuf, "%s.u.msg_ctrl_skip_to_tick_ack", name);
         asn1Print_MsgControlSkipToTickAck (namebuf, pvalue->u.msg_ctrl_skip_to_tick_ack);
         break;

      case 3:
         sprintf (namebuf, "%s.u.msg_authentication_response", name);
         asn1Print_MsgAuthenticationResponse (namebuf, pvalue->u.msg_authentication_response);
         break;

      case 4:
         sprintf (namebuf, "%s.u.msg_profile_creation", name);
         asn1Print_MsgProfileCreation (namebuf, pvalue->u.msg_profile_creation);
         break;

      case 5:
         sprintf (namebuf, "%s.u.msg_profile_creation_request_ack", name);
         asn1Print_MsgProfileCreationRequestAck (namebuf, pvalue->u.msg_profile_creation_request_ack);
         break;

      case 6:
         sprintf (namebuf, "%s.u.msg_profile_update", name);
         asn1Print_MsgProfileUpdate (namebuf, pvalue->u.msg_profile_update);
         break;

      case 7:
         sprintf (namebuf, "%s.u.msg_profile_update_request_ack", name);
         asn1Print_MsgProfileUpdateRequestAck (namebuf, pvalue->u.msg_profile_update_request_ack);
         break;

      case 8:
         sprintf (namebuf, "%s.u.msg_profile_destruction", name);
         asn1Print_MsgProfileDestruction (namebuf, &pvalue->u.msg_profile_destruction);
         break;

      case 9:
         sprintf (namebuf, "%s.u.msg_profile_destruction_request_ack", name);
         asn1Print_MsgProfileDestructionRequestAck (namebuf, pvalue->u.msg_profile_destruction_request_ack);
         break;

      default:;
   }
}

void ASN1C_MsgsMiddleToClient_msg::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsMiddleToClient_msg (name, &msgData);
}

void asn1Print_MsgsMiddleToClient (ASN1ConstCharPtr name, ASN1T_MsgsMiddleToClient* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

   sprintf (namebuf, "%s.msg", name);
   asn1Print_MsgsMiddleToClient_msg (namebuf, &pvalue->msg);

}

void ASN1C_MsgsMiddleToClient::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsMiddleToClient (name, &msgData);
}

void asn1Print_MsgsClientToMiddle_msg (ASN1ConstCharPtr name, ASN1T_MsgsClientToMiddle_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_ctrl_skip_to_tick", name);
         asn1Print_MsgControlSkipToTick (namebuf, &pvalue->u.msg_ctrl_skip_to_tick);
         break;

      case 2:
         sprintf (namebuf, "%s.u.msg_authentication_request", name);
         asn1Print_MsgAuthenticationRequest (namebuf, pvalue->u.msg_authentication_request);
         break;

      case 3:
         sprintf (namebuf, "%s.u.msg_profile_creation_request", name);
         asn1Print_MsgProfileCreationRequest (namebuf, pvalue->u.msg_profile_creation_request);
         break;

      case 4:
         sprintf (namebuf, "%s.u.msg_profile_update_request", name);
         asn1Print_MsgProfileUpdateRequest (namebuf, pvalue->u.msg_profile_update_request);
         break;

      case 5:
         sprintf (namebuf, "%s.u.msg_profile_destruction_request", name);
         asn1Print_MsgProfileDestructionRequest (namebuf, &pvalue->u.msg_profile_destruction_request);
         break;

      default:;
   }
}

void ASN1C_MsgsClientToMiddle_msg::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsClientToMiddle_msg (name, &msgData);
}

void asn1Print_MsgsClientToMiddle (ASN1ConstCharPtr name, ASN1T_MsgsClientToMiddle* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.context", name);
   rtPrintInteger (namebuf, pvalue->context);

   sprintf (namebuf, "%s.msg", name);
   asn1Print_MsgsClientToMiddle_msg (namebuf, &pvalue->msg);

}

void ASN1C_MsgsClientToMiddle::Print (ASN1ConstCharPtr name)

{
   asn1Print_MsgsClientToMiddle (name, &msgData);
}


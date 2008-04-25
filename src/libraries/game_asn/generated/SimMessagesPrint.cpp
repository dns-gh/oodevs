/* This file was generated by ASN1C V5.31 on 24-Apr-2008 */

#include "SimMessages.h"

void asn1Print_SimMessagesIncludes (ASN1ConstCharPtr name, ASN1T_SimMessagesIncludes* pvalue)
{
   char namebuf[512];

   sprintf (namebuf, "%s.baseExports", name);

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

   sprintf (namebuf, "%s.folkExports", name);

}

void ASN1C_SimMessagesIncludes::Print (ASN1ConstCharPtr name)

{
   asn1Print_SimMessagesIncludes (name, &msgData);
}

void asn1Print_SimMessagesExports (ASN1ConstCharPtr name)
{
}

void ASN1C_SimMessagesExports::Print (ASN1ConstCharPtr name)

{
   asn1Print_SimMessagesExports (name);
}

void asn1Print_MsgsSimToClient_msg (ASN1ConstCharPtr name, ASN1T_MsgsSimToClient_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_unit_order_ack", name);
         asn1Print_MsgUnitOrderAck (namebuf, pvalue->u.msg_unit_order_ack);
         break;

      case 2:
         sprintf (namebuf, "%s.u.msg_automat_order_ack", name);
         asn1Print_MsgAutomatOrderAck (namebuf, pvalue->u.msg_automat_order_ack);
         break;

      case 3:
         sprintf (namebuf, "%s.u.msg_population_order_ack", name);
         asn1Print_MsgPopulationOrderAck (namebuf, pvalue->u.msg_population_order_ack);
         break;

      case 4:
         sprintf (namebuf, "%s.u.msg_frag_order_ack", name);
         asn1Print_MsgFragOrderAck (namebuf, pvalue->u.msg_frag_order_ack);
         break;

      case 5:
         sprintf (namebuf, "%s.u.msg_set_automat_mode_ack", name);
         asn1Print_MsgSetAutomatModeAck (namebuf, pvalue->u.msg_set_automat_mode_ack);
         break;

      case 6:
         sprintf (namebuf, "%s.u.msg_unit_creation_request_ack", name);
         asn1Print_MsgUnitCreationRequestAck (namebuf, &pvalue->u.msg_unit_creation_request_ack);
         break;

      case 7:
         sprintf (namebuf, "%s.u.msg_unit_magic_action_ack", name);
         asn1Print_MsgUnitMagicActionAck (namebuf, pvalue->u.msg_unit_magic_action_ack);
         break;

      case 8:
         sprintf (namebuf, "%s.u.msg_object_magic_action_ack", name);
         asn1Print_MsgObjectMagicActionAck (namebuf, pvalue->u.msg_object_magic_action_ack);
         break;

      case 9:
         sprintf (namebuf, "%s.u.msg_population_magic_action_ack", name);
         asn1Print_MsgPopulationMagicActionAck (namebuf, pvalue->u.msg_population_magic_action_ack);
         break;

      case 10:
         sprintf (namebuf, "%s.u.msg_change_diplomacy_ack", name);
         asn1Print_MsgChangeDiplomacyAck (namebuf, pvalue->u.msg_change_diplomacy_ack);
         break;

      case 11:
         sprintf (namebuf, "%s.u.msg_automat_change_knowledge_group_ack", name);
         asn1Print_MsgAutomatChangeKnowledgeGroupAck (namebuf, &pvalue->u.msg_automat_change_knowledge_group_ack);
         break;

      case 12:
         sprintf (namebuf, "%s.u.msg_automat_change_logistic_links_ack", name);
         asn1Print_MsgAutomatChangeLogisticLinksAck (namebuf, &pvalue->u.msg_automat_change_logistic_links_ack);
         break;

      case 13:
         sprintf (namebuf, "%s.u.msg_automat_change_superior_ack", name);
         asn1Print_MsgAutomatChangeSuperiorAck (namebuf, &pvalue->u.msg_automat_change_superior_ack);
         break;

      case 14:
         sprintf (namebuf, "%s.u.msg_unit_change_superior_ack", name);
         asn1Print_MsgUnitChangeSuperiorAck (namebuf, &pvalue->u.msg_unit_change_superior_ack);
         break;

      case 15:
         sprintf (namebuf, "%s.u.msg_log_supply_push_flow_ack", name);
         asn1Print_MsgLogSupplyPushFlowAck (namebuf, &pvalue->u.msg_log_supply_push_flow_ack);
         break;

      case 16:
         sprintf (namebuf, "%s.u.msg_log_supply_change_quotas_ack", name);
         asn1Print_MsgLogSupplyChangeQuotasAck (namebuf, &pvalue->u.msg_log_supply_change_quotas_ack);
         break;

      case 17:
         sprintf (namebuf, "%s.u.msg_control_information", name);
         asn1Print_MsgControlInformation (namebuf, pvalue->u.msg_control_information);
         break;

      case 18:
         sprintf (namebuf, "%s.u.msg_control_profiling_information", name);
         asn1Print_MsgControlProfilingInformation (namebuf, pvalue->u.msg_control_profiling_information);
         break;

      case 19:
         sprintf (namebuf, "%s.u.msg_control_begin_tick", name);
         asn1Print_MsgControlBeginTick (namebuf, pvalue->u.msg_control_begin_tick);
         break;

      case 20:
         sprintf (namebuf, "%s.u.msg_control_end_tick", name);
         asn1Print_MsgControlEndTick (namebuf, pvalue->u.msg_control_end_tick);
         break;

      case 21:
         sprintf (namebuf, "%s.u.msg_control_stop_ack", name);
         asn1Print_MsgControlStopAck (namebuf, &pvalue->u.msg_control_stop_ack);
         break;

      case 22:
         sprintf (namebuf, "%s.u.msg_control_pause_ack", name);
         asn1Print_MsgControlPauseAck (namebuf, &pvalue->u.msg_control_pause_ack);
         break;

      case 23:
         sprintf (namebuf, "%s.u.msg_control_resume_ack", name);
         asn1Print_MsgControlResumeAck (namebuf, &pvalue->u.msg_control_resume_ack);
         break;

      case 24:
         sprintf (namebuf, "%s.u.msg_control_change_time_factor_ack", name);
         asn1Print_MsgControlChangeTimeFactorAck (namebuf, pvalue->u.msg_control_change_time_factor_ack);
         break;

      case 25:
         sprintf (namebuf, "%s.u.msg_control_date_time_change_ack", name);
         asn1Print_MsgControlDatetimeChangeAck (namebuf, &pvalue->u.msg_control_date_time_change_ack);
         break;

      case 26:
         sprintf (namebuf, "%s.u.msg_control_global_meteo_ack", name);
         break;

      case 27:
         sprintf (namebuf, "%s.u.msg_control_local_meteo_ack", name);
         break;

      case 28:
         sprintf (namebuf, "%s.u.msg_control_checkpoint_save_begin", name);
         break;

      case 29:
         sprintf (namebuf, "%s.u.msg_control_checkpoint_save_end", name);
         asn1Print_MsgControlCheckPointSaveEnd (namebuf, pvalue->u.msg_control_checkpoint_save_end);
         break;

      case 30:
         sprintf (namebuf, "%s.u.msg_control_checkpoint_set_frequency_ack", name);
         break;

      case 31:
         sprintf (namebuf, "%s.u.msg_control_checkpoint_save_now_ack", name);
         break;

      case 32:
         sprintf (namebuf, "%s.u.msg_control_send_current_state_begin", name);
         break;

      case 33:
         sprintf (namebuf, "%s.u.msg_control_send_current_state_end", name);
         break;

      case 34:
         sprintf (namebuf, "%s.u.msg_formation_creation", name);
         asn1Print_MsgFormationCreation (namebuf, pvalue->u.msg_formation_creation);
         break;

      case 35:
         sprintf (namebuf, "%s.u.msg_knowledge_group_creation", name);
         asn1Print_MsgKnowledgeGroupCreation (namebuf, pvalue->u.msg_knowledge_group_creation);
         break;

      case 36:
         sprintf (namebuf, "%s.u.msg_side_creation", name);
         asn1Print_MsgTeamCreation (namebuf, pvalue->u.msg_side_creation);
         break;

      case 37:
         sprintf (namebuf, "%s.u.msg_automat_creation", name);
         asn1Print_MsgAutomatCreation (namebuf, pvalue->u.msg_automat_creation);
         break;

      case 38:
         sprintf (namebuf, "%s.u.msg_automat_attributes", name);
         asn1Print_MsgAutomatAttributes (namebuf, pvalue->u.msg_automat_attributes);
         break;

      case 39:
         sprintf (namebuf, "%s.u.msg_unit_creation", name);
         asn1Print_MsgUnitCreation (namebuf, pvalue->u.msg_unit_creation);
         break;

      case 40:
         sprintf (namebuf, "%s.u.msg_unit_attributes", name);
         asn1Print_MsgUnitAttributes (namebuf, pvalue->u.msg_unit_attributes);
         break;

      case 41:
         sprintf (namebuf, "%s.u.msg_unit_pathfind", name);
         asn1Print_MsgUnitPathFind (namebuf, pvalue->u.msg_unit_pathfind);
         break;

      case 42:
         sprintf (namebuf, "%s.u.msg_unit_destruction", name);
         asn1Print_MsgUnitDestruction (namebuf, &pvalue->u.msg_unit_destruction);
         break;

      case 43:
         sprintf (namebuf, "%s.u.msg_unit_environment_type", name);
         asn1Print_MsgUnitEnvironmentType (namebuf, pvalue->u.msg_unit_environment_type);
         break;

      case 44:
         sprintf (namebuf, "%s.u.msg_change_diplomacy", name);
         asn1Print_MsgChangeDiplomacy (namebuf, pvalue->u.msg_change_diplomacy);
         break;

      case 45:
         sprintf (namebuf, "%s.u.msg_unit_change_superior", name);
         asn1Print_MsgUnitChangeSuperior (namebuf, pvalue->u.msg_unit_change_superior);
         break;

      case 46:
         sprintf (namebuf, "%s.u.msg_automat_change_logistic_links", name);
         asn1Print_MsgAutomatChangeLogisticLinks (namebuf, pvalue->u.msg_automat_change_logistic_links);
         break;

      case 47:
         sprintf (namebuf, "%s.u.msg_automat_change_knowledge_group", name);
         asn1Print_MsgAutomatChangeKnowledgeGroup (namebuf, pvalue->u.msg_automat_change_knowledge_group);
         break;

      case 48:
         sprintf (namebuf, "%s.u.msg_automat_change_superior", name);
         asn1Print_MsgAutomatChangeSuperior (namebuf, pvalue->u.msg_automat_change_superior);
         break;

      case 49:
         sprintf (namebuf, "%s.u.msg_unit_knowledge_creation", name);
         asn1Print_MsgUnitKnowledgeCreation (namebuf, pvalue->u.msg_unit_knowledge_creation);
         break;

      case 50:
         sprintf (namebuf, "%s.u.msg_unit_knowledge_update", name);
         asn1Print_MsgUnitKnowledgeUpdate (namebuf, pvalue->u.msg_unit_knowledge_update);
         break;

      case 51:
         sprintf (namebuf, "%s.u.msg_unit_knowledge_destruction", name);
         asn1Print_MsgUnitKnowledgeDestruction (namebuf, pvalue->u.msg_unit_knowledge_destruction);
         break;

      case 52:
         sprintf (namebuf, "%s.u.msg_start_unit_fire", name);
         asn1Print_MsgStartUnitFire (namebuf, pvalue->u.msg_start_unit_fire);
         break;

      case 53:
         sprintf (namebuf, "%s.u.msg_stop_unit_fire", name);
         asn1Print_MsgStopUnitFire (namebuf, pvalue->u.msg_stop_unit_fire);
         break;

      case 54:
         sprintf (namebuf, "%s.u.msg_start_population_fire", name);
         asn1Print_MsgStartPopulationFire (namebuf, pvalue->u.msg_start_population_fire);
         break;

      case 55:
         sprintf (namebuf, "%s.u.msg_stop_population_fire", name);
         asn1Print_MsgStopPopulationFire (namebuf, pvalue->u.msg_stop_population_fire);
         break;

      case 56:
         sprintf (namebuf, "%s.u.msg_explosion", name);
         asn1Print_MsgExplosion (namebuf, pvalue->u.msg_explosion);
         break;

      case 57:
         sprintf (namebuf, "%s.u.msg_start_fire_effect", name);
         asn1Print_MsgStartFireEffect (namebuf, pvalue->u.msg_start_fire_effect);
         break;

      case 58:
         sprintf (namebuf, "%s.u.msg_stop_fire_effect", name);
         asn1Print_MsgStopFireEffect (namebuf, &pvalue->u.msg_stop_fire_effect);
         break;

      case 59:
         sprintf (namebuf, "%s.u.msg_report", name);
         asn1Print_MsgReport (namebuf, pvalue->u.msg_report);
         break;

      case 60:
         sprintf (namebuf, "%s.u.msg_invalidate_report", name);
         asn1Print_MsgInvalidateReport (namebuf, pvalue->u.msg_invalidate_report);
         break;

      case 61:
         sprintf (namebuf, "%s.u.msg_trace", name);
         asn1Print_MsgTrace (namebuf, pvalue->u.msg_trace);
         break;

      case 62:
         sprintf (namebuf, "%s.u.msg_decisional_state", name);
         asn1Print_MsgDecisionalState (namebuf, pvalue->u.msg_decisional_state);
         break;

      case 63:
         sprintf (namebuf, "%s.u.msg_debug_points", name);
         asn1Print_MsgDebugPoints (namebuf, pvalue->u.msg_debug_points);
         break;

      case 64:
         sprintf (namebuf, "%s.u.msg_unit_vision_cones", name);
         asn1Print_MsgUnitVisionCones (namebuf, pvalue->u.msg_unit_vision_cones);
         break;

      case 65:
         sprintf (namebuf, "%s.u.msg_unit_detection", name);
         asn1Print_MsgUnitDetection (namebuf, pvalue->u.msg_unit_detection);
         break;

      case 66:
         sprintf (namebuf, "%s.u.msg_object_detection", name);
         asn1Print_MsgObjectDetection (namebuf, pvalue->u.msg_object_detection);
         break;

      case 67:
         sprintf (namebuf, "%s.u.msg_population_concentration_detection", name);
         asn1Print_MsgPopulationConcentrationDetection (namebuf, pvalue->u.msg_population_concentration_detection);
         break;

      case 68:
         sprintf (namebuf, "%s.u.msg_population_flow_detection", name);
         asn1Print_MsgPopulationFlowDetection (namebuf, pvalue->u.msg_population_flow_detection);
         break;

      case 69:
         sprintf (namebuf, "%s.u.msg_unit_order", name);
         asn1Print_MsgUnitOrder (namebuf, pvalue->u.msg_unit_order);
         break;

      case 70:
         sprintf (namebuf, "%s.u.msg_automat_order", name);
         asn1Print_MsgAutomatOrder (namebuf, pvalue->u.msg_automat_order);
         break;

      case 71:
         sprintf (namebuf, "%s.u.msg_population_order", name);
         asn1Print_MsgPopulationOrder (namebuf, pvalue->u.msg_population_order);
         break;

      case 72:
         sprintf (namebuf, "%s.u.msg_object_creation", name);
         asn1Print_MsgObjectCreation (namebuf, pvalue->u.msg_object_creation);
         break;

      case 73:
         sprintf (namebuf, "%s.u.msg_object_destruction", name);
         asn1Print_MsgObjectDestruction (namebuf, &pvalue->u.msg_object_destruction);
         break;

      case 74:
         sprintf (namebuf, "%s.u.msg_object_update", name);
         asn1Print_MsgObjectUpdate (namebuf, pvalue->u.msg_object_update);
         break;

      case 75:
         sprintf (namebuf, "%s.u.msg_object_knowledge_creation", name);
         asn1Print_MsgObjectKnowledgeCreation (namebuf, pvalue->u.msg_object_knowledge_creation);
         break;

      case 76:
         sprintf (namebuf, "%s.u.msg_object_knowledge_update", name);
         asn1Print_MsgObjectKnowledgeUpdate (namebuf, pvalue->u.msg_object_knowledge_update);
         break;

      case 77:
         sprintf (namebuf, "%s.u.msg_object_knowledge_destruction", name);
         asn1Print_MsgObjectKnowledgeDestruction (namebuf, pvalue->u.msg_object_knowledge_destruction);
         break;

      case 78:
         sprintf (namebuf, "%s.u.msg_log_medical_handling_creation", name);
         asn1Print_MsgLogMedicalHandlingCreation (namebuf, pvalue->u.msg_log_medical_handling_creation);
         break;

      case 79:
         sprintf (namebuf, "%s.u.msg_log_medical_handling_update", name);
         asn1Print_MsgLogMedicalHandlingUpdate (namebuf, pvalue->u.msg_log_medical_handling_update);
         break;

      case 80:
         sprintf (namebuf, "%s.u.msg_log_medical_handling_destruction", name);
         asn1Print_MsgLogMedicalHandlingDestruction (namebuf, pvalue->u.msg_log_medical_handling_destruction);
         break;

      case 81:
         sprintf (namebuf, "%s.u.msg_log_medical_state", name);
         asn1Print_MsgLogMedicalState (namebuf, pvalue->u.msg_log_medical_state);
         break;

      case 82:
         sprintf (namebuf, "%s.u.msg_log_maintenance_handling_creation", name);
         asn1Print_MsgLogMaintenanceHandlingCreation (namebuf, pvalue->u.msg_log_maintenance_handling_creation);
         break;

      case 83:
         sprintf (namebuf, "%s.u.msg_log_maintenance_handling_update", name);
         asn1Print_MsgLogMaintenanceHandlingUpdate (namebuf, pvalue->u.msg_log_maintenance_handling_update);
         break;

      case 84:
         sprintf (namebuf, "%s.u.msg_log_maintenance_handling_destruction", name);
         asn1Print_MsgLogMaintenanceHandlingDestruction (namebuf, pvalue->u.msg_log_maintenance_handling_destruction);
         break;

      case 85:
         sprintf (namebuf, "%s.u.msg_log_maintenance_state", name);
         asn1Print_MsgLogMaintenanceState (namebuf, pvalue->u.msg_log_maintenance_state);
         break;

      case 86:
         sprintf (namebuf, "%s.u.msg_log_supply_handling_creation", name);
         asn1Print_MsgLogSupplyHandlingCreation (namebuf, pvalue->u.msg_log_supply_handling_creation);
         break;

      case 87:
         sprintf (namebuf, "%s.u.msg_log_supply_handling_update", name);
         asn1Print_MsgLogSupplyHandlingUpdate (namebuf, pvalue->u.msg_log_supply_handling_update);
         break;

      case 88:
         sprintf (namebuf, "%s.u.msg_log_supply_handling_destruction", name);
         asn1Print_MsgLogSupplyHandlingDestruction (namebuf, pvalue->u.msg_log_supply_handling_destruction);
         break;

      case 89:
         sprintf (namebuf, "%s.u.msg_log_supply_state", name);
         asn1Print_MsgLogSupplyState (namebuf, pvalue->u.msg_log_supply_state);
         break;

      case 90:
         sprintf (namebuf, "%s.u.msg_log_supply_quotas", name);
         asn1Print_MsgLogSupplyQuotas (namebuf, pvalue->u.msg_log_supply_quotas);
         break;

      case 91:
         sprintf (namebuf, "%s.u.msg_population_creation", name);
         asn1Print_MsgPopulationCreation (namebuf, pvalue->u.msg_population_creation);
         break;

      case 92:
         sprintf (namebuf, "%s.u.msg_population_update", name);
         asn1Print_MsgPopulationUpdate (namebuf, pvalue->u.msg_population_update);
         break;

      case 93:
         sprintf (namebuf, "%s.u.msg_population_concentration_creation", name);
         asn1Print_MsgPopulationConcentrationCreation (namebuf, pvalue->u.msg_population_concentration_creation);
         break;

      case 94:
         sprintf (namebuf, "%s.u.msg_population_concentration_destruction", name);
         asn1Print_MsgPopulationConcentrationDestruction (namebuf, pvalue->u.msg_population_concentration_destruction);
         break;

      case 95:
         sprintf (namebuf, "%s.u.msg_population_concentration_update", name);
         asn1Print_MsgPopulationConcentrationUpdate (namebuf, pvalue->u.msg_population_concentration_update);
         break;

      case 96:
         sprintf (namebuf, "%s.u.msg_population_flow_creation", name);
         asn1Print_MsgPopulationFlowCreation (namebuf, pvalue->u.msg_population_flow_creation);
         break;

      case 97:
         sprintf (namebuf, "%s.u.msg_population_flow_destruction", name);
         asn1Print_MsgPopulationFlowDestruction (namebuf, pvalue->u.msg_population_flow_destruction);
         break;

      case 98:
         sprintf (namebuf, "%s.u.msg_population_flow_update", name);
         asn1Print_MsgPopulationFlowUpdate (namebuf, pvalue->u.msg_population_flow_update);
         break;

      case 99:
         sprintf (namebuf, "%s.u.msg_population_knowledge_creation", name);
         asn1Print_MsgPopulationKnowledgeCreation (namebuf, pvalue->u.msg_population_knowledge_creation);
         break;

      case 100:
         sprintf (namebuf, "%s.u.msg_population_knowledge_update", name);
         asn1Print_MsgPopulationKnowledgeUpdate (namebuf, pvalue->u.msg_population_knowledge_update);
         break;

      case 101:
         sprintf (namebuf, "%s.u.msg_population_knowledge_destruction", name);
         asn1Print_MsgPopulationKnowledgeDestruction (namebuf, pvalue->u.msg_population_knowledge_destruction);
         break;

      case 102:
         sprintf (namebuf, "%s.u.msg_population_concentration_knowledge_creation", name);
         asn1Print_MsgPopulationConcentrationKnowledgeCreation (namebuf, pvalue->u.msg_population_concentration_knowledge_creation);
         break;

      case 103:
         sprintf (namebuf, "%s.u.msg_population_concentration_knowledge_destruction", name);
         asn1Print_MsgPopulationConcentrationKnowledgeDestruction (namebuf, pvalue->u.msg_population_concentration_knowledge_destruction);
         break;

      case 104:
         sprintf (namebuf, "%s.u.msg_population_concentration_knowledge_update", name);
         asn1Print_MsgPopulationConcentrationKnowledgeUpdate (namebuf, pvalue->u.msg_population_concentration_knowledge_update);
         break;

      case 105:
         sprintf (namebuf, "%s.u.msg_population_flow_knowledge_creation", name);
         asn1Print_MsgPopulationFlowKnowledgeCreation (namebuf, pvalue->u.msg_population_flow_knowledge_creation);
         break;

      case 106:
         sprintf (namebuf, "%s.u.msg_population_flow_knowledge_destruction", name);
         asn1Print_MsgPopulationFlowKnowledgeDestruction (namebuf, pvalue->u.msg_population_flow_knowledge_destruction);
         break;

      case 107:
         sprintf (namebuf, "%s.u.msg_population_flow_knowledge_update", name);
         asn1Print_MsgPopulationFlowKnowledgeUpdate (namebuf, pvalue->u.msg_population_flow_knowledge_update);
         break;

      case 108:
         sprintf (namebuf, "%s.u.msg_folk_creation", name);
         asn1Print_MsgFolkCreation (namebuf, pvalue->u.msg_folk_creation);
         break;

      case 109:
         sprintf (namebuf, "%s.u.msg_folk_graph_update", name);
         asn1Print_MsgFolkGraphUpdate (namebuf, pvalue->u.msg_folk_graph_update);
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
         sprintf (namebuf, "%s.u.msg_control_stop", name);
         break;

      case 2:
         sprintf (namebuf, "%s.u.msg_control_pause", name);
         break;

      case 3:
         sprintf (namebuf, "%s.u.msg_control_resume", name);
         break;

      case 4:
         sprintf (namebuf, "%s.u.msg_control_change_time_factor", name);
         asn1Print_MsgControlChangeTimeFactor (namebuf, &pvalue->u.msg_control_change_time_factor);
         break;

      case 5:
         sprintf (namebuf, "%s.u.msg_control_date_time_change", name);
         asn1Print_MsgControlDatetimeChange (namebuf, pvalue->u.msg_control_date_time_change);
         break;

      case 6:
         sprintf (namebuf, "%s.u.msg_control_global_meteo", name);
         asn1Print_MsgControlGlobalMeteo (namebuf, pvalue->u.msg_control_global_meteo);
         break;

      case 7:
         sprintf (namebuf, "%s.u.msg_control_local_meteo", name);
         asn1Print_MsgControlLocalMeteo (namebuf, pvalue->u.msg_control_local_meteo);
         break;

      case 8:
         sprintf (namebuf, "%s.u.msg_control_checkpoint_save_now", name);
         asn1Print_MsgControlCheckPointSaveNow (namebuf, pvalue->u.msg_control_checkpoint_save_now);
         break;

      case 9:
         sprintf (namebuf, "%s.u.msg_control_checkpoint_set_frequency", name);
         asn1Print_MsgControlCheckPointSetFrequency (namebuf, &pvalue->u.msg_control_checkpoint_set_frequency);
         break;

      case 10:
         sprintf (namebuf, "%s.u.msg_control_toggle_vision_cones", name);
         asn1Print_MsgControlToggleVisionCones (namebuf, &pvalue->u.msg_control_toggle_vision_cones);
         break;

      case 11:
         sprintf (namebuf, "%s.u.msg_unit_order", name);
         asn1Print_MsgUnitOrder (namebuf, pvalue->u.msg_unit_order);
         break;

      case 12:
         sprintf (namebuf, "%s.u.msg_automat_order", name);
         asn1Print_MsgAutomatOrder (namebuf, pvalue->u.msg_automat_order);
         break;

      case 13:
         sprintf (namebuf, "%s.u.msg_population_order", name);
         asn1Print_MsgPopulationOrder (namebuf, pvalue->u.msg_population_order);
         break;

      case 14:
         sprintf (namebuf, "%s.u.msg_frag_order", name);
         asn1Print_MsgFragOrder (namebuf, pvalue->u.msg_frag_order);
         break;

      case 15:
         sprintf (namebuf, "%s.u.msg_set_automat_mode", name);
         asn1Print_MsgSetAutomatMode (namebuf, pvalue->u.msg_set_automat_mode);
         break;

      case 16:
         sprintf (namebuf, "%s.u.msg_unit_creation_request", name);
         asn1Print_MsgUnitCreationRequest (namebuf, pvalue->u.msg_unit_creation_request);
         break;

      case 17:
         sprintf (namebuf, "%s.u.msg_unit_magic_action", name);
         asn1Print_MsgUnitMagicAction (namebuf, pvalue->u.msg_unit_magic_action);
         break;

      case 18:
         sprintf (namebuf, "%s.u.msg_object_magic_action", name);
         asn1Print_MsgObjectMagicAction (namebuf, pvalue->u.msg_object_magic_action);
         break;

      case 19:
         sprintf (namebuf, "%s.u.msg_population_magic_action", name);
         asn1Print_MsgPopulationMagicAction (namebuf, pvalue->u.msg_population_magic_action);
         break;

      case 20:
         sprintf (namebuf, "%s.u.msg_change_diplomacy", name);
         asn1Print_MsgChangeDiplomacy (namebuf, pvalue->u.msg_change_diplomacy);
         break;

      case 21:
         sprintf (namebuf, "%s.u.msg_automat_change_knowledge_group", name);
         asn1Print_MsgAutomatChangeKnowledgeGroup (namebuf, pvalue->u.msg_automat_change_knowledge_group);
         break;

      case 22:
         sprintf (namebuf, "%s.u.msg_automat_change_logistic_links", name);
         asn1Print_MsgAutomatChangeLogisticLinks (namebuf, pvalue->u.msg_automat_change_logistic_links);
         break;

      case 23:
         sprintf (namebuf, "%s.u.msg_automat_change_superior", name);
         asn1Print_MsgAutomatChangeSuperior (namebuf, pvalue->u.msg_automat_change_superior);
         break;

      case 24:
         sprintf (namebuf, "%s.u.msg_unit_change_superior", name);
         asn1Print_MsgUnitChangeSuperior (namebuf, pvalue->u.msg_unit_change_superior);
         break;

      case 25:
         sprintf (namebuf, "%s.u.msg_log_supply_push_flow", name);
         asn1Print_MsgLogSupplyPushFlow (namebuf, pvalue->u.msg_log_supply_push_flow);
         break;

      case 26:
         sprintf (namebuf, "%s.u.msg_log_supply_change_quotas", name);
         asn1Print_MsgLogSupplyChangeQuotas (namebuf, pvalue->u.msg_log_supply_change_quotas);
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

void asn1Print_MsgsMiddleToSim_msg (ASN1ConstCharPtr name, ASN1T_MsgsMiddleToSim_msg* pvalue)
{
   char namebuf[512];

   printf ("%s.t = %d\n", name, pvalue->t);
   switch (pvalue->t) {
      case 1:
         sprintf (namebuf, "%s.u.msg_control_client_announcement", name);
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


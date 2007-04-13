// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "SimulationDispatcher.h"
#include "ClientsNetworker.h"
#include "Model.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SimulationDispatcher::SimulationDispatcher( ClientsNetworker& clients, Model& model )
    : clients_( clients )
    , model_  ( model )
    , synching_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SimulationDispatcher::~SimulationDispatcher()
{
    // NOTHING
}


// =============================================================================
// MESSAGES
// =============================================================================

#define DISPATCH_ASN_MSG( NAME )                                        \
    case T_MsgsOutSim_msg_msg_##NAME:                                   \
    {                                                                   \
        ASN1T_MsgsInClient asnOutMsg;                                   \
        asnOutMsg.context          = asnInMsg.context;                  \
        asnOutMsg.msg.t            = T_MsgsInClient_msg_msg_##NAME;     \
        asnOutMsg.msg.u.msg_##NAME = asnInMsg.msg.u.msg_##NAME;         \
        clients_.Send( asnOutMsg );                                     \
        break;                                                          \
    } 

#define DISPATCH_EMPTY_ASN_MSG( NAME )                                  \
    case T_MsgsOutSim_msg_msg_##NAME:                                   \
    {                                                                   \
        ASN1T_MsgsInClient asnOutMsg;                                   \
        asnOutMsg.context          = asnInMsg.context;                  \
        asnOutMsg.msg.t            = T_MsgsInClient_msg_msg_##NAME;     \
        clients_.Send( asnOutMsg );                                     \
        break;                                                          \
    } 

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::IsNotDestruction
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool SimulationDispatcher::IsNotDestruction( const ASN1T_MsgsOutSim& asnMsg ) const
{
    switch( asnMsg.msg.t )
    {
    case T_MsgsInClient_msg_msg_limit_destruction:
    case T_MsgsInClient_msg_msg_lima_destruction:
    case T_MsgsInClient_msg_msg_unit_knowledge_destruction:
    case T_MsgsInClient_msg_msg_object_destruction:
    case T_MsgsInClient_msg_msg_object_knowledge_destruction:
//    case T_MsgsInClient_msg_msg_log_sante_traitement_humain_destruction:
//    case T_MsgsInClient_msg_msg_log_maintenance_traitement_equipement_destruction:
//    case T_MsgsInClient_msg_msg_log_ravitaillement_traitement_destruction:
    case T_MsgsInClient_msg_msg_population_concentration_destruction:
    case T_MsgsInClient_msg_msg_population_flux_destruction:
    case T_MsgsInClient_msg_msg_population_knowledge_destruction:
    case T_MsgsInClient_msg_msg_population_concentration_knowledge_destruction:
    case T_MsgsInClient_msg_msg_population_flux_knowledge_destruction:
        return false;
    default:
        return true;
    };
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::OnReceive
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void SimulationDispatcher::OnReceive( const ASN1T_MsgsOutSim& asnInMsg )
{
    model_.Update( asnInMsg );
    if( synching_ && IsNotDestruction( asnInMsg ) )
        return;
    switch( asnInMsg.msg.t )
    {
        DISPATCH_ASN_MSG( limit_creation_request_ack              );
        DISPATCH_ASN_MSG( limit_destruction_request_ack           );
        DISPATCH_ASN_MSG( limit_update_request_ack                );
        DISPATCH_ASN_MSG( lima_creation_request_ack               );
        DISPATCH_ASN_MSG( lima_destruction_request_ack            );
        DISPATCH_ASN_MSG( lima_update_request_ack                 );
        DISPATCH_ASN_MSG( pion_order_ack                          );
        DISPATCH_ASN_MSG( automate_order_ack                      );
        DISPATCH_ASN_MSG( population_order_ack                    );
        DISPATCH_ASN_MSG( frag_order_ack                          );
        DISPATCH_ASN_MSG( set_automate_mode_ack                   );
        DISPATCH_ASN_MSG( unit_magic_action_ack                   );
        DISPATCH_ASN_MSG( object_magic_action_ack                 );
        DISPATCH_ASN_MSG( population_magic_action_ack             );
        DISPATCH_ASN_MSG( change_diplomatie_ack                   );
        DISPATCH_ASN_MSG( automate_change_groupe_connaissance_ack );
        DISPATCH_ASN_MSG( automate_change_liens_logistiques_ack   );
        DISPATCH_ASN_MSG( pion_change_superior_ack                );
        DISPATCH_ASN_MSG( log_ravitaillement_pousser_flux_ack     );
        DISPATCH_ASN_MSG( log_ravitaillement_change_quotas_ack    );
        DISPATCH_ASN_MSG( ctrl_info                               );
        DISPATCH_ASN_MSG( ctrl_begin_tick                         );
        DISPATCH_ASN_MSG( ctrl_end_tick                           );
        DISPATCH_ASN_MSG( ctrl_stop_ack                           );
        DISPATCH_ASN_MSG( ctrl_pause_ack                          );
        DISPATCH_ASN_MSG( ctrl_resume_ack                         );
        DISPATCH_ASN_MSG( ctrl_change_time_factor_ack             );
        DISPATCH_EMPTY_ASN_MSG( ctrl_meteo_globale_ack            );
        DISPATCH_EMPTY_ASN_MSG( ctrl_meteo_locale_ack             );
        DISPATCH_EMPTY_ASN_MSG( ctrl_checkpoint_save_begin        );
        DISPATCH_EMPTY_ASN_MSG( ctrl_checkpoint_save_end          );
        DISPATCH_EMPTY_ASN_MSG( ctrl_checkpoint_set_frequency_ack );
        DISPATCH_EMPTY_ASN_MSG( ctrl_checkpoint_save_now_ack      );

        DISPATCH_ASN_MSG( limit_creation                                    );
        DISPATCH_ASN_MSG( limit_destruction                                 );
        DISPATCH_ASN_MSG( lima_creation                                     );
        DISPATCH_ASN_MSG( lima_destruction                                  );
        DISPATCH_ASN_MSG( automate_creation                                 );
        DISPATCH_ASN_MSG( pion_creation                                     );
        DISPATCH_ASN_MSG( side_creation                                     );
        DISPATCH_ASN_MSG( knowledge_group_creation                          );
        DISPATCH_ASN_MSG( formation_creation                                );
        DISPATCH_ASN_MSG( change_diplomatie                                 );
        DISPATCH_ASN_MSG( automate_change_liens_logistiques                 );
        DISPATCH_ASN_MSG( unit_knowledge_creation                           );
        DISPATCH_ASN_MSG( unit_knowledge_update                             );
        DISPATCH_ASN_MSG( unit_knowledge_destruction                        );
        DISPATCH_ASN_MSG( unit_attributes                                   );
        DISPATCH_ASN_MSG( automate_attributes                               );
        DISPATCH_ASN_MSG( unit_pathfind                                     );
        DISPATCH_ASN_MSG( start_pion_fire                                   );
        DISPATCH_ASN_MSG( stop_pion_fire                                    );
        DISPATCH_ASN_MSG( start_population_fire                             );
        DISPATCH_ASN_MSG( stop_population_fire                              );
        DISPATCH_ASN_MSG( explosion                                         );
        DISPATCH_ASN_MSG( start_fire_effect                                 );
        DISPATCH_ASN_MSG( stop_fire_effect                                  );
        DISPATCH_ASN_MSG( cr                                                );
        DISPATCH_ASN_MSG( pion_order_management                             );
        DISPATCH_ASN_MSG( automate_order_management                         );
        DISPATCH_ASN_MSG( population_order_management                       );
        DISPATCH_ASN_MSG( pion_order                                        );
        DISPATCH_ASN_MSG( automate_order                                    );
        DISPATCH_ASN_MSG( population_order                                  );
        DISPATCH_ASN_MSG( object_creation                                   );
        DISPATCH_ASN_MSG( object_destruction                                );
        DISPATCH_ASN_MSG( object_update                                     );
        DISPATCH_ASN_MSG( object_knowledge_creation                         );
        DISPATCH_ASN_MSG( object_knowledge_update                           );
        DISPATCH_ASN_MSG( object_knowledge_destruction                      );
        DISPATCH_ASN_MSG( pion_change_superior                              );
        DISPATCH_ASN_MSG( log_sante_traitement_humain_creation              );
        DISPATCH_ASN_MSG( log_sante_traitement_humain_update                );
        DISPATCH_ASN_MSG( log_sante_traitement_humain_destruction           );
        DISPATCH_ASN_MSG( log_sante_etat                                    );
        DISPATCH_ASN_MSG( log_maintenance_traitement_equipement_creation    );
        DISPATCH_ASN_MSG( log_maintenance_traitement_equipement_update      );
        DISPATCH_ASN_MSG( log_maintenance_traitement_equipement_destruction );
        DISPATCH_ASN_MSG( log_maintenance_etat                              );
        DISPATCH_ASN_MSG( log_ravitaillement_traitement_creation            );
        DISPATCH_ASN_MSG( log_ravitaillement_traitement_update              );
        DISPATCH_ASN_MSG( log_ravitaillement_traitement_destruction         );
        DISPATCH_ASN_MSG( log_ravitaillement_etat                           );
        DISPATCH_ASN_MSG( log_ravitaillement_quotas                         );
        DISPATCH_ASN_MSG( population_creation                               );
        DISPATCH_ASN_MSG( population_update                                 );
        DISPATCH_ASN_MSG( population_concentration_creation                 );
        DISPATCH_ASN_MSG( population_concentration_destruction              );
        DISPATCH_ASN_MSG( population_concentration_update                   );
        DISPATCH_ASN_MSG( population_flux_creation                          );
        DISPATCH_ASN_MSG( population_flux_destruction                       );
        DISPATCH_ASN_MSG( population_flux_update                            );
        DISPATCH_ASN_MSG( population_knowledge_creation                     );
        DISPATCH_ASN_MSG( population_knowledge_update                       );
        DISPATCH_ASN_MSG( population_knowledge_destruction                  );
        DISPATCH_ASN_MSG( population_concentration_knowledge_creation       );
        DISPATCH_ASN_MSG( population_concentration_knowledge_destruction    );
        DISPATCH_ASN_MSG( population_concentration_knowledge_update         );
        DISPATCH_ASN_MSG( population_flux_knowledge_creation                );
        DISPATCH_ASN_MSG( population_flux_knowledge_destruction             );
        DISPATCH_ASN_MSG( population_flux_knowledge_update                  );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::OnReceive
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void SimulationDispatcher::OnReceive( const ASN1T_MsgsInClient& asnMsg )
{
    model_.Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::Send
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void SimulationDispatcher::Send( const ASN1T_MsgsInClient& msg )
{
    clients_.Send( msg );
    model_.Update( msg );
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::StartSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void SimulationDispatcher::StartSynchronisation()
{
    synching_ = true;
    model_.StartSynchronisation( *this );
}

// -----------------------------------------------------------------------------
// Name: SimulationDispatcher::EndSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void SimulationDispatcher::EndSynchronisation()
{
    model_.EndSynchronisation();
    synching_ = false;
}

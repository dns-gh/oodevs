// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Simulation.h"

#include "Network_Def.h"
#include "AsnTypes.h"
#include "AsnMessageEncoder.h"
#include "Dispatcher.h"
#include "Model.h"
#include "SimulationPublisher.h"

using namespace dispatcher;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Simulation::Simulation( Dispatcher& dispatcher, DIN_MessageService_ABC& messageService, DIN_Link& link )
    : DIN_UserData_ABC()
    , dispatcher_     ( dispatcher )
    , messageService_ ( messageService )
    , link_           ( link )
{
    assert( !link_.GetUserData() );
    link_.SetUserData( this );        
    messageService_.Enable( link_ );

    AsnMsgInSimCtrlClientAnnouncement asnMsg;
    asnMsg() = MsgCtrlClientAnnouncement::mos_light;

    SimulationPublisher publisher( *this );
    asnMsg.Send( publisher );
}

//-----------------------------------------------------------------------------
// Name: Simulation destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
Simulation::~Simulation()
{
    messageService_.Disable( link_ );

    assert( link_.GetUserData() == this );
    link_.SetUserData( 0 );
}

// =============================================================================
// MESSAGES
// =============================================================================

//  $$$
#define DISPATCH_ASN_MSG( NAME )                                        \
    case T_MsgsOutSim_msg_msg_##NAME:                                   \
    {                                                                   \
        asnOutMsg.context          = asnInMsg.context;                  \
        asnOutMsg.msg.t            = T_MsgsInClient_msg_msg_##NAME;     \
        asnOutMsg.msg.u.msg_##NAME = asnInMsg.msg.u.msg_##NAME;         \
        dispatcher_.DispatchToClients( asnOutMsg) ;                     \
        break;                                                          \
    } 

#define DISPATCH_EMPTY_ASN_MSG( NAME )                                  \
    case T_MsgsOutSim_msg_msg_##NAME:                                   \
    {                                                                   \
        asnOutMsg.context          = asnInMsg.context;                  \
        asnOutMsg.msg.t            = T_MsgsInClient_msg_msg_##NAME;     \
        dispatcher_.DispatchToClients( asnOutMsg) ;                     \
        break;                                                          \
    } 

// -----------------------------------------------------------------------------
// Name: Simulation::OnReceive
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Simulation::OnReceive( const ASN1T_MsgsOutSim& asnInMsg )
{
    dispatcher_.GetModel().Update( asnInMsg );

    ASN1T_MsgsInClient asnOutMsg;
    switch( asnInMsg.msg.t )
    {
        DISPATCH_ASN_MSG( limit_creation_ack                                );
        DISPATCH_ASN_MSG( limit_destruction_ack                             );
        DISPATCH_ASN_MSG( limit_update_ack                                  );
        DISPATCH_ASN_MSG( lima_creation_ack                                 );
        DISPATCH_ASN_MSG( lima_destruction_ack                              );
        DISPATCH_ASN_MSG( lima_update_ack                                   );
        DISPATCH_ASN_MSG( pion_order_ack                                    );
        DISPATCH_ASN_MSG( order_conduite_ack                                );
        DISPATCH_ASN_MSG( automate_order_ack                                );
        DISPATCH_ASN_MSG( population_order_ack                              );
        DISPATCH_ASN_MSG( set_automate_mode_ack                             );
        DISPATCH_ASN_MSG( unit_magic_action_ack                             );
        DISPATCH_ASN_MSG( object_magic_action_ack                           );
        DISPATCH_ASN_MSG( population_magic_action_ack                       );
        DISPATCH_ASN_MSG( change_diplomatie_ack                             );
        DISPATCH_ASN_MSG( change_groupe_connaissance_ack                    );
        DISPATCH_ASN_MSG( change_liens_logistiques_ack                      );
        DISPATCH_ASN_MSG( change_automate_ack                               );
        DISPATCH_ASN_MSG( log_ravitaillement_pousser_flux_ack               );
        DISPATCH_ASN_MSG( log_ravitaillement_change_quotas_ack              );
        DISPATCH_ASN_MSG( ctrl_info                                         );
        DISPATCH_ASN_MSG( ctrl_begin_tick                                   );
        DISPATCH_ASN_MSG( ctrl_end_tick                                     );
        DISPATCH_ASN_MSG( ctrl_stop_ack                                     );
        DISPATCH_ASN_MSG( ctrl_pause_ack                                    );
        DISPATCH_ASN_MSG( ctrl_resume_ack                                   );
        DISPATCH_ASN_MSG( ctrl_change_time_factor_ack                       );
        DISPATCH_EMPTY_ASN_MSG( ctrl_meteo_globale_ack                            );
        DISPATCH_EMPTY_ASN_MSG( ctrl_meteo_locale_ack                             );
        DISPATCH_EMPTY_ASN_MSG( ctrl_checkpoint_save_begin                        );
        DISPATCH_EMPTY_ASN_MSG( ctrl_checkpoint_save_end                          );
        DISPATCH_EMPTY_ASN_MSG( ctrl_checkpoint_load_begin                        );
        DISPATCH_EMPTY_ASN_MSG( ctrl_checkpoint_load_end                          );
        DISPATCH_EMPTY_ASN_MSG( ctrl_checkpoint_set_frequency_ack                 );
        DISPATCH_EMPTY_ASN_MSG( ctrl_checkpoint_save_now_ack                      );
        DISPATCH_EMPTY_ASN_MSG( ctrl_send_current_state_begin                     );
        DISPATCH_EMPTY_ASN_MSG( ctrl_send_current_state_end                       );
        DISPATCH_ASN_MSG( limit_creation                                    );
        DISPATCH_ASN_MSG( limit_destruction                                 );
        DISPATCH_ASN_MSG( lima_creation                                     );
        DISPATCH_ASN_MSG( lima_destruction                                  );
        DISPATCH_ASN_MSG( automate_creation                                 );
        DISPATCH_ASN_MSG( pion_creation                                     );
        DISPATCH_ASN_MSG( change_diplomatie                                 );
        DISPATCH_ASN_MSG( unit_knowledge_creation                           );
        DISPATCH_ASN_MSG( unit_knowledge_update                             );
        DISPATCH_ASN_MSG( unit_knowledge_destruction                        );
        DISPATCH_ASN_MSG( unit_attributes                                   );
        DISPATCH_ASN_MSG( unit_dotations                                    );
        DISPATCH_ASN_MSG( unit_pathfind                                     );
        DISPATCH_ASN_MSG( start_pion_fire                                   );
        DISPATCH_ASN_MSG( stop_pion_fire                                    );
        DISPATCH_ASN_MSG( start_population_fire                             );
        DISPATCH_ASN_MSG( stop_population_fire                              );
        DISPATCH_ASN_MSG( explosion                                         );
        DISPATCH_ASN_MSG( start_fire_effect                                 );
        DISPATCH_ASN_MSG( stop_fire_effect                                  );
        DISPATCH_ASN_MSG( cr                                                );
        DISPATCH_ASN_MSG( order_management                                  );
        DISPATCH_ASN_MSG( automate_mrt                                      );
        DISPATCH_ASN_MSG( pion_order                                        );
        DISPATCH_ASN_MSG( automate_order                                    );
        DISPATCH_ASN_MSG( object_creation                                   );
        DISPATCH_ASN_MSG( object_destruction                                );
        DISPATCH_ASN_MSG( object_update                                     );
        DISPATCH_ASN_MSG( object_knowledge_creation                         );
        DISPATCH_ASN_MSG( object_knowledge_update                           );
        DISPATCH_ASN_MSG( object_knowledge_destruction                      );
        DISPATCH_ASN_MSG( change_automate                                   );
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
// Name: Simulation::OnReceive
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Simulation::OnReceive( unsigned int nMsgID, DIN::DIN_Input& dinMsg )
{
    dispatcher_.DispatchToClients( nMsgID, dinMsg );
    dispatcher_.GetModel().Update( nMsgID, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Simulation::Send( const ASN1T_MsgsInSim& asnMsg )
{
    AsnMessageEncoder< ASN1T_MsgsInSim, ASN1C_MsgsInSim > asnEncoder( messageService_, asnMsg );
    messageService_.Send( link_, eMsgInSim, asnEncoder.GetDinMsg() );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Simulation::Send( const ASN1T_MsgsInSim& /*asnMsg*/, const DIN_BufferedMessage& dinMsg )
{
    messageService_.Send( link_, eMsgInSim, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Simulation::Send( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg )
{
    messageService_.Send( link_, nMsgID, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetDinMsg
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
DIN_BufferedMessage Simulation::GetDinMsg()
{
    return DIN_BufferedMessage( messageService_ );
}

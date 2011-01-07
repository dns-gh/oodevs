// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "SimulationToClient.h"
#include "Common.h"

using namespace shield;

#define CONVERT_POSTURE( field ) \
    CONVERT_ENUM( field, ( sword::UnitAttributes::mouvement, MsgsSimToClient::MsgUnitAttributes::mouvement ) \
                         ( sword::UnitAttributes::mouvement_discret, MsgsSimToClient::MsgUnitAttributes::mouvement_discret ) \
                         ( sword::UnitAttributes::arret, MsgsSimToClient::MsgUnitAttributes::arret ) \
                         ( sword::UnitAttributes::poste_reflexe, MsgsSimToClient::MsgUnitAttributes::poste_reflexe ) \
                         ( sword::UnitAttributes::poste, MsgsSimToClient::MsgUnitAttributes::poste ) \
                         ( sword::UnitAttributes::poste_amenage, MsgsSimToClient::MsgUnitAttributes::poste_amenage ) \
                         ( sword::UnitAttributes::poste_prepare_genie, MsgsSimToClient::MsgUnitAttributes::poste_prepare_genie ) )

#define CONVERT_IDENTIFICATION_LEVEL( field ) \
    if( from.has_##field() ) \
        CONVERT_ENUM( field, ( sword::identifiee, MsgsSimToClient::identifiee ) \
                             ( sword::reconnue, MsgsSimToClient::reconnue ) \
                             ( sword::detectee, MsgsSimToClient::detectee ) \
                             ( sword::signale, MsgsSimToClient::signale ) )

#define CONVERT_RANK( field ) \
        CONVERT_ENUM( field, ( sword::officier, Common::officier ) \
                             ( sword::sous_officer, Common::sous_officer ) \
                             ( sword::mdr, Common::mdr ) )

namespace
{
    template< typename From, typename To >
    void ConvertOrderAckErrorCode( const From& from, To* to )
    {
        CONVERT_ENUM( error_code, ( sword::OrderAck::no_error, MsgsSimToClient::OrderAck::no_error )
                                  ( sword::OrderAck::error_invalid_unit, MsgsSimToClient::OrderAck::error_invalid_unit )
                                  ( sword::OrderAck::error_invalid_limit, MsgsSimToClient::OrderAck::error_invalid_limit )
                                  ( sword::OrderAck::error_invalid_lima, MsgsSimToClient::OrderAck::error_invalid_lima )
                                  ( sword::OrderAck::error_invalid_mission, MsgsSimToClient::OrderAck::error_invalid_mission )
                                  ( sword::OrderAck::error_invalid_mission_parameters, MsgsSimToClient::OrderAck::error_invalid_mission_parameters )
                                  ( sword::OrderAck::error_unit_cannot_receive_order, MsgsSimToClient::OrderAck::error_unit_cannot_receive_order )
                                  ( sword::OrderAck::error_invalid_order_conduite, MsgsSimToClient::OrderAck::error_invalid_order_conduite )
                                  ( sword::OrderAck::error_invalid_order_mission, MsgsSimToClient::OrderAck::error_invalid_order_mission )
                                  ( sword::OrderAck::error_invalid_order_initial, MsgsSimToClient::OrderAck::error_invalid_order_initial )
                                  ( sword::OrderAck::error_invalid_order_conduite_parameters, MsgsSimToClient::OrderAck::error_invalid_order_conduite_parameters )
                                  ( sword::OrderAck::error_unit_surrendered, MsgsSimToClient::OrderAck::error_unit_surrendered )
                                  ( sword::OrderAck::error_invalid_lima_function, MsgsSimToClient::OrderAck::error_invalid_lima_function ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::ConvertOrderAckToSpecificOrderAck
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void SimulationToClient::ConvertOrderAckToSpecificOrderAck( const sword::TaskCreationRequestAck& from, MsgsSimToClient::MsgSimToClient_Content* to )
{
    if( from.tasker().has_unit() )
    {
        to->mutable_unit_order_ack()->mutable_tasker()->set_id( from.tasker().unit().id() );
        ConvertOrderAckErrorCode( from, to->mutable_unit_order_ack() );
    }
    else if( from.tasker().has_automat() )
    {
        to->mutable_automat_order_ack()->mutable_tasker()->set_id( from.tasker().automat().id() );
        ConvertOrderAckErrorCode( from, to->mutable_automat_order_ack() );
    }
    else if( from.tasker().has_crowd() )
    {
        to->mutable_crowd_order_ack()->mutable_tasker()->set_id( from.tasker().crowd().id() );
        ConvertOrderAckErrorCode( from, to->mutable_crowd_order_ack() );
    } 
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::FragOrderAck& from, MsgsSimToClient::MsgFragOrderAck* to )
{
    ConvertTasker( from.tasker(), to->mutable_tasker() );
    ConvertOrderAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::SetAutomatModeAck& from, MsgsSimToClient::MsgSetAutomatModeAck* to )
{
    CONVERT_ID( automate );
    CONVERT_ENUM( error_code, ( sword::SetAutomatModeAck::no_error, MsgsSimToClient::MsgSetAutomatModeAck::no_error )
                              ( sword::SetAutomatModeAck::error_invalid_unit, MsgsSimToClient::MsgSetAutomatModeAck::error_invalid_unit )
                              ( sword::SetAutomatModeAck::error_not_allowed, MsgsSimToClient::MsgSetAutomatModeAck::error_not_allowed ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitCreationRequestAck& from, MsgsSimToClient::MsgUnitCreationRequestAck* to )
{
    CONVERT_ENUM( error, ( sword::UnitActionAck::no_error, MsgsSimToClient::UnitActionAck::no_error )
                         ( sword::UnitActionAck::error_invalid_unit, MsgsSimToClient::UnitActionAck::error_invalid_unit )
                         ( sword::UnitActionAck::error_automate_embraye, MsgsSimToClient::UnitActionAck::error_automate_embraye )
                         ( sword::UnitActionAck::error_invalid_attribute, MsgsSimToClient::UnitActionAck::error_invalid_attribute )
                         ( sword::UnitActionAck::error_unit_surrendered, MsgsSimToClient::UnitActionAck::error_unit_surrendered ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::MagicActionAck& from, MsgsSimToClient::MsgMagicActionAck* to )
{
    CONVERT_ENUM( error_code, ( sword::MagicActionAck::no_error, MsgsSimToClient::MsgMagicActionAck::no_error )
                              ( sword::MagicActionAck::error_invalid_attribute, MsgsSimToClient::MsgMagicActionAck::error_invalid_attribute ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitMagicActionAck& from, MsgsSimToClient::MsgUnitMagicActionAck* to )
{
    CONVERT_ID( unit );
    CONVERT_ENUM( error_code, ( sword::UnitActionAck::no_error, MsgsSimToClient::UnitActionAck::no_error )
                              ( sword::UnitActionAck::error_invalid_unit, MsgsSimToClient::UnitActionAck::error_invalid_unit )
                              ( sword::UnitActionAck::error_automate_embraye, MsgsSimToClient::UnitActionAck::error_automate_embraye )
                              ( sword::UnitActionAck::error_invalid_attribute, MsgsSimToClient::UnitActionAck::error_invalid_attribute )
                              ( sword::UnitActionAck::error_unit_surrendered, MsgsSimToClient::UnitActionAck::error_unit_surrendered ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ObjectMagicActionAck& from, MsgsSimToClient::MsgObjectMagicActionAck* to )
{
    CONVERT_ENUM( error_code, ( sword::ObjectMagicActionAck::no_error, MsgsSimToClient::MsgObjectMagicActionAck::no_error )
                              ( sword::ObjectMagicActionAck::error_invalid_object, MsgsSimToClient::MsgObjectMagicActionAck::error_invalid_object )
                              ( sword::ObjectMagicActionAck::error_invalid_id, MsgsSimToClient::MsgObjectMagicActionAck::error_invalid_id )
                              ( sword::ObjectMagicActionAck::error_invalid_camp, MsgsSimToClient::MsgObjectMagicActionAck::error_invalid_camp )
                              ( sword::ObjectMagicActionAck::error_invalid_localisation, MsgsSimToClient::MsgObjectMagicActionAck::error_invalid_localisation )
                              ( sword::ObjectMagicActionAck::error_missing_specific_attributes, MsgsSimToClient::MsgObjectMagicActionAck::error_missing_specific_attributes )
                              ( sword::ObjectMagicActionAck::error_invalid_specific_attributes, MsgsSimToClient::MsgObjectMagicActionAck::error_invalid_specific_attributes ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdMagicActionAck& from, MsgsSimToClient::MsgCrowdMagicActionAck* to )
{
    CONVERT_ID( crowd );
    CONVERT_ENUM( error_code, ( sword::CrowdMagicActionAck::no_error, MsgsSimToClient::MsgCrowdMagicActionAck::no_error )
                              ( sword::CrowdMagicActionAck::error_invalid_unit, MsgsSimToClient::MsgCrowdMagicActionAck::error_invalid_unit )
                              ( sword::CrowdMagicActionAck::error_invalid_attribute, MsgsSimToClient::MsgCrowdMagicActionAck::error_invalid_attribute ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ChangeDiplomacyAck& from, MsgsSimToClient::MsgChangeDiplomacyAck* to )
{
    CONVERT_ID( party1 );
    CONVERT_ID( party2 );
    CONVERT_DIPLOMACY( diplomatie );
    CONVERT_ENUM( error_code, ( sword::ChangeDiplomacyAck::no_error_diplomacy, MsgsSimToClient::MsgChangeDiplomacyAck::no_error_diplomacy )
                              ( sword::ChangeDiplomacyAck::error_invalid_camp_diplomacy, MsgsSimToClient::MsgChangeDiplomacyAck::error_invalid_camp_diplomacy ) );
}

namespace
{
    template< typename From, typename To >
    void ConvertHierarchyModificationAckErrorCode( const From& from, To* to )
    {
        CONVERT_ENUM( error_code, ( sword::HierarchyModificationAck::no_error_hierarchy, MsgsSimToClient::HierarchyModificationAck::no_error_hierarchy )
                                  ( sword::HierarchyModificationAck::error_invalid_pion, MsgsSimToClient::HierarchyModificationAck::error_invalid_pion )
                                  ( sword::HierarchyModificationAck::error_invalid_automate, MsgsSimToClient::HierarchyModificationAck::error_invalid_automate )
                                  ( sword::HierarchyModificationAck::error_invalid_formation, MsgsSimToClient::HierarchyModificationAck::error_invalid_formation )
                                  ( sword::HierarchyModificationAck::error_invalid_automate_tc2, MsgsSimToClient::HierarchyModificationAck::error_invalid_automate_tc2 )
                                  ( sword::HierarchyModificationAck::error_invalid_automate_maintenance, MsgsSimToClient::HierarchyModificationAck::error_invalid_automate_maintenance )
                                  ( sword::HierarchyModificationAck::error_invalid_automate_sante, MsgsSimToClient::HierarchyModificationAck::error_invalid_automate_sante )
                                  ( sword::HierarchyModificationAck::error_invalid_automate_supply, MsgsSimToClient::HierarchyModificationAck::error_invalid_automate_supply )
                                  ( sword::HierarchyModificationAck::error_unit_surrendered_hierarchy, MsgsSimToClient::HierarchyModificationAck::error_unit_surrendered_hierarchy )
                                  ( sword::HierarchyModificationAck::error_invalid_party_hierarchy, MsgsSimToClient::HierarchyModificationAck::error_invalid_party_hierarchy )
                                  ( sword::HierarchyModificationAck::error_invalid_knowledge_group, MsgsSimToClient::HierarchyModificationAck::error_invalid_knowledge_group )
                                  ( sword::HierarchyModificationAck::error_parties_mismatched, MsgsSimToClient::HierarchyModificationAck::error_parties_mismatched ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyPushFlowAck& from, MsgsSimToClient::MsgLogSupplyPushFlowAck* to )
{
    CONVERT_ENUM( ack, ( sword::LogSupplyPushFlowAck::no_error_pushflow, MsgsSimToClient::MsgLogSupplyPushFlowAck::no_error_pushflow )
                       ( sword::LogSupplyPushFlowAck::error_invalid_donneur_pushflow, MsgsSimToClient::MsgLogSupplyPushFlowAck::error_invalid_donneur_pushflow )
                       ( sword::LogSupplyPushFlowAck::error_invalid_receveur_pushflow, MsgsSimToClient::MsgLogSupplyPushFlowAck::error_invalid_receveur_pushflow ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyPullFlowAck& from, MsgsSimToClient::MsgLogSupplyPullFlowAck* to )
{
    CONVERT_ENUM( ack, ( sword::LogSupplyPullFlowAck::no_error_pullflow, MsgsSimToClient::MsgLogSupplyPullFlowAck::no_error_pullflow )
                       ( sword::LogSupplyPullFlowAck::error_invalid_provider_pullflow, MsgsSimToClient::MsgLogSupplyPullFlowAck::error_invalid_provider_pullflow )
                       ( sword::LogSupplyPullFlowAck::error_invalid_receiver_pullflow, MsgsSimToClient::MsgLogSupplyPullFlowAck::error_invalid_receiver_pullflow ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyChangeQuotasAck& from, MsgsSimToClient::MsgLogSupplyChangeQuotasAck* to )
{
    CONVERT_ENUM( ack, ( sword::LogSupplyChangeQuotasAck::no_error_quotas, MsgsSimToClient::MsgLogSupplyChangeQuotasAck::no_error_quotas )
                          ( sword::LogSupplyChangeQuotasAck::error_invalid_donneur_quotas, MsgsSimToClient::MsgLogSupplyChangeQuotasAck::error_invalid_donneur_quotas )
                          ( sword::LogSupplyChangeQuotasAck::error_invalid_receveur_quotas, MsgsSimToClient::MsgLogSupplyChangeQuotasAck::error_invalid_receveur_quotas ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlInformation& from, MsgsSimToClient::MsgControlInformation* to )
{
    CONVERT( current_tick );
    to->mutable_initial_date_time()->set_data( from.initial_date_time().data() );
    to->mutable_date_time()->set_data( from.date_time().data() );
    CONVERT( tick_duration );
    CONVERT( time_factor );
    CONVERT( checkpoint_frequency );
    CONVERT_SIMULATION_STATE( status );
    CONVERT( send_vision_cones );
    CONVERT( profiling_enabled );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlProfilingInformation& from, MsgsSimToClient::MsgControlProfilingInformation* to )
{
    CONVERT( perception );
    CONVERT( decision );
    CONVERT( action );
    CONVERT( main_loop );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlBeginTick& from, MsgsSimToClient::MsgControlBeginTick* to )
{
    CONVERT( current_tick );
    to->mutable_date_time()->set_data( from.date_time().data() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlEndTick& from, MsgsSimToClient::MsgControlEndTick* to )
{
    CONVERT( current_tick );
    CONVERT( tick_duration );
    CONVERT( long_pathfinds );
    CONVERT( short_pathfinds );
    CONVERT( memory );
    CONVERT( virtual_memory );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::AutomatChangeKnowledgeGroupAck& from, MsgsSimToClient::MsgAutomatChangeKnowledgeGroupAck* to )
{
    ConvertHierarchyModificationAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ChangeLogisticLinksAck& from, MsgsSimToClient::MsgChangeLogisticLinksAck* to )
{
    ConvertHierarchyModificationAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::AutomatChangeSuperiorAck& from, MsgsSimToClient::MsgAutomatChangeSuperiorAck* to )
{
    ConvertHierarchyModificationAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitChangeSuperiorAck& from, MsgsSimToClient::MsgUnitChangeSuperiorAck* to )
{
    ConvertHierarchyModificationAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlStopAck& from, MsgsSimToClient::MsgControlStopAck* to )
{
    ConvertControlAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlPauseAck& from, MsgsSimToClient::MsgControlPauseAck* to )
{
    ConvertControlAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlResumeAck& from, MsgsSimToClient::MsgControlResumeAck* to )
{
    ConvertControlAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlChangeTimeFactorAck& from, MsgsSimToClient::MsgControlChangeTimeFactorAck* to )
{
    CONVERT( time_factor );
    ConvertControlAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlDatetimeChangeAck& from, MsgsSimToClient::MsgControlDatetimeChangeAck* to )
{
    ConvertControlAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlCheckPointSaveEnd& from, MsgsSimToClient::MsgControlCheckPointSaveEnd* to )
{
    CONVERT( name );
}

namespace
{
    template< typename From, typename To >
    void ConvertParentEntity( const From& from, To* to )
    {
        CONVERT_ID( automat );
        CONVERT_ID( formation );
    }
    template< typename From, typename To >
    void ConvertEntry( const From& from, To* to )
    {
        CONVERT( name );
        CONVERT( value );
    }
    template< typename From, typename To >
    void ConvertExtension( const From& from, To* to )
    {
        for( int i = 0; i < from.entries().size(); ++i )
            ConvertEntry( from.entries( i ), to->add_entries() );
    }
    template< typename From, typename To >
    void ConvertLogisticLevel( const From& from, To* to )
    {
        CONVERT_ENUM( logistic_level, ( sword::none, Common::none )
                                      ( sword::tc2, Common::tc2 )
                                      ( sword::logistic_base, Common::logistic_base ) );
    }
    template< typename From, typename To >
    void ConvertRgbColor( const From& from, To* to )
    {
        CONVERT( red );
        CONVERT( green );
        CONVERT( blue );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::FormationCreation& from, MsgsSimToClient::MsgFormationCreation* to )
{
    CONVERT_ID( formation );
    CONVERT_ID( party );
    CONVERT_ID( parent );
    ConvertNatureLevel( from, to );
    CONVERT( name );
    if( from.has_extension() )
        ConvertExtension( from.extension(), to->mutable_extension() );
    CONVERT( app6symbol );
    ConvertLogisticLevel( from, to );
    if( from.has_color() )
        ConvertRgbColor( from.color(), to->mutable_color() );
    if( from.has_logistic_base_organic() )
        ConvertParentEntity( from.logistic_base_organic(), to->mutable_logistic_base_organic() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::PartyCreation& from, MsgsSimToClient::MsgPartyCreation* to )
{
    CONVERT_ID( party );
    CONVERT( name );
    CONVERT_DIPLOMACY( type );
    if( from.has_extension() )
        ConvertExtension( from.extension(), to->mutable_extension() );
    if( from.has_color() )
        ConvertRgbColor( from.color(), to->mutable_color() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::AutomatCreation& from, MsgsSimToClient::MsgAutomatCreation* to )
{
    CONVERT_ID( automat );
    CONVERT_ID( type );
    CONVERT( nom ); // $$$$ MCO : rename to 'name'
    ConvertParentEntity( from.parent(), to->mutable_parent() );
    CONVERT_ID( party );
    CONVERT_ID( knowledge_group );
    if( from.has_extension() )
        ConvertExtension( from.extension(), to->mutable_extension() );
    CONVERT( app6symbol );
    ConvertLogisticLevel( from, to );
    if( from.has_color() )
        ConvertRgbColor( from.color(), to->mutable_color() );
}

namespace
{
    template< typename From, typename To >
    void ConvertForceRatio( const From& from, To* to )
    {
        CONVERT_ENUM( rapport_de_force, ( sword::ForceRatio::none_force_ratio, MsgsSimToClient::ForceRatio::none_force_ratio )
                                           ( sword::ForceRatio::neutre, MsgsSimToClient::ForceRatio::neutre )
                                           ( sword::ForceRatio::favorable, MsgsSimToClient::ForceRatio::favorable )
                                           ( sword::ForceRatio::defavorable, MsgsSimToClient::ForceRatio::defavorable ) );
    }
    template< typename From, typename To >
    void ConvertMeetingEngagementStatus( const From& from, To* to )
    {
        CONVERT_ENUM( combat_de_rencontre, ( sword::none_meeting, Common::none_meeting )
                                              ( sword::etat_esquive, Common::etat_esquive )
                                              ( sword::etat_fixe, Common::etat_fixe )
                                              ( sword::etat_poursuite_mission, Common::etat_poursuite_mission ) );
    }
    template< typename From, typename To >
    void ConvertOperationalStatus( const From& from, To* to )
    {
        CONVERT_ENUM( etat_operationnel, ( sword::detruit_totalement, Common::detruit_totalement )
                                            ( sword::detruit_tactiquement, Common::detruit_tactiquement )
                                            ( sword::operationnel, Common::operationnel ) );
    }
    template< typename From, typename To >
    void ConvertRulesOfEngagement( const From& from, To* to )
    {
        CONVERT_ENUM( roe, ( sword::RulesOfEngagement::none_roe, MsgsSimToClient::RulesOfEngagement::none_roe )
                              ( sword::RulesOfEngagement::tir_libre, MsgsSimToClient::RulesOfEngagement::tir_libre )
                              ( sword::RulesOfEngagement::tir_sur_riposte, MsgsSimToClient::RulesOfEngagement::tir_sur_riposte )
                              ( sword::RulesOfEngagement::tir_interdit, MsgsSimToClient::RulesOfEngagement::tir_interdit ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::AutomatAttributes& from, MsgsSimToClient::MsgAutomatAttributes* to )
{
    CONVERT_ID( automat );
    CONVERT_AUTOMAT_MODE( etat_automate );
    ConvertForceRatio( from, to );
    ConvertMeetingEngagementStatus( from, to );
    ConvertOperationalStatus( from, to );
    ConvertRulesOfEngagement( from, to );
    CONVERT_ID( tc2_organic );
    if( from.has_logistic_base_organic() )
        ConvertParentEntity( from.logistic_base_organic(), to->mutable_logistic_base_organic() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitCreation& from, MsgsSimToClient::MsgUnitCreation* to )
{
    CONVERT_ID( unit );
    CONVERT_ID( type );
    CONVERT( nom ); // $$$$ MCO : rename to 'name'
    CONVERT_ID( automat );
    CONVERT( pc );
    if( from.has_color() )
        ConvertRgbColor( from.color(), to->mutable_color() );
}

namespace
{
    template< typename From, typename To >
    void ConvertHumanDotation( const From& from, To* to )
    {
        CONVERT_RANK( rang );
        CONVERT( nb_total );
        CONVERT( nb_operationnels );
        CONVERT( nb_morts );
        CONVERT( nb_blesses );
        CONVERT( nb_blesses_mentaux );
        CONVERT( nb_contamines_nbc );
        CONVERT( nb_dans_chaine_sante );
        CONVERT( nb_utilises_pour_maintenance );
        CONVERT( nb_blesses_non_evacues );
    }
    template< typename From, typename To >
    void ConvertEquipmentDotation( const From& from, To* to )
    {
        CONVERT_ID( type );
        CONVERT( nb_disponibles );
        CONVERT( nb_indisponibles );
        CONVERT( nb_reparables );
        CONVERT( nb_dans_chaine_maintenance );
        CONVERT( nb_prisonniers );
    }
    template< typename From, typename To >
    void ConvertResourceDotation( const From& from, To* to )
    {
        CONVERT_ID( type );
        CONVERT( quantite_disponible );
    }
    template< typename From, typename To >
    void ConvertLentEquipment( const From& from, To* to )
    {
        CONVERT_ID( borrower );
        CONVERT_ID( type );
        CONVERT( nombre );
    }
    template< typename From, typename To >
    void ConvertBorrowedEquipment( const From& from, To* to )
    {
        CONVERT_ID( owner );
        CONVERT_ID( type );
        CONVERT( nombre );
    }
    template< typename From, typename To >
    void ConvertContaminationState( const From& from, To* to )
    {
        CONVERT( percentage );
        CONVERT( quantity );
    }
    template< typename From, typename To >
    void ConvertContaminations( const From& from, To* to )
    {
        CONVERT( jammed );
        CONVERT_ID( knowledge_group );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitAttributes& from, MsgsSimToClient::MsgUnitAttributes* to )
{
    CONVERT_ID( unit );
    CONVERT_LIST( dotation_eff_personnel, elem, ConvertHumanDotation );
    CONVERT_LIST( dotation_eff_materiel, elem, ConvertEquipmentDotation );
    CONVERT_LIST( dotation_eff_ressource, elem, ConvertResourceDotation );
    CONVERT_LIST( equipements_pretes, elem, ConvertLentEquipment );
    CONVERT_LIST( equipements_empruntes, elem, ConvertBorrowedEquipment );
    if( from.has_position() )
        ConvertCoordLatLong( from.position(), to->mutable_position() );
    if( from.has_direction() )
        ConvertHeading( from.direction(), to->mutable_direction() );
    CONVERT( hauteur );
    CONVERT( altitude );
    CONVERT( vitesse );
    CONVERT( etat_operationnel_brut );
    CONVERT_LIST( reinforcements, elem, ConvertIdentifier );
    CONVERT_ID( reinforced_unit );
    CONVERT( mort );
    CONVERT( neutralise );
    CONVERT( mode_furtif_actif );
    CONVERT( embarque );
    CONVERT( transporteurs_disponibles );
    CONVERT_POSTURE( posture_old );
    CONVERT_POSTURE( posture_new );
    CONVERT( posture_pourcentage );
    CONVERT( etat_installation );
    CONVERT( en_tenue_de_protection_nbc );
    CONVERT_LIST( contamine_par_agents_nbc, elem, ConvertIdentifier );
    if( from.has_etat_contamination() )
        ConvertContaminationState( from.etat_contamination(), to->mutable_etat_contamination() );
    if( from.has_communications() )
        ConvertContaminations( from.communications(), to->mutable_communications() );
    CONVERT( radio_emitter_disabled );
    CONVERT( radio_receiver_disabled );
    CONVERT( radar_actif );
    CONVERT_LIST( transported_units, elem, ConvertIdentifier );
    CONVERT_ID( transporting_unit );
    ConvertForceRatio( from, to );
    ConvertMeetingEngagementStatus( from, to );
    ConvertOperationalStatus( from, to );
    CONVERT_ENUM( disponibilite_au_tir_indirect, ( sword::UnitAttributes::none_fire_available, MsgsSimToClient::MsgUnitAttributes::none_fire_available )
                                                    ( sword::UnitAttributes::pret_au_tir, MsgsSimToClient::MsgUnitAttributes::pret_au_tir )
                                                    ( sword::UnitAttributes::indisponible, MsgsSimToClient::MsgUnitAttributes::indisponible ) );
    ConvertRulesOfEngagement( from, to );
    CONVERT_ENUM( roe_crowd, ( sword::UnitAttributes::none, MsgsSimToClient::MsgUnitAttributes::none )
                                ( sword::UnitAttributes::emploi_force_interdit, MsgsSimToClient::MsgUnitAttributes::emploi_force_interdit )
                                ( sword::UnitAttributes::maintien_a_distance_par_moyens_non_letaux, MsgsSimToClient::MsgUnitAttributes::maintien_a_distance_par_moyens_non_letaux )
                                ( sword::UnitAttributes::dispersion_par_moyens_de_defense_actifs, MsgsSimToClient::MsgUnitAttributes::dispersion_par_moyens_de_defense_actifs )
                                ( sword::UnitAttributes::armes_letales_autorisees, MsgsSimToClient::MsgUnitAttributes::armes_letales_autorisees ) );
    CONVERT_ENUM( fatigue, ( sword::normal, Common::normal )
                              ( sword::fatigue, Common::fatigue )
                              ( sword::epuise, Common::epuise ) );
    CONVERT_ENUM( moral, ( sword::fanatique, Common::fanatique )
                            ( sword::bon, Common::bon )
                            ( sword::moyen, Common::moyen )
                            ( sword::mauvais, Common::mauvais ) );
    CONVERT_ENUM( experience, ( sword::veteran, Common::veteran )
                                 ( sword::experimente, Common::experimente )
                                 ( sword::conscrit, Common::conscrit ) );
    CONVERT_ID( surrendered_unit );
    CONVERT( prisonnier );
    CONVERT( refugie_pris_en_compte );
    if( from.has_extension() )
        ConvertExtension( from.extension(), to->mutable_extension() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitPathFind& from, MsgsSimToClient::MsgUnitPathFind* to )
{
    CONVERT_ID( unit );
    ConvertLocation( from.itineraire().location(), to->mutable_itineraire()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitDestruction& from, MsgsSimToClient::MsgUnitDestruction* to )
{
    CONVERT_ID( unit );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitEnvironmentType& from, MsgsSimToClient::MsgUnitEnvironmentType* to )
{
    CONVERT_ID( unit );
    CONVERT( area );
    CONVERT( left );
    CONVERT( right );
    CONVERT( linear );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ChangeDiplomacy& from, Common::MsgChangeDiplomacy* to )
{
    CONVERT_ID( party1 );
    CONVERT_ID( party2 );
    CONVERT_DIPLOMACY( diplomatie );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitChangeSuperior& from, Common::MsgUnitChangeSuperior* to )
{
    CONVERT_ID( unit );
    CONVERT_ID( parent );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ChangeLogisticLinks& from, Common::MsgChangeLogisticLinks* to )
{
    ConvertParentEntity( from.requester(), to->mutable_requester() );
    CONVERT_ID( tc2 );
    if( from.has_logistic_base() )
        ConvertParentEntity( from.logistic_base(), to->mutable_logistic_base() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::AutomatChangeKnowledgeGroup& from, Common::MsgAutomatChangeKnowledgeGroup* to )
{
    CONVERT_ID( automat );
    CONVERT_ID( party );
    CONVERT_ID( knowledge_group );
}

namespace
{
    template< typename From, typename To >
    void ConvertSuperior( const From& from, To* to )
    {
        CONVERT_ID( formation );
        CONVERT_ID( automat );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::AutomatChangeSuperior& from, Common::MsgAutomatChangeSuperior* to )
{
    CONVERT_ID( automat );
    ConvertSuperior( from.superior(), to->mutable_superior() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitKnowledgeCreation& from, MsgsSimToClient::MsgUnitKnowledgeCreation* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( knowledge_group );
    CONVERT_ID( unit );
    CONVERT_ID( type );
}

namespace
{
    template< typename From, typename To >
    void ConvertAutomatPerception( const From& from, To* to )
    {
        CONVERT_ID( automat );
        CONVERT_IDENTIFICATION_LEVEL( identification_level );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitKnowledgeUpdate& from, MsgsSimToClient::MsgUnitKnowledgeUpdate* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( knowledge_group );
    CONVERT( pertinence );
    CONVERT_IDENTIFICATION_LEVEL( identification_level );
    CONVERT_IDENTIFICATION_LEVEL( max_identification_level );
    CONVERT( etat_op );
    CONVERT( mort );
    if( from.has_position() )
        ConvertCoordLatLong( from.position(), to->mutable_position() );
    if( from.has_direction() )
        ConvertHeading( from.direction(), to->mutable_direction() );
    CONVERT( speed );
    CONVERT_ID( party );
    CONVERT( nature_pc );
    CONVERT_LIST( perception_par_compagnie, elem, ConvertAutomatPerception );
    CONVERT_ID( surrendered_unit );
    CONVERT( prisonnier );
    CONVERT( refugie_pris_en_compte );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitKnowledgeDestruction& from, MsgsSimToClient::MsgUnitKnowledgeDestruction* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( knowledge_group );
}

namespace
{
    template< typename From, typename To >
    void ConvertUnitFireTarget( const From& from, To* to )
    {
        CONVERT_ID( unit );
        CONVERT_ID( crowd );
        if( from.has_position() )
            ConvertCoordLatLong( from.position(), to->mutable_position() );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::StartUnitFire& from, MsgsSimToClient::MsgStartUnitFire* to )
{
    CONVERT_ID( fire );
    CONVERT_ID( firing_unit );
    ConvertUnitFireTarget( from.target(), to->mutable_target() );
    CONVERT_ENUM( type, ( sword::direct, Common::direct )
                        ( sword::indirect, Common::indirect ) );
    CONVERT_ID( ammunition );
}

namespace
{
    template< typename From, typename To >
    void ConvertUnitHumanFireDamage( const From& from, To* to )
    {
        CONVERT_RANK( rank );
        CONVERT( alive_nbr );
        CONVERT( dead_nbr );
        CONVERT( wounded_u1_nbr );
        CONVERT( wounded_u2_nbr );
        CONVERT( wounded_u3_nbr );
        CONVERT( wounded_ue_nbr );
    }
    template< typename From, typename To >
    void ConvertUnitEquipmentFireDamage( const From& from, To* to )
    {
        CONVERT_ID( equipement_type );
        CONVERT( available_nbr );
        CONVERT( unavailable_nbr );
        CONVERT( repairable_nbr );
    }
    template< typename From, typename To >
    void ConvertUnitsFireDamages( const From& from, To* to )
    {
        CONVERT_ID( target );
        CONVERT_LIST( humans, elem, ConvertUnitHumanFireDamage );
        CONVERT_LIST( equipments, elem, ConvertUnitEquipmentFireDamage );
    }
    template< typename From, typename To >
    void ConvertCrowdsFireDamages( const From& from, To* to )
    {
        CONVERT_ID( target );
        CONVERT( dead_nbr );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::StopUnitFire& from, MsgsSimToClient::MsgStopUnitFire* to )
{
    CONVERT_ID( fire );
    CONVERT_LIST( units_damages, elem, ConvertUnitsFireDamages );
    CONVERT_LIST( crowds_damages, elem, ConvertCrowdsFireDamages );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::StartCrowdFire& from, MsgsSimToClient::MsgStartCrowdFire* to )
{
    CONVERT_ID( fire );
    CONVERT_ID( firing_crowd );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::StopCrowdFire& from, MsgsSimToClient::MsgStopCrowdFire* to )
{
    CONVERT_ID( fire );
    CONVERT_LIST( units_damages, elem, ConvertUnitsFireDamages );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::Explosion& from, MsgsSimToClient::MsgExplosion* to )
{
    CONVERT_ID( object );
    CONVERT_LIST( units_damages, elem, ConvertUnitsFireDamages );
    CONVERT_LIST( crowds_damages, elem, ConvertCrowdsFireDamages );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::StartFireEffect& from, MsgsSimToClient::MsgStartFireEffect* to )
{
    CONVERT_ID( fire_effect );
    ConvertLocation( from.location(), to->mutable_location() );
    CONVERT_ENUM( type, ( sword::smoke, Common::smoke )
                        ( sword::light, Common::light ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::StopFireEffect& from, MsgsSimToClient::MsgStopFireEffect* to )
{
    CONVERT_ID( fire_effect );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::Report& from, MsgsSimToClient::MsgReport* to )
{
    CONVERT_ID( report );
    ConvertTasker( from.source(), to->mutable_source() );
    CONVERT_ID( type );
    CONVERT_ID( category );
    to->mutable_time()->set_data( from.time().data() );
    CONVERT_LIST( parameters, elem, ConvertMissionParameter );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::InvalidateReport& from, MsgsSimToClient::MsgInvalidateReport* to )
{
    CONVERT_ID( report );
    ConvertTasker( from.source(), to->mutable_source() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::Trace& from, MsgsSimToClient::MsgTrace* to )
{
    ConvertTasker( from.source(), to->mutable_source() );
    CONVERT( message );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::DecisionalState& from, MsgsSimToClient::MsgDecisionalState* to )
{
    ConvertTasker( from.source(), to->mutable_source() );
    CONVERT( key );
    CONVERT( value );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::DebugPoints& from, MsgsSimToClient::MsgDebugPoints* to )
{
    ConvertTasker( from.source(), to->mutable_source() );
    CONVERT_LIST( coordinates, elem, ConvertCoordLatLong );
}

namespace
{
    template< typename From, typename To >
    void ConvertVisionCone( const From& from, To* to )
    {
        ConvertCoordLatLong( from.origin(), to->mutable_origin() );
        CONVERT( height );
        CONVERT( sensor );
        CONVERT_LIST( directions, elem, ConvertHeading );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitVisionCones& from, MsgsSimToClient::MsgUnitVisionCones* to )
{
    CONVERT_ID( unit );
    CONVERT_LIST( cones, elem, ConvertVisionCone );
    CONVERT( elongation );
}

#define CONVERT_UNIT_VISIBILITY( field ) \
    CONVERT_ENUM( field, ( sword::invisible, Common::invisible ) \
                         ( sword::detected, Common::detected ) \
                         ( sword::recognized, Common::recognized ) \
                         ( sword::identified, Common::identified ) \
                         ( sword::recorded, Common::recorded ) )

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitDetection& from, MsgsSimToClient::MsgUnitDetection* to )
{
    CONVERT_ID( observer );
    CONVERT_ID( detected_unit );
    CONVERT_UNIT_VISIBILITY( current_visibility );
    CONVERT_UNIT_VISIBILITY( max_visibility );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ObjectDetection& from, MsgsSimToClient::MsgObjectDetection* to )
{
    CONVERT_ID( observer );
    CONVERT_ID( detected_object );
    CONVERT_UNIT_VISIBILITY( visibility );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdConcentrationDetection& from, MsgsSimToClient::MsgCrowdConcentrationDetection* to )
{
    CONVERT_ID( observer );
    CONVERT_ID( detected_crowd );
    CONVERT_ID( detected_concentration );
    CONVERT_UNIT_VISIBILITY( visibility );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdFlowDetection& from, MsgsSimToClient::MsgCrowdFlowDetection* to )
{
    CONVERT_ID( observer );
    CONVERT_ID( detected_crowd );
    CONVERT_ID( detected_flow );
    ConvertLocation( from.visible_flow().location(), to->mutable_visible_flow()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitOrder& from, Common::MsgUnitOrder* to )
{
    ConvertOrder( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::AutomatOrder& from, Common::MsgAutomatOrder* to )
{
    ConvertOrder( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdOrder& from, Common::MsgCrowdOrder* to )
{
    ConvertOrder( from, to );
}

namespace
{
    template< typename From, typename To >
    void ConvertObjectAttributeConstruction( const From& from, To* to )
    {
        CONVERT_ID( resource );
        CONVERT( dotation_nbr );
        CONVERT( density );
        CONVERT( percentage );
    }
    template< typename From, typename To >
    void ConvertObjectAttributeObstacle( const From& from, To* to )
    {
        CONVERT_ENUM( type, ( sword::ObstacleType::preliminary, Common::ObstacleType::preliminary )
                            ( sword::ObstacleType::reserved, Common::ObstacleType::reserved ) );
        CONVERT( activated );
        CONVERT( activation_time );
    }
    template< typename From, typename To >
    void ConvertObjectAttributeMine( const From& from, To* to )
    {
        CONVERT_ID( resource );
        CONVERT( dotation_nbr );
        CONVERT( density );
        CONVERT( percentage );
    }
    template< typename From, typename To >
    void ConvertObjectAttributeCrossingSite( const From& from, To* to )
    {
        CONVERT( width );
        CONVERT( depth );
        CONVERT( flow_rate );
        CONVERT( banks_require_fitting );
    }
    template< typename From, typename To >
    void ConvertObjectAttributeSupplyRoute( const From& from, To* to )
    {
        CONVERT( equipped );
        CONVERT( max_weight );
        CONVERT( width );
        CONVERT( length );
        CONVERT( flow_rate );
    }
    template< typename From, typename To >
    void ConvertLocatedQuantity( const From& from, To* to )
    {
        ConvertCoordLatLong( from.coordinate(), to->mutable_coordinate() );
        CONVERT( quantity );
    }
    template< typename From, typename To >
    void ConvertObjectAttributeToxicCloud( const From& from, To* to )
    {
        CONVERT_LIST( quantities, elem, ConvertLocatedQuantity );
    }
    template< typename From, typename To >
    void ConvertObjectAttributeMedicalTreatment( const From& from, To* to )
    {
        CONVERT( external_reference_id );
        CONVERT_ENUM( facility_status, ( sword::ObjectAttributeMedicalTreatment::normal, Common::ObjectAttributeMedicalTreatment::normal )
                                          ( sword::ObjectAttributeMedicalTreatment::on_divert, Common::ObjectAttributeMedicalTreatment::on_divert )
                                          ( sword::ObjectAttributeMedicalTreatment::closed, Common::ObjectAttributeMedicalTreatment::closed ) );
        CONVERT( doctors );
        CONVERT( available_doctors );
    }
    template< typename From, typename To >
    void ConvertMedicalTreatmentBedCapacity( const From& from, To* to )
    {
        CONVERT( type_id );
        CONVERT( baseline_count );
        CONVERT( available_count );
        CONVERT( emergency_count );
    }
    template< typename From, typename To >
    void ConvertStockResource( const From& from, To* to )
    {
        CONVERT_ID( resource );
        CONVERT( current );
        CONVERT( maximum );
    }
    template< typename From, typename To >
    void ConvertObjectAttributeNBCType( const From& from, To* to )
    {
        CONVERT_ID( agent );
        CONVERT( concentration );
        CONVERT( source_life_duration );
    }
    template< typename From, typename To >
    void ConvertLink( const From& from, To* to )
    {
        CONVERT_ENUM( kind, ( sword::ResourceNetwork::Link::urban, Common::ResourceNetwork::Link::urban )
                            ( sword::ResourceNetwork::Link::object, Common::ResourceNetwork::Link::object ) );
        CONVERT( target_id );
        CONVERT( capacity );
        CONVERT( flow );
    }
    template< typename From, typename To >
    void ConvertResourceNetwork( const From& from, To* to )
    {
        to->mutable_resource()->set_name( from.resource().name() );
        for( int i = 0; i < from.link().size(); ++i )
            ConvertLink( from.link( i ), to->add_link() );
        CONVERT( enabled );
        CONVERT( max_stock );
        CONVERT( stock );
        CONVERT( production );
        CONVERT( consumption );
        CONVERT( critical );
        CONVERT( initial_stock );
        CONVERT( max_production );
        CONVERT( max_consumption );
    }
    template< typename From, typename To >
    void ConvertObjectAttributes( const From& from, To* to )
    {
        if( from.has_construction() )
            ConvertObjectAttributeConstruction( from.construction(), to->mutable_construction() );
        if( from.has_obstacle() )
            ConvertObjectAttributeObstacle( from.obstacle(), to->mutable_obstacle() );
        if( from.has_mine() )
            ConvertObjectAttributeMine( from.mine(), to->mutable_mine() );
        if( from.has_activity_time() )
            to->mutable_activity_time()->set_value( from.activity_time().value() );
        if( from.has_bypass() )
            to->mutable_bypass()->set_percentage( from.bypass().percentage() );
        if( from.has_logistic() )
            to->mutable_logistic()->mutable_tc2()->set_id( from.logistic().tc2().id() );
        if( from.has_nbc() )
        {
            to->mutable_nbc()->set_danger_level( from.nbc().danger_level() );
            CONVERT_LIST( nbc, nbc_agents, ConvertIdentifier );
        }
        if( from.has_crossing_site() )
            ConvertObjectAttributeCrossingSite( from.crossing_site(), to->mutable_crossing_site() );
        if( from.has_supply_route() )
            ConvertObjectAttributeSupplyRoute( from.supply_route(), to->mutable_supply_route() );
        if( from.has_toxic_cloud() )
            ConvertObjectAttributeToxicCloud( from.toxic_cloud(), to->mutable_toxic_cloud() );
        if( from.has_fire() )
        {
            to->mutable_fire()->set_class_name( from.fire().class_name() );
            to->mutable_fire()->set_max_combustion_energy( from.fire().max_combustion_energy() );
        }
        if( from.has_burn() )
        {
            to->mutable_burn()->set_current_heat( from.burn().current_heat() );
            to->mutable_burn()->set_combustion_energy_sum( from.burn().combustion_energy_sum() );
            to->mutable_burn()->set_combustion_energy_count( from.burn().combustion_energy_count() );
        }
        if( from.has_medical_treatment() )
        {
            ConvertObjectAttributeMedicalTreatment( from.medical_treatment(), to->mutable_medical_treatment() );
            CONVERT_LIST( medical_treatment, bed_capacities, ConvertMedicalTreatmentBedCapacity );
        }
        if( from.has_interaction_height() )
            to->mutable_interaction_height()->set_height( from.interaction_height().height() );
        if( from.has_stock() )
            CONVERT_LIST( stock, resources, ConvertStockResource );
        if( from.has_nbc_agent() )
            ConvertObjectAttributeNBCType( from.nbc_agent(), to->mutable_nbc_agent() );
        if( from.has_effect_delay() )
            to->mutable_effect_delay()->set_value( from.effect_delay().value() );
        if( from.has_seal_off() )
            to->mutable_seal_off()->set_level( from.seal_off().level() );
        if( from.has_resource_networks() )
            CONVERT_LIST( resource_networks, network, ConvertResourceNetwork );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ObjectCreation& from, MsgsSimToClient::MsgObjectCreation* to )
{
    CONVERT_ID( object );
    CONVERT_ID( type );
    CONVERT( name );
    CONVERT_ID( party );
    ConvertLocation( from.location(), to->mutable_location() );
    ConvertObjectAttributes( from.attributes(), to->mutable_attributes() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ObjectDestruction& from, MsgsSimToClient::MsgObjectDestruction* to )
{
    CONVERT_ID( object );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ObjectUpdate& from, MsgsSimToClient::MsgObjectUpdate* to )
{
    CONVERT_ID( object );
    ConvertLocation( from.location(), to->mutable_location() );
    ConvertObjectAttributes( from.attributes(), to->mutable_attributes() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ObjectKnowledgeCreation& from, MsgsSimToClient::MsgObjectKnowledgeCreation* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( party );
    CONVERT_ID( object );
    CONVERT_ID( type );
    ConvertObjectAttributes( from.attributes(), to->mutable_attributes() );
    CONVERT_ID( knowledge_group );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ObjectKnowledgeUpdate& from, MsgsSimToClient::MsgObjectKnowledgeUpdate* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( party );
    CONVERT_ID( object );
    CONVERT( relevance );
    if( from.has_location() )
        ConvertLocation( from.location(), to->mutable_location() );
    ConvertObjectAttributes( from.attributes(), to->mutable_attributes() );
    CONVERT( perceived );
    CONVERT_LIST( perceiving_automats, elem, ConvertIdentifier );
    CONVERT_ID( knowledge_group );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ObjectKnowledgeDestruction& from, MsgsSimToClient::MsgObjectKnowledgeDestruction* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( party );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogMedicalHandlingCreation& from, MsgsSimToClient::MsgLogMedicalHandlingCreation* to )
{
    CONVERT_ID( request );
    CONVERT_ID( unit );
    CONVERT( tick_creation );
    CONVERT_RANK( rang );
    CONVERT_ENUM( blessure, HUMAN_WOUND );
    CONVERT( blesse_mental );
    CONVERT( contamine_nbc );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogMedicalHandlingUpdate& from, MsgsSimToClient::MsgLogMedicalHandlingUpdate* to )
{
    CONVERT_ID( request );
    CONVERT_ID( unit );
    CONVERT_ID( provider );
    CONVERT_ENUM( blessure, HUMAN_WOUND );
    CONVERT( blesse_mental );
    CONVERT( contamine_nbc );
    CONVERT_ENUM( etat, ( sword::attente_disponibilite_ambulance_releve, Common::attente_disponibilite_ambulance_releve )
                        ( sword::ambulance_releve_deplacement_aller, Common::ambulance_releve_deplacement_aller )
                        ( sword::ambulance_releve_chargement, Common::ambulance_releve_chargement )
                        ( sword::attente_chargement_complet_ambulance_releve, Common::attente_chargement_complet_ambulance_releve )
                        ( sword::ambulance_releve_deplacement_retour, Common::ambulance_releve_deplacement_retour )
                        ( sword::ambulance_releve_dechargement, Common::ambulance_releve_dechargement )
                        ( sword::attente_disponibilite_medecin_pour_diagnostique, Common::attente_disponibilite_medecin_pour_diagnostique )
                        ( sword::diagnostique, Common::diagnostique )
                        ( sword::recherche_secteur_tri, Common::recherche_secteur_tri )
                        ( sword::attente_disponibilite_medecin_pour_tri, Common::attente_disponibilite_medecin_pour_tri )
                        ( sword::tri, Common::tri )
                        ( sword::recherche_secteur_soin, Common::recherche_secteur_soin )
                        ( sword::attente_disponibilite_medecin_pour_soin, Common::attente_disponibilite_medecin_pour_soin )
                        ( sword::soin, Common::soin )
                        ( sword::hospitalisation, Common::hospitalisation )
                        ( sword::attente_disponibilite_ambulance_ramassage, Common::attente_disponibilite_ambulance_ramassage )
                        ( sword::ambulance_ramassage_chargement, Common::ambulance_ramassage_chargement )
                        ( sword::attente_chargement_complet_ambulance_ramassage, Common::attente_chargement_complet_ambulance_ramassage )
                        ( sword::ambulance_ramassage_deplacement_aller, Common::ambulance_ramassage_deplacement_aller )
                        ( sword::ambulance_ramassage_dechargement, Common::ambulance_ramassage_dechargement )
                        ( sword::termine_medical, Common::termine ) );
    CONVERT( diagnostique_effectue );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogMedicalHandlingDestruction& from, MsgsSimToClient::MsgLogMedicalHandlingDestruction* to )
{
    CONVERT_ID( request );
    CONVERT_ID( unit );
}

namespace
{
    template< typename From, typename To >
    void ConvertLogMedicalEquipmentAvailability( const From& from, To* to )
    {
        CONVERT_ID( equipment_type );
        CONVERT( nbr_total );
        CONVERT( nbr_disponibles );
        CONVERT( nbr_au_travail );
        CONVERT( nbr_pretes );
        CONVERT( nbr_au_repos );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogMedicalState& from, MsgsSimToClient::MsgLogMedicalState* to )
{
    CONVERT_ID( unit );
    CONVERT( chaine_activee );
    if( from.has_priorites() )
        ConvertLogMedicalPriorities( from.priorites(), to->mutable_priorites() );
    CONVERT_LIST( tactical_priorities, elem, ConvertIdentifier );
    CONVERT_LIST( disponibilites_ambulances_releve, elem, ConvertLogMedicalEquipmentAvailability );
    CONVERT_LIST( disponibilites_ambulances_ramassage, elem, ConvertLogMedicalEquipmentAvailability );
    CONVERT_LIST( disponibilites_medecins, elem, ConvertLogMedicalEquipmentAvailability );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogMaintenanceHandlingCreation& from, MsgsSimToClient::MsgLogMaintenanceHandlingCreation* to )
{
    CONVERT_ID( request );
    CONVERT_ID( unit );
    CONVERT( tick_creation );
    CONVERT_ID( equipement );
    CONVERT_ID( breakdown );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogMaintenanceHandlingUpdate& from, MsgsSimToClient::MsgLogMaintenanceHandlingUpdate* to )
{
    CONVERT_ID( request );
    CONVERT_ID( unit );
    CONVERT_ID( provider );
    CONVERT_ENUM( etat, ( sword::deplacement_vers_chaine, Common::deplacement_vers_chaine )
                           ( sword::attente_disponibilite_remorqueur, Common::attente_disponibilite_remorqueur )
                           ( sword::remorqueur_deplacement_aller, Common::remorqueur_deplacement_aller )
                           ( sword::remorqueur_chargement, Common::remorqueur_chargement )
                           ( sword::remorqueur_deplacement_retour, Common::remorqueur_deplacement_retour )
                           ( sword::remorqueur_dechargement, Common::remorqueur_dechargement )
                           ( sword::diagnostique_maintenance, Common::diagnostique_maintenance )
                           ( sword::attente_prise_en_charge_par_niveau_superieur, Common::attente_prise_en_charge_par_niveau_superieur )
                           ( sword::attente_disponibilite_pieces, Common::attente_disponibilite_pieces )
                           ( sword::attente_disponibilite_reparateur, Common::attente_disponibilite_reparateur )
                           ( sword::reparation, Common::reparation )
                           ( sword::retour_pion, Common::retour_pion )
                           ( sword::termine_maintenance, Common::termine_maintenance ) );
    CONVERT( diagnostique_effectue );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogMaintenanceHandlingDestruction& from, MsgsSimToClient::MsgLogMaintenanceHandlingDestruction* to )
{
    CONVERT_ID( request );
    CONVERT_ID( unit );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogMaintenanceState& from, MsgsSimToClient::MsgLogMaintenanceState* to )
{
    CONVERT_ID( unit );
    CONVERT( chaine_activee );
    CONVERT_ENUM( regime_travail, ( sword::regime_1, Common::regime_1 )
                                     ( sword::regime_2, Common::regime_2 )
                                     ( sword::regime_3, Common::regime_3 )
                                     ( sword::regime_4, Common::regime_4 ) );
    CONVERT_LIST( priorites, elem, ConvertIdentifier );
    CONVERT_LIST( priorites_tactiques, elem, ConvertIdentifier );
    CONVERT_LIST( disponibilites_remorqueurs, elem, ConvertLogMedicalEquipmentAvailability );
    CONVERT_LIST( disponibilites_reparateurs, elem, ConvertLogMedicalEquipmentAvailability );
}

namespace
{
    template< typename From, typename To >
    void ConvertDotationQuery( const From& from, To* to )
    {
        CONVERT_ID( resource );
        CONVERT( quantite_demandee );
        CONVERT( quantite_accordee );
        CONVERT( quantite_en_transit );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyHandlingCreation& from, MsgsSimToClient::MsgLogSupplyHandlingCreation* to )
{
    CONVERT_ID( request );
    CONVERT_ID( consumer );
    CONVERT( tick_creation );
    CONVERT_LIST( dotations, elem, ConvertDotationQuery );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyHandlingUpdate& from, MsgsSimToClient::MsgLogSupplyHandlingUpdate* to )
{
    CONVERT_ID( request );
    CONVERT_ID( consumer );
    if( from.has_supplier() )
        ConvertParentEntity( from.supplier(), to->mutable_supplier() );
    if( from.has_convoy_provider() )
        ConvertParentEntity( from.convoy_provider(), to->mutable_convoy_provider() );
    CONVERT_ID( convoying_unit );
    CONVERT_ENUM( etat, ( sword::LogSupplyHandlingUpdate::convoy_waiting_for_transporters, MsgsSimToClient::convoi_en_attente_camions )
                        ( sword::LogSupplyHandlingUpdate::convoy_forming, MsgsSimToClient::convoi_constitution )
                        ( sword::LogSupplyHandlingUpdate::convoy_moving_to_loading_point, MsgsSimToClient::convoi_deplacement_vers_point_chargement )
                        ( sword::LogSupplyHandlingUpdate::convoy_loading, MsgsSimToClient::convoi_chargement )
                        ( sword::LogSupplyHandlingUpdate::convoy_moving_to_unloading_point, MsgsSimToClient::convoi_deplacement_vers_point_dechargement )
                        ( sword::LogSupplyHandlingUpdate::convoy_unloading, MsgsSimToClient::convoi_dechargement )
                        ( sword::LogSupplyHandlingUpdate::convoy_moving_back_to_loading_point, MsgsSimToClient::convoi_deplacement_retour )
                        ( sword::LogSupplyHandlingUpdate::convoy_finished, MsgsSimToClient::termine ) );
    CONVERT_LIST( dotations, elem, ConvertDotationQuery );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyHandlingDestruction& from, MsgsSimToClient::MsgLogSupplyHandlingDestruction* to )
{
    CONVERT_ID( request );
    CONVERT_ID( consumer );
}

namespace
{
    template< typename From, typename To >
    void ConvertDotationStock( const From& from, To* to )
    {
        CONVERT_ID( ressource_id );
        CONVERT( quantite_disponible );
    }
    template< typename From, typename To >
    void ConvertLogSupplyEquimentAvailability( const From& from, To* to )
    {
        CONVERT_ID( equipment );
        CONVERT( nbr_total );
        CONVERT( nbr_disponibles );
        CONVERT( nbr_au_travail );
        CONVERT( nbr_pretes );
        CONVERT( nbr_au_repos );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-23
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyState& from, MsgsSimToClient::MsgLogSupplyState* to )
{
    CONVERT_ID( unit );
    CONVERT( chaine_activee );
    CONVERT_LIST( stocks, elem, ConvertDotationStock );
    CONVERT_LIST( disponibilites_transporteurs_convois, elem, ConvertLogSupplyEquimentAvailability );
}

namespace
{
    template< typename From, typename To >
    void ConvertDotationQuota( const From& from, To* to )
    {
        CONVERT_ID( ressource_id );
        CONVERT( quota_disponible );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-23
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyQuotas& from, MsgsSimToClient::MsgLogSupplyQuotas* to )
{
    ConvertParentEntity( from.supplied(), to->mutable_supplied() );
    CONVERT_LIST( quotas, elem, ConvertDotationQuota );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdCreation& from, MsgsSimToClient::MsgCrowdCreation* to )
{
    CONVERT_ID( crowd );
    CONVERT_ID( type );
    CONVERT( nom );
    CONVERT_ID( party );
    if( from.has_extension() )
        ConvertExtension( from.extension(), to->mutable_extension() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdUpdate& from, MsgsSimToClient::MsgCrowdUpdate* to )
{
    CONVERT_ID( crowd );
    CONVERT( etat_domination );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdConcentrationCreation& from, MsgsSimToClient::MsgCrowdConcentrationCreation* to )
{
    CONVERT_ID( concentration );
    CONVERT_ID( crowd );
    ConvertCoordLatLong( from.position(), to->mutable_position() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdConcentrationDestruction& from, MsgsSimToClient::MsgCrowdConcentrationDestruction* to )
{
    CONVERT_ID( concentration );
    CONVERT_ID( crowd );
}

namespace
{
    template< typename From, typename To >
    void ConvertCrowdAttitude( const From& from, To* to )
    {
        CONVERT_ENUM( attitude, ( sword::calme, Common::calme )
                                ( sword::agitee, Common::agitee )
                                ( sword::excitee, Common::excitee )
                                ( sword::agressive, Common::agressive ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdConcentrationUpdate& from, MsgsSimToClient::MsgCrowdConcentrationUpdate* to )
{
    CONVERT_ID( concentration );
    CONVERT_ID( crowd );
    CONVERT( nb_humains_vivants );
    CONVERT( nb_humains_morts );
    ConvertCrowdAttitude( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdFlowCreation& from, MsgsSimToClient::MsgCrowdFlowCreation* to )
{
    CONVERT_ID( flow );
    CONVERT_ID( crowd );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdFlowDestruction& from, MsgsSimToClient::MsgCrowdFlowDestruction* to )
{
    CONVERT_ID( flow );
    CONVERT_ID( crowd );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdFlowUpdate& from, MsgsSimToClient::MsgCrowdFlowUpdate* to )
{
    CONVERT_ID( flow );
    CONVERT_ID( crowd );
    if( from.has_itineraire() )
        ConvertLocation( from.itineraire().location(), to->mutable_itineraire()->mutable_location() );
    if( from.has_parts() )
        ConvertLocation( from.parts().location(), to->mutable_parts()->mutable_location() );
    if( from.has_direction() )
        ConvertHeading( from.direction(), to->mutable_direction() );
    CONVERT( vitesse );
    CONVERT( nb_humains_vivants );
    CONVERT( nb_humains_morts );
    ConvertCrowdAttitude( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdKnowledgeCreation& from, MsgsSimToClient::MsgCrowdKnowledgeCreation* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( knowledge_group );
    CONVERT_ID( crowd );
    CONVERT_ID( party );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdKnowledgeUpdate& from, MsgsSimToClient::MsgCrowdKnowledgeUpdate* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( knowledge_group );
    CONVERT( etat_domination );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdKnowledgeDestruction& from, MsgsSimToClient::MsgCrowdKnowledgeDestruction* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( knowledge_group );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdConcentrationKnowledgeCreation& from, MsgsSimToClient::MsgCrowdConcentrationKnowledgeCreation* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( crowd );
    CONVERT_ID( knowledge_group );
    CONVERT_ID( concentration );
    if( from.has_position() )
        ConvertCoordLatLong( from.position(), to->mutable_position() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdConcentrationKnowledgeDestruction& from, MsgsSimToClient::MsgCrowdConcentrationKnowledgeDestruction* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( crowd );
    CONVERT_ID( knowledge_group );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdConcentrationKnowledgeUpdate& from, MsgsSimToClient::MsgCrowdConcentrationKnowledgeUpdate* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( crowd );
    CONVERT_ID( knowledge_group );
    CONVERT_ID( concentration );
    CONVERT( nb_humains_vivants );
    CONVERT( nb_humains_morts );
    ConvertCrowdAttitude( from, to );
    CONVERT( pertinence );
    CONVERT( est_percu );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdFlowKnowledgeCreation& from, MsgsSimToClient::MsgCrowdFlowKnowledgeCreation* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( crowd );
    CONVERT_ID( knowledge_group );
    CONVERT_ID( flow );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdFlowKnowledgeDestruction& from, MsgsSimToClient::MsgCrowdFlowKnowledgeDestruction* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( crowd );
    CONVERT_ID( knowledge_group );
}

namespace
{
    template< typename From, typename To >
    void ConvertFlowPart( const From& from, To* to )
    {
        ConvertLocation( from.forme().location(), to->mutable_forme()->mutable_location() );
        CONVERT( pertinence );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdFlowKnowledgeUpdate& from, MsgsSimToClient::MsgCrowdFlowKnowledgeUpdate* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( crowd );
    CONVERT_ID( knowledge_group );
    CONVERT_ID( flow );
    CONVERT_LIST( portions_flux, elem, ConvertFlowPart );
    if( from.has_direction() )
        ConvertHeading( from.direction(), to->mutable_direction() );
    CONVERT( vitesse );
    CONVERT( nb_humains_vivants );
    CONVERT( nb_humains_morts );
    ConvertCrowdAttitude( from, to );
    CONVERT( est_percu );
}

namespace
{
    template< typename From, typename To >
    void Assign( const From& from, To* to )
    {
        *to = from;
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::FolkCreation& from, MsgsSimToClient::MsgFolkCreation* to )
{
    CONVERT_LIST( profiles, elem, Assign );
    CONVERT_LIST( activities, elem, Assign );
    CONVERT( container_size );
    CONVERT( edge_number );
}

namespace
{
    template< typename From, typename To >
    void ConvertFolkGraphEdgeUpdate( const From& from, To* to )
    {
        CONVERT( folk );
        CONVERT( shape_id );
        for( int i = 0; i < from.crowd_occupation().size(); ++i )
            to->add_crowd_occupation( from.crowd_occupation( i ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::FolkGraphUpdate& from, MsgsSimToClient::MsgFolkGraphUpdate* to )
{
    for( int i = 0; i < from.elem().size(); ++i )
        ConvertFolkGraphEdgeUpdate( from.elem( i ), to->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlGlobalWeatherAck& /*from*/, MsgsSimToClient::MsgControlGlobalWeatherAck* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlLocalWeatherAck& /*from*/, MsgsSimToClient::MsgControlLocalWeatherAck* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlCheckPointSaveBegin& /*from*/, MsgsSimToClient::MsgControlCheckPointSaveBegin* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlCheckPointSetFrequencyAck& /*from*/, MsgsSimToClient::MsgControlCheckPointSetFrequencyAck* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlCheckPointSaveNowAck& /*from*/, MsgsSimToClient::MsgControlCheckPointSaveNowAck* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlSendCurrentStateBegin& /*from*/, MsgsSimToClient::MsgControlSendCurrentStateBegin* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlSendCurrentStateEnd& /*from*/, MsgsSimToClient::MsgControlSendCurrentStateEnd* /*to*/ )
{
    // NOTHING
}

namespace
{
    template< typename From, typename To >
    void ConvertUrbanArchitecture( const From& from, To* to )
    {
        CONVERT( height );
        CONVERT( floor_number );
        CONVERT( roof_shape );
        CONVERT( material );
        CONVERT( occupation );
        CONVERT( trafficability );
        CONVERT( parking_available );
    }
    template< typename From, typename To >
    void ConvertRgbaColor( const From& from, To* to )
    {
        ConvertRgbColor( from, to );
        CONVERT( alpha );
    }
    template< typename From, typename To >
    void ConvertInfrastructure( const From& from, To* to )
    {
        CONVERT( type );
        CONVERT( active );
        CONVERT( threshold );
    }
    template< typename From, typename To >
    void ConvertUrbanAttributes( const From& from, To* to )
    {
        if( from.has_architecture() )
            ConvertUrbanArchitecture( from.architecture(), to->mutable_architecture() );
        if( from.has_structure() )
            to->mutable_structure()->set_state( from.structure().state() );
        if( from.has_color() )
            ConvertRgbaColor( from.color(), to->mutable_color() );
        CONVERT_LIST( infrastructures, resource_network, ConvertResourceNetwork );
        if( from.infrastructures().has_infrastructure() )
            ConvertInfrastructure( from.infrastructures().infrastructure(), to->mutable_infrastructures()->mutable_infrastructure() );
        for( int i = 0; i < from.usages().size(); ++i )
            ConvertUrbanUsage( from.usages( i ), to->add_usages() );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UrbanCreation& from, MsgsSimToClient::MsgUrbanCreation* to )
{
    CONVERT_ID( urban_object );
    CONVERT( name );
    ConvertLocation( from.location(), to->mutable_location() );
    if( from.has_attributes() )
        ConvertUrbanAttributes( from.attributes(), to->mutable_attributes() );
    CONVERT_ID( parent );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UrbanUpdate& from, MsgsSimToClient::MsgUrbanUpdate* to )
{
    CONVERT_ID( urban_object );
    ConvertLocation( from.location(), to->mutable_location() );
    if( from.has_attributes() )
        ConvertUrbanAttributes( from.attributes(), to->mutable_attributes() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UrbanKnowledgeCreation& from, MsgsSimToClient::MsgUrbanKnowledgeCreation* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( party );
    CONVERT_ID( urban_block );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UrbanKnowledgeUpdate& from, MsgsSimToClient::MsgUrbanKnowledgeUpdate* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( party );
    CONVERT_ID( urban_block );
    CONVERT( progress );
    CONVERT( maxprogress );
    CONVERT( perceived );
    CONVERT_LIST( automat_perceptions, elem, ConvertIdentifier );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UrbanKnowledgeDestruction& from, MsgsSimToClient::MsgUrbanKnowledgeDestruction* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( party );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::StockResource& from, Common::StockResource* to )
{
    ConvertStockResource( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UrbanDetection& from, MsgsSimToClient::MsgUrbanDetection* to )
{
    CONVERT_ID( observer );
    CONVERT_ID( urban_object );
    CONVERT_UNIT_VISIBILITY( visibility );
}

namespace
{
    template< typename From, typename To >
    void ConvertKnowledgeGroupAck( const From& from, To* to )
    {
        CONVERT_ID( knowledge_group );
        CONVERT_ENUM( error_code, ( sword::KnowledgeGroupAck::no_error, MsgsSimToClient::KnowledgeGroupAck::no_error )
                                  ( sword::KnowledgeGroupAck::error_invalid_unit, MsgsSimToClient::KnowledgeGroupAck::error_invalid_unit )
                                  ( sword::KnowledgeGroupAck::error_invalid_superior, MsgsSimToClient::KnowledgeGroupAck::error_invalid_superior )
                                  ( sword::KnowledgeGroupAck::error_invalid_camp, MsgsSimToClient::KnowledgeGroupAck::error_invalid_camp )
                                  ( sword::KnowledgeGroupAck::error_invalid_knowledgegroup, MsgsSimToClient::KnowledgeGroupAck::error_invalid_knowledgegroup )
                                  ( sword::KnowledgeGroupAck::error_invalid_type, MsgsSimToClient::KnowledgeGroupAck::error_invalid_type ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::KnowledgeGroupMagicActionAck& from, MsgsSimToClient::MsgKnowledgeGroupMagicActionAck* to )
{
    ConvertKnowledgeGroupAck( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::KnowledgeGroupCreation& from, MsgsSimToClient::MsgKnowledgeGroupCreation* to )
{
    CONVERT_ID( knowledge_group );
    CONVERT_ID( party );
    CONVERT_ID( parent );
    CONVERT( type );
    CONVERT( jam );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::KnowledgeGroupUpdate& from, MsgsSimToClient::MsgKnowledgeGroupUpdate* to )
{
    CONVERT_ID( knowledge_group );
    CONVERT_ID( party );
    CONVERT_ID( parent );
    CONVERT( enabled );
    CONVERT( type );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::KnowledgeGroupCreationAck& from, MsgsSimToClient::MsgKnowledgeGroupCreationAck* to )
{
    ConvertKnowledgeGroupAck( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::KnowledgeGroupUpdateAck& from, MsgsSimToClient::MsgKnowledgeGroupUpdateAck* to )
{
    ConvertKnowledgeGroupAck( from, to );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::KnowledgeGroupDestruction& from, MsgsSimToClient::MsgKnowledgeGroupDestruction* to )
{
    CONVERT_ID( knowledge_group );
    CONVERT_ID( party );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ActionCreateFireOrderAck& from, MsgsSimToClient::MsgActionCreateFireOrderAck* to )
{
    CONVERT_ENUM( error_code, ( sword::ActionCreateFireOrderAck::no_error, MsgsSimToClient::MsgActionCreateFireOrderAck::no_error )
                              ( sword::ActionCreateFireOrderAck::error_invalid_reporter, MsgsSimToClient::MsgActionCreateFireOrderAck::error_invalid_reporter )
                              ( sword::ActionCreateFireOrderAck::error_invalid_target, MsgsSimToClient::MsgActionCreateFireOrderAck::error_invalid_target )
                              ( sword::ActionCreateFireOrderAck::error_invalid_munition, MsgsSimToClient::MsgActionCreateFireOrderAck::error_invalid_munition )
                              ( sword::ActionCreateFireOrderAck::error_target_no_illuminated, MsgsSimToClient::MsgActionCreateFireOrderAck::error_target_no_illuminated ) );
}

namespace
{
    template< typename From, typename To >
    void ConvertWeatherAttributes( const From& from, To* to )
    {
        CONVERT( temperature );
        CONVERT( wind_speed );
        ConvertHeading( from.wind_direction(), to->mutable_wind_direction() );
        CONVERT( cloud_floor );
        CONVERT( cloud_ceiling );
        CONVERT( cloud_density );
        CONVERT_ENUM( precipitation, ( sword::pas_de_precipitation, Common::pas_de_precipitation )
                                     ( sword::tempete_sable, Common::tempete_sable )
                                     ( sword::brouillard, Common::brouillard )
                                     ( sword::crachin, Common::crachin )
                                     ( sword::pluie, Common::pluie )
                                     ( sword::neige, Common::neige ) );
        CONVERT_ENUM( lighting, ( sword::jour_sans_nuage, Common::jour_sans_nuage )
                                ( sword::jour_peu_nuageux, Common::jour_peu_nuageux )
                                ( sword::jour_moyennement_nuageux, Common::jour_moyennement_nuageux )
                                ( sword::jour_assez_nuageux, Common::jour_assez_nuageux )
                                ( sword::jour_tres_nuageux, Common::jour_tres_nuageux )
                                ( sword::nuit_pleine_lune, Common::nuit_pleine_lune )
                                ( sword::nuit_trois_quart_de_lune, Common::nuit_trois_quart_de_lune )
                                ( sword::nuit_demi_lune, Common::nuit_demi_lune )
                                ( sword::nuit_quart_de_lune, Common::nuit_quart_de_lune )
                                ( sword::nuit_nouvelle_lune, Common::nuit_nouvelle_lune )
                                ( sword::eclairant, Common::eclairant )
                                ( sword::global_weather, Common::global_weather ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlGlobalWeather& from, MsgsSimToClient::MsgControlGlobalWeather* to )
{
    CONVERT_ID( weather );
    ConvertWeatherAttributes( from.attributes(), to->mutable_attributes() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlLocalWeatherCreation& from, MsgsSimToClient::MsgControlLocalWeatherCreation* to )
{
    CONVERT_ID( weather );
    if( from.has_top_left_coordinate() )
        ConvertCoordLatLong( from.top_left_coordinate(), to->mutable_top_left_coordinate() );
    if( from.has_bottom_right_coordinate() )
        ConvertCoordLatLong( from.bottom_right_coordinate(), to->mutable_bottom_right_coordinate() );
    if( from.has_attributes() )
        ConvertWeatherAttributes( from.attributes(), to->mutable_attributes() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlLocalWeatherDestruction& from, MsgsSimToClient::MsgControlLocalWeatherDestruction* to )
{
    CONVERT_ID( weather );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlCheckPointListAck& /*from*/, MsgsSimToClient::MsgControlCheckPointListAck* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlCheckPointList& from, MsgsSimToClient::MsgControlCheckPointList* to )
{
    for( int i = 0; i < from.checkpoint().size(); ++i )
        to->add_checkpoint( from.checkpoint( i ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlCheckPointDeleteAck& /*from*/, MsgsSimToClient::MsgControlCheckPointDeleteAck* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::FormationDestruction& from, MsgsSimToClient::MsgFormationDestruction* to )
{
    CONVERT_ID( formation );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::AutomatDestruction& from, MsgsSimToClient::MsgAutomatDestruction* to )
{
    CONVERT_ID( automat );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdDestruction& from, MsgsSimToClient::MsgCrowdDestruction* to )
{
    CONVERT_ID( crowd );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::PopulationCreation& from, MsgsSimToClient::MsgPopulationCreation* to )
{
    CONVERT_ID( id );
    CONVERT_ID( party );
    CONVERT_ID( type );
    CONVERT( name );
    CONVERT( text );
    if( from.has_extension() )
        ConvertExtension( from.extension(), to->mutable_extension() );
    for( int i = 0; i < from.blocks().size(); ++i )
        to->add_blocks()->set_id( from.blocks( i ).id() );
}

namespace
{
    template< typename From, typename To >
    void ConvertSatisfaction( const From& from, To* to )
    {
        CONVERT( lodging );
        CONVERT( health );
        CONVERT( safety );
    }
    template< typename From, typename To >
    void ConvertResourceSatisfaction( const From& from, To* to )
    {
        CONVERT_ID( resource );
        CONVERT( value );
    }
    template< typename From, typename To >
    void ConvertMotivationSatisfaction( const From& from, To* to )
    {
        CONVERT_ID( motivation );
        CONVERT( value );
    }
    template< typename From, typename To >
    void ConvertBlockOccupation( const From& from, To* to )
    {
        CONVERT_ID( block );
        CONVERT( number );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::PopulationUpdate& from, MsgsSimToClient::MsgPopulationUpdate* to )
{
    CONVERT_ID( id );
    CONVERT( healthy );
    CONVERT( wounded );
    CONVERT( dead );
    for( int i = 0; i < from.adhesions().size(); ++i )
        ConvertPartyAdhesion( from.adhesions( i ), to->add_adhesions() );
    if( from.has_satisfaction() )
        ConvertSatisfaction( from.satisfaction(), to->mutable_satisfaction() );
    CONVERT_LIST( satisfaction, resources, ConvertResourceSatisfaction );
    CONVERT_LIST( satisfaction, motivations, ConvertMotivationSatisfaction );
    CONVERT_ID( motivation );
    for( int i = 0; i < from.occupations().size(); ++i )
        ConvertBlockOccupation( from.occupations( i ), to->add_occupations() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ChangePopulationMagicActionAck& from, MsgsSimToClient::MsgChangePopulationMagicActionAck* to )
{
    CONVERT_ENUM( error_code, ( sword::ChangePopulationMagicActionAck::no_error, MsgsSimToClient::MsgChangePopulationMagicActionAck::no_error )
                              ( sword::ChangePopulationMagicActionAck::error_invalid_population, MsgsSimToClient::MsgChangePopulationMagicActionAck::error_invalid_population )
                              ( sword::ChangePopulationMagicActionAck::error_invalid_adhesion, MsgsSimToClient::MsgChangePopulationMagicActionAck::error_invalid_adhesion )
                              ( sword::ChangePopulationMagicActionAck::error_invalid_number, MsgsSimToClient::MsgChangePopulationMagicActionAck::error_invalid_number ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UrbanMagicActionAck& from, MsgsSimToClient::MsgUrbanMagicActionAck* to )
{
    CONVERT_ENUM( error_code, ( sword::UrbanMagicActionAck::no_error, MsgsSimToClient::MsgUrbanMagicActionAck::no_error )
                              ( sword::UrbanMagicActionAck::error_invalid_urbanblock, MsgsSimToClient::MsgUrbanMagicActionAck::error_invalid_urbanblock )
                              ( sword::UrbanMagicActionAck::error_invalid_role, MsgsSimToClient::MsgUrbanMagicActionAck::error_invalid_role )
                              ( sword::UrbanMagicActionAck::error_invalid_threshold, MsgsSimToClient::MsgUrbanMagicActionAck::error_invalid_threshold ) );
}

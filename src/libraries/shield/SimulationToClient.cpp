// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "SimulationToClient.h"
#include "SimulationTools.h"

using namespace shield;

#define CONVERT_POSTURE( from_field, to_field ) \
    CONVERT_ENUM_TO( from_field, to_field, ( sword::UnitAttributes::moving, MsgsSimToClient::MsgUnitAttributes::mouvement ) \
                                           ( sword::UnitAttributes::infiltrating, MsgsSimToClient::MsgUnitAttributes::mouvement_discret ) \
                                           ( sword::UnitAttributes::stopping, MsgsSimToClient::MsgUnitAttributes::arret ) \
                                           ( sword::UnitAttributes::short_stopping, MsgsSimToClient::MsgUnitAttributes::poste_reflexe ) \
                                           ( sword::UnitAttributes::parked, MsgsSimToClient::MsgUnitAttributes::poste ) \
                                           ( sword::UnitAttributes::parked_on_self_prepared_area, MsgsSimToClient::MsgUnitAttributes::poste_amenage ) \
                                           ( sword::UnitAttributes::parked_on_engineer_prepared_area, MsgsSimToClient::MsgUnitAttributes::poste_prepare_genie ) )

#define CONVERT_IDENTIFICATION_LEVEL( field ) \
    CONVERT_ENUM( field, ( sword::UnitIdentification::identified, MsgsSimToClient::identifiee ) \
                         ( sword::UnitIdentification::recognized, MsgsSimToClient::reconnue ) \
                         ( sword::UnitIdentification::detected, MsgsSimToClient::detectee ) \
                         ( sword::UnitIdentification::unseen, MsgsSimToClient::signale ) )

#define CONVERT_RANK( from_field, to_field ) \
        CONVERT_ENUM_TO( from_field, to_field, ( sword::officer, Common::officier ) \
                                     ( sword::sub_officer, Common::sous_officier ) \
                                     ( sword::trooper, Common::mdr ) )

#define CONVERT_UNIT_ACTION_ACK( from_field, to_field ) \
    CONVERT_ENUM_TO( from_field, to_field, ( sword::UnitActionAck::no_error, MsgsSimToClient::UnitActionAck::no_error ) \
                                        ( sword::UnitActionAck::error_invalid_unit, MsgsSimToClient::UnitActionAck::error_invalid_unit ) \
                                        ( sword::UnitActionAck::error_automat_engaged, MsgsSimToClient::UnitActionAck::error_automate_embraye ) \
                                    ( sword::UnitActionAck::error_invalid_parameter, MsgsSimToClient::UnitActionAck::error_invalid_attribute ) \
                                        ( sword::UnitActionAck::error_unit_surrendered, MsgsSimToClient::UnitActionAck::error_unit_surrendered ) )

#define CONVERT_UNIT_VISIBILITY( field ) \
    CONVERT_ENUM( field, ( sword::UnitVisibility::invisible, Common::invisible ) \
                         ( sword::UnitVisibility::detected, Common::detected ) \
                         ( sword::UnitVisibility::recognized, Common::recognized ) \
                         ( sword::UnitVisibility::identified, Common::identified ) \
                         ( sword::UnitVisibility::recorded, Common::recorded ) )

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
                                  ( sword::OrderAck::error_invalid_parameter, MsgsSimToClient::OrderAck::error_invalid_mission_parameters )
                                  ( sword::OrderAck::error_unit_cannot_receive_order, MsgsSimToClient::OrderAck::error_unit_cannot_receive_order )
                                  ( sword::OrderAck::error_invalid_frag_order, MsgsSimToClient::OrderAck::error_invalid_order_conduite )
                                  ( sword::OrderAck::error_invalid_order_mission, MsgsSimToClient::OrderAck::error_invalid_order_mission )
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
    CONVERT_CB( tasker, ConvertTasker );
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
    CONVERT_UNIT_ACTION_ACK( error_code, error );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::MagicActionAck& from, MsgsSimToClient::MsgMagicActionAck* to )
{
    CONVERT_ENUM( error_code, ( sword::MagicActionAck::no_error, MsgsSimToClient::MsgMagicActionAck::no_error )
                              ( sword::MagicActionAck::error_invalid_parameter, MsgsSimToClient::MsgMagicActionAck::error_invalid_attribute ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitMagicActionAck& from, MsgsSimToClient::MsgUnitMagicActionAck* to )
{
    CONVERT_ID( unit );
    CONVERT_UNIT_ACTION_ACK( error_code, error_code );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ObjectMagicActionAck& from, MsgsSimToClient::MsgObjectMagicActionAck* to )
{
    CONVERT_ENUM( error_code, ( sword::ObjectMagicActionAck::no_error, MsgsSimToClient::MsgObjectMagicActionAck::no_error )
                              ( sword::ObjectMagicActionAck::error_invalid_object, MsgsSimToClient::MsgObjectMagicActionAck::error_invalid_object )
                              ( sword::ObjectMagicActionAck::error_invalid_party, MsgsSimToClient::MsgObjectMagicActionAck::error_invalid_camp )
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
                              ( sword::CrowdMagicActionAck::error_invalid_parameter, MsgsSimToClient::MsgCrowdMagicActionAck::error_invalid_attribute ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ChangeDiplomacyAck& from, MsgsSimToClient::MsgChangeDiplomacyAck* to )
{
    CONVERT_ID( party1 );
    CONVERT_ID( party2 );
    CONVERT_DIPLOMACY( diplomacy, diplomatie );
    CONVERT_ENUM( error_code, ( sword::ChangeDiplomacyAck::no_error_diplomacy, MsgsSimToClient::MsgChangeDiplomacyAck::no_error_diplomacy )
                              ( sword::ChangeDiplomacyAck::error_invalid_party_diplomacy, MsgsSimToClient::MsgChangeDiplomacyAck::error_invalid_camp_diplomacy ) );
}

namespace
{
    template< typename From, typename To >
    void ConvertHierarchyModificationAckErrorCode( const From& from, To* to )
    {
        CONVERT_ENUM( error_code, ( sword::HierarchyModificationAck::no_error_hierarchy, MsgsSimToClient::HierarchyModificationAck::no_error_hierarchy )
                                  ( sword::HierarchyModificationAck::error_invalid_agent, MsgsSimToClient::HierarchyModificationAck::error_invalid_pion )
                                  ( sword::HierarchyModificationAck::error_invalid_automate, MsgsSimToClient::HierarchyModificationAck::error_invalid_automate )
                                  ( sword::HierarchyModificationAck::error_invalid_formation, MsgsSimToClient::HierarchyModificationAck::error_invalid_formation )
                                  ( sword::HierarchyModificationAck::error_invalid_supply_automat, MsgsSimToClient::HierarchyModificationAck::error_invalid_automate_tc2 )
                                  ( sword::HierarchyModificationAck::error_agent_surrendered, MsgsSimToClient::HierarchyModificationAck::error_unit_surrendered_hierarchy )
                                  ( sword::HierarchyModificationAck::error_invalid_party, MsgsSimToClient::HierarchyModificationAck::error_invalid_party_hierarchy )
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
    CONVERT_ENUM_TO( error_code, ack, ( sword::LogSupplyPushFlowAck::no_error_pushflow, MsgsSimToClient::MsgLogSupplyPushFlowAck::no_error_pushflow )
                                      ( sword::LogSupplyPushFlowAck::error_invalid_supplier, MsgsSimToClient::MsgLogSupplyPushFlowAck::error_invalid_donneur_pushflow )
                                      ( sword::LogSupplyPushFlowAck::error_invalid_receiver, MsgsSimToClient::MsgLogSupplyPushFlowAck::error_invalid_receveur_pushflow ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyPullFlowAck& from, MsgsSimToClient::MsgLogSupplyPullFlowAck* to )
{
    CONVERT_ENUM_TO( error_code, ack, ( sword::LogSupplyPullFlowAck::no_error_pullflow, MsgsSimToClient::MsgLogSupplyPullFlowAck::no_error_pullflow )
                                      ( sword::LogSupplyPullFlowAck::error_invalid_supplier, MsgsSimToClient::MsgLogSupplyPullFlowAck::error_invalid_provider_pullflow )
                                      ( sword::LogSupplyPullFlowAck::error_invalid_receiver, MsgsSimToClient::MsgLogSupplyPullFlowAck::error_invalid_receiver_pullflow ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyChangeQuotasAck& from, MsgsSimToClient::MsgLogSupplyChangeQuotasAck* to )
{
    CONVERT_ENUM_TO( error_code , ack, ( sword::LogSupplyChangeQuotasAck::no_error_quotas, MsgsSimToClient::MsgLogSupplyChangeQuotasAck::no_error_quotas )
                                                  ( sword::LogSupplyChangeQuotasAck::error_invalid_supplier, MsgsSimToClient::MsgLogSupplyChangeQuotasAck::error_invalid_supplier )
                                                  ( sword::LogSupplyChangeQuotasAck::error_invalid_receiver, MsgsSimToClient::MsgLogSupplyChangeQuotasAck::error_invalid_receiver )
                                                  ( sword::LogSupplyChangeQuotasAck::error_invalid_dotation, MsgsSimToClient::MsgLogSupplyChangeQuotasAck::error_invalid_dotation ));
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-09
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlInformation& from, MsgsSimToClient::MsgControlInformation* to )
{
    CONVERT( current_tick );
    CONVERT_DATE( initial_date_time );
    CONVERT_DATE( date_time );
    CONVERT( tick_duration );
    CONVERT( time_factor );
    CONVERT( checkpoint_frequency );
    CONVERT_SIMULATION_STATE( status );
    CONVERT( send_vision_cones );
    CONVERT( profiling_enabled );
    CONVERT_DATE( checkpoint_real_time );
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
    CONVERT_DATE( date_time );
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
void SimulationToClient::Convert( const sword::ControlDateTimeChangeAck& from, MsgsSimToClient::MsgControlDatetimeChangeAck* to )
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
    void ConvertRgbColor( const From& from, To* to )
    {
        CONVERT( red );
        CONVERT( green );
        CONVERT( blue );
    }
    template< typename From, typename To >
    void ConvertRepartition( const From& from, To* to )
    {
        CONVERT( male );
        CONVERT( female );
        CONVERT( children );
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
    CONVERT( app6symbol );

    CONVERT_ENUM( logistic_level, ( sword::none, Common::none )
                                  ( sword::logistic_base, Common::logistic_base ) );
    CONVERT_CB( color, ConvertRgbColor );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::FormationUpdate& from, MsgsSimToClient::MsgFormationUpdate* to )
{
    CONVERT_ID( formation );
    CONVERT_CB( extension, ConvertExtension );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::PartyCreation& from, MsgsSimToClient::MsgPartyCreation* to )
{
    CONVERT_ID( party );
    CONVERT( name );
    CONVERT_DIPLOMACY( type, type );
    CONVERT_CB( extension, ConvertExtension );
    CONVERT_CB( color, ConvertRgbColor );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::AutomatCreation& from, MsgsSimToClient::MsgAutomatCreation* to )
{
    CONVERT_ID( automat );
    CONVERT_ID( type );
    CONVERT_TO( name, nom );
    CONVERT_CB( parent, ConvertParentEntity );
    CONVERT_ID( party );
    CONVERT_ID( knowledge_group );
    CONVERT( app6symbol );
    CONVERT_ENUM( logistic_level, ( sword::none, Common::none )
                                  ( sword::logistic_base, Common::logistic_base ) );
    CONVERT_CB( color, ConvertRgbColor );
}

namespace
{
    template< typename From, typename To >
    void ConvertForceRatio( const From& from, To* to )
    {
        CONVERT_ENUM_TO( force_ratio, rapport_de_force, ( sword::ForceRatio::none, MsgsSimToClient::ForceRatio::none_force_ratio )
                                                        ( sword::ForceRatio::neutral, MsgsSimToClient::ForceRatio::neutre )
                                                        ( sword::ForceRatio::favorable, MsgsSimToClient::ForceRatio::favorable )
                                                        ( sword::ForceRatio::unfavorable, MsgsSimToClient::ForceRatio::defavorable ) );
    }
    template< typename From, typename To >
    void ConvertMeetingEngagementStatus( const From& from, To* to )
    {
        CONVERT_ENUM_TO( meeting_engagement, combat_de_rencontre, ( sword::no_contact, Common::none_meeting )
                                                                  ( sword::avoiding, Common::etat_esquive )
                                                                  ( sword::pinned_down, Common::etat_fixe )
                                                                  ( sword::proceeding_with_mission, Common::etat_poursuite_mission ) );
    }
    template< typename From, typename To >
    void ConvertOperationalStatus( const From& from, To* to )
    {
        CONVERT_ENUM_TO( operational_state, etat_operationnel, ( sword::totally_destroyed, Common::detruit_totalement )
                                                               ( sword::tactically_destroyed, Common::detruit_tactiquement )
                                                               ( sword::operational, Common::operationnel ) );
    }
    template< typename From, typename To >
    void ConvertRulesOfEngagement( const From& from, To* to )
    {
        CONVERT_ENUM( roe, ( sword::RulesOfEngagement::none, MsgsSimToClient::RulesOfEngagement::none_roe )
                           ( sword::RulesOfEngagement::free_fire, MsgsSimToClient::RulesOfEngagement::tir_libre )
                           ( sword::RulesOfEngagement::retaliation_only, MsgsSimToClient::RulesOfEngagement::tir_sur_riposte )
                           ( sword::RulesOfEngagement::fire_upon_order, MsgsSimToClient::RulesOfEngagement::tir_interdit ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::AutomatAttributes& from, MsgsSimToClient::MsgAutomatAttributes* to )
{
    CONVERT_ID( automat );
    CONVERT_AUTOMAT_MODE( mode, etat_automate );
    ConvertMeetingEngagementStatus( from, to );
    ConvertOperationalStatus( from, to );
    ConvertRulesOfEngagement( from, to );
    CONVERT_CB( extension, ConvertExtension );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitCreation& from, MsgsSimToClient::MsgUnitCreation* to )
{
    CONVERT_ID( unit );
    CONVERT_ID( type );
    CONVERT_TO( name, nom );
    CONVERT_ID( automat );
    CONVERT( pc );
    CONVERT_CB( color, ConvertRgbColor );
    CONVERT_CB( repartition, ConvertRepartition );
}

namespace
{
    void ConvertHumanDotations( const sword::UnitAttributes& from, MsgsSimToClient::MsgUnitAttributes* to )
    {
        //NB: Different message prototype for HumanDotations
        if( !from.has_human_dotations() )
            return;
        to->mutable_dotation_eff_personnel();
        for( int i = 0; i < 3; ++i )
        {
            MsgsSimToClient::HumanDotations_HumanDotation* dotation = to->mutable_dotation_eff_personnel()->add_elem();
            dotation->set_rang                          ( static_cast< Common::EnumHumanRank >( i ) );
            dotation->set_nb_total                      ( 0 );
            dotation->set_nb_operationnels              ( 0 );
            dotation->set_nb_morts                      ( 0 );
            dotation->set_nb_blesses                    ( 0 );
            dotation->set_nb_blesses_mentaux            ( 0 );
            dotation->set_nb_contamines_nbc             ( 0 );
            dotation->set_nb_dans_chaine_sante          ( 0 );
            dotation->set_nb_utilises_pour_maintenance  ( 0 );
            dotation->set_nb_blesses_non_evacues        ( 0 );
            dotation->set_nb_morts_dans_chaine_mortuaire( 0 );
        }
        for( int i = 0; i < from.human_dotations().elem().size(); ++i )
        {
            const sword::HumanDotations_HumanDotation& dotationFrom = from.human_dotations().elem( i );
            MsgsSimToClient::HumanDotations_HumanDotation* dotationTo = to->mutable_dotation_eff_personnel()->mutable_elem( static_cast< int >( dotationFrom.rank() ) );
            sword::EnumHumanState state = dotationFrom.state();
            sword::EnumHumanLocation location = dotationFrom.location();
            int quantity = dotationFrom.quantity();

            dotationTo->set_nb_total( dotationTo->nb_total() + quantity );
            if( state == sword::healthy && location != sword::medical && !dotationFrom.mentally_wounded() && !dotationFrom.contaminated() )
                dotationTo->set_nb_operationnels( dotationTo->nb_operationnels() + quantity );
            if( state == sword::deadly )
                dotationTo->set_nb_morts( dotationTo->nb_morts() + quantity );
            if( state != sword::healthy && state != sword::deadly )
                dotationTo->set_nb_blesses( dotationTo->nb_blesses() + quantity );
            if( dotationFrom.mentally_wounded() )
                dotationTo->set_nb_blesses_mentaux( dotationTo->nb_blesses_mentaux() + quantity );
            if( dotationFrom.contaminated() )
                dotationTo->set_nb_contamines_nbc( dotationTo->nb_contamines_nbc() + quantity );
            if( state != sword::deadly && location == sword::medical )
                dotationTo->set_nb_dans_chaine_sante( dotationTo->nb_dans_chaine_sante() + quantity );
            if( location == sword::maintenance )
                dotationTo->set_nb_utilises_pour_maintenance( dotationTo->nb_utilises_pour_maintenance() + quantity );
            if( state == sword::injured && location == sword::battlefield )
                dotationTo->set_nb_blesses_non_evacues( dotationTo->nb_blesses_non_evacues() + quantity );
            dotationTo->set_nb_morts_dans_chaine_mortuaire( 0 );
        }
    }
    template< typename From, typename To >
    void ConvertEquipmentDotation( const From& from, To* to )
    {
        CONVERT_ID( type );
        CONVERT_TO( available, nb_disponibles );
        CONVERT_TO( unavailable, nb_indisponibles );
        //NB: OnSiteFixable doesn't exist in "to"
        to->set_nb_reparables( from.repairable() + from.on_site_fixable() );
        CONVERT_TO( repairing, nb_dans_chaine_maintenance );
        CONVERT_TO( captured, nb_prisonniers );
        //NB: Breakdowns doesn't exist in "to"
    }
    template< typename From, typename To >
    void ConvertResourceDotation( const From& from, To* to )
    {
        CONVERT_ID( type );
        CONVERT_TO( quantity, quantite_disponible );
        //NB: seuil_logistique / threshold doesn't exist in "to"
    }
    template< typename From, typename To >
    void ConvertLentEquipment( const From& from, To* to )
    {
        CONVERT_ID( borrower );
        CONVERT_ID( type );
        CONVERT_TO( quantity, nombre );
    }
    template< typename From, typename To >
    void ConvertBorrowedEquipment( const From& from, To* to )
    {
        CONVERT_ID( owner );
        CONVERT_ID( type );
        CONVERT_TO( quantity, nombre );
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
    template< typename From, typename To >
    void ConvertUnitSatisfaction( const From& from, To* to )
    {
        CONVERT( safety );
        CONVERT( lodging );
        CONVERT( access_to_health_care );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitAttributes& from, MsgsSimToClient::MsgUnitAttributes* to )
{
    CONVERT_ID( unit );
    ConvertHumanDotations( from, to );
    CONVERT_LIST_TO( equipment_dotations, dotation_eff_materiel, elem, ConvertEquipmentDotation );
    CONVERT_LIST_TO( resource_dotations, dotation_eff_ressource, elem, ConvertResourceDotation );
    CONVERT_LIST_TO( lent_equipments, equipements_pretes, elem, ConvertLentEquipment );
    CONVERT_LIST_TO( borrowed_equipments, equipements_empruntes, elem, ConvertBorrowedEquipment );
    CONVERT_CB( position, ConvertCoordLatLong );
    CONVERT_CB( direction, ConvertHeading );
    CONVERT_TO( height, hauteur );
    CONVERT( altitude );
    CONVERT_TO( speed, vitesse );
    CONVERT_TO( raw_operational_state, etat_operationnel_brut );
    CONVERT_LIST( reinforcements, elem, ConvertIdentifier );
    CONVERT_ID( reinforced_unit );
    CONVERT_TO( dead, mort );
    CONVERT_TO( neutralized, neutralise );
    CONVERT_TO( stealth, mode_furtif_actif );
    CONVERT( underground );
    CONVERT_TO( embarked, embarque );
    CONVERT_TO( transporters_available, transporteurs_disponibles );
    CONVERT_POSTURE( old_posture, posture_old );
    CONVERT_POSTURE( new_posture, posture_new );
    CONVERT_TO( posture_transition, posture_pourcentage );
    CONVERT_TO( installation, etat_installation );
    CONVERT_TO( protective_suits, en_tenue_de_protection_nbc );
    CONVERT_LIST_TO( contamination_agents, contamine_par_agents_nbc, elem, ConvertIdentifier );
    CONVERT_CB_TO( contamination_state, etat_contamination, ConvertContaminationState );
    CONVERT_CB( communications, ConvertContaminations );
    CONVERT( radio_emitter_disabled );
    CONVERT( radio_receiver_disabled );
    CONVERT_TO( radar_active, radar_actif );
    CONVERT_LIST( transported_units, elem, ConvertIdentifier );
    CONVERT_ID( transporting_unit );
    ConvertForceRatio( from, to );
    ConvertMeetingEngagementStatus( from, to );
    ConvertOperationalStatus( from, to );
    CONVERT_ENUM_TO( indirect_fire_availability, disponibilite_au_tir_indirect, ( sword::UnitAttributes::no_fire, MsgsSimToClient::MsgUnitAttributes::none_fire_available )
                                                                                ( sword::UnitAttributes::fire_ready, MsgsSimToClient::MsgUnitAttributes::pret_au_tir )
                                                                                ( sword::UnitAttributes::fire_unavailable, MsgsSimToClient::MsgUnitAttributes::indisponible )
                                                                                ( sword::UnitAttributes::engaged, MsgsSimToClient::MsgUnitAttributes::engaged ) );
    ConvertRulesOfEngagement( from, to );
    CONVERT_ENUM( roe_crowd, ( sword::UnitAttributes::no_rule, MsgsSimToClient::MsgUnitAttributes::none )
                             ( sword::UnitAttributes::no_force, MsgsSimToClient::MsgUnitAttributes::emploi_force_interdit )
                             ( sword::UnitAttributes::non_lethal_force, MsgsSimToClient::MsgUnitAttributes::maintien_a_distance_par_moyens_non_letaux )
                             ( sword::UnitAttributes::lethal_force, MsgsSimToClient::MsgUnitAttributes::armes_letales_autorisees ) );
    CONVERT_ENUM_TO( tiredness, fatigue, ( sword::UnitAttributes::rested, Common::normal )
                                         ( sword::UnitAttributes::tired, Common::fatigue )
                                         ( sword::UnitAttributes::exhausted, Common::epuise ) );
    CONVERT_ENUM_TO( morale, moral, ( sword::UnitAttributes::fanatical, Common::fanatique )
                                    ( sword::UnitAttributes::high, Common::bon )
                                    ( sword::UnitAttributes::standard, Common::moyen )
                                    ( sword::UnitAttributes::low, Common::mauvais ) );
    CONVERT_ENUM( experience, ( sword::UnitAttributes::veteran, Common::veteran )
                              ( sword::UnitAttributes::expert, Common::experimente )
                              ( sword::UnitAttributes::novice, Common::conscrit ) );
    CONVERT_ENUM( stress, ( sword::UnitAttributes::calm, Common::calm )
                          ( sword::UnitAttributes::worried, Common::worried )
                          ( sword::UnitAttributes::stressed, Common::stressed ) );
    if ( from.has_surrendered_unit() )
        to->mutable_surrendered_to_party()->set_id( from.surrendered_unit().id() );
    CONVERT_TO( prisoner, prisonnier );
    CONVERT_TO( refugees_managed, refugie_pris_en_compte );
    CONVERT_CB( extension, ConvertExtension );
    CONVERT( critical_intelligence );
    CONVERT_CB( satisfaction, ConvertUnitSatisfaction );
    if( from.has_adhesions() )
        for( int i = 0; i < from.adhesions().adhesion_size(); ++i )
            ConvertPartyAdhesion( from.adhesions().adhesion( i ), to->mutable_adhesions()->add_adhesion() );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UnitPathFind& from, MsgsSimToClient::MsgUnitPathFind* to )
{
    CONVERT_ID( unit );
    if( from.has_path() && from.path().has_location() )
        ConvertLocation( from.path().location(), to->mutable_itineraire()->mutable_location() );
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
    CONVERT_DIPLOMACY( diplomacy, diplomatie );
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
    CONVERT_CB( requester, ConvertParentEntity );
    for( int i = 0; i < from.superior().size(); ++i )
            ConvertParentEntity( from.superior( i ), to->add_superior() );
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
    CONVERT_CB( superior, ConvertSuperior );
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
    CONVERT_TO( operational_state, etat_op );
    CONVERT_TO( dead, mort );
    CONVERT_CB( position, ConvertCoordLatLong );
    CONVERT_CB( direction, ConvertHeading );
    CONVERT( speed );
    CONVERT_ID( party );
    CONVERT_TO( command_post, nature_pc );
    CONVERT_LIST_TO( perceptions, perception_par_compagnie, elem, ConvertAutomatPerception );
    CONVERT_ID( surrendered_unit );
    CONVERT_TO( prisoner, prisonnier );
    CONVERT_TO( refugees_managed, refugie_pris_en_compte );
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
        CONVERT_CB( position, ConvertCoordLatLong );
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
    CONVERT_CB( target, ConvertUnitFireTarget );
    CONVERT_ENUM( type, ( sword::StartUnitFire::direct, Common::direct )
                        ( sword::StartUnitFire::indirect, Common::indirect ) );
    CONVERT_ID( ammunition );
}

namespace
{
    template< typename From, typename To >
    void ConvertUnitHumanFireDamage( const From& from, To* to )
    {
        CONVERT_RANK( rank, rank );
        CONVERT_TO( alive, alive_nbr );
        CONVERT_TO( dead, dead_nbr );
        CONVERT_TO( wounded_u1, wounded_u1_nbr );
        CONVERT_TO( wounded_u2, wounded_u2_nbr );
        CONVERT_TO( wounded_u3, wounded_u3_nbr );
        CONVERT_TO( wounded_ue, wounded_ue_nbr );
    }
    template< typename From, typename To >
    void ConvertUnitEquipmentFireDamage( const From& from, To* to )
    {
        CONVERT_ID_TO( equipement, equipement_type );
        CONVERT_TO( available, available_nbr );
        CONVERT_TO( unavailable, unavailable_nbr );
        CONVERT_TO( repairable, repairable_nbr );
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
        CONVERT_TO( dead, dead_nbr );
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
    CONVERT_CB( location, ConvertLocation );
    CONVERT_ENUM( type, ( sword::StartFireEffect::smoke, Common::smoke )
                        ( sword::StartFireEffect::light, Common::light )
                        ( sword::StartFireEffect::explosion, Common::explosion )
                        ( sword::StartFireEffect::neutralization, Common::neutralization ) );
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
    CONVERT_CB( source, ConvertTasker );
    CONVERT_ID( type );
    if( from.has_category() )
        to->mutable_category()->set_id( from.category() );
    CONVERT_DATE( time );
    CONVERT_LIST( parameters, elem, ConvertMissionParameter );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::InvalidateReport& from, MsgsSimToClient::MsgInvalidateReport* to )
{
    CONVERT_ID( report );
    CONVERT_CB( source, ConvertTasker );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::DecisionalState& from, MsgsSimToClient::MsgDecisionalState* to )
{
    CONVERT_CB( source, ConvertTasker );
    CONVERT( key );
    CONVERT( value );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-10
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::DebugPoints& from, MsgsSimToClient::MsgDebugPoints* to )
{
    CONVERT_CB( source, ConvertTasker );
    CONVERT_LIST( coordinates, elem, ConvertCoordLatLong );
}

namespace
{
    template< typename From, typename To >
    void ConvertVisionCone( const From& from, To* to )
    {
        CONVERT_CB( origin, ConvertCoordLatLong );
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
        CONVERT_TO( dotation, dotation_nbr );
        CONVERT( density );
        CONVERT( percentage );
    }
    template< typename From, typename To >
    void ConvertObjectAttributeObstacle( const From& from, To* to )
    {
        to->mutable_obstacle()->set_type( from.obstacle().type() == sword::ObstacleType::preliminary ?
                                          Common::ObstacleType::preliminary : Common::ObstacleType::reserved );
        if( from.obstacle().has_activated() )
            to->mutable_obstacle()->set_activated( from.obstacle().activated() );
        if( from.obstacle().has_activation_time() )
            to->mutable_activity_time()->set_activation_time( from.obstacle().activation_time() );
        if( from.obstacle().has_activity_time() )
            to->mutable_activity_time()->set_activity_time( from.obstacle().activity_time() );
    }
    template< typename From, typename To >
    void ConvertObjectAttributeMine( const From& from, To* to )
    {
        CONVERT_ID( resource );
        CONVERT_TO( dotation, dotation_nbr );
        CONVERT( density );
        CONVERT( percentage );
    }
    template< typename From, typename To >
    void ConvertObjectAttributeLogistic( const From& from, To* to )
    {
        CONVERT_CB( logistic_superior, ConvertParentEntity );
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
        CONVERT_CB( coordinate, ConvertCoordLatLong );
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
    void ConvertPreIgnition( const From& from, To* to )
    {
        CONVERT( ignition_energy );
        CONVERT( ignition_threshold );
    }
    template< typename From, typename To >
    void ConvertCombustion( const From& from, To* to )
    {
        CONVERT( current_heat );
        CONVERT( combustion_energy );
        CONVERT( max_combustion_energy );
    }
    template< typename From, typename To >
    void ConvertDecline( const From& from, To* to )
    {
        CONVERT( current_heat );
    }
    template< typename From, typename To >
    void ConvertBurnSurfaceBurningCell( const From& from, To* to )
    {
        CONVERT( origin_x );
        CONVERT( origin_y );
        CONVERT_ENUM( phase, ( sword::pre_ignition, Common::pre_ignition )
                             ( sword::combustion, Common::combustion )
                             ( sword::decline, Common::decline )
                             ( sword::extinguished, Common::extinguished ) );
        CONVERT_CB( pre_ignition, ConvertPreIgnition );
        CONVERT_CB( combustion, ConvertCombustion );
        CONVERT_CB( decline, ConvertDecline );
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
        CONVERT_ID( object );
        CONVERT( capacity );
        CONVERT( flow );
    }
    template< typename From, typename To >
    void ConvertResourceNetwork( const From& from, To* to )
    {
        if( from.has_resource() && from.resource().has_name() )
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
        CONVERT( functional_state );
    }
    template< typename From, typename To >
    void ConvertObjectAttributes( const From& from, To* to )
    {
        CONVERT_CB( construction, ConvertObjectAttributeConstruction );
        ConvertObjectAttributeObstacle( from, to );
        CONVERT_CB_TO( mine, valorisation, ConvertObjectAttributeMine );
        if( from.has_bypass() )
            to->mutable_bypass()->set_percentage( from.bypass().percentage() );
        CONVERT_CB( logistic, ConvertObjectAttributeLogistic );
        if( from.has_nbc() && from.nbc().has_danger_level() )
            to->mutable_nbc()->set_danger_level( from.nbc().danger_level() );
        CONVERT_LIST( nbc, nbc_agents, ConvertIdentifier );
        CONVERT_CB( crossing_site, ConvertObjectAttributeCrossingSite );
        CONVERT_CB( supply_route, ConvertObjectAttributeSupplyRoute );
        CONVERT_CB( toxic_cloud, ConvertObjectAttributeToxicCloud );
        if( from.has_fire() )
        {
            if( from.fire().has_class_name() )
                to->mutable_fire()->set_class_name( from.fire().class_name() );
            if( from.fire().has_max_combustion_energy() )
                to->mutable_fire()->set_max_combustion_energy( from.fire().max_combustion_energy() );
        }
        if( from.has_burn() )
        {
            to->mutable_burn()->set_current_heat( from.burn().current_heat() );
            to->mutable_burn()->set_combustion_energy( from.burn().combustion_energy() );
        }
        if( from.has_burn_surface() )
        {
            to->mutable_burn_surface()->set_cell_size( from.burn_surface().cell_size() );
            CONVERT_LIST( burn_surface, burning_cells, ConvertBurnSurfaceBurningCell );
        }
        if( from.has_medical_treatment() )
        {
            ConvertObjectAttributeMedicalTreatment( from.medical_treatment(), to->mutable_medical_treatment() );
            CONVERT_LIST( medical_treatment, bed_capacities, ConvertMedicalTreatmentBedCapacity );
        }
        if( from.has_interaction_height() )
            to->mutable_interaction_height()->set_height( from.interaction_height().height() );
        CONVERT_LIST( stock, resources, ConvertStockResource );
        CONVERT_CB( nbc_agent, ConvertObjectAttributeNBCType );
        if( from.has_effect_delay() )
            to->mutable_effect_delay()->set_value( from.effect_delay().value() );
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
    CONVERT_CB( location, ConvertLocation );
    CONVERT_CB( attributes, ConvertObjectAttributes );
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
    CONVERT_CB( location, ConvertLocation );
    CONVERT_CB( attributes, ConvertObjectAttributes );
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
    CONVERT_CB( attributes, ConvertObjectAttributes );
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
    CONVERT_CB( location, ConvertLocation );
    CONVERT_CB( attributes, ConvertObjectAttributes );
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
    CONVERT_TO( tick, tick_creation );
    CONVERT_RANK( rank, rang );
    CONVERT_ENUM_TO( wound, blessure, HUMAN_WOUND );
    CONVERT_TO( mental_wound, blesse_mental );
    CONVERT_TO( nbc_contaminated, contamine_nbc );
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
    CONVERT_ENUM_TO( wound, blessure, HUMAN_WOUND );
    CONVERT_TO( mental_wound, blesse_mental );
    CONVERT_TO( nbc_contaminated, contamine_nbc );
    CONVERT_ENUM_TO( state, etat, ( sword::LogMedicalHandlingUpdate::waiting_for_evacuation, Common::attente_disponibilite_ambulance_releve )
                                  ( sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_in, Common::ambulance_releve_deplacement_aller )
                                  ( sword::LogMedicalHandlingUpdate::evacuation_ambulance_loading, Common::ambulance_releve_chargement )
                                  ( sword::LogMedicalHandlingUpdate::waiting_for_evacuation_loading_completion, Common::attente_chargement_complet_ambulance_releve )
                                  ( sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_out, Common::ambulance_releve_deplacement_retour )
                                  ( sword::LogMedicalHandlingUpdate::evacuation_ambulance_unloading, Common::ambulance_releve_dechargement )
                                  ( sword::LogMedicalHandlingUpdate::waiting_for_diagnostic, Common::attente_disponibilite_medecin_pour_diagnostique )
                                  ( sword::LogMedicalHandlingUpdate::diagnosing, Common::diagnostique )
                                  ( sword::LogMedicalHandlingUpdate::looking_for_triage, Common::recherche_secteur_tri )
                                  ( sword::LogMedicalHandlingUpdate::waiting_for_triage, Common::attente_disponibilite_medecin_pour_tri )
                                  ( sword::LogMedicalHandlingUpdate::triaging, Common::tri )
                                  ( sword::LogMedicalHandlingUpdate::looking_for_medical_attention, Common::recherche_secteur_soin )
                                  ( sword::LogMedicalHandlingUpdate::waiting_for_medical_attention, Common::attente_disponibilite_medecin_pour_soin )
                                  ( sword::LogMedicalHandlingUpdate::receiving_medical_attention, Common::soin )
                                  ( sword::LogMedicalHandlingUpdate::resting, Common::hospitalisation )
                                  ( sword::LogMedicalHandlingUpdate::waiting_for_collection, Common::attente_disponibilite_ambulance_ramassage )
                                  ( sword::LogMedicalHandlingUpdate::collection_ambulance_loading, Common::ambulance_ramassage_chargement )
                                  ( sword::LogMedicalHandlingUpdate::waiting_for_collection_loading_completion, Common::attente_chargement_complet_ambulance_ramassage )
                                  ( sword::LogMedicalHandlingUpdate::collection_ambulance_moving_in, Common::ambulance_ramassage_deplacement_aller )
                                  ( sword::LogMedicalHandlingUpdate::collection_ambulance_unloading, Common::ambulance_ramassage_dechargement )
                                  ( sword::LogMedicalHandlingUpdate::finished, Common::termine ) );
    CONVERT_TO( diagnosed, current_state_end_tick );
    CONVERT_TO( diagnosed, diagnostique_effectue );
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
        CONVERT_ID_TO( equipment, equipment_type );
        CONVERT_TO( total, nbr_total );
        CONVERT_TO( available, nbr_disponibles );
        CONVERT_TO( working, nbr_au_travail );
        CONVERT_TO( lent, nbr_pretes );
        CONVERT_TO( resting, nbr_au_repos );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogMedicalState& from, MsgsSimToClient::MsgLogMedicalState* to )
{
    CONVERT_ID( unit );
    CONVERT_TO( chain, chaine_activee );
    CONVERT_CB_TO( priorities, priorites, ConvertLogMedicalPriorities );
    CONVERT_LIST_TO( tactical_priorities, tactical_priorities, elem, ConvertIdentifier );
    CONVERT_LIST_TO( evacuation_ambulances, disponibilites_ambulances_releve, elem, ConvertLogMedicalEquipmentAvailability );
    CONVERT_LIST_TO( collection_ambulances, disponibilites_ambulances_ramassage, elem, ConvertLogMedicalEquipmentAvailability );
    CONVERT_LIST_TO( doctors, disponibilites_medecins, elem, ConvertLogMedicalEquipmentAvailability );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogMaintenanceHandlingCreation& from, MsgsSimToClient::MsgLogMaintenanceHandlingCreation* to )
{
    CONVERT_ID( request );
    CONVERT_ID( unit );
    CONVERT_TO( tick, tick_creation );
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
    CONVERT_ENUM_TO( state, etat, ( sword::LogMaintenanceHandlingUpdate::moving_to_supply, Common::deplacement_vers_chaine )
                                  ( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter, Common::attente_disponibilite_remorqueur )
                                  ( sword::LogMaintenanceHandlingUpdate::transporter_moving_to_supply, Common::remorqueur_deplacement_aller )
                                  ( sword::LogMaintenanceHandlingUpdate::transporter_loading, Common::remorqueur_chargement )
                                  ( sword::LogMaintenanceHandlingUpdate::transporter_moving_back, Common::remorqueur_deplacement_retour )
                                  ( sword::LogMaintenanceHandlingUpdate::transporter_unloading, Common::remorqueur_dechargement )
                                  ( sword::LogMaintenanceHandlingUpdate::diagnosing, Common::diagnostique_maintenance )
                                  ( sword::LogMaintenanceHandlingUpdate::searching_upper_levels, Common::attente_prise_en_charge_par_niveau_superieur )
                                  ( sword::LogMaintenanceHandlingUpdate::waiting_for_parts, Common::attente_disponibilite_pieces )
                                  ( sword::LogMaintenanceHandlingUpdate::waiting_for_repairer, Common::attente_disponibilite_reparateur )
                                  ( sword::LogMaintenanceHandlingUpdate::repairing, Common::reparation )
                                  ( sword::LogMaintenanceHandlingUpdate::moving_back, Common::retour_pion )
                                  ( sword::LogMaintenanceHandlingUpdate::finished, Common::termine_maintenance ) );
    CONVERT_TO( diagnosed, current_state_end_tick );
    CONVERT_TO( diagnosed, diagnostique_effectue );
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
    CONVERT_TO( chain, chaine_activee );
    CONVERT_ENUM_TO( work_rate, regime_travail, ( sword::rate_1, Common::regime_1 )
                                                ( sword::rate_2, Common::regime_2 )
                                                ( sword::rate_3, Common::regime_3 )
                                                ( sword::rate_4, Common::regime_4 ) );
    CONVERT_LIST_TO( priorities, priorites, elem, ConvertIdentifier );
    CONVERT_LIST_TO( tactical_priorities, priorites_tactiques, elem, ConvertIdentifier );
    CONVERT_LIST_TO( haulers, disponibilites_remorqueurs, elem, ConvertLogMedicalEquipmentAvailability );
    CONVERT_LIST_TO( repairers, disponibilites_reparateurs, elem, ConvertLogMedicalEquipmentAvailability );
}

namespace
{
    template< typename From, typename To >
    void ConvertSupplyResourceRequest( const From& from, To* to )
    {
        CONVERT_ID( resource );
        CONVERT( requested );
        CONVERT( granted );
        CONVERT( convoyed );
    }

    template< typename From, typename To >
    void ConvertSupplyRecipientResourceRequest( const From& from, To* to )
    {
        CONVERT_ID( recipient );
        CONVERT_SIMPLE_LIST( resources, ConvertSupplyResourceRequest );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyHandlingCreation& from, MsgsSimToClient::MsgLogSupplyHandlingCreation* to )
{
    CONVERT_ID( request );
    CONVERT_TO( tick, tick_creation );
    CONVERT_CB( supplier, ConvertParentEntity );
    CONVERT_CB( transporters_provider, ConvertParentEntity );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyHandlingUpdate& from, MsgsSimToClient::MsgLogSupplyHandlingUpdate* to )
{
    CONVERT_ID( request );
    CONVERT_ID_TO( convoyer, convoying_unit );
    CONVERT_ENUM_TO( state, etat, ( sword::LogSupplyHandlingUpdate::convoy_waiting_for_transporters, MsgsSimToClient::convoi_en_attente_camions )
                         ( sword::LogSupplyHandlingUpdate::convoy_setup, MsgsSimToClient::convoi_constitution )
                         ( sword::LogSupplyHandlingUpdate::convoy_moving_to_loading_point, MsgsSimToClient::convoi_deplacement_vers_point_chargement )
                         ( sword::LogSupplyHandlingUpdate::convoy_loading, MsgsSimToClient::convoi_chargement )
                         ( sword::LogSupplyHandlingUpdate::convoy_moving_to_unloading_point, MsgsSimToClient::convoi_deplacement_vers_point_dechargement )
                         ( sword::LogSupplyHandlingUpdate::convoy_unloading, MsgsSimToClient::convoi_dechargement )
                         ( sword::LogSupplyHandlingUpdate::convoy_moving_back_to_loading_point, MsgsSimToClient::convoi_deplacement_retour )
                         ( sword::LogSupplyHandlingUpdate::convoy_finished, MsgsSimToClient::termine ) );
    CONVERT( current_state_end_tick );
    CONVERT_LIST( requests, requests, ConvertSupplyRecipientResourceRequest );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-22
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyHandlingDestruction& from, MsgsSimToClient::MsgLogSupplyHandlingDestruction* to )
{
    CONVERT_ID( request );
}

namespace
{
    template< typename From, typename To >
    void ConvertDotationStock( const From& from, To* to )
    {
        CONVERT_ID_TO( resource, ressource_id );
        CONVERT_TO( quantity, quantite_disponible );
    }
    template< typename From, typename To >
    void ConvertLogSupplyEquimentAvailability( const From& from, To* to )
    {
        CONVERT_ID( equipment );
        CONVERT_TO( total, nbr_total );
        CONVERT_TO( available, nbr_disponibles );
        CONVERT_TO( working, nbr_au_travail );
        CONVERT_TO( lent, nbr_pretes );
        CONVERT_TO( resting, nbr_au_repos );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-23
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyState& from, MsgsSimToClient::MsgLogSupplyState* to )
{
    CONVERT_ID( unit );
    CONVERT_TO( chain, chaine_activee );
    CONVERT_LIST( stocks, elem, ConvertDotationStock );
    CONVERT_LIST_TO( transporters, disponibilites_transporteurs_convois, elem, ConvertLogSupplyEquimentAvailability );
}

namespace
{
    template< typename From, typename To >
    void ConvertDotationQuota( const From& from, To* to )
    {
        CONVERT_ID_TO( resource, ressource_id );
        CONVERT_TO( quantity, quota_disponible );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-23
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::LogSupplyQuotas& from, MsgsSimToClient::MsgLogSupplyQuotas* to )
{
    CONVERT_CB( supplied, ConvertParentEntity );
    CONVERT_CB( supplier, ConvertParentEntity );
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
    CONVERT_TO( name, nom );
    CONVERT_ID( party );
    CONVERT_CB( repartition, ConvertRepartition );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdUpdate& from, MsgsSimToClient::MsgCrowdUpdate* to )
{
    CONVERT_ID( crowd );
    CONVERT( domination );
    if( from.has_adhesions() )
        for( int i = 0; i < from.adhesions().adhesion_size(); ++i )
            ConvertPartyAdhesion( from.adhesions().adhesion( i ), to->mutable_adhesions()->add_adhesion() );
    CONVERT( critical_intelligence );
    CONVERT( armed_individuals );
    CONVERT( healthy );
    CONVERT( wounded );
    CONVERT( contaminated );
    CONVERT( dead );
    CONVERT_CB( extension, ConvertExtension );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::CrowdConcentrationCreation& from, MsgsSimToClient::MsgCrowdConcentrationCreation* to )
{
    CONVERT_ID( concentration );
    CONVERT_ID( crowd );
    CONVERT_CB( position, ConvertCoordLatLong );
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
        CONVERT_ENUM( attitude, ( sword::peaceful, Common::calme )
                                ( sword::agitated, Common::agitee )
                                ( sword::excited, Common::excitee )
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
    CONVERT( healthy );
    CONVERT( wounded );
    CONVERT( contaminated );
    CONVERT( dead );
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
    if( from.has_parts() && from.parts().has_location() )
        ConvertLocation( from.parts().location(), to->mutable_parts()->mutable_location() );
    CONVERT_CB( direction, ConvertHeading );
    CONVERT( speed );
    CONVERT( healthy );
    CONVERT( wounded );
    CONVERT( contaminated );
    CONVERT( dead );
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
    CONVERT_TO( domination, etat_domination );
    CONVERT( critical_intelligence );
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
    CONVERT_CB( position, ConvertCoordLatLong );
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
    CONVERT_TO( alive, nb_humains_vivants );
    CONVERT_TO( dead, nb_humains_morts );
    ConvertCrowdAttitude( from, to );
    CONVERT( pertinence );
    CONVERT_TO( perceived, est_percu );
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
        if( from.has_shape() && from.shape().has_location() )
            ConvertLocation( from.shape().location(), to->mutable_forme()->mutable_location() );
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
    CONVERT_LIST_TO( parts, portions_flux, elem, ConvertFlowPart );
    CONVERT_CB( direction, ConvertHeading );
    CONVERT_TO( speed, vitesse );
    CONVERT_TO( alive, nb_humains_vivants );
    CONVERT_TO( dead, nb_humains_morts );
    ConvertCrowdAttitude( from, to );
    CONVERT_TO( perceived, est_percu );
}

namespace
{
    template< typename From, typename To >
    void Assign( const From& from, To* to )
    {
        *to = from;
    }
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
        if( from.has_parking_floors() )
            to->set_parking_available( from.parking_floors() > 0 );
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
        CONVERT_CB( architecture, ConvertUrbanArchitecture );
        if( from.has_structure() && from.structure().has_state() )
            to->mutable_structure()->set_state( from.structure().state() );
        CONVERT_CB( color, ConvertRgbaColor );
        CONVERT_LIST( infrastructures, resource_network, ConvertResourceNetwork );
        if( from.has_infrastructures() && from.infrastructures().has_infrastructure() )
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
    CONVERT_ID( object );
    CONVERT( name );
    CONVERT_CB( location, ConvertLocation );
    CONVERT_CB( attributes, ConvertUrbanAttributes );
    CONVERT_ID( parent );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UrbanUpdate& from, MsgsSimToClient::MsgUrbanUpdate* to )
{
    CONVERT_ID( object );
    CONVERT_CB( location, ConvertLocation );
    CONVERT_CB( attributes, ConvertUrbanAttributes );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UrbanKnowledgeCreation& from, MsgsSimToClient::MsgUrbanKnowledgeCreation* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( party );
    CONVERT_ID( object );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::UrbanKnowledgeUpdate& from, MsgsSimToClient::MsgUrbanKnowledgeUpdate* to )
{
    CONVERT_ID( knowledge );
    CONVERT_ID( party );
    CONVERT_ID( object );
    CONVERT( progress );
    CONVERT_TO( max_progress, maxprogress );
    CONVERT( perceived );
    CONVERT_LIST( automat_perceptions, elem, ConvertIdentifier );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::StockResource& from, Common::StockResource* to )
{
    ConvertStockResource( from, to );
}

namespace
{
    template< typename From, typename To >
    void ConvertKnowledgeGroupAck( const From& from, To* to )
    {
        CONVERT_ID( knowledge_group );
        CONVERT_NON_INJECTIVE_ENUM( error_code, error_code, ( sword::KnowledgeGroupAck::no_error, MsgsSimToClient::KnowledgeGroupAck::no_error )
                                  ( sword::KnowledgeGroupAck::error_invalid_unit, MsgsSimToClient::KnowledgeGroupAck::error_invalid_unit )
                                  ( sword::KnowledgeGroupAck::error_invalid_superior, MsgsSimToClient::KnowledgeGroupAck::error_invalid_superior )
                                  ( sword::KnowledgeGroupAck::error_invalid_party, MsgsSimToClient::KnowledgeGroupAck::error_invalid_camp )
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
    CONVERT( name );
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
                              ( sword::ActionCreateFireOrderAck::error_invalid_ammunition, MsgsSimToClient::MsgActionCreateFireOrderAck::error_invalid_munition )
                              ( sword::ActionCreateFireOrderAck::error_invalid_iteration, MsgsSimToClient::MsgActionCreateFireOrderAck::error_invalid_iteration )
                              ( sword::ActionCreateFireOrderAck::error_target_not_illuminated, MsgsSimToClient::MsgActionCreateFireOrderAck::error_target_no_illuminated ) );
}

namespace
{
    template< typename From, typename To >
    void ConvertWeatherAttributes( const From& from, To* to )
    {
        CONVERT( temperature );
        CONVERT( wind_speed );
        CONVERT_CB( wind_direction, ConvertHeading );
        CONVERT( cloud_floor );
        CONVERT( cloud_ceiling );
        CONVERT( cloud_density );
        CONVERT_ENUM( precipitation, ( sword::WeatherAttributes::dry, Common::pas_de_precipitation )
                                     ( sword::WeatherAttributes::sand_storm, Common::tempete_sable )
                                     ( sword::WeatherAttributes::fog, Common::brouillard )
                                     ( sword::WeatherAttributes::drizzle, Common::crachin )
                                     ( sword::WeatherAttributes::rain, Common::pluie )
                                     ( sword::WeatherAttributes::snow, Common::neige ) );
        CONVERT_ENUM( lighting, ( sword::WeatherAttributes::clear_day, Common::jour_sans_nuage )
                                ( sword::WeatherAttributes::slightly_cloudy_day, Common::jour_peu_nuageux )
                                ( sword::WeatherAttributes::mildly_cloudy_day, Common::jour_moyennement_nuageux )
                                ( sword::WeatherAttributes::fairly_cloudy_day, Common::jour_assez_nuageux )
                                ( sword::WeatherAttributes::very_cloudy_day, Common::jour_tres_nuageux )
                                ( sword::WeatherAttributes::full_moon_night, Common::nuit_pleine_lune )
                                ( sword::WeatherAttributes::three_quater_moon_night, Common::nuit_trois_quart_de_lune )
                                ( sword::WeatherAttributes::half_quater_moon_night, Common::nuit_demi_lune )
                                ( sword::WeatherAttributes::quater_moon_night, Common::nuit_quart_de_lune )
                                ( sword::WeatherAttributes::new_moon_night, Common::nuit_nouvelle_lune )
                                ( sword::WeatherAttributes::artificial_light, Common::eclairant )
                                ( sword::WeatherAttributes::global_weather, Common::global_weather ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlGlobalWeather& from, MsgsSimToClient::MsgControlGlobalWeather* to )
{
    CONVERT_ID( weather );
    CONVERT_CB( attributes, ConvertWeatherAttributes );
}

// -----------------------------------------------------------------------------
// Name: SimulationToClient::Convert
// Created: MCO 2010-11-24
// -----------------------------------------------------------------------------
void SimulationToClient::Convert( const sword::ControlLocalWeatherCreation& from, MsgsSimToClient::MsgControlLocalWeatherCreation* to )
{
    CONVERT_ID( weather );
    CONVERT_CB_TO( top_left, top_left_coordinate, ConvertCoordLatLong );
    CONVERT_CB_TO( bottom_right, bottom_right_coordinate, ConvertCoordLatLong );
    CONVERT_CB( attributes, ConvertWeatherAttributes );
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
    for( int i = 0; i < from.objects().size(); ++i )
        to->add_objects()->set_id( from.objects( i ).id() );
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
        CONVERT( motivation );
        CONVERT( percentage );
    }
    template< typename From, typename To >
    void ConvertUsageOccupation( const From& from, To* to )
    {
        CONVERT( usage );
        CONVERT( number );
    }
    template< typename From, typename To >
    void ConvertBlockOccupation( const From& from, To* to )
    {
        CONVERT_ID( object );
        for( int i = 0; i < from.persons().size(); ++i )
            ConvertUsageOccupation( from.persons( i ), to->add_persons() );
#ifdef SHIELD_CLIENT
        CONVERT( alerted )
        CONVERT( confined )
        CONVERT( angriness )
#endif
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
    if( from.has_adhesions() )
        for( int i = 0; i < from.adhesions().adhesion_size(); ++i )
            ConvertPartyAdhesion( from.adhesions().adhesion( i ), to->mutable_adhesions()->add_adhesion() );
    CONVERT_CB( satisfaction, ConvertSatisfaction );
    CONVERT_LIST( satisfaction, resources, ConvertResourceSatisfaction );
    CONVERT_LIST( satisfaction, motivations, ConvertMotivationSatisfaction );
    CONVERT( motivation );
    for( int i = 0; i < from.occupations().size(); ++i )
        ConvertBlockOccupation( from.occupations( i ), to->add_occupations() );
    CONVERT_CB( extension, ConvertExtension );
}

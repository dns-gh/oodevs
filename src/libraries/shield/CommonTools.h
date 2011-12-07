// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_Common_h
#define shield_Common_h

#if !defined( SHIELD_CLIENT ) && !defined( SHIELD_SIMULATION )
#error do not include CommonTools.h directly but ClientTools.h or SimulationTools.h // $$$$ MAT : yep, totally crappy !
#endif

#include "Tools.h"

#define CONVERT_DIPLOMACY( from_field, to_field ) \
    CONVERT_ENUM_TO( from_field, to_field, ( sword::unknown, Common::unknown_diplo ) \
                                           ( sword::friendly, Common::friend_diplo ) \
                                           ( sword::enemy, Common::enemy_diplo ) \
                                           ( sword::neutral, Common::neutral_diplo ) )

#define HUMAN_WOUND ( sword::unwounded, Common::non_blesse ) \
                    ( sword::dead, Common::mort ) \
                    ( sword::wounded_urgency_1, Common::blesse_urgence_1 ) \
                    ( sword::wounded_urgency_2, Common::blesse_urgence_2 ) \
                    ( sword::wounded_urgency_3, Common::blesse_urgence_3 ) \
                    ( sword::wounded_extreme_urgency, Common::blesse_urgence_extreme )

#define CONVERT_AUTOMAT_MODE( from_field, to_field ) \
    CONVERT_ENUM_TO( from_field, to_field, ( sword::engaged, Common::embraye ) \
                                           ( sword::disengaged, Common::debraye ) )

#define CONVERT_SIMULATION_STATE( field ) \
    CONVERT_ENUM( field, ( sword::running, Common::running ) \
                         ( sword::paused, Common::paused ) \
                         ( sword::stopped, Common::stopped ) \
                         ( sword::loading, Common::loading ) )

namespace shield
{
    template< typename From, typename To >
    void ConvertNatureLevel( const From& from, To* to )
    {
        CONVERT_ENUM( level, ( sword::none_naturelevel, Common::none_naturelevel )
                             ( sword::b, Common::b )
                             ( sword::o, Common::o )
                             ( sword::c, Common::c )
                             ( sword::oo, Common::oo )
                             ( sword::ooo, Common::ooo )
                             ( sword::i, Common::i )
                             ( sword::ii, Common::ii )
                             ( sword::iii, Common::iii )
                             ( sword::x, Common::x )
                             ( sword::xx, Common::xx )
                             ( sword::xxx, Common::xxx )
                             ( sword::xxxx, Common::xxxx )
                             ( sword::xxxxx, Common::xxxxx ) );
    }
    template< typename From, typename To >
    static void ConvertControlAckErrorCode( const From& from, To* to )
    {
        CONVERT_ENUM( error_code, ( sword::ControlAck::no_error, MsgsSimToClient::ControlAck::no_error )
                                  ( sword::ControlAck::error_already_started, MsgsSimToClient::ControlAck::error_already_started )
                                  ( sword::ControlAck::error_not_started, MsgsSimToClient::ControlAck::error_not_started )
                                  ( sword::ControlAck::error_not_paused, MsgsSimToClient::ControlAck::error_not_paused )
                                  ( sword::ControlAck::error_already_paused, MsgsSimToClient::ControlAck::error_already_paused )
                                  ( sword::ControlAck::error_invalid_time_factor, MsgsSimToClient::ControlAck::error_invalid_time_factor )
                                  ( sword::ControlAck::error_invalid_date_time, MsgsSimToClient::ControlAck::error_invalid_date_time ) );
    }
    template< typename From, typename To >
    void ConvertCoordLatLong( const From& from, To* to )
    {
        CONVERT( latitude );
        CONVERT( longitude );
    }
    template< typename From, typename To >
    void ConvertIdentifier( const From& from, To* to )
    {
        CONVERT( id );
    }
    template< typename From, typename To >
    void ConvertLocation( const From& from, To* to )
    {
        CONVERT_ENUM( type, ( sword::Location::circle, Common::MsgLocation::circle )
                            ( sword::Location::ellipse, Common::MsgLocation::ellipse )
                            ( sword::Location::line, Common::MsgLocation::line )
                            ( sword::Location::rectangle, Common::MsgLocation::rectangle )
                            ( sword::Location::polygon, Common::MsgLocation::polygon )
                            ( sword::Location::point, Common::MsgLocation::point )
                            ( sword::Location::sector, Common::MsgLocation::sector )
                            ( sword::Location::none, Common::MsgLocation::none ) );
        CONVERT_LIST( coordinates, elem, ConvertCoordLatLong );
    }
    template< typename From, typename To >
    void ConvertParentEntity( const From& from, To* to )
    {
        CONVERT_ID( automat );
        CONVERT_ID( formation );
    }
    template< typename From, typename To >
    void ConvertProfileDescription( const From& from, To* to )
    {
        CONVERT( login );
        CONVERT( password );
        CONVERT( supervisor );
    }
    template< typename From, typename To >
    void ConvertDiffusion( const From& from, To* to )
    {
        CONVERT_ID( automat );
        CONVERT_ID( formation );
        CONVERT_ID( knowledge_group );
        for( int i = 0; i < from.party().size(); ++i )
            to->add_party()->set_id( from.party( i ).id() );
    }
    template< typename From, typename To >
    void ConvertPointsToLocation( const From& from, To* to )
    {
        to->set_type( Common::MsgLocation::polygon );
        for( int i = 0; i < from.elem().size(); ++i )
            ConvertCoordLatLong( from.elem( i ), to->mutable_coordinates()->add_elem() );
    }
    template< typename From, typename To >
    void ConvertLocationToPoints( const From& from, To* to )
    {
        for( int i = 0; i < from.coordinates().elem().size(); ++i )
            ConvertCoordLatLong( from.coordinates().elem( i ), to->add_elem() );
    }
    template< typename From, typename To >
    void ConvertColor( const From& from, To* to )
    {
        CONVERT( red );
        CONVERT( green );
        CONVERT( blue );
    }
    template< typename From, typename To >
    void ConvertShape( const From& from, To* to )
    {
        CONVERT_CB( color, ConvertColor );
#ifdef SHIELD_SIMULATION
        CONVERT_TO( pattern, external_identifier );
        CONVERT_CB_TO( points, location, ConvertPointsToLocation );
#elif defined SHIELD_CLIENT
        CONVERT_TO( external_identifier, pattern );
        CONVERT_CB_TO( location, points, ConvertLocationToPoints );
        to->set_category( "" );
#endif
        CONVERT_CB( diffusion, ConvertDiffusion);
    }
    template< typename From, typename To >
    void ConvertMarker( const From& from, To* to )
    {
        CONVERT( name );
        CONVERT( description );
        CONVERT_ID( parent );
#ifdef SHIELD_SIMULATION
        CONVERT_TO( number, optional_information );
#else
        CONVERT_TO( optional_information, number );
#endif
    }
    template< typename From, typename To >
    void ConvertPropertyValue( const From& from, To* to )
    {
        CONVERT( string_value );
        CONVERT( integer_value );
        CONVERT( unsigned_integer_value );
        CONVERT( float_value );
        CONVERT( bool_value );
    }
    template< typename From, typename To >
    void ConvertProperty( const From& from, To* to )
    {
        CONVERT( name );
        CONVERT_CB( value, ConvertPropertyValue );
    }
    template< typename From, typename To >
    void ConvertProperties( const From& from, To* to )
    {
        for( int i = 0; i < from.properties().size(); ++i )
            ConvertProperty( from.properties( i ), to->add_properties() );
    }
    template< typename From, typename To >
    void ConvertHeading( const From& from, To* to )
    {
        CONVERT( heading );
    }
    template< typename From, typename To >
    void ConvertLocationElem( const From& from, To* to )
    {
        ConvertLocation( from.location(), to->mutable_location() );
    }
    template< typename From, typename To >
    void ConvertPlannedWork( const From& from, To* to )
    {
        CONVERT( type );
        CONVERT_CB( position, ConvertLocation );
        CONVERT_ENUM( type_obstacle, ( sword::ObstacleType::preliminary, Common::ObstacleType::preliminary )
                                     ( sword::ObstacleType::reserved, Common::ObstacleType::reserved ) );
#ifdef SHIELD_SIMULATION
        CONVERT_TO( density, densite );
        CONVERT_ID_TO( combat_train, tc2 );
#else
        CONVERT_TO( densite, density );
        CONVERT_ID_TO( tc2, combat_train );
#endif
        CONVERT( activity_time );
    }
    template< typename From, typename To >
    void ConvertLogMedicalPriorities( const From& from, To* to )
    {
        CONVERT_ENUM_LIST( elem, HUMAN_WOUND );
    }
    template< typename From, typename To >
    void ConvertResourceNetworkElement( const From& from, To* to )
    {
        CONVERT_ID( object );
        CONVERT_NAME( resource );
    }
    template< typename From, typename To >
    void ConvertObjective( const From& from, To* to )
    {
#ifdef SHIELD_CLIENT
        CONVERT_CB_TO( localisation, location, ConvertLocation );
        CONVERT_DATE_TO( horaire, time );
#elif defined SHIELD_SIMULATION
        CONVERT_CB_TO( location, localisation, ConvertLocation );
        CONVERT_DATE_TO( time, horaire );
#endif
    }
    template< typename From, typename To >
    void ConvertLimaOrder( const From& from, To* to )
    {
#ifdef SHIELD_CLIENT
        ConvertLocation( from.lima().location(), to->mutable_line()->mutable_location() );
        CONVERT_DATE_TO( horaire, time );
#elif defined SHIELD_SIMULATION
        ConvertLocation( from.line().location(), to->mutable_lima()->mutable_location() );
        CONVERT_DATE_TO( time, horaire );
#endif
        CONVERT_ENUM_LIST( fonctions, ( sword::PhaseLineOrder::line_of_departure, Common::MsgLimaOrder::ligne_debouche )
                                      ( sword::PhaseLineOrder::attitude_change_line, Common::MsgLimaOrder::ligne_changement_attitude )
                                      ( sword::PhaseLineOrder::coordination_line, Common::MsgLimaOrder::ligne_coordination )
                                      ( sword::PhaseLineOrder::denial_line, Common::MsgLimaOrder::ligne_interdire )
                                      ( sword::PhaseLineOrder::objective_line, Common::MsgLimaOrder::ligne_objectif )
                                      ( sword::PhaseLineOrder::blocking_line, Common::MsgLimaOrder::ligne_coup_arret )
                                      ( sword::PhaseLineOrder::handover_line, Common::MsgLimaOrder::ligne_recueil )
                                      ( sword::PhaseLineOrder::start_of_mission_line, Common::MsgLimaOrder::ligne_debut_mission )
                                      ( sword::PhaseLineOrder::end_of_mission_line, Common::MsgLimaOrder::ligne_fin_mission )
                                      ( sword::PhaseLineOrder::recognition_and_reception_line, Common::MsgLimaOrder::ligne_identification_accueil ) );
    }
    template< typename From, typename To >
    void ConvertValue( const From& from, To* to )
    {
        CONVERT( booleanvalue );
        CONVERT( intvalue );
        CONVERT_CB( heading, ConvertHeading );
        CONVERT( enumeration );
        CONVERT_DATE( datetime );
        CONVERT_CB( point, ConvertLocationElem );
        CONVERT_CB( area, ConvertLocationElem );
        CONVERT_CB( path, ConvertLocationElem );
        CONVERT_CB( limit, ConvertLocationElem );
        CONVERT_ID( automat );
        CONVERT_ID( agent );
        CONVERT_ID( agentknowledge );
        CONVERT_ID( crowdknowledge );
        CONVERT_ID( objectknowledge );
        CONVERT_ID( urbanknowledge );
        CONVERT_CB( plannedwork, ConvertPlannedWork );
#ifdef SHIELD_CLIENT
        if( from.has_atlasnature() )
            to->mutable_nature()->set_flags( from.atlasnature().nature() );
#elif defined SHIELD_SIMULATION
        if( from.has_nature() )
            to->mutable_atlasnature()->set_nature( from.nature().flags() );
#endif
        CONVERT_ID( resourcetype );
        CONVERT_LIST( logmaintenancepriorities, elem, ConvertIdentifier );
        CONVERT_CB( logmedicalpriorities, ConvertLogMedicalPriorities );
        CONVERT_CB( resourcenetwork, ConvertResourceNetworkElement );
        CONVERT( areal );
        CONVERT_LIST( pathlist, elem, ConvertLocationElem );
        CONVERT_LIST( pointlist, elem, ConvertLocationElem );
        CONVERT_LIST( polygonlist, elem, ConvertLocationElem );
        CONVERT_CB( location, ConvertLocation );
        CONVERT_LIST( locationlist, elem, ConvertLocation );
        CONVERT_LIST( unitlist, elem, ConvertIdentifier );
        CONVERT_LIST( automatlist, elem, ConvertIdentifier );
        CONVERT_LIST( unitknowledgelist, elem, ConvertIdentifier );
        CONVERT_LIST( objectknowledgelist, elem, ConvertIdentifier );
        CONVERT_ID( equipmenttype );
#ifdef SHIELD_CLIENT
        CONVERT_ID_TO( tirindirect, indirectfire );
#elif defined SHIELD_SIMULATION
        CONVERT_ID_TO( indirectfire, tirindirect );
#endif
        CONVERT( acharstr );
        CONVERT_CB( missionobjective, ConvertObjective );
        CONVERT_LIST( missionobjectivelist, elem, ConvertObjective );
        CONVERT_LIST( phaseline, elem, ConvertLimaOrder );
        CONVERT_ID( object );
        CONVERT_ID( party );
        CONVERT_ID( formation );
        CONVERT( identifier );
        CONVERT( quantity );
        CONVERT_ID( knowledgegroup );
        CONVERT_LIST( extensionlist, entries, ConvertExtensionEntry );
        CONVERT_CB( push_flow_parameters, ConvertPushFlow );
        CONVERT_CB( pull_flow_parameters, ConvertPullFlow );
        for( int i = 0; i < from.list().size(); ++i )
            ConvertValue( from.list( i ), to->add_list() );
        CONVERT( external_identifier );
    }
    template< typename From, typename To >
    void ConvertMissionParameter( const From& from, To* to )
    {
        CONVERT( null_value );
        for( int i = 0; i < from.value().size(); ++i )
            ConvertValue( from.value( i ), to->add_value() );
    }
    template< typename From, typename To >
    void ConvertOrder( const From& from, To* to )
    {
        CONVERT_ID( tasker );
        CONVERT_ID( type );
        CONVERT_LIST( parameters, elem, ConvertMissionParameter );
        CONVERT( label );
        CONVERT_CB( symbollocation, ConvertLocation );
        CONVERT_DATE( start_time );
    }
    template< typename From, typename To >
    void ConvertTasker( const From& from, To* to )
    {
        CONVERT_ID( automat );
        CONVERT_ID( formation );
        CONVERT_ID( crowd );
        CONVERT_ID( unit );
        CONVERT_ID( party );
        CONVERT_ID( population );
    }
    template< typename From, typename To >
    void ConvertPartyAdhesion( const From& from, To* to )
    {
        CONVERT_ID( party );
        CONVERT( value );
    }
    template< typename From, typename To >
    void ConvertExercise( const From& from, To* to )
    {
        CONVERT( name );
        CONVERT( running );
        CONVERT( port );
    }
    template< typename From, typename To >
    void ConvertUrbanUsage( const From& from, To* to )
    {
        CONVERT( role );
        CONVERT( percentage );
    }
    template< typename From, typename To >
    void ConvertExtensionEntry( const From& from, To* to)
    {
        CONVERT( name );
        CONVERT( value );
    }
    template< typename From, typename To >
    void ConvertSupplyResource( const From& from, To* to)
    {
        CONVERT_ID( resourcetype );
        CONVERT( quantity );
    }
    template< typename From, typename To >
    void ConvertSupplyTransporter( const From& from, To* to)
    {
        CONVERT_ID( equipmenttype );
        CONVERT( quantity );
    }
    template< typename From, typename To >
    void ConvertSupplyRecipient( const From& from, To* to)
    {
        CONVERT_ID( receiver );
        CONVERT_SIMPLE_LIST( resources, ConvertSupplyResource );
        CONVERT_LIST( path, elem, ConvertLocationElem );
    }
    template< typename From, typename To >
    void ConvertPushFlow( const From& from, To* to)
    {
        CONVERT_SIMPLE_LIST( recipients, ConvertSupplyRecipient );
        CONVERT_SIMPLE_LIST( transporters, ConvertSupplyTransporter );
        CONVERT_LIST( waybackpath, elem, ConvertLocationElem );
    }
    template< typename From, typename To >
    void ConvertPullFlow( const From& from, To* to)
    {
        CONVERT_CB( supplier, ConvertParentEntity );
        CONVERT_SIMPLE_LIST( resources, ConvertSupplyResource );
        CONVERT_SIMPLE_LIST( transporters, ConvertSupplyTransporter );
        CONVERT_LIST( wayoutpath, elem, ConvertLocationElem );
        CONVERT_LIST( waybackpath, elem, ConvertLocationElem );
    }
}

#endif // shield_Common_h

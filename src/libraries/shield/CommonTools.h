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

#include "Tools.h"

#define CONVERT_DIPLOMACY( from_field, to_field ) \
    CONVERT_ENUM_TO( from_field, to_field, ( sword::unknown_diplo, Common::unknown_diplo ) \
                                           ( sword::friend_diplo, Common::friend_diplo ) \
                                           ( sword::enemy_diplo, Common::enemy_diplo ) \
                                           ( sword::neutral_diplo, Common::neutral_diplo ) )

#define HUMAN_WOUND ( sword::non_blesse, Common::non_blesse ) \
                    ( sword::mort, Common::mort ) \
                    ( sword::blesse_urgence_1, Common::blesse_urgence_1 ) \
                    ( sword::blesse_urgence_2, Common::blesse_urgence_2 ) \
                    ( sword::blesse_urgence_3, Common::blesse_urgence_3 ) \
                    ( sword::blesse_urgence_extreme, Common::blesse_urgence_extreme )

#define CONVERT_AUTOMAT_MODE( field ) \
    CONVERT_ENUM( field, ( sword::embraye, Common::embraye ) \
                         ( sword::debraye, Common::debraye ) )

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
    void ConvertIntelligence( const From& from, To* to )
    {
        CONVERT( name );
        CONVERT( nature );
        ConvertNatureLevel( from, to );
        CONVERT( embarked );
        ConvertCoordLatLong( from.location(), to->mutable_location() );
        CONVERT_DIPLOMACY( diplomacy, diplomacy );
        CONVERT_ID( formation );
    }
    template< typename From, typename To >
    void ConvertProfileDescription( const From& from, To* to )
    {
        CONVERT( login );
        CONVERT( password );
        CONVERT( supervisor );
        CONVERT_ID( role );
    }
    template< typename From, typename To >
    void ConvertDiffusion( const From& from, To* to )
    {
        CONVERT_ID( automat );
        CONVERT_ID( formation );
    }
    template< typename From, typename To >
    void ConvertTacticalLine( const From& from, To* to )
    {
        to->mutable_tactical_line()->set_name( from.tactical_line().name() );
        ConvertLocation( from.tactical_line().geometry(), to->mutable_tactical_line()->mutable_geometry() );
        ConvertDiffusion( from.tactical_line().diffusion(), to->mutable_tactical_line()->mutable_diffusion() );
    }
    template< typename From, typename To >
    void ConvertShape( const From& from, To* to )
    {
        CONVERT( category );
        CONVERT( color );
        CONVERT( pattern );
        CONVERT_LIST( points, elem, ConvertCoordLatLong );
    }
    template< typename From, typename To >
    void ConvertMarker( const From& from, To* to )
    {
        CONVERT( name );
        CONVERT( description );
        CONVERT_ID( parent );
        CONVERT( number );
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
        ConvertPropertyValue( from.value(), to->mutable_value() );
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
        ConvertLocation( from.position(), to->mutable_position() );
        CONVERT_ENUM( type_obstacle, ( sword::ObstacleType::preliminary, Common::ObstacleType::preliminary )
                                     ( sword::ObstacleType::reserved, Common::ObstacleType::reserved ) );
        CONVERT( densite );
        CONVERT_ID( tc2 );
        CONVERT( activity_time );
        CONVERT_ID( planned_work );
    }
    template< typename From, typename To >
    void ConvertLogMedicalPriorities( const From& from, To* to )
    {
        CONVERT_ENUM_LIST( elem, HUMAN_WOUND );
    }
    template< typename From, typename To >
    void ConvertObjective( const From& from, To* to )
    {
        ConvertLocation( from.localisation(), to->mutable_localisation() );
        to->mutable_horaire()->set_data( from.horaire().data() );
    }
    template< typename From, typename To >
    void ConvertLimaOrder( const From& from, To* to )
    {
        ConvertLocation( from.lima().location(), to->mutable_lima()->mutable_location() );
        to->mutable_horaire()->set_data( from.horaire().data() );
        CONVERT_ENUM_LIST( fonctions, ( sword::LimaOrder::ligne_debouche, Common::MsgLimaOrder::ligne_debouche )
                                      ( sword::LimaOrder::ligne_changement_attitude, Common::MsgLimaOrder::ligne_changement_attitude )
                                      ( sword::LimaOrder::ligne_coordination, Common::MsgLimaOrder::ligne_coordination )
                                      ( sword::LimaOrder::ligne_interdire, Common::MsgLimaOrder::ligne_interdire )
                                      ( sword::LimaOrder::ligne_objectif, Common::MsgLimaOrder::ligne_objectif )
                                      ( sword::LimaOrder::ligne_coup_arret, Common::MsgLimaOrder::ligne_coup_arret )
                                      ( sword::LimaOrder::ligne_recueil, Common::MsgLimaOrder::ligne_recueil )
                                      ( sword::LimaOrder::ligne_debut_mission, Common::MsgLimaOrder::ligne_debut_mission )
                                      ( sword::LimaOrder::ligne_fin_mission, Common::MsgLimaOrder::ligne_fin_mission )
                                      ( sword::LimaOrder::ligne_identification_accueil, Common::MsgLimaOrder::ligne_identification_accueil ) );
    }
    template< typename From, typename To >
    void ConvertValue( const From& from, To* to )
    {
        CONVERT( booleanvalue );
        CONVERT( intvalue );
        if( from.has_heading() )
            ConvertHeading( from.heading(), to->mutable_heading() );
        CONVERT( enumeration );
        if( from.has_datetime() )
            to->mutable_datetime()->set_data( from.datetime().data() );
        if( from.has_point() )
            ConvertLocationElem( from.point(), to->mutable_point() );
        if( from.has_area() )
            ConvertLocationElem( from.area(), to->mutable_area() );
        if( from.has_path() )
            ConvertLocationElem( from.path(), to->mutable_path() );
        if( from.has_limit() )
            ConvertLocationElem( from.limit(), to->mutable_limit() );
        CONVERT_LIST( phaseline, elem, ConvertLimaOrder );
        CONVERT_ID( automat );
        CONVERT_ID( agent );
        CONVERT_ID( agentknowledge );
        CONVERT_ID( crowdknowledge );
        CONVERT_ID( objectknowledge );
        CONVERT_ID( urbanknowledge );
        if( from.has_plannedwork() )
            ConvertPlannedWork( from.plannedwork(), to->mutable_plannedwork() );
        if( from.has_atlasnature() )
            to->mutable_atlasnature()->set_nature( from.atlasnature().nature() );
        CONVERT_ID( resourcetype );
        CONVERT_LIST( logmaintenancepriorities, elem, ConvertIdentifier );
        if( from.has_logmedicalpriorities() )
            ConvertLogMedicalPriorities( from.logmedicalpriorities(), to->mutable_logmedicalpriorities() );
        CONVERT( areal );
        CONVERT_LIST( pathlist, elem, ConvertLocationElem );
        CONVERT_LIST( pointlist, elem, ConvertLocationElem );
        CONVERT_LIST( polygonlist, elem, ConvertLocationElem );
        if( from.has_location() )
            ConvertLocation( from.location(), to->mutable_location() );
        CONVERT_LIST( locationlist, elem, ConvertLocation );
        CONVERT_LIST( unitlist, elem, ConvertIdentifier );
        CONVERT_LIST( automatlist, elem, ConvertIdentifier );
        CONVERT_LIST( unitknowledgelist, elem, ConvertIdentifier );
        CONVERT_LIST( objectknowledgelist, elem, ConvertIdentifier );
        CONVERT_LIST( plannedworklist, elem, ConvertPlannedWork );
        CONVERT_ID( equipmenttype );
        CONVERT_ID( tirindirect );
        CONVERT( acharstr );
        if( from.has_missionobjective() )
            ConvertObjective( from.missionobjective(), to->mutable_missionobjective() );
        CONVERT_LIST( missionobjectivelist, elem, ConvertObjective );
        CONVERT_LIST( limasorder, elem, ConvertLimaOrder );
        CONVERT_LIST( intelligencelist, elem, ConvertIntelligence );
        CONVERT_ID( urbanblock );
        CONVERT_ID( party );
        CONVERT_ID( formation );
        CONVERT( identifier );
        CONVERT( quantity );
        CONVERT_ID( knowledgegroup );
        for( int i = 0; i < from.list().size(); ++i )
            ConvertValue( from.list( i ), to->add_list() );
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
        if( from.has_symbollocation() )
            ConvertLocation( from.symbollocation(), to->mutable_symbollocation() );
        if( from.has_start_time() )
            to->mutable_start_time()->set_data( from.start_time().data() );
    }
    template< typename From, typename To >
    void ConvertTasker( const From& from, To* to )
    {
        CONVERT_ID( automat );
        CONVERT_ID( formation );
        CONVERT_ID( crowd );
        CONVERT_ID( unit );
        CONVERT_ID( party );
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
        CONVERT_ID( role );
        CONVERT( value );
    }
}

#endif // shield_Common_h

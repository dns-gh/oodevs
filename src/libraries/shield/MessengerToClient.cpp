// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "MessengerToClient.h"
#include "SimulationTools.h"

using namespace shield;

namespace
{
    template< typename From, typename To >
    void ConvertLimit( const From& from, To* to )
    {
        CONVERT_ID( id );
        ConvertTacticalLine( from, to );
    }
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::LimitCreation& from, MsgsMessengerToClient::MsgLimitCreation* to )
{
    ConvertLimit( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::LimitUpdate& from, MsgsMessengerToClient::MsgLimitUpdate* to )
{
    ConvertLimit( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::LimitDestruction& from, MsgsMessengerToClient::MsgLimitDestruction* to )
{
    CONVERT_ID( id );
}

namespace
{
    template< typename From, typename To >
    void ConvertTacticalLineErrorCode( const From& from, To* to )
    {
        CONVERT_ENUM( error_code, ( sword::TacticalLineAck::no_error, MsgsMessengerToClient::TacticalLineAck::no_error )
                                  ( sword::TacticalLineAck::error_invalid_id, MsgsMessengerToClient::TacticalLineAck::error_invalid_id )
                                  ( sword::TacticalLineAck::error_invalid_geometry, MsgsMessengerToClient::TacticalLineAck::error_invalid_geometry )
                                  ( sword::TacticalLineAck::error_invalid_diffusion, MsgsMessengerToClient::TacticalLineAck::error_invalid_diffusion ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::LimitCreationRequestAck& from, MsgsMessengerToClient::MsgLimitCreationRequestAck* to )
{
    ConvertTacticalLineErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::LimitUpdateRequestAck& from, MsgsMessengerToClient::MsgLimitUpdateRequestAck* to )
{
    ConvertTacticalLineErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::LimitDestructionRequestAck& from, MsgsMessengerToClient::MsgLimitDestructionRequestAck* to )
{
    ConvertTacticalLineErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::PhaseLineCreation& from, MsgsMessengerToClient::MsgLimaCreation* to )
{
    ConvertLimit( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::PhaseLineUpdate& from, MsgsMessengerToClient::MsgLimaUpdate* to )
{
    ConvertLimit( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::PhaseLineDestruction& from, MsgsMessengerToClient::MsgLimaDestruction* to )
{
    CONVERT_ID( id );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::PhaseLineCreationAck& from, MsgsMessengerToClient::MsgLimaCreationRequestAck* to )
{
    ConvertTacticalLineErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::PhaseLineUpdateRequestAck& from, MsgsMessengerToClient::MsgLimaUpdateRequestAck* to )
{
    ConvertTacticalLineErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::PhaseLineDestructionRequestAck& from, MsgsMessengerToClient::MsgLimaDestructionRequestAck* to )
{
    ConvertTacticalLineErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::IntelligenceCreation& from, MsgsMessengerToClient::MsgIntelligenceCreation* to )
{
    CONVERT_ID( id );
    CONVERT_CB( intelligence, ConvertIntelligence );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::IntelligenceUpdate& from, MsgsMessengerToClient::MsgIntelligenceUpdate* to )
{
    CONVERT_ID( id );
    if( from.has_formation() )
        to->mutable_formation()->set_id( from.formation().id() );
    CONVERT( name );
    CONVERT( nature );
    if( from.has_level() )
        ConvertNatureLevel( from, to );
    CONVERT( embarked );
    CONVERT_CB( location, ConvertCoordLatLong );
    CONVERT_DIPLOMACY( diplomacy, diplomacy );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::IntelligenceDestruction& from, MsgsMessengerToClient::MsgIntelligenceDestruction* to )
{
    CONVERT_ID( id );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ShapeCreation& from, MsgsMessengerToClient::MsgShapeCreation* to )
{
    CONVERT_ID( id );
    CONVERT_CB( shape, ConvertShape );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ShapeUpdate& from, MsgsMessengerToClient::MsgShapeUpdate* to )
{
    CONVERT_ID( id );
    CONVERT_CB( shape, ConvertShape );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ShapeDestruction& from, MsgsMessengerToClient::MsgShapeDestruction* to )
{
    CONVERT_ID( id );
}

namespace
{
    template< typename From, typename To >
    void ConvertShapeRequestAckErrorCode( const From& from, To* to )
    {
        CONVERT_ENUM( error_code, ( sword::ShapeRequestAck::no_error, MsgsMessengerToClient::ShapeRequestAck::no_error )
                                  ( sword::ShapeRequestAck::error_invalid_oid, MsgsMessengerToClient::ShapeRequestAck::error_invalid_oid )
                                  ( sword::ShapeRequestAck::error_invalid_geometry, MsgsMessengerToClient::ShapeRequestAck::error_invalid_geometry ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ShapeCreationRequestAck& from, MsgsMessengerToClient::MsgShapeCreationRequestAck* to )
{
    ConvertShapeRequestAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ShapeDestructionRequestAck& from, MsgsMessengerToClient::MsgShapeDestructionRequestAck* to )
{
    ConvertShapeRequestAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ShapeUpdateRequestAck& from, MsgsMessengerToClient::MsgShapeUpdateRequestAck* to )
{
    if( from.has_error_code() )
       ConvertShapeRequestAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::TextMessage& from, Common::MsgTextMessage* to )
{
    if( from.has_source() && from.source().has_profile() )
        to->mutable_source()->set_profile( from.source().profile() );
    if( from.has_target() && from.target().has_profile() )
        to->mutable_target()->set_profile( from.target().profile() );
    CONVERT( message );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::MarkerCreation& from, MsgsMessengerToClient::MsgMarkerCreation* to )
{
    CONVERT_ID( marker );
    CONVERT_CB( definition, ConvertMarker );
    CONVERT_DATE( date );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::MarkerUpdate& from, MsgsMessengerToClient::MsgMarkerUpdate* to )
{
    CONVERT_ID( marker );
    CONVERT_DATE( date );
    CONVERT( name );
    CONVERT( number );
    CONVERT( description );
    CONVERT_ID( parent );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::MarkerDestruction& from, MsgsMessengerToClient::MsgMarkerDestruction* to )
{
    CONVERT_ID( marker );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ClientObjectCreation& from, MsgsMessengerToClient::MsgClientObjectCreation* to )
{
    CONVERT_ID( object );
    CONVERT( name );
    ConvertProperties( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ClientObjectUpdate& from, MsgsMessengerToClient::MsgClientObjectUpdate* to )
{
    CONVERT_ID( object );
    CONVERT( name );
    ConvertProperties( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ClientObjectDestruction& from, MsgsMessengerToClient::MsgClientObjectDestruction* to )
{
    CONVERT_ID( object );
}

namespace
{
    template< typename From, typename To >
    void ConvertClientObjectAckErrorCode( const From& from, To* to )
    {
        CONVERT_ENUM( error_code, ( sword::ClientObjectAck::success, MsgsMessengerToClient::ClientObjectAck::success )
                                  ( sword::ClientObjectAck::duplicate_attribute_name, MsgsMessengerToClient::ClientObjectAck::duplicate_attribute_name )
                                  ( sword::ClientObjectAck::invalid_attribute_name, MsgsMessengerToClient::ClientObjectAck::invalid_attribute_name )
                                  ( sword::ClientObjectAck::invalid_object_id, MsgsMessengerToClient::ClientObjectAck::invalid_object_id ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ClientObjectCreationRequestAck& from, MsgsMessengerToClient::MsgClientObjectCreationRequestAck* to )
{
    CONVERT( name );
    ConvertClientObjectAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ClientObjectDestructionRequestAck& from, MsgsMessengerToClient::MsgClientObjectDestructionRequestAck* to )
{
    CONVERT_ID( object );
    ConvertClientObjectAckErrorCode( from, to );
}

// -----------------------------------------------------------------------------
// Name: MessengerToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void MessengerToClient::Convert( const sword::ClientObjectUpdateRequestAck& from, MsgsMessengerToClient::MsgClientObjectUpdateRequestAck* to )
{
    CONVERT_ID( object );
    ConvertClientObjectAckErrorCode( from, to );
}

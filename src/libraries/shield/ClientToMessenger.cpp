// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ClientToMessenger.h"
#include "ClientTools.h"

using namespace shield;

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgLimitCreationRequest& from, sword::LimitCreationRequest* to )
{
    ConvertTacticalLine( from, to );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgLimitUpdateRequest& from, sword::LimitUpdateRequest* to )
{
    CONVERT_ID( id );
    ConvertTacticalLine( from, to );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgLimitDestructionRequest& from, sword::LimitDestructionRequest* to )
{
    CONVERT_ID( id );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgLimaCreationRequest& from, sword::PhaseLineCreationRequest* to )
{
    ConvertTacticalLine( from, to );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgLimaUpdateRequest& from, sword::PhaseLineUpdateRequest* to )
{
    CONVERT_ID( id );
    ConvertTacticalLine( from, to );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgLimaDestructionRequest& from, sword::PhaseLineDestructionRequest* to )
{
    CONVERT_ID( id );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgIntelligenceCreationRequest& from, sword::IntelligenceCreationRequest* to )
{
    CONVERT_CB( intelligence, ConvertIntelligence );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgIntelligenceUpdateRequest& from, sword::IntelligenceUpdateRequest* to )
{
    CONVERT_ID( intelligence );
    ConvertIntelligence( from, to );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgIntelligenceDestructionRequest& from, sword::IntelligenceDestructionRequest* to )
{
    CONVERT_ID( id );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgShapeCreationRequest& from, sword::ShapeCreationRequest* to )
{
    CONVERT_CB( shape, ConvertShape );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgShapeUpdateRequest& from, sword::ShapeUpdateRequest* to )
{
    CONVERT_ID( shape );
    CONVERT( category );
    CONVERT_CB( color, ConvertColor );
    CONVERT_TO( template_, pattern );
    CONVERT_LIST( points, elem, ConvertCoordLatLong );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgShapeDestructionRequest& from, sword::ShapeDestructionRequest* to )
{
    CONVERT_ID( id );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const Common::MsgTextMessage& from, sword::TextMessage* to )
{
    if( from.has_source() && from.source().has_profile() )
        to->mutable_source()->set_profile( from.source().profile() );
    if( from.has_target() && from.target().has_profile() )
        to->mutable_target()->set_profile( from.target().profile() );
    CONVERT( message );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgMarkerCreationRequest& from, sword::MarkerCreationRequest* to )
{
    CONVERT_CB( marker, ConvertMarker );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgMarkerUpdateRequest& from, sword::MarkerUpdateRequest* to )
{
    CONVERT_ID( marker );
    ConvertMarker( from, to );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgMarkerDestructionRequest& from, sword::MarkerDestructionRequest* to )
{
    CONVERT_ID( marker );
    CONVERT( delete_all );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgClientObjectCreationRequest& from, sword::ClientObjectCreationRequest* to )
{
    CONVERT( name );
    ConvertProperties( from, to );
    CONVERT( persistent );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgClientObjectUpdateRequest& from, sword::ClientObjectUpdateRequest* to )
{
    CONVERT_ID( object );
    CONVERT( name );
    ConvertProperties( from, to );
}

// -----------------------------------------------------------------------------
// Name: ClientToMessenger::Convert
// Created: SBO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToMessenger::Convert( const MsgsClientToMessenger::MsgClientObjectDestructionRequest& from, sword::ClientObjectDestructionRequest* to )
{
    CONVERT_ID( object );
}

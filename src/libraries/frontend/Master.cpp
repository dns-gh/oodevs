// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Master.h"
#include "Publisher.h"
#include "Messages.h"
#include "frontend/Model.h"
#include "frontend/ExercisesModel.h"
#include "frontend/Exercise.h"
#include "frontend/DatasetsModel.h"
#include "frontend/TerrainsModel.h"
#include "frontend/PhysicalModelsModel.h"
#include "frontend/Exceptions.h"
#include "frontend/Profile.h"
#include "tools/MessageSender_ABC.h"
#include "clients_gui/Tools.h"

using namespace tools;
using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Master constructor
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
Master::Master( tools::MessageSender_ABC& messageService, const std::string& endpoint, Model& model, Profile& profile )
    : messageService_( messageService )
    , model_( model )
    , profile_( profile )
    , endpoint_( endpoint )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Master destructor
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
Master::~Master()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceive
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
void Master::OnReceive( const ASN1T_MsgsOutMaster& message )
{
    // $$$$ SBO 2007-01-25: process
    switch( message.msg.t )
    {
    case T_MsgsOutMaster_msg_msg_authentication_response:   OnReceiveMsgAuthenticationResponse( *message.msg.u.msg_authentication_response ); break;
    case T_MsgsOutMaster_msg_msg_dataset_creation:          OnReceiveMsgDatasetCreation( *message.msg.u.msg_dataset_creation ); break;
    case T_MsgsOutMaster_msg_msg_terrain_creation:          OnReceiveMsgTerrainCreation( *message.msg.u.msg_terrain_creation ); break;
    case T_MsgsOutMaster_msg_msg_physical_model_creation:   OnReceiveMsgPhysicalModelCreation( *message.msg.u.msg_physical_model_creation ); break;
    case T_MsgsOutMaster_msg_msg_exercise_creation:         OnReceiveMsgExerciseCreation( *message.msg.u.msg_exercise_creation ); break;
    case T_MsgsOutMaster_msg_msg_exercise_creation_ack:     OnReceiveMsgExerciseCreationRequestAck( *message.msg.u.msg_exercise_creation_ack ); break;
    case T_MsgsOutMaster_msg_msg_exercise_update:           OnReceiveMsgExerciseUpdate( *message.msg.u.msg_exercise_update ); break;
    case T_MsgsOutMaster_msg_msg_exercise_update_ack:       OnReceiveMsgExerciseUpdateRequestAck( *message.msg.u.msg_exercise_update_ack ); break;
    case T_MsgsOutMaster_msg_msg_exercise_destruction:      OnReceiveMsgExerciseDestruction( message.msg.u.msg_exercise_destruction ); break;
    case T_MsgsOutMaster_msg_msg_exercise_destruction_ack:  OnReceiveMsgExerciseDestructionRequestAck( *message.msg.u.msg_exercise_destruction_ack ); break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: Master::Send
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
void Master::Send( const ASN1T_MsgsInMaster& message )
{
    messageService_.Send( endpoint_, message );
}

// =============================================================================
// Message Handlers
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Master::OnReceiveMsgAuthenticationResponse
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Master::OnReceiveMsgAuthenticationResponse( const ASN1T_MsgAuthenticationResponse& message )
{
    profile_.DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceiveMsgDatasetCreation
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
void Master::OnReceiveMsgDatasetCreation( const ASN1T_MsgDatasetCreation& message )
{
    model_.datasets_.CreateDataset( message );
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceiveMsgTerrainCreation
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
void Master::OnReceiveMsgTerrainCreation( const ASN1T_MsgTerrainCreation& message )
{
    model_.terrains_.CreateTerrain( message );
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceiveMsgPhysicalModelCreation
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
void Master::OnReceiveMsgPhysicalModelCreation( const ASN1T_MsgPhysicalModelCreation& message )
{
    model_.physicalModels_.CreatePhysicalModel( message );
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceiveMsgExerciseCreation
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Master::OnReceiveMsgExerciseCreation( const ASN1T_MsgExerciseCreation& message )
{
    model_.exercises_.CreateExercise( message );
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceiveMsgExerciseUpdate
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Master::OnReceiveMsgExerciseUpdate( const ASN1T_MsgExerciseUpdate& message )
{
    model_.exercises_.Get( message.name ).DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceiveMsgExerciseDestruction
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Master::OnReceiveMsgExerciseDestruction( const ASN1T_MsgExerciseDestruction& message )
{
    model_.exercises_.DeleteExercise( message );
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceiveMsgExerciseCreationRequestAck
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Master::OnReceiveMsgExerciseCreationRequestAck( const ASN1T_MsgExerciseCreationRequestAck& message )
{
    if( message.return_code )
        throw UserRequestFailed( tools::translate( "Master", "Could not create exercise '%1': %2." ).arg( message.name ).arg( message.return_code ) );
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceiveMsgExerciseUpdateRequestAck
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Master::OnReceiveMsgExerciseUpdateRequestAck( const ASN1T_MsgExerciseUpdateRequestAck& message )
{
    if( message.return_code )
        throw UserRequestFailed( tools::translate( "Master", "Could not update exercise '%1': %2." ).arg( message.name ).arg( message.return_code ) );
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceiveMsgExerciseDestructionRequestAck
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Master::OnReceiveMsgExerciseDestructionRequestAck( const ASN1T_MsgExerciseDestructionRequestAck& message )
{
    if( message.return_code )
        throw UserRequestFailed( tools::translate( "Master", "Could not delete exercise '%1': %2." ).arg( message.name ).arg( message.return_code ) );
}

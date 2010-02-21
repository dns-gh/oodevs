// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "IntelligencesModel.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IntelligencesModel constructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
IntelligencesModel::IntelligencesModel( IntelligenceFactory& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel destructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
IntelligencesModel::~IntelligencesModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::Purge
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void IntelligencesModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::Create
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void IntelligencesModel::Create( const MsgsMessengerToClient::MsgIntelligenceCreation& message )
{
    std::auto_ptr< Intelligence_ABC > intelligence( factory_.Create( message ) );
    Register( intelligence->GetId(), *intelligence );
    intelligence.release();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::Delete
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void IntelligencesModel::Delete( const MsgsMessengerToClient::MsgIntelligenceDestruction& message )
{
    if( Intelligence_ABC* element = Find( message.oid() ) )
    {
        delete element;
        Remove( message.oid() );
    }
}

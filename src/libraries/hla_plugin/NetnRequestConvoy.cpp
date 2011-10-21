// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnRequestConvoy.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include "SerializationTools.h"
#include "NetnService.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnRequestConvoy constructor
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
NetnRequestConvoy::NetnRequestConvoy( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnRequestConvoy >& receiver )
    : pInteraction_( new ::hla::Interaction< interactions::NetnRequestConvoy >( receiver ) )
{
    RegisterNetnService( *pInteraction_ );
    pInteraction_->Register( "RequestTimeOut", ::hla::CreateParameter( &interactions::NetnRequestConvoy::requestTimeOut ) );
    pInteraction_->Register( "TransportData" , ::hla::CreateParameter( &interactions::NetnRequestConvoy::transportData ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_RequestService.NETN_RequestConvoy" ), *pInteraction_, true, true );
}

// -----------------------------------------------------------------------------
// Name: NetnRequestConvoy destructor
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
NetnRequestConvoy::~NetnRequestConvoy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRequestConvoy::Send
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
void NetnRequestConvoy::Send( const interactions::NetnRequestConvoy& interaction )
{
    pInteraction_->Send( interaction );
}

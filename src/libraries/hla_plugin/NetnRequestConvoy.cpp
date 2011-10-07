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
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnRequestConvoy constructor
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
NetnRequestConvoy::NetnRequestConvoy( Federate_ABC& federate )
    : pInteraction_( new ::hla::Interaction< interactions::NetnRequestConvoy >( *this ) )
{
    pInteraction_->Register( "ServiceID"     , ::hla::CreateParameter( &interactions::NetnRequestConvoy::serviceId ) );
    pInteraction_->Register( "Consumer"      , ::hla::CreateParameter( &interactions::NetnRequestConvoy::consumer ) );
    pInteraction_->Register( "Provider"      , ::hla::CreateParameter( &interactions::NetnRequestConvoy::provider ) );
    pInteraction_->Register( "ServiceType"   , ::hla::CreateParameter( &interactions::NetnRequestConvoy::serviceType ) );
    pInteraction_->Register( "RequestTimeOut", ::hla::CreateParameter( &interactions::NetnRequestConvoy::requestTimeOut ) );
    pInteraction_->Register( "TransportData" , ::hla::CreateParameter( &interactions::NetnRequestConvoy::transportData ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_RequestService.NETN_RequestConvoy" ), *pInteraction_, true, false );
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

// -----------------------------------------------------------------------------
// Name: NetnRequestConvoy::Receive
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
void NetnRequestConvoy::Receive( interactions::NetnRequestConvoy& /*interaction*/ )
{
    throw std::runtime_error( __FUNCTION__ " unimplemented function" );
}

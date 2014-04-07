// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NETNv1_InteractionBuilder.h"

#include "InteractionsNetn.h"
#include "Federate_ABC.h"
#include "Encodable.h"
#include "dispatcher/Logger_ABC.h"

#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

namespace
{

    template< typename T, typename R, typename Interaction >
    void Register( ::hla::Interaction< Interaction >& interaction, const std::string& name, const std::string& parameter, R T::* pMemberData, dispatcher::Logger_ABC& logger )
    {
        interaction.Register( parameter, *new Encodable< T, R >( pMemberData, name, parameter, logger ) );
    }
#   define REGISTER( parameter, member ) Register( interaction, name, parameter, member, logger_ )
#   define REGISTER_TEMPLATE( parameter, member, T ) Register< T >( interaction, name, parameter, member, logger_ )

    template< typename T >
    void RegisterNetnService( ::hla::Interaction< T >& interaction, const std::string& name, dispatcher::Logger_ABC& logger_ )
    {
        REGISTER_TEMPLATE( "ServiceID"  , &T::serviceId, T );
        REGISTER_TEMPLATE( "Consumer"   , &T::consumer, T );
        REGISTER_TEMPLATE( "Provider"   , &T::provider, T );
        REGISTER_TEMPLATE( "ServiceType", &T::serviceType, T );
    }
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
NETNv1_InteractionBuilder::NETNv1_InteractionBuilder( dispatcher::Logger_ABC& logger, Federate_ABC& federate )
    : logger_ ( logger )
    , federate_ ( federate )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder destructor
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
NETNv1_InteractionBuilder::~NETNv1_InteractionBuilder()
{
    // TODO Auto-generated destructor stub
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnRequestConvoy >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_RequestService.NETN_RequestConvoy";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "RequestTimeOut", &interactions::NetnRequestConvoy::requestTimeOut );
    REGISTER( "TransportData" , &interactions::NetnRequestConvoy::transportData );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnOfferConvoy >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_OfferService.NETN_OfferConvoy";
    REGISTER( "IsOffering"        , &interactions::NetnOfferConvoy::isOffering );
    REGISTER( "RequestTimeOut"    , &interactions::NetnOfferConvoy::requestTimeOut );
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "TransportData"     , &interactions::NetnOfferConvoy::transportData );
    REGISTER( "OfferType"         , &interactions::NetnOfferConvoy::offerType );
    REGISTER( "ListOfTransporters", &interactions::NetnOfferConvoy::listOfTransporters );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnAcceptOffer >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_AcceptOffer";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnRejectOfferConvoy >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_RejectOffer.NETN_RejectOfferConvoy";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "Reason", &interactions::NetnRejectOfferConvoy::reason );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnCancelConvoy >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_CancelService.NETN_CancelConvoy";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "Reason", &interactions::NetnCancelConvoy::reason );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnReadyToReceiveService >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ReadyToReceiveService";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceStarted >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ServiceStarted";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ConvoyEmbarkmentStatus";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "ListOfObjectEmbarked"   , &interactions::NetnConvoyEmbarkmentStatus::listOfObjectEmbarked );
    REGISTER( "TransportUnitIdentifier", &interactions::NetnConvoyEmbarkmentStatus::transportUnitIdentifier );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ConvoyDisembarkmentStatus";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "ListOfObjectDisembarked", &interactions::NetnConvoyDisembarkmentStatus::listOfObjectDisembarked );
    REGISTER( "TransportUnitIdentifier", &interactions::NetnConvoyDisembarkmentStatus::transportUnitIdentifier );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ConvoyDestroyedEntities";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "ListOfEmbarkedObjectDestroyed", &interactions::NetnConvoyDestroyedEntities::listOfEmbarkedObjectDestroyed );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceComplete >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ServiceComplete";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceReceived >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ServiceReceived";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::DoRegister
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::DoRegister( const std::string& name, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe ) const
{
    try
    {
        federate_.Register( ::hla::InteractionIdentifier( name ), interactionClass, publish, subscribe );
    }
    catch( const ::hla::HLAException& e )
    {
        logger_.LogError("Failed to register interaction "+name+ " : " + tools::GetExceptionMsg( e ) );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_OfferTransferModellingResponsibility >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_InitiateTransferModellingResponsibility >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_RequestTransferModellingResponsibility >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-07-02
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_CancelRequest >& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: NETNv1_InteractionBuilder::Build
// Created: AHC 2012-07-02
// -----------------------------------------------------------------------------
bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_TransferResult >& ) const
{
    return false;
}

bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_DisaggregationRequest >& ) const
{
    return false;
}

bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_DisaggregationResponse >& ) const
{
    return false;
}

bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_AggregationRequest >& ) const
{
    return false;
}

bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_AggregationResponse >& ) const
{
    return false;
}

bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_CancelRequest >& ) const
{
    return false;
}

bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_ActionComplete >& ) const
{
    return false;
}

bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_Trigger >& ) const
{
    return false;
}

bool NETNv1_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_TriggerResponse >& ) const
{
    return false;
}

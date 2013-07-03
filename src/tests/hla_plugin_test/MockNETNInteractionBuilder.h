// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockNETNInteractionBuilder_h
#define MockNETNInteractionBuilder_h

#include "hla_plugin/NETN_InteractionBuilder_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockInteractionSender
    @brief  Mock Interaction sender
*/
// Created: SLI 2011-09-23
// =============================================================================
MOCK_BASE_CLASS( MockNETNInteractionBuilder, NETN_InteractionBuilder_ABC )
{
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnRequestConvoy >& ), BuildNetnRequestConvoy );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnOfferConvoy >& interaction ), BuildNetnOfferConvoy );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnAcceptOffer >& interaction ), BuildNetnAcceptConvoy );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnRejectOfferConvoy >& interaction ), BuildNetnRejectOfferConvoy );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnCancelConvoy >& interaction ), BuildNetnCancelConvoy );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnReadyToReceiveService >& interaction ), BuildNetnReadyToReceiveService );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnServiceStarted >& interaction ), BuildNetnServiceStarted );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >& interaction ), BuildNetnConvoyEmbarkmentStatus );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >& interaction ), BuildNetnConvoyDisembarkmentStatus );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >& interaction ), BuildNetnConvoyDestroyedEntities );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnServiceComplete >& interaction ), BuildNetnServiceComplete );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::NetnServiceReceived >& interaction ), BuildNetnServiceReceived );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::TMR_OfferTransferModellingResponsibility >& interaction ), BuildTMR_OfferTransferModellingResponsibility );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::TMR_InitiateTransferModellingResponsibility >& interaction ), BuildTMR_InitiateTransferModellingResponsibility );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::TMR_RequestTransferModellingResponsibility >& interaction ), BuildTMR_RequestTransferModellingResponsibility );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::TMR_CancelRequest >& interaction ), BuildTMR_CancelRequest );
    MOCK_CONST_METHOD_EXT( Build, 1, bool ( ::hla::Interaction< interactions::TMR_TransferResult >& interaction ), BuildTMR_TransferResult );
};

}
}

#endif // MockNETNInteractionBuilder_h

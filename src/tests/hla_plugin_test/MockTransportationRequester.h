// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockTransportationRequester_h
#define plugins_hla_MockTransportationRequester_h

#include "hla_plugin/TransportationRequester_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockTransportationRequester
    @brief  Mock Transportation controller
*/
// Created: SLI 2011-10-07
// =============================================================================
MOCK_BASE_CLASS( MockTransportationRequester, TransportationRequester_ABC )
{
    MOCK_METHOD( Register, 1 )
    MOCK_METHOD( Unregister, 1 )
    MOCK_METHOD( OfferReceived, 4 )
    MOCK_METHOD( ServiceStarted, 1 )
    MOCK_METHOD( NotifyEmbarkationStatus, 3 )
    MOCK_METHOD( NotifyDisembarkationStatus, 3 )
    MOCK_METHOD( ServiceComplete, 2 )
};

}
}

#endif // plugins_hla_MockTransportationRequester_h

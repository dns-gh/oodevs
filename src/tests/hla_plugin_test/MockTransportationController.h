// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockTransportationController_h
#define plugins_hla_MockTransportationController_h

#include "hla_plugin/TransportationController_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockTransportationController
    @brief  Mock Transportation controller
*/
// Created: SLI 2011-10-07
// =============================================================================
MOCK_BASE_CLASS( MockTransportationController, TransportationController_ABC )
{
    MOCK_METHOD( Register, 1 )
    MOCK_METHOD( Unregister, 1 )
    MOCK_METHOD( OfferReceived, 4 )
    MOCK_METHOD( ServiceStarted, 1 )
    MOCK_METHOD( NotifyEmbarkationStatus, 3 )
};

}
}

#endif // plugins_hla_MockTransportationController_h

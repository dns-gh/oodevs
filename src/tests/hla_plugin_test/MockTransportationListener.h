// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockTransportationListener_h
#define plugins_hla_MockTransportationListener_h

#include "hla_plugin/TransportationListener_ABC.h"

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  MockTransportationListener
    @brief  Mock Transportation listener
*/
// Created: SLI 2011-10-06
// =============================================================================
MOCK_BASE_CLASS( MockTransportationListener, TransportationListener_ABC )
{
    MOCK_METHOD( ConvoyRequested, 6 )
};

}
}

#endif // plugins_hla_MockTransportationListener_h

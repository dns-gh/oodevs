// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_MockFederateAmbassadorFactory_h
#define dispatcher_MockFederateAmbassadorFactory_h

#include "hla_plugin/FederateAmbassadorFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockFederateAmbassadorFactory
    @brief  Mock Federate ambassador factory
*/
// Created: SLI 2011-01-11
// =============================================================================
MOCK_BASE_CLASS( MockFederateAmbassadorFactory, plugins::hla::FederateAmbassadorFactory_ABC )
{
    MOCK_METHOD( Create, 3 );

};

}
}

#endif // dispatcher_MockFederateAmbassadorFactory_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_MockRtiAmbassadorFactory_h
#define dispatcher_MockRtiAmbassadorFactory_h

#include "hla_plugin/RtiAmbassadorFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockRtiAmbassadorFactory
    @brief  Mock Rti ambassador factory
*/
// Created: SLI 2011-01-11
// =============================================================================
MOCK_BASE_CLASS( MockRtiAmbassadorFactory, plugins::hla::RtiAmbassadorFactory_ABC )
{
    MOCK_METHOD( CreateAmbassador, 4 );
    MOCK_METHOD( DeleteAmbassador, 1 );

};

}
}

#endif // dispatcher_MockRtiAmbassadorFactory_h

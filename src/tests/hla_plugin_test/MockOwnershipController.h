// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockOwnershipContoller_h
#define hla_plugin_MockOwnershipContoller_h

#include "hla_plugin/OwnershipController_ABC.h"
namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockOwnershipController
    @brief  MockOwnershipController
*/
// Created: AHC 2012-03-12
// =============================================================================
MOCK_BASE_CLASS( MockOwnershipController, OwnershipController_ABC )
{
    MOCK_METHOD( PerformDivestiture, 3 )
    MOCK_METHOD( PerformAcquisition, 3 )
};
}
}

#endif // hla_plugin_MockOwnershipContoller_h

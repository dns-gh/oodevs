// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockOwnershipStrategy_h
#define hla_plugin_MockOwnershipStrategy_h

#include "hla_plugin/OwnershipStrategy_ABC.h"
namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockOwnershipStrategy
    @brief  MockOwnershipStrategy
*/
// Created: AHC 2012-03-28
// =============================================================================
MOCK_BASE_CLASS( MockOwnershipStrategy, OwnershipStrategy_ABC )
{
    MOCK_METHOD( PerformAttributeOwnershipNegotiation, 0 )
    MOCK_METHOD( AcceptAcquisition, 2 )
    MOCK_METHOD( AcceptDivestiture, 2 )
};
}
}

#endif // hla_plugin_MockOwnershipStrategy_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockExtentResolver_h
#define plugins_hla_MockExtentResolver_h

#include "hla_plugin/ExtentResolver_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockExtentResolver
    @brief  Mock ExtentResolver
*/
// Created: SLI 2011-11-29
// =============================================================================
MOCK_BASE_CLASS( MockExtentResolver, ExtentResolver_ABC )
{
    MOCK_METHOD( IsInBoundaries, 1 )
};

}
}

#endif // plugins_hla_MockExtentResolver_h

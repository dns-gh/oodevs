// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockComponentTypes_h
#define plugins_hla_MockComponentTypes_h

#include "hla_plugin/ComponentTypes_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockComponentTypes
    @brief  Mock Component types
*/
// Created: SLI 2011-09-29
// =============================================================================
MOCK_BASE_CLASS( MockComponentTypes, ComponentTypes_ABC )
{
    MOCK_METHOD( Apply, 2 )
};

}
}

#endif // plugins_hla_MockComponentTypes_h

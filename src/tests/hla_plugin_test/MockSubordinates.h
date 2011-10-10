// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockSubordinates_h
#define plugins_hla_MockSubordinates_h

#include "hla_plugin/Subordinates_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockSubordinates
    @brief  Mock Subordinates
*/
// Created: SLI 2011-10-10
// =============================================================================
MOCK_BASE_CLASS( MockSubordinates, Subordinates_ABC )
{
    MOCK_METHOD( Apply, 2 )
};

}
}

#endif // plugins_hla_MockSubordinates_h

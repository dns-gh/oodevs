// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockSideChecker_h
#define plugins_hla_MockSideChecker_h

#include "hla_plugin/SideChecker_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockSideChecker
    @brief  Mock Side checker
*/
// Created: SLI 2011-10-11
// =============================================================================
MOCK_BASE_CLASS( MockSideChecker, SideChecker_ABC )
{
    MOCK_METHOD( AreSameSide, 2 )
};

}
}

#endif // plugins_hla_MockSideChecker_h

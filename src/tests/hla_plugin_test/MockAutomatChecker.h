// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockAutomatChecker_h
#define plugins_hla_MockAutomatChecker_h

#include "hla_plugin/AutomatChecker_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockAutomatChecker
    @brief  Mock Automat checker
*/
// Created: SLI 2011-10-12
// =============================================================================
MOCK_BASE_CLASS( MockAutomatChecker, AutomatChecker_ABC )
{
    MOCK_METHOD( IsAutomatDisengaged, 1 )
};

}
}

#endif // plugins_hla_MockAutomatChecker_h

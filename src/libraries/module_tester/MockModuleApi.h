// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MODULE_TESTER_MOCK_MODULE_API_H
#define MODULE_TESTER_MOCK_MODULE_API_H

#include <core/ModuleApi_ABC.h>

namespace core
{
// =============================================================================
/** @class  MockModuleApi
    @brief  Mock module API
*/
// Created: SLI 2011-12-12
// =============================================================================
MOCK_BASE_CLASS( MockModuleApi, ModuleApi_ABC )
{
    MOCK_METHOD( Configure, 2 )
};

}

#endif // MODULE_TESTER_MOCK_MODULE_API_H

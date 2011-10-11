// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockAutomatTypeResolver_h
#define plugins_hla_MockAutomatTypeResolver_h

#include "hla_plugin/AutomatTypeResolver_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockAutomatTypeResolver
    @brief  Mock Automat type resolver
*/
// Created: SLI 2011-10-11
// =============================================================================
MOCK_BASE_CLASS( MockAutomatTypeResolver, AutomatTypeResolver_ABC )
{
    MOCK_METHOD( Resolve, 1 )
};

}
}

#endif // plugins_hla_MockAutomatTypeResolver_h

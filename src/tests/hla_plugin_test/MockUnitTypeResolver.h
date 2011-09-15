// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockUnitTypeResolver_h
#define MockUnitTypeResolver_h

#include "hla_plugin/UnitTypeResolver_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockUnitTypeResolver
    @brief  Mock Unit type resolver
*/
// Created: SLI 2011-09-15
// =============================================================================
MOCK_BASE_CLASS( MockUnitTypeResolver, UnitTypeResolver_ABC )
{
    MOCK_METHOD( Resolve, 1 );
};

}
}

#endif // MockUnitTypeResolver_h

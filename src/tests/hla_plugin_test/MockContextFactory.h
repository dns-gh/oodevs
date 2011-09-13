// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockContextFactory_h
#define MockContextFactory_h

#include "hla_plugin/ContextFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockContextFactory
    @brief  Mock Context factory
*/
// Created: SLI 2011-09-09
// =============================================================================
MOCK_BASE_CLASS( MockContextFactory, ContextFactory_ABC )
{
    MOCK_METHOD( Create, 0 )
};

}
}

#endif // MockContextFactory_h

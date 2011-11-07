// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockMarkingFactory_h
#define plugins_hla_MockMarkingFactory_h

#include "hla_plugin/MarkingFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockMarkingFactory
    @brief  Mock marking factory
*/
// Created: SLI 2011-11-07
// =============================================================================
MOCK_BASE_CLASS( MockMarkingFactory, MarkingFactory_ABC )
{
    MOCK_METHOD( CreateAggregateMarking, 2 )
    MOCK_METHOD( CreateMarking, 2 )
};

}
}

#endif // plugins_hla_MockMarkingFactory_h

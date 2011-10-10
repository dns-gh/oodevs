// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockTransportedUnitsVisitor_h
#define plugins_hla_MockTransportedUnitsVisitor_h

#include "hla_plugin/TransportedUnitsVisitor_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockTransportedUnitsVisitor
    @brief  Mock Transported units visitor
*/
// Created: SLI 2011-10-10
// =============================================================================
MOCK_BASE_CLASS( MockTransportedUnitsVisitor, TransportedUnitsVisitor_ABC )
{
    MOCK_METHOD( Notify, 2 )
};

}
}

#endif // plugins_hla_MockTransportedUnitsVisitor_h

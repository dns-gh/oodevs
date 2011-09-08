// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockRemoteAgentSubject_h
#define MockRemoteAgentSubject_h

#include "hla_plugin/RemoteAgentSubject_ABC.h"

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  MockRemoteAgentSubject
    @brief  Mock remote agent subject
*/
// Created: VPR 2011-09-07
// =============================================================================
MOCK_BASE_CLASS( MockRemoteAgentSubject, RemoteAgentSubject_ABC )
{
    MOCK_METHOD( Register, 1 );
    MOCK_METHOD( Unregister, 1 );
};

}
}

#endif // MockRemoteAgentSubject_h

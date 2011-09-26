// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockRemoteAgentResolver_h
#define MockRemoteAgentResolver_h

#include "hla_plugin/RemoteAgentResolver_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockRemoteAgentResolver
    @brief  Mock Remote agent resolver
*/
// Created: SLI 2011-09-22
// =============================================================================
MOCK_BASE_CLASS( MockRemoteAgentResolver, RemoteAgentResolver_ABC )
{
    MOCK_METHOD_EXT( Resolve, 1, std::string( unsigned int ), ResolveIdentifier )
    MOCK_METHOD_EXT( Resolve, 1, unsigned int( const std::string& ), ResolveName )
};

}
}

#endif // MockRemoteAgentResolver_h

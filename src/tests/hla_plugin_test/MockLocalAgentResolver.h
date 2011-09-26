// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockLocalAgentResolver_h
#define MockLocalAgentResolver_h

#include "hla_plugin/LocalAgentResolver_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockLocalAgentResolver
    @brief  Mock Local agent resolver
*/
// Created: SLI 2011-09-26
// =============================================================================
MOCK_BASE_CLASS( MockLocalAgentResolver, LocalAgentResolver_ABC )
{
    MOCK_METHOD( Add, 2 )
    MOCK_METHOD_EXT( Resolve, 1, std::string( unsigned int ), ResolveIdentifier )
    MOCK_METHOD_EXT( Resolve, 1, unsigned int( const std::string& ), ResolveName )
};

}
}
#endif // MockLocalAgentResolver_h

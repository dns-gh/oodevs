// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockExtentResolver_h
#define plugins_hla_MockExtentResolver_h

#include "hla_plugin/EntityIdentifierResolver_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockEntityIdentifierResolver
    @brief  MockEntityIdentifierResolver
*/
// Created: SLI 2012-04-23
// =============================================================================
MOCK_BASE_CLASS( MockEntityIdentifierResolver, EntityIdentifierResolver_ABC )
{
    MOCK_METHOD( Create, 2 )
    MOCK_METHOD( Register, 2 )
    MOCK_METHOD( Unregister, 1, void( const rpr::EntityIdentifier&), UnregisterId )
    MOCK_METHOD( Unregister, 1, void( const std::string&), UnregisterName )
    MOCK_METHOD( Resolve, 1, const std::string&( const rpr::EntityIdentifier&), ResolveId )
    MOCK_METHOD( Resolve, 1, const rpr::EntityIdentifier&( const std::string&), ResolveName )
};

}
}

#endif // plugins_hla_MockExtentResolver_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockMunitionTypeResolver_h
#define MockMunitionTypeResolver_h

#include "hla_plugin/MunitionTypeResolver_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockMunitionTypeResolver
    @brief  Mock munition type resolver
*/
// Created: VPR 2011-09-26
// =============================================================================
MOCK_BASE_CLASS( MockMunitionTypeResolver, MunitionTypeResolver_ABC )
{
    MOCK_METHOD_EXT( Resolve, 1, rpr::EntityType( unsigned int ), ResolveIdentifier );
    MOCK_METHOD_EXT( Resolve, 1, unsigned int( const rpr::EntityType& ), ResolveType );
};

}
}

#endif // MockMunitionTypeResolver_h

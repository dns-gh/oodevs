// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockObjectIdentifierFactory_h
#define MockObjectIdentifierFactory_h

#include <hla/ObjectIdentifierFactory_ABC.h>
#include <hla/ObjectIdentifier.h>

namespace hla
{
// =============================================================================
/** @class  MockObjectIdentifierFactory
    @brief  Mock Object identifier factory
*/
// Created: SLI 2011-10-04
// =============================================================================
MOCK_BASE_CLASS( MockObjectIdentifierFactory, ::hla::ObjectIdentifierFactory_ABC )
{
    MOCK_METHOD( CreateIdentifier, 1 )
    MOCK_METHOD( ReleaseIdentifier, 1 )
};

}

#endif // MockObjectIdentifierFactory_h

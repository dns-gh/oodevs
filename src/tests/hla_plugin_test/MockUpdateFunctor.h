// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockUpdateFunctor_h
#define hla_plugin_MockUpdateFunctor_h

#include <hla/UpdateFunctor_ABC.h>

namespace hla
{
// =============================================================================
/** @class  MockUpdateFunctor
    @brief  Mock update functor
*/
// Created: SLI 2009-02-09
// =============================================================================
MOCK_BASE_CLASS( MockUpdateFunctor, UpdateFunctor_ABC )
{
    MOCK_METHOD( Begin, 1 )
    MOCK_METHOD( Visit, 2 )
    MOCK_METHOD( End, 0 )
};

}

#endif // hla_plugin_MockUpdateFunctor_h

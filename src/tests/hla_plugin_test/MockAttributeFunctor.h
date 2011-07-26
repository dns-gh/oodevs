// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockAttributeFunctor_h
#define hla_plugin_MockAttributeFunctor_h

#include <hla/AttributeFunctor_ABC.h>

namespace hla
{
// =============================================================================
/** @class  MockAttributeFunctor
    @brief  Mock attribute functor
*/
// Created: SLI 2011-07-26
// =============================================================================
MOCK_BASE_CLASS( MockAttributeFunctor, ::hla::AttributeFunctor_ABC )
{
    MOCK_METHOD( Visit, 1 )
};

}

#endif // hla_plugin_MockAttributeFunctor_h

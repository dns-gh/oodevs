// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockClassBuilder_h
#define hla_plugin_MockClassBuilder_h

#include "hla_plugin/ClassBuilder_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockClassBuilder
    @brief  Mock class builder
*/
// Created: SLI 2011-07-26
// =============================================================================
MOCK_BASE_CLASS( MockClassBuilder, ClassBuilder_ABC )
{
    MOCK_METHOD( Build, 4 )
};

}
}

#endif // hla_plugin_MockClassBuilder_h

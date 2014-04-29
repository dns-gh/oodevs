// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef hla_plugin_MockHlaClass_h
#define hla_plugin_MockHlaClass_h

#include "hla_plugin/HlaClass_ABC.h"
namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockHlaClass
    @brief  MockHlaClass
*/
// Created: AHC 2012-02-27
// =============================================================================
MOCK_BASE_CLASS( MockHlaClass, HlaClass_ABC )
{
    MOCK_METHOD( GetAttributes, 0 )
    MOCK_METHOD( Divest, 3 )
    MOCK_METHOD( Acquire, 3 )
};
}
}

#endif // hla_plugin_MockHlaClass_h

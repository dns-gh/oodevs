// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef hla_plugin_MockClassListener_h
#define hla_plugin_MockClassListener_h

#include "hla_plugin/ClassListener_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockClassListener
    @brief  MockClassListener
*/
// Created: AHC 2012-02-27
// =============================================================================
MOCK_BASE_CLASS( MockClassListener, ClassListener_ABC )
{
    MOCK_METHOD( RemoteCreated, 3 )
    MOCK_METHOD( RemoteDestroyed, 1 )
    MOCK_METHOD( LocalCreated, 3 )
    MOCK_METHOD( LocalDestroyed, 1 )
    MOCK_METHOD( Divested, 2 )
    MOCK_METHOD( Acquired, 2 )
};
}
}
#endif // hla_plugin_MockClassListener_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockMessageController_h
#define hla_plugin_MockMessageController_h

#include "tools/MessageController_ABC.h"

namespace tools
{
// =============================================================================
/** @class  MockMessageController
    @brief  Mock message controller
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Category >
MOCK_BASE_CLASS( MockMessageController, MessageController_ABC< Category > )
{
    MOCK_METHOD_TPL( Register, 1, void( MessageHandler_ABC< Category >& ) )
    MOCK_METHOD_TPL( Unregister, 1, void( MessageHandler_ABC< Category >& ) )
    MOCK_METHOD_TPL( Dispatch, 2, void( const Category&, int ) )
};

}

#endif // hla_plugin_MockMessageController_h

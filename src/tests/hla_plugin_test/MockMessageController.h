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

#include "hla_plugin/MessageController_ABC.h"

namespace plugins
{
namespace hla
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
    MOCK_METHOD_EXT_TPL( Register, 1, void( MessageHandler_ABC< Category >& ), Register )
    MOCK_METHOD_EXT_TPL( Unregister, 1, void( MessageHandler_ABC< Category >& ), Unregister )
    MOCK_METHOD_EXT_TPL( Dispatch, 1, void( const Category& ), Dispatch )
};

}
}

#endif // hla_plugin_MockMessageController_h

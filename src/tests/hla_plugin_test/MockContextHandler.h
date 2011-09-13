// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockContextHandler_h
#define MockContextHandler_h

#include "hla_plugin/ContextHandler_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockContextHandler
    @brief  Mock Context handler
*/
// Created: SLI 2011-09-13
// =============================================================================
template< typename MessageResponse >
MOCK_BASE_CLASS( MockContextHandler, ContextHandler_ABC< MessageResponse > )
{
    MOCK_METHOD_EXT_TPL( Register, 1, void( ResponseObserver_ABC< MessageResponse >& ), Register );
    MOCK_METHOD_EXT_TPL( Unregister, 1, void( ResponseObserver_ABC< MessageResponse >& ), Unregister );
    MOCK_METHOD_EXT_TPL( Send, 2, void( simulation::UnitMagicAction&, const std::string& ), Send );
};

}
}

#endif // MockContextHandler_h

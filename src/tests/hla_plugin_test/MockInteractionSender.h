// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockInteractionSender_h
#define MockInteractionSender_h

#include "hla_plugin/InteractionSender_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockInteractionSender
    @brief  Mock Interaction sender
*/
// Created: SLI 2011-09-23
// =============================================================================
template< typename Interaction >
MOCK_BASE_CLASS( MockInteractionSender, InteractionSender_ABC< Interaction > )
{
    MOCK_METHOD_EXT_TPL( Send, 1, void( const Interaction& ), Send )
};

}
}

#endif // MockInteractionSender_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_MockInteractionNotification_h
#define hla_MockInteractionNotification_h

#include <hla/InteractionNotification_ABC.h>

namespace hla
{
// =============================================================================
/** @class  MockInteractionNotification
    @brief  Mock interaction notification
*/
// Created: SLI 2011-09-26
// =============================================================================
template< typename T >
MOCK_BASE_CLASS( MockInteractionNotification, InteractionNotification_ABC< T > )
{
    MOCK_METHOD_EXT_TPL( Receive, 1, void( T& ), Receive )
};

}

#endif // hla_MockInteractionNotification_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_MockInteractionHandler_h
#define hla_MockInteractionHandler_h

#include <hla/HLA_lib.h>

namespace hla
{
// =============================================================================
/** @class  MockInteractionHandler
    @brief  Mock Interaction handler
*/
// Created: SLI 2011-06-24
// =============================================================================
MOCK_BASE_CLASS( MockInteractionHandler, hla::InteractionHandler_ABC )
{
    MOCK_METHOD( Visit, 2 );
    MOCK_METHOD( End, 0 );
    MOCK_METHOD( Flush, 0 );
};

}

#endif // hla_MockInteractionHandler_h
// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockPropagationManager_h
#define MockPropagationManager_h

#include "hla_plugin/PropagationManager_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockPropagationManager
    @brief  MockPropagationManager
*/
// Created: AHC 2014-01-27
// =============================================================================
MOCK_BASE_CLASS( MockPropagationManager, PropagationManager_ABC )
{
    MOCK_METHOD( saveDataFile, 7 );
};
}
}

#endif // MockPropagationmanager_h

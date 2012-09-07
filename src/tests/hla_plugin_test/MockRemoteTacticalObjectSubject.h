// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MockRemoteTacticalObjectSubjectt_h
#define plugins_hla_MockRemoteTacticalObjectSubjectt_h

#include "hla_plugin/RemoteTacticalObjectSubject_ABC.h"

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  MockRemoteTacticalObjectSubject
    @brief  MockRemoteTacticalObjectSubject
*/
// Created: AHC 2012-09-07
// =============================================================================
MOCK_BASE_CLASS( MockRemoteTacticalObjectSubject, RemoteTacticalObjectSubject_ABC )
{
    MOCK_METHOD( RegisterTactical, 1 );
    MOCK_METHOD( UnregisterTactical, 1 );
};

}
}

#endif // plugins_hla_MockRemoteTacticalObjectSubjectt_h

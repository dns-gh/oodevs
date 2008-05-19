// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AarMessages_h_
#define __AarMessages_h_

#include "ASN_Messages.h"
#include "game_asn/Aar.h"

namespace aar
{

GENERATE_ASN_MSG_SENDER( ClientToAar, IndicatorRequest, indicator_request )

}

#endif // __AarMessages_h_

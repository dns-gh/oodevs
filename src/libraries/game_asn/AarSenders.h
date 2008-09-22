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

#include "MessageSenders.h"
#include "Aar.h"
#include "tools/AsnControllers.h"

namespace aar
{
    struct Service {};

GENERATE_ASN_MSG_SENDER( ClientToAar, PlotRequest,     plot_request )

GENERATE_ASN_MSG_SENDER( AarToClient, AarInformation,  aar_information );
GENERATE_ASN_MSG_SENDER( AarToClient, PlotResult    ,  plot_result );
GENERATE_ASN_MSG_SENDER( AarToClient, Indicator     ,  indicator );

}

DECLARE_ASN_CONTROLLER( MsgsClientToAar )
DECLARE_ASN_CONTROLLER( MsgsAarToClient )

#endif // __AarMessages_h_

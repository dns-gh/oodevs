// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_AarSenders_h_
#define __protocol_AarSenders_h_

#include "Aar.h"
#include "Senders.h"

namespace aar
{
    struct Service {};

GENERATE_ASN_MSG_SENDER( ClientToAar, MsgsClientToAar, PlotRequest   , plot_request );

GENERATE_ASN_MSG_SENDER( AarToClient, MsgsAarToClient, AarInformation, aar_information );
GENERATE_ASN_MSG_SENDER( AarToClient, MsgsAarToClient, PlotResult    , plot_result );
GENERATE_ASN_MSG_SENDER( AarToClient, MsgsAarToClient, Indicator     , indicator );

}

#endif // __protocol_AarSenders_h_

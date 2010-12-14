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

    GENERATE_MESSAGE( sword::ClientToAar, sword, PlotRequest, plot_request );

    GENERATE_MESSAGE( sword::AarToClient, sword, AarInformation, aar_information );
    GENERATE_MESSAGE( sword::AarToClient, sword, PlotResult    , plot_result );
    GENERATE_MESSAGE( sword::AarToClient, sword, Indicator     , indicator );
}

#endif // __protocol_AarSenders_h_

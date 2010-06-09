// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_PluginSenders_h_
#define __protocol_PluginSenders_h_

#include "Plugin.h"
#include "Senders.h"

namespace plugin
{
    struct Service {};

GENERATE_ASN_MSG_SENDER( PluginToClient, MsgsPluginToClient, PluginTextMessage, plugin_text_message );
GENERATE_ASN_MSG_SENDER( PluginToClient, MsgsMessengerToClient, IntelligenceCreation, plugin_intelligence_creation );

}

#endif // __protocol_PluginSenders_h_

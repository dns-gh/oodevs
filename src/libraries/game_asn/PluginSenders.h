// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PluginMessages_h_
#define __PluginMessages_h_

#include "MessageSenders.h"
#include "Plugin.h"
#include "tools/AsnControllers.h"

namespace plugin
{
    struct Service {};

GENERATE_ASN_MSG_SENDER( PluginToClient, TextMessage, text_message );
GENERATE_ASN_MSG_SENDER( PluginToClient, IntelligenceCreation, intelligence_creation );

}

DECLARE_ASN_CONTROLLER( MsgsPluginToClient )

#endif // __XmliaMessages_h_

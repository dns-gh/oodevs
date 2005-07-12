//*****************************************************************************
//
// $Created: AML 03-04-24 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_AdminServiceMsg.h $
// $Author: Aml $
// $Modtime: 24/04/03 15:17 $
// $Revision: 2 $
// $Workfile: LAU_AdminServiceMsg.h $
//
//*****************************************************************************

#ifndef __LAU_AdminServiceMsg_h_
#define __LAU_AdminServiceMsg_h_

#include "LAU.h"

enum
{
        EVMSG_INSTALLED = 1000,
        EVMSG_REMOVED,
        EVMSG_NOTREMOVED,
        EVMSG_CTRLHANDLERNOTINSTALLED,
        EVMSG_FAILEDINIT,
        EVMSG_STARTED,
        EVMSG_BADREQUEST,
        EVMSG_DEBUG,
        EVMSG_STOPPED,
        EVMSG_USER,
        EVMSG_UNKNOWNMESSAGE
};

inline
const char* GetLibFromEvMsg( const int nEvMsg )
{
    static char aszLib[][50] =
    {
        "The service was installed.",
        "The service was removed.",
        "The service could not be removed.",
        "The control handler could not be installed.",
        "The initialization process failed.",
        "The service was started.",
        "The service received an unsupported request.",
        "Debug: %1",
        "The service was stopped.",
        "%1",
        "UnknownMessage"
    };

    if( nEvMsg < EVMSG_INSTALLED || nEvMsg > EVMSG_UNKNOWNMESSAGE )
    {
        return aszLib[EVMSG_UNKNOWNMESSAGE-EVMSG_INSTALLED];
    }

    return aszLib[nEvMsg-EVMSG_INSTALLED];
}



#endif // __LAU_AdminServiceMsg_h_
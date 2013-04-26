// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockTransferSender_h
#define hla_plugin_MockTransferSender_h

#include "hla_plugin/TransferSender_ABC.h"
namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockTransferSender
    @brief  MockTransferSender
*/
// Created: AHC 2012-03-01
// =============================================================================
MOCK_BASE_CLASS( MockTransferSender, TransferSender_ABC )
{
    MOCK_METHOD( RequestTransfer, 4 )
};
}
}

#endif // hla_plugin_MockTransferSender_h
// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DetectionEvents_h_
#define __DetectionEvents_h_

#include <wiseinterfacetypes.h>

namespace MsgsSimToClient
{
    class MsgObjectDetection;
    class MsgUnitDetection;
}

class Agent;
class CWISEDriver;
class Model;

// =============================================================================
/** @class  DetectionEvents
    @brief  DetectionEvents
*/
// Created: SEB 2010-10-27
// =============================================================================
class DetectionEvents
{
public:
    //! @name Operations
    //@{
    static void Trigger( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const MsgsSimToClient::MsgUnitDetection& message );
    static void Trigger( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const MsgsSimToClient::MsgObjectDetection& message );
    //@}
};

#endif // __DetectionEvents_h_

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

#pragma warning( push )
#pragma warning( disable: 4201 )
#include <wise/wiseinterfacetypes.h>
#pragma warning( pop )

namespace sword
{
    class ObjectDetection;
    class UnitDetection;
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
    static void Trigger( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const sword::UnitDetection& message );
    static void Trigger( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const sword::ObjectDetection& message );
    //@}
};

#endif // __DetectionEvents_h_

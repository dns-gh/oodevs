// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Task_h_
#define __Task_h_

#pragma warning( push )
#pragma warning( disable: 4201 )
#include <wise/wiseinterfacetypes.h>
#pragma warning( pop )

namespace sword
{
    class AutomatOrder;
    class UnitOrder;
}

class CWISEDriver;
class Model;
class SwordMessagePublisher_ABC;

// =============================================================================
/** @class  Task
    @brief  Task
*/
// Created: SEB 2010-10-15
// =============================================================================
class Task
{

public:
    //! @name Operations
    //@{
    static void Notify( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const sword::UnitOrder& message );
    static void Notify( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const sword::AutomatOrder& message );
    static void Issue( CWISEDriver& driver, const WISE_HANDLE& database, const Model& model, SwordMessagePublisher_ABC& publisher, const WISE_HANDLE& handle );
    //@}
};

#endif // __Task_h_

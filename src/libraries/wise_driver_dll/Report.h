// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Report_h_
#define __Report_h_

#pragma warning( push )
#pragma warning( disable: 4201 )
#include <wise/wiseinterfacetypes.h>
#pragma warning( pop )

namespace sword { class Report; }

class CWISEDriver;
class Model;

// =============================================================================
/** @class  Report
    @brief  Report
*/
// Created: SBO 2010-12-15
// =============================================================================
class Report
{

public:
    //! @name Operations
    //@{
    static void Trigger( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const sword::Report& message );
    //@}
};

#endif // __Report_h_

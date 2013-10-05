// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Trace_h_
#define __Trace_h_

#include "Report.h"

namespace kernel
{
    class Entity_ABC;
    class Time_ABC;
}

namespace sword
{
    class Trace;
}

// =============================================================================
/** @class  ReportFactory
    @brief  Report factory
*/
// Created: SBO 2006-12-07
// =============================================================================
class Trace : public Report
{
public:
    //! @name Constructor/Destructor
    //@{
             Trace( const kernel::Entity_ABC& agent, const kernel::Time_ABC& simulation, const sword::Trace& input );
    virtual ~Trace();
    //@}

private:
    //! @name Copy / Assignment
    //@{
    Trace( const Trace& );
    Trace& operator=( const Trace& );
    //@}
};

#endif // __Trace_h_

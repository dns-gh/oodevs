// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MT_PROFILER_GUARD_H
#define MT_PROFILER_GUARD_H

#include "MT_Profiler.h"

// =============================================================================
/** @class  MT_ProfilerGuard
    @brief  Guard helper to start profiling on construction and stop it on
            destruction
*/
// Created: MCO 2012-11-12
// =============================================================================
class MT_ProfilerGuard
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MT_ProfilerGuard( MT_Profiler& profiler )
        : profiler_( profiler )
    {
        profiler_.Start();
    }
    ~MT_ProfilerGuard()
    {
        profiler_.Stop();
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MT_ProfilerGuard( const MT_ProfilerGuard& );            //!< Copy constructor
    MT_ProfilerGuard& operator=( const MT_ProfilerGuard& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    MT_Profiler& profiler_;
    //@}
};

#endif // MT_PROFILER_GUARD_H

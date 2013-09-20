// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Profiling_h_
#define __Profiling_h_

#include "protocol/SimulationSenders.h"
#include <ctime>

namespace sword
{
    class ControlProfilingInformation;
    class ControlEndTick;
}

// =============================================================================
/** @class  Profiling
    @brief  Profiling
*/
// Created: AGE 2006-09-15
// =============================================================================
class Profiling
{
public:
    //! @name Constructors/Destructor
    //@{
             Profiling();
    virtual ~Profiling();
    //@}

    //! @name Operations
    //@{
    void Tick();
    void Clear();

    void Update( const sword::ControlProfilingInformation& message );
    void Update( const sword::ControlEndTick& message );
    float EffectiveSpeed() const;
    float ActualTickDuration() const;
    unsigned long GetMemory() const;
    unsigned long GetVirtualMemory() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Profiling( const Profiling& );            //!< Copy constructor
    Profiling& operator=( const Profiling& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< double >       T_History;
    typedef T_History::const_iterator CIT_History;
    //@}

private:
    //! @name Member data
    //@{
    clock_t lastCall_;
    T_History ticks_;
    double tickSum_;

    T_History perception_;
    T_History decision_;
    T_History action_;
    T_History total_;

    unsigned long memory_;
    unsigned long virtualMemory_;
    float actualTickDuration_;
    //@}
};

#endif // __Profiling_h_

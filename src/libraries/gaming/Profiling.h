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

#include <ctime>
struct ProfilingValuesMessage;

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

    void Update( const ProfilingValuesMessage& message );
    float EffectiveSpeed() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Profiling( const Profiling& );            //!< Copy constructor
    Profiling& operator=( const Profiling& ); //!< Assignement operator
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
    float tickSum_;

    T_History perception_;
    T_History decision_;
    T_History action_;
    T_History total_;
    //@}
};

#endif // __Profiling_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Profiler_h_
#define __Profiler_h_

#include <iostream>
#include "MT_Tools/MT_Profiler.h"

//#define PROFILE() Profiler profiler( __FUNCTION__ )
#define PROFILE()

// =============================================================================
/** @class  Profiler
    @brief  Profiler
*/
// Created: AGE 2006-04-26
// =============================================================================
class Profiler
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Profiler( const std::string& name = "", std::ostream& out = std::cout );
    virtual ~Profiler();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Profiler( const Profiler& );            //!< Copy constructor
    Profiler& operator=( const Profiler& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    bool InitializeProfiler();
    static MT_Profiler CreateProfiler();
    //@}

private:
    //! @name Member data
    //@{
    bool bDenis_;
    MT_Profiler profiler_;
    std::string name_;
    std::ostream* out_;
    //@}
};

#endif // __Profiler_h_

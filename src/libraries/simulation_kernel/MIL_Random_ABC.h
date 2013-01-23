// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef MIL_Random_ABC_h
#define MIL_Random_ABC_h

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  MIL_Random_ABC
    @brief  MIL_Random_ABC
*/
// Created: SLI 2013-01-23
// =============================================================================
class MIL_Random_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Random_ABC() {}
    virtual ~MIL_Random_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual double rand_oi( double min, double max, int ctxt = -1 ) const = 0;
    //@}
};

#endif // MIL_Random_ABC_h

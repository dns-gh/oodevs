// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Timeout_h_
#define __Timeout_h_
#include "boost/date_time/posix_time/posix_time.hpp" 

namespace mission_tester
{

// =============================================================================
/** @class  Timeout
    @brief  Timeout
*/
// Created: HBD 2011-04-21
// =============================================================================
class Timeout
{
public:
    //! @name Constructors/Destructor
    //@{
              explicit Timeout( unsigned int duration );
    virtual ~Timeout();
    //@}

    //! @name Operations
    //@{
    void Start();
    bool Expired() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Timeout( const Timeout& );            //!< Copy constructor
    Timeout& operator=( const Timeout& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int duration_;
    boost::posix_time::ptime start_;
   //@}
};
}

#endif // __Timeout_h_

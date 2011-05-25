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

#pragma warning( push )
#pragma warning( disable : 4244 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include <boost/noncopyable.hpp>

namespace mission_tester
{
// =============================================================================
/** @class  Timeout
    @brief  Timeout
*/
// Created: HBD 2011-04-21
// =============================================================================
class Timeout : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Timeout( unsigned int duration );
    virtual ~Timeout();
    //@}

    //! @name Operations
    //@{
    bool Expired() const;
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

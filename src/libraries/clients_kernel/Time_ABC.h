// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Time_ABC_h_
#define __Time_ABC_h_

#pragma warning( push, 0 )
#include <qdatetime.h>
#pragma warning( pop )
#include <boost/noncopyable.hpp>

namespace kernel {

// =============================================================================
/** @class  Time_ABC
    @brief  Time_ABC
*/
// Created: LDC 2010-03-17
// =============================================================================
class Time_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Time_ABC() {}
    virtual ~Time_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QDateTime GetDateTime() const = 0;
    virtual QDateTime GetInitialDateTime() const = 0;
    virtual unsigned int GetTickDuration() const = 0;
    //@}
};

} // end namespace Time

#endif // __Time_ABC_h_

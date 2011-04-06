// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Scheduler_h_
#define __Scheduler_h_

#include "Scheduler_ABC.h"
#include <boost/shared_ptr.hpp>
#pragma warning( push, 0 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace mission_tester
{
    class Criteria;
    class Filter_ABC;
    class Schedulable_ABC;
// =============================================================================
/** @class  Scheduler
    @brief  Scheduler
*/
// Created: PHC 2011-03-28
// =============================================================================
class Scheduler : public Scheduler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Scheduler( const std::string& criteria );
    explicit Scheduler( Filter_ABC& filter );
    virtual ~Scheduler();
    //@}

    //! @name Operations
    //@{
    virtual void Schedule( Schedulable_ABC& schedulable );
    virtual void Step( unsigned int delta );
    //@}

private:
    //! @name types
    //@{
    typedef std::vector< Schedulable_ABC* >  T_Schedulables;
    typedef T_Schedulables::const_iterator CIT_Schedulables;
    //@}

    //! @name Member data
    //@{
    std::auto_ptr< Criteria > criteria_;
    std::auto_ptr< Filter_ABC > filter_;
    T_Schedulables schedulables_;
    CIT_Schedulables next_;
    boost::posix_time::ptime last_;
    //@}
};
}

#endif // __Scheduler_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Schedule_h
#define __MIL_Schedule_h

#include "MIL_Schedule_ABC.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
#include <vector>

class MIL_LivingArea_ABC;

namespace bpt = boost::posix_time;

class bpt::ptime;

// =============================================================================
/** @class  MIL_Schedule
    @brief  MIL schedule
*/
// Created: LGY 2011-01-19
// =============================================================================
class MIL_Schedule : public MIL_Schedule_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_Schedule( MIL_LivingArea_ABC& livingArea );
    virtual ~MIL_Schedule();
    //@}

    //! @name Operations
    //@{
    virtual void Configure( xml::xistream& xis );
    virtual void Update( unsigned int date, unsigned int duration );
    virtual bool IsMoving() const;
    virtual void AddEvent( const std::string& motivation, int transfertTimeInSecond = 0 );
    virtual void RestartLastEvent();
    //@}

private:
    //! @name Types
    //@{
    struct Event
    {
        unsigned int day_;
        boost::posix_time::time_duration from_;
        boost::posix_time::time_duration to_;
        std::string motivation_;
        int occurence_;
        int transfertTime_;
    };

    typedef std::vector< Event > T_Events;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadEvent( xml::xistream& xis );
    bool Check( Event& event, const bpt::ptime& pdate, unsigned int duration );
    void Initialize( unsigned int date );
    //@}

private:
    //! @name Member data
    //@{
    MIL_LivingArea_ABC& livingArea_;
    T_Events events_;
    int transferTime_;
    bool isMoving_;
    bool initialized_;
    boost::optional< Event > optionalEvent_;
    boost::optional< std::string > currentMotivation_;
    //@}
};

#endif // __MIL_Schedule_h

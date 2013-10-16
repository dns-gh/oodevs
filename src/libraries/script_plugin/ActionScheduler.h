// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ActionScheduler_h_
#define __ActionScheduler_h_

#include <tools/ElementObserver_ABC.h>
#include <boost/noncopyable.hpp>
#include <vector>

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace kernel
{
    class XmlAdapter;
}

namespace sword
{
    class ClientToSim;
}

namespace plugins
{
namespace script
{
namespace events
{
    struct SimulationTimeChanged;
}
}
}

namespace boost
{
namespace posix_time
{
    class ptime;
}
}

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace script
{
// =============================================================================
/** @class  ActionScheduler
    @brief  ActionScheduler
*/
// Created: SBO 2011-03-29
// =============================================================================
class ActionScheduler : public boost::noncopyable
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< events::SimulationTimeChanged >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionScheduler( xml::xistream& xis,
                              const kernel::XmlAdapter& adapter,
                              dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~ActionScheduler();
    //@}

    //! @name Typedef helpers
    //@{
    struct T_Action;
    typedef std::vector< T_Action > T_Actions;
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const events::SimulationTimeChanged& event );
    //@}

private:
    //! @name Helpers
    //@{
    void Tick( const boost::posix_time::ptime& time );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    const T_Actions                      actions_;
    T_Actions::const_iterator            cursor_;
    //@}
};
}
}

#endif // __ActionScheduler_h_

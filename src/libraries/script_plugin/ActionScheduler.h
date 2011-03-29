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

#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>

class Publisher_ABC;
class QDateTime;

namespace actions
{
    class Action_ABC;
    class ActionFactory_ABC;
    class ActionsModel;
}

namespace kernel
{
    class Controller;
}

namespace tools
{
    class ExerciseConfig;
}

namespace plugins
{
namespace script
{
    namespace events
    {
        struct SimulationTimeChanged;
    }
// =============================================================================
/** @class  ActionScheduler
    @brief  ActionScheduler
*/
// Created: SBO 2011-03-29
// =============================================================================
class ActionScheduler : private boost::noncopyable
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< events::SimulationTimeChanged >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionScheduler( const tools::ExerciseConfig& config, const std::string& filename, kernel::Controller& controller, actions::ActionFactory_ABC& factory, Publisher_ABC& publisher );
    virtual ~ActionScheduler();
    //@}

    //! @name Operations
    //@{
    void Tick( const QDateTime& time );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const events::SimulationTimeChanged& event );
    //@}

    //! @name Types
    //@{
    struct sTimingComparator
    {
        bool operator()( const actions::Action_ABC* lhs, const actions::Action_ABC* rhs ) const;
    };
    typedef std::set< const actions::Action_ABC*, sTimingComparator > T_Actions;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    std::auto_ptr< actions::ActionsModel > model_;
    T_Actions actions_;
    T_Actions::const_iterator cursor_;
    //@}
};
}
}

#endif // __ActionScheduler_h_

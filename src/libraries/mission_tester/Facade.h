// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Facade_h_
#define __Facade_h_

#include "Listener_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace kernel
{
    class StaticModel;
}

namespace tools
{
    class ExerciseConfig;
}
namespace mission_tester
{
    class Config;
    class Listener_ABC;
    class MainFactory_ABC;
    class Scheduler_ABC;
    class SchedulerFactory;

// =============================================================================
/** @class  Facade
    @brief  Facade
*/
// Created: PHC 2011-04-05
// =============================================================================
class Facade : public Listener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Facade( const tools::ExerciseConfig& config, const MainFactory_ABC& factory );
    virtual ~Facade();
    //@}

    //! @name Operations
    //@{
    void AddListener( boost::shared_ptr< Listener_ABC > listener );
    void Run();
    //@}

    //! @name Listener
    //@{
    virtual void MissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void FragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void MissionAcknowledged( const sword::Tasker& tasker );
    virtual void FragOrderAcknowledged( const sword::Tasker& tasker );
    virtual void MissionErrorAck( const sword::Tasker& tasker );
    virtual void FragOrderErrorAck( const sword::Tasker& tasker );
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void AuthenticationSucceeded( const std::string& profile );
    virtual void ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const kernel::OrderParameter& parameter );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< Listener_ABC > > T_Listeners;
    //@}

private:
    //! @name Member data
    //@{
    const MainFactory_ABC& mainFactory_;
    std::auto_ptr< kernel::StaticModel > staticModel_;
    std::auto_ptr< SchedulerFactory > factory_;
    T_Listeners listeners_;
    boost::shared_ptr< Scheduler_ABC > scheduler_;
    //@}
};

}

#endif // __Facade_h_

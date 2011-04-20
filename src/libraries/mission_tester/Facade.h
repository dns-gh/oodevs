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

namespace kernel
{
    class StaticModel;
}

namespace mission_tester
{
    class Config;
    class Listener_ABC;
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
    explicit Facade( const Config& config );
    virtual ~Facade();
    //@}

    //! @name Operations
    //@{
    void AddListener( boost::shared_ptr< Listener_ABC > listener );
    void Run();
    //@}

    //! @name Listener
    //@{
    virtual void MissionCreated( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const;
    virtual void MissionAcknowledged( const sword::Tasker& tasker ) const;
    virtual void MissionErrorAck( const sword::Tasker& tasker ) const;
    virtual void ConnectionSucceeded( const std::string& endpoint ) const;
    virtual void AuthenticationSucceeded( const std::string& profile ) const;
    virtual void ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::MissionType& mission, const kernel::OrderParameter& parameter ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< Listener_ABC > > T_Listeners;
    //@}

private:
    //! @name Member Data
    //@{
    std::auto_ptr< kernel::StaticModel > staticModel_;
    std::auto_ptr< SchedulerFactory > factory_;
    T_Listeners listeners_;
    //@}
};
}

#endif // __Facade_h_

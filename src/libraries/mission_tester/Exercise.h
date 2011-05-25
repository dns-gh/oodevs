// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef mission_tester_exercise_h
#define mission_tester_exercise_h

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class SwordMessagePublisher_ABC;
class Publisher_ABC;

namespace actions
{
    class Action_ABC;
    class ActionFactory;
    class ParameterFactory_ABC;
    class ActionParameterFactory;
}

namespace kernel
{
    class AgentKnowledgeConverter_ABC;
    class Controller;
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
    class ObjectKnowledgeConverter_ABC;
    class OrderParameter;
    class StaticModel;
    class Time_ABC;
    class Entity_ABC;
    class FragOrderType;
    class MissionType;
    class OrderType;
}

namespace xml
{
    class xistream;
}

namespace mission_tester
{
    class Client;
    class Listener_ABC;
    class ParameterFactory;

// =============================================================================
/** @class  Exercise
    @brief  Exercise
*/
// Created: PHC 2011-03-28
// =============================================================================
class Exercise : private boost::noncopyable
{
public:
    //! @name Constructors / Destructors
    //@{
             Exercise( kernel::EntityResolver_ABC& entities, const kernel::StaticModel& staticModel, Publisher_ABC& publisher, xml::xistream& xis );
    virtual ~Exercise();
    //@}

    //! @name operations
    //@{
    bool CreateFragOrder( const kernel::Entity_ABC& target, const kernel::FragOrderType& mission );
    bool CreateMission( const kernel::Entity_ABC& target, const kernel::MissionType& mission );
    bool CreateOrder( const kernel::Entity_ABC& target, const kernel::OrderType& order, actions::Action_ABC* action ) const;
    void Register( Listener_ABC& listener );
    //@}

private:
    //! @name Helpers
    //@{
    void NotifyInvalidParameter( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const kernel::OrderParameter& parameter ) const;
    void NotifyMissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    void NotifyFragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    //@}

private:
    //! @name members data
    //@{
    Publisher_ABC& publisher_;
    std::vector< Listener_ABC* > listeners_;
    std::auto_ptr< kernel::Controller > controller_;
    std::auto_ptr< kernel::Time_ABC > time_;
    std::auto_ptr< kernel::AgentKnowledgeConverter_ABC > agentKnowledgeConverter_;
    std::auto_ptr< kernel::ObjectKnowledgeConverter_ABC > objectKnowledgeConverter_;
    std::auto_ptr< actions::ActionParameterFactory > parameterFactory_;
    std::auto_ptr< actions::ActionFactory > actionFactory_;
    std::auto_ptr< ParameterFactory > factory_;
    //@}
};

}

#endif // mission_tester_exercise_h
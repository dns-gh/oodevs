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
#include <memory>
#include <string>

class SwordMessagePublisher_ABC;

namespace actions
{
    class ActionFactory;
    class ParameterFactory_ABC;
}

namespace kernel
{
    class AgentKnowledgeConverter_ABC;
    class Controller;
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
    class ObjectKnowledgeConverter_ABC;
    class StaticModel;
    class Time_ABC;
}

namespace mission_tester
{
    class Client;

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
             Exercise( kernel::EntityResolver_ABC& entities, const kernel::StaticModel& staticModel );
    virtual ~Exercise();
    //@}

    //! @name operations
    //@{
    void SendOrder( const std::string& message, Client& client ) const;
    //@}

private:
    //! @name members data
    //@{
    std::auto_ptr< kernel::Controller > controller_;
    std::auto_ptr< kernel::Time_ABC > time_;
    std::auto_ptr< kernel::AgentKnowledgeConverter_ABC > agentKnowledgeConverter_;
    std::auto_ptr< kernel::ObjectKnowledgeConverter_ABC > objectKnowledgeConverter_;
    std::auto_ptr< actions::ParameterFactory_ABC > parameters_;
    std::auto_ptr< actions::ActionFactory > factory_;
    //@}
};

}

#endif // mission_tester_exercise_h
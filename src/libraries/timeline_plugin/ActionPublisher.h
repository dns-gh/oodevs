// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __plugins_timeline_ActionPublisher_h_
#define __plugins_timeline_ActionPublisher_h_


namespace actions
{
    class ActionFactory_ABC;
    class ParameterFactory_ABC;
}

namespace kernel
{
    class StaticModel;
    class CoordinateConverter_ABC;
    class Controller;
    class EntityResolver_ABC;
    class AgentKnowledgeConverter_ABC;
    class ObjectKnowledgeConverter_ABC;
    class UrbanKnowledgeConverter_ABC;
    class Time_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

namespace dispatcher
{
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace xml
{
    class xistream;
}

// game_asn
class Publisher_ABC;

namespace plugins
{
namespace timeline
{

// =============================================================================
/** @class  ActionPublisher
    @brief  ActionPublisher
*/
// Created: JCR 2010-09-07
// =============================================================================
class ActionPublisher
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionPublisher( kernel::Controller& controller, const tools::ExerciseConfig& config, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& sim );
    virtual ~ActionPublisher();
    //@}

    //! @name Operations
    //@{
    void Execute( xml::xistream& xis );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionPublisher( const ActionPublisher& );            //!< Copy constructor
    ActionPublisher& operator=( const ActionPublisher& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Publisher_ABC > publisher_;
    std::auto_ptr< kernel::EntityResolver_ABC > entities_;
    std::auto_ptr< kernel::AgentKnowledgeConverter_ABC > agentsKnowledges_;
    std::auto_ptr< kernel::ObjectKnowledgeConverter_ABC > objectsKnowledges_;
    std::auto_ptr< kernel::UrbanKnowledgeConverter_ABC > urbansKnowledges_;
    std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    std::auto_ptr< kernel::Time_ABC > time_;
    std::auto_ptr< actions::ParameterFactory_ABC > parameters_;
    std::auto_ptr< actions::ActionFactory_ABC > factory_;
    //@}
};

}
}

#endif // __ActionPublisher_h_

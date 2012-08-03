// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Actions_h_
#define __Actions_h_

#include "dispatcher/Registrable_ABC.h"
#include <boost/shared_ptr.hpp>

namespace actions
{
    class ActionFactory_ABC;
    class ParameterFactory_ABC;
}

namespace kernel
{
    class AgentKnowledgeConverter_ABC;
    class CoordinateConverter_ABC;
    class Controller;
    class EntityResolver_ABC;
    class ObjectKnowledgeConverter_ABC;
    class StaticModel;
    class Time_ABC;
}

namespace xml
{
    class xistream;
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

namespace plugins
{
namespace script
{
    class ActionScheduler;
// =============================================================================
/** @class  Actions
    @brief  Actions
*/
// Created: AGE 2008-07-16
// =============================================================================
class Actions : public dispatcher::Registrable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Actions( kernel::Controller& controller, const tools::ExerciseConfig& config, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& sim );
    virtual ~Actions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::brain::Brain& brain );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Actions( const Actions& );            //!< Copy constructor
    Actions& operator=( const Actions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void IssueOrder( const std::string& name );
    void IssueOrderFromFile( const std::string& name, const std::string& filename );
    void IssueXmlOrder( const std::string& name );
    void StartScheduler( const std::string& filename );
    void StopScheduler();
    void Read( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    struct Publisher;
    typedef std::vector< boost::shared_ptr< ActionScheduler > > T_Schedulers;
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    kernel::Controller& controller_;
    std::auto_ptr< kernel::EntityResolver_ABC > entities_;
    std::auto_ptr< Publisher > publisher_;
    std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    std::auto_ptr< kernel::Time_ABC > time_;
    std::auto_ptr< kernel::AgentKnowledgeConverter_ABC > agentsKnowledges_;
    std::auto_ptr< kernel::ObjectKnowledgeConverter_ABC > objectsKnowledges_;
    std::auto_ptr< actions::ParameterFactory_ABC > parameters_;
    std::auto_ptr< actions::ActionFactory_ABC > factory_;
    T_Schedulers schedulers_;
    //@}
};

}
}

#endif // __Actions_h_

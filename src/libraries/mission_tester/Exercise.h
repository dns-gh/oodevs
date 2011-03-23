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

#include "clients_kernel/CoordinateConverter_ABC.h"
#include <boost/noncopyable.hpp>

namespace actions
{
    class ActionFactory;
    class ParameterFactory_ABC;
}

namespace kernel
{
    class Controller;
    class StaticModel;
    class EntityResolver_ABC;
    class Time_ABC;
    class AgentKnowledgeConverter_ABC;
    class ObjectKnowledgeConverter_ABC;
}

namespace tools
{
    class RealFileLoaderObserver_ABC;
    class ExerciseConfig;
}

namespace mission_tester
{

class Exercise : private boost::noncopyable
{
public:
    //! @name Constructors / Destructors
    //@{
             Exercise( const std::string& exercise, const std::string& rootDir );
    virtual ~Exercise();
    //@}

private:
    //! @name members data
    //@{
    std::auto_ptr< tools::RealFileLoaderObserver_ABC > observer_;
    std::auto_ptr< tools::ExerciseConfig > config_;
    std::auto_ptr< kernel::StaticModel > staticModel_;
    std::auto_ptr< kernel::Controller > controller_;
    std::auto_ptr< kernel::EntityResolver_ABC > entities_;
    std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    std::auto_ptr< kernel::Time_ABC > time_;
    std::auto_ptr< kernel::AgentKnowledgeConverter_ABC > agentKnowledgeConverter_;
    std::auto_ptr< kernel::ObjectKnowledgeConverter_ABC > objectKnowledgeConverter_;
    std::auto_ptr< actions::ParameterFactory_ABC > parameters_;
    std::auto_ptr< actions::ActionFactory > factory_;
    //@}
};
}

#endif // mission_tester_exercise_h
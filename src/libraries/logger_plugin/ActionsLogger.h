// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ActionsLogger_h_
#define __ActionsLogger_h_

#include <memory>
#include <string>

namespace actions
{
    class ActionFactory_ABC;
    class ActionsModel;
    class ParameterFactory_ABC;
}

namespace Common
{
    class MsgAutomatOrder;
    class MsgPopulationOrder;
    class MsgUnitOrder;
}

namespace MsgsClientToSim
{
    class MsgFragOrder;
}

namespace dispatcher
{
    class Model;
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

namespace tools
{
    class SessionConfig;
}

class Publisher_ABC;

namespace plugins
{
namespace logger
{
// =============================================================================
/** @class  ActionsLogger
    @brief  ActionsLogger
*/
// Created: SBO 2010-05-11
// =============================================================================
class ActionsLogger
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionsLogger( const tools::SessionConfig& config, const dispatcher::Model& model, const kernel::StaticModel& staticModel, const kernel::Time_ABC& simulation );
    virtual ~ActionsLogger();
    //@}

    //! @name Operations
    //@{
    void Log( const Common::MsgUnitOrder& message );
    void Log( const Common::MsgAutomatOrder& message );
    void Log( const Common::MsgPopulationOrder& message );
    void Log( const MsgsClientToSim::MsgFragOrder& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsLogger( const ActionsLogger& );            //!< Copy constructor
    ActionsLogger& operator=( const ActionsLogger& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void LogAction( const T& message );
    void Commit() const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::SessionConfig& config_;
    std::auto_ptr< kernel::EntityResolver_ABC > entities_;
    std::auto_ptr< kernel::Controller > controller_;
    std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    std::auto_ptr< Publisher_ABC > publisher_;
    std::auto_ptr< kernel::AgentKnowledgeConverter_ABC > agentsKnowledges_;
    std::auto_ptr< kernel::ObjectKnowledgeConverter_ABC > objectsKnowledges_;
    std::auto_ptr< actions::ParameterFactory_ABC > parameters_;
    std::auto_ptr< actions::ActionFactory_ABC > factory_;
    std::auto_ptr< actions::ActionsModel > actions_;
    //@}
};

}
}

#endif // __ActionsLogger_h_

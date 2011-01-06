// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_ActionDispatcher_h_
#define __crossbow_ActionDispatcher_h_

#include "ActionSerializer_ABC.h"

namespace dispatcher
{
    class Config;
    class Model_ABC;
}

namespace kernel
{
    class EntityResolver_ABC;
    class Time_ABC;
    class AgentKnowledgeConverter_ABC;
    class ObjectKnowledgeConverter_ABC;
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
    class StaticModel;
    class Controller;
    class OrderParameter;
    class Entity_ABC;
}

namespace actions
{
    class ParameterFactory_ABC;
    class ActionFactory_ABC;
    class Action_ABC;
    class Parameter_ABC;
}

namespace tools
{
    class Observer_ABC;
}

namespace plugins
{
namespace crossbow
{
    class Workspace_ABC;
    class ActionParameterSerializer;
    class Row_ABC;

// =============================================================================
/** @class  ActionDispatcher
    @brief  ActionDispatcher
*/
// Created: JCR 2010-12-07
// =============================================================================
class ActionSerializer : public ActionSerializer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionSerializer( const dispatcher::Config& config, dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, Workspace_ABC& workspace );
    virtual ~ActionSerializer();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeMission( const dispatcher::Agent_ABC& agent, const Row_ABC& row, std::auto_ptr< actions::Action_ABC >& action );
    virtual void SerializeMission( const dispatcher::Automat_ABC& automat, const Row_ABC& row, std::auto_ptr< actions::Action_ABC >& action );
    virtual void SerializeFragOrder( const kernel::Entity_ABC& target, const Row_ABC& row, std::auto_ptr< actions::Action_ABC >& action );
    virtual void SerializeObjectMagicCreation( const Row_ABC& row, std::auto_ptr< actions::Action_ABC >& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionSerializer( const ActionSerializer& );            //!< Copy constructor
    ActionSerializer& operator=( const ActionSerializer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void                    SetParameters( unsigned long actionId, const kernel::Entity_ABC* entity, actions::Action_ABC& mission );
    actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& type, const std::string& value, const kernel::Entity_ABC* entity ) const;
    //@}

private:

    //! @name Reference member data
    //@{
    Workspace_ABC& workspace_;
    
    //@}

    //! @name Member data
    //@{
    std::auto_ptr< kernel::Controller > controller_;
    std::auto_ptr< tools::Observer_ABC > observer_;
    std::auto_ptr< kernel::EntityResolver_ABC > entities_;
    std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    std::auto_ptr< kernel::Time_ABC > time_;
    std::auto_ptr< kernel::AgentKnowledgeConverter_ABC > agentsKnowledges_;
    std::auto_ptr< kernel::ObjectKnowledgeConverter_ABC > objectsKnowledges_;
    std::auto_ptr< actions::ParameterFactory_ABC > nullParameters_;
    std::auto_ptr< actions::ActionFactory_ABC > factory_;
    std::auto_ptr< ActionParameterSerializer > serializer_;
    //@}
};

}
}

#endif // __crossbow_ActionDispatcher_h_

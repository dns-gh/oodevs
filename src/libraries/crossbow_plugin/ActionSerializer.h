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
#include "tools/Resolver_ABC.h"

namespace xml
{
    class xistream;
}

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
             ActionSerializer( const kernel::CoordinateConverter_ABC& converter, dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel );
    virtual ~ActionSerializer();
    //@}

    //! @name
    //@{
    virtual void SerializeCreation( const Row_ABC& row, std::auto_ptr< actions::Action_ABC >& action ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionSerializer( const ActionSerializer& );            //!< Copy constructor
    ActionSerializer& operator=( const ActionSerializer& ); //!< Assignment operator
    //@}

    void ReadAction(  xml::xistream& xis, std::auto_ptr< actions::Action_ABC >& action ) const;

private:
    //! @name Member data
    //@{
    std::auto_ptr< kernel::Controller > controller_;
    std::auto_ptr< tools::Observer_ABC > observer_;
    std::auto_ptr< kernel::EntityResolver_ABC > entities_;
    const kernel::CoordinateConverter_ABC& converter_;
    std::auto_ptr< kernel::Time_ABC > time_;
    std::auto_ptr< kernel::AgentKnowledgeConverter_ABC > agentsKnowledges_;
    std::auto_ptr< kernel::ObjectKnowledgeConverter_ABC > objectsKnowledges_;
    std::auto_ptr< actions::ParameterFactory_ABC > parameters_;
    std::auto_ptr< actions::ActionFactory_ABC > factory_;
    //@}
};

}
}

#endif // __crossbow_ActionDispatcher_h_

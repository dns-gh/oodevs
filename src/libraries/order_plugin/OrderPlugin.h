// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __OrderPlugin_h_
#define __OrderPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "dispatcher/OrderResolver_ABC.h"
#include <memory>

namespace actions
{
    class ActionFactory_ABC;
    class ParameterFactory_ABC;
}

namespace dispatcher
{
    class Config;
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace kernel
{
    class StaticModel;
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
    class AgentKnowledgeConverter_ABC;
    class ObjectKnowledgeConverter_ABC;
    class Controller;
    class Time_ABC;
}

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace order
{
// =============================================================================
/** @class  OrderPlugin
    @brief  OrderPlugin
*/
// Created: LGY 2011-09-02
// =============================================================================
class OrderPlugin : public dispatcher::Plugin_ABC
                  , public dispatcher::OrderResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             OrderPlugin( const dispatcher::Config& config, const kernel::StaticModel& staticModel,
                          const dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& simulation );
    virtual ~OrderPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Resolve( const sword::ClientToSim& message );

    virtual void Receive( const sword::SimToClient& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderPlugin( const OrderPlugin& );            //!< Copy constructor
    OrderPlugin& operator=( const OrderPlugin& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    struct Publisher;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadAction( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< kernel::CoordinateConverter_ABC > pConverter_;
    std::auto_ptr< kernel::EntityResolver_ABC > pEntities_;
    std::auto_ptr< kernel::AgentKnowledgeConverter_ABC > pAgentsKnowledges_;
    std::auto_ptr< kernel::ObjectKnowledgeConverter_ABC > pObjectsKnowledges_;
    std::auto_ptr< kernel::Controller > pController_;
    std::auto_ptr< actions::ParameterFactory_ABC > pParameters_;
    std::auto_ptr< kernel::Time_ABC > pTime_;
    std::auto_ptr< actions::ActionFactory_ABC > pFactory_;
    std::auto_ptr< Publisher > pPublisher_;
    //@}
};
}
}

#endif // __OrderPlugin_h_

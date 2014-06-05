// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef PLUGINS_ORDER_PLUGIN_H__
#define PLUGINS_ORDER_PLUGIN_H__

#include "dispatcher/Plugin_ABC.h"
#include "dispatcher/OrderResolver_ABC.h"
#include <memory>

namespace dispatcher
{
    class Config;
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
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
             OrderPlugin( const dispatcher::Config& config,
                          const dispatcher::Model_ABC& model,
                          dispatcher::SimulationPublisher_ABC& simulation );
    virtual ~OrderPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Resolve( const sword::ClientToSim& message );
    virtual void Receive( const sword::SimToClient& ) {}
    //@}

private:
    //! @name Helper
    //@{
    void ReadAction( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    std::unique_ptr< kernel::CoordinateConverter_ABC > converter_;
    std::unique_ptr< kernel::EntityResolver_ABC > entities_;
    //@}
};
}
}

#endif // PLUGINS_ORDER_PLUGIN_H__

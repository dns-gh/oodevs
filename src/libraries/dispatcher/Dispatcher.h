// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dispatcher_h_
#define __Dispatcher_h_

#include "tools/Log.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace dispatcher
{
    class StaticModel;
    class ClientsNetworker;
    class CompositeRegistrable;
    class Config;
    class Model;
    class MemoryLogger_ABC;
    class PluginContainer;
    class PluginFactory_ABC;
    class PluginFactory;
    class Services;
    class SimulationNetworker;

// =============================================================================
/** @class  Dispatcher
    @brief  Dispatcher
*/
// Created: NLD 2006-09-19
// =============================================================================
class Dispatcher : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Dispatcher( const Config& config, int maxConnections );
    virtual ~Dispatcher();
    //@}

    //! @name Operations
    //@{
    void Update();
    void Register( PluginFactory_ABC& factory );
    void CreatePlugins();
    //@}

private:
    std::unique_ptr< dispatcher::StaticModel > staticModel_;
    std::unique_ptr< MemoryLogger_ABC >        memoryLogger_;
    boost::shared_ptr< Model >                 model_;
    std::unique_ptr< PluginContainer >         handler_;
    std::unique_ptr< CompositeRegistrable >    registrables_;
    std::unique_ptr< Services >                services_;
    tools::Log                                 log_;
    boost::shared_ptr< ClientsNetworker >      clientsNetworker_;
    std::unique_ptr< SimulationNetworker >     simulationNetworker_;
    std::unique_ptr< PluginFactory >           factory_;
};

}

#endif // __Dispatcher_h_

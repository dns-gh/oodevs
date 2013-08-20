// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Dispatcher_h_
#define __Dispatcher_h_

#include "CompositePlugin.h"
#include "CompositeRegistrable.h"
#include "LogFactory.h"
#include "tools/RotatingLog.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace dispatcher
{
    class StaticModel;
    class ClientsNetworker;
    class Config;
    class Model;
    class MemoryLogger_ABC;
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
    void RegisterPluginFactory( PluginFactory_ABC& factory );
    void CreatePlugins();
    //@}

private:
    std::auto_ptr< dispatcher::StaticModel >  staticModel_;
    std::auto_ptr< MemoryLogger_ABC >     memoryLogger_;
    boost::shared_ptr< Model >            model_;
    std::auto_ptr< CompositePlugin >      handler_;
    std::auto_ptr< CompositeRegistrable > registrables_;
    std::auto_ptr< Services >             services_;
    LogFactory                            logFactory_;
    tools::RotatingLog                    log_;
    boost::shared_ptr< ClientsNetworker > clientsNetworker_;
    std::auto_ptr< SimulationNetworker >  simulationNetworker_;
    std::auto_ptr< PluginFactory >        factory_;
};

}

#endif // __Dispatcher_h_

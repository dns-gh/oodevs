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

#include "CompositePlugin.h"
#include "CompositeRegistrable.h"
#include "clients_kernel/Application_ABC.h"
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class StaticModel;
}

namespace dispatcher
{
    class ClientsNetworker;
    class Config;
    class Model;
    class MemoryLogger;
    class PluginFactory_ABC;
    class PluginFactory;
    class Services;
    class Shield;
    class SimulationNetworker;
// =============================================================================
/** @class  Dispatcher
    @brief  Dispatcher
*/
// Created: NLD 2006-09-19
// =============================================================================
class Dispatcher
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
    //! @name Copy/Assignment
    //@{
    Dispatcher( const Dispatcher& );            //!< Copy constructor
    Dispatcher& operator=( const Dispatcher& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
     void AddTranslations();
    //@}

private:
    std::auto_ptr< kernel::StaticModel >  staticModel_;
    boost::shared_ptr< Model >            model_;
    std::auto_ptr< CompositeRegistrable > registrables_;
    std::auto_ptr< CompositePlugin >      handler_;
    std::auto_ptr< Services >             services_;
    boost::shared_ptr< ClientsNetworker > clientsNetworker_;
    std::auto_ptr< SimulationNetworker >  simulationNetworker_;
    std::auto_ptr< Shield >               shield_;
    std::auto_ptr< PluginFactory >        factory_;
    std::auto_ptr< MemoryLogger >         memoryLogger_;
    std::auto_ptr< Application_ABC >      qapp_;
};

}

#endif // __Dispatcher_h_

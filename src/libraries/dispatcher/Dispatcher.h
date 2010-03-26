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
#include "boost/shared_ptr.hpp"

namespace dispatcher
{
    class Config;
    class Model;
    class SimulationNetworker;
    class ClientsNetworker;
    class PluginFactory_ABC;
    class PluginFactory;
    class Services;
    class Logger;

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
    explicit Dispatcher( const Config& config );
    virtual ~Dispatcher();
    //@}

    //! @name Operations
    //@{
    void Update();
    void RegisterPluginFactory( PluginFactory_ABC& factory );
    void CreatePlugins();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Dispatcher( const Dispatcher& );            //!< Copy constructor
    Dispatcher& operator=( const Dispatcher& ); //!< Assignement operator
    //@}

private:
    const Config&                             config_;
    boost::shared_ptr< Model >                model_;
    std::auto_ptr< CompositeRegistrable >     registrables_;
    std::auto_ptr< CompositePlugin >          handler_;
    std::auto_ptr< Services >                 services_;
    boost::shared_ptr< ClientsNetworker >     clientsNetworker_;
    boost::shared_ptr< SimulationNetworker >  simulationNetworker_;
    std::auto_ptr< PluginFactory >            factory_;
    std::auto_ptr< Logger >                   logger_;
};

}

#endif // __Dispatcher_h_

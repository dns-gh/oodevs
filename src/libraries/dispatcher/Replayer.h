// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Replayer_h_
#define __Replayer_h_

#include "CompositePlugin.h"
#include <memory>
#include <boost/shared_ptr.hpp>
#include "dispatcher/CompositeRegistrable.h"

namespace dispatcher
{
    class Config;
    class Model;
    class ClientsNetworker;
    class SimulationDispatcher;
    class Loader;
    class ReplayPlugin;
    class Services;

// =============================================================================
/** @class  Replayer
    @brief  Replayer
*/
// Created: AGE 2007-04-10
// =============================================================================
class Replayer
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Replayer( const Config& config );
    virtual ~Replayer();
    //@}

    //! @name Operations
    //@{
    void Update();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Replayer( const Replayer& );            //!< Copy constructor
    Replayer& operator=( const Replayer& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    CompositePlugin                            handler_;
    dispatcher::CompositeRegistrable           registrables_;
    std::auto_ptr< Services >                  services_;
    boost::shared_ptr< Model >                 model_;
    boost::shared_ptr< ClientsNetworker >      clientsNetworker_;
    boost::shared_ptr< SimulationDispatcher >  simulation_;
    std::auto_ptr< Loader >                    loader_;
    boost::shared_ptr< ReplayPlugin >          plugin_;
    //@}
};

}

#endif // __Replayer_h_

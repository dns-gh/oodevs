// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Replayer_h_
#define __Replayer_h_

#include "CompositePlugin.h"
#include "CompositeRegistrable.h"
#include <boost/shared_ptr.hpp>
#include <memory>

namespace kernel
{
    class StaticModel;
}

namespace plugins
{
namespace replay
{
    class ReplayPlugin;
}
}

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Config;
    class Model;
    class ClientsNetworker;
    class SimulationDispatcher;
    class Loader;
    class Services;
    class MemoryLogger_ABC;
    class SimulationPublisher_ABC;

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
    void OnWebControl( xml::xistream& xis );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Replayer( const Replayer& );            //!< Copy constructor
    Replayer& operator=( const Replayer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Services >                          services_;
    std::auto_ptr< kernel::StaticModel >               staticModel_;
    std::auto_ptr< MemoryLogger_ABC >                  logger_;
    boost::shared_ptr< Model >                         model_;
    dispatcher::CompositeRegistrable                   registrables_;
    boost::shared_ptr< ClientsNetworker >              clientsNetworker_;
    CompositePlugin                                    handler_;
    boost::shared_ptr< SimulationDispatcher >          simulation_;
    std::auto_ptr< Loader >                            loader_;
    boost::shared_ptr< plugins::replay::ReplayPlugin > plugin_;
    std::auto_ptr< SimulationPublisher_ABC >           publisher_;
    bool                                               started_;
    //@}
};
}

#endif // __Replayer_h_

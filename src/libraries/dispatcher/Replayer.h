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

#include "PluginContainer.h"
#include "CompositeRegistrable.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
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
namespace rights
{
    class RightsPlugin;
}
}

namespace sword
{
    class ClientToSim;
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
class Replayer : boost::noncopyable
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
    void ReceiveClientToSim( const std::string& link, const sword::ClientToSim& );
    void ReceiveClientToReplay( const std::string& link, const sword::ClientToReplay& );

private:
    //! @name Member data
    //@{
    std::auto_ptr< Services >                          services_;
    std::auto_ptr< kernel::StaticModel >               staticModel_;
    std::auto_ptr< MemoryLogger_ABC >                  logger_;
    boost::shared_ptr< Model >                         model_;
    dispatcher::CompositeRegistrable                   registrables_;
    boost::shared_ptr< ClientsNetworker >              clientsNetworker_;
    PluginContainer                                    handler_;
    boost::shared_ptr< SimulationDispatcher >          simulation_;
    std::auto_ptr< Loader >                            loader_;
    boost::shared_ptr< plugins::replay::ReplayPlugin > plugin_;
    std::auto_ptr< SimulationPublisher_ABC >           publisher_;
    bool                                               started_;
    boost::shared_ptr< plugins::rights::RightsPlugin > rights_;
    //@}
};
}

#endif // __Replayer_h_

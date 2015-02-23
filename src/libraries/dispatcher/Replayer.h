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
    class ClientToReplay;
}

namespace tools
{
    class Log;
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
    class Loader;
    class Services;
    class MemoryLogger_ABC;
    class SimulationPublisher_ABC;
    class ReplayModel_ABC;

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
    bool Update();
    void OnWebControl( xml::xistream& xis );
    //@}

private:
    void ReceiveClientToReplay( const std::string& link, sword::ClientToReplay& );
    void ReceiveClientToSim( const std::string& link, sword::ClientToSim& );

private:
    //! @name Member data
    //@{
    struct SenderToReceiver;
    std::unique_ptr< Services >                        services_;
    std::unique_ptr< kernel::StaticModel >             staticModel_;
    std::unique_ptr< MemoryLogger_ABC >                logger_;
    boost::shared_ptr< Model >                         model_;
    dispatcher::CompositeRegistrable                   registrables_;
    boost::shared_ptr< ClientsNetworker >              clientsNetworker_;
    PluginContainer                                    handler_;
    std::unique_ptr< SimulationPublisher_ABC >         publisher_;
    boost::shared_ptr< plugins::rights::RightsPlugin > rights_;
    boost::shared_ptr< SenderToReceiver >              modelHandler_;
    boost::shared_ptr< ReplayModel_ABC >               synchronizer_;
    std::unique_ptr< Loader >                          loader_;
    boost::shared_ptr< plugins::replay::ReplayPlugin > replay_;
    bool                                               started_;
    bool                                               stopped_;
    std::unique_ptr< tools::Log >                      log_;
    //@}
};
}

#endif // __Replayer_h_

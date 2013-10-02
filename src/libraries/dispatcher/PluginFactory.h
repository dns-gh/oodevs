    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PluginFactory_h_
#define __PluginFactory_h_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace rights
{
    class RightsPlugin;
}
namespace order
{
    class OrderPlugin;
}
}

namespace tools
{
    class Path;
    class Log;
}

namespace dispatcher
{
    class Config;
    class Model;
    class StaticModel;
    class SimulationPublisher_ABC;
    class ClientsNetworker;
    class CompositePlugin;
    class CompositeRegistrable;
    class PluginFactory_ABC;
    class Services;

// =============================================================================
/** @class  PluginFactory
    @brief  PluginFactory
*/
// Created: SBO 2007-07-24
// =============================================================================
class PluginFactory : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PluginFactory( const Config& config, Model& model, const dispatcher::StaticModel& staticModel,
                            SimulationPublisher_ABC& simulation, ClientsNetworker& clients,
                            CompositePlugin& handler, CompositeRegistrable& registrables,
                            const Services& services, tools::Log& log, int maxConnections );
    virtual ~PluginFactory();
    //@}

    //! @name Operations
    //@{
    void Register( PluginFactory_ABC& factory );
    void Instanciate();
    void Close();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadPlugin( const std::string& name, xml::xistream& xis );
    void LoadPlugin( const tools::Path& name, xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const Config& config_;
    Model& model_;
    const dispatcher::StaticModel& staticModel_;
    SimulationPublisher_ABC& simulation_;
    ClientsNetworker& clients_;
    CompositePlugin& handler_;
    CompositeRegistrable& registrables_;
    boost::ptr_vector< PluginFactory_ABC > factories_;
    boost::shared_ptr< plugins::rights::RightsPlugin > rights_;
    boost::shared_ptr< plugins::order::OrderPlugin > pOrder_;
    const Services& services_;
    //@}
};

}

#endif // __PluginFactory_h_

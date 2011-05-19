    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PluginFactory_h_
#define __PluginFactory_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class StaticModel;
}

namespace plugins
{
namespace rights
{
    class RightsPlugin;
}
}

namespace dispatcher
{
    class Config;
    class Model;
    class SimulationPublisher_ABC;
    class ClientsNetworker;
    class CompositePlugin;
    class CompositeRegistrable;
    class Logger_ABC;
    class PluginFactory_ABC;
    class Services;

// =============================================================================
/** @class  PluginFactory
    @brief  PluginFactory
*/
// Created: SBO 2007-07-24
// =============================================================================
class PluginFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             PluginFactory( const Config& config, Model& model, const kernel::StaticModel& staticModel, SimulationPublisher_ABC& simulation, ClientsNetworker& clients,
                            CompositePlugin& handler, CompositeRegistrable& registrables, const Services& services, int maxConnections );
    virtual ~PluginFactory();
    //@}

    //! @name Operations
    //@{
    void Register( PluginFactory_ABC& factory );
    void Instanciate();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PluginFactory( const PluginFactory& );            //!< Copy constructor
    PluginFactory& operator=( const PluginFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadPlugin( const std::string& name, xml::xistream& xis );
    void LoadPlugin( const std::string& name, xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::vector< PluginFactory_ABC* > T_Factories;
    typedef std::vector< boost::shared_ptr< Logger_ABC > > T_Loggers;
    //@}

private:
    //! @name Member data
    //@{
    const Config& config_;
    Model& model_;
    const kernel::StaticModel& staticModel_;
    SimulationPublisher_ABC& simulation_;
    ClientsNetworker& clients_;
    T_Loggers loggers_;
    CompositePlugin& handler_;
    CompositeRegistrable& registrables_;
    T_Factories factories_;
    boost::shared_ptr< plugins::rights::RightsPlugin > rights_;
    const Services& services_;
    //@}
};

}

#endif // __PluginFactory_h_

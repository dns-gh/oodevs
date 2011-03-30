// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __vrforces_Plugin_h_
#define __vrforces_Plugin_h_

#include "dispatcher/Plugin_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <set>

class DtExerciseConn;
class DtFilePrinter;

namespace dispatcher
{
    class Config;
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace rpr
{
    class EntityTypeResolver;
}

namespace sword
{
    class UnitAttributes;
    class UnitCreation;
    class UnitDestruction;
}

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace vrforces
{
    class Agent;
    class DisaggregationStrategy_ABC;
    class Facade;
    class ForceResolver_ABC;

// =============================================================================
/** @class  Plugin
    @brief  Plugin
*/
// Created: SBO 2011-01-19
// =============================================================================
class Plugin : public dispatcher::Plugin_ABC
             , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Plugin( dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& simulation, const dispatcher::Config& config, xml::xistream& xis );
    virtual ~Plugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );
    virtual void Receive( const sword::MessengerToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Helpers
    //@{
    void Create( const sword::UnitCreation& message );
    void Update( const sword::UnitAttributes& message );
    void Destroy( const sword::UnitDestruction& message );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, boost::shared_ptr< Agent > > T_Agents;
    typedef std::map< unsigned long, std::set< boost::shared_ptr< Agent > > > T_AutomatAgents;
    typedef std::map< unsigned long, unsigned long > T_AgentAutomat;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& model_;
    dispatcher::SimulationPublisher_ABC& simulation_;
    T_Agents agents_;
    std::auto_ptr< DtExerciseConn > connection_;
    std::auto_ptr< DtFilePrinter > logger_;
    std::auto_ptr< Facade > vrForces_;
    std::auto_ptr< ForceResolver_ABC > forceResolver_;
    std::auto_ptr< DisaggregationStrategy_ABC > disaggregator_;
    std::auto_ptr< rpr::EntityTypeResolver > typeResolver_;
    //@}
};

}
}

#endif // __vrforces_Plugin_h_

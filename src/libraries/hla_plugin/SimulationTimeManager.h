// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_SimulationTimeManager_h
#define plugins_hla_SimulationTimeManager_h

#include "SimulationTimeManager_ABC.h"
#include "tools/MessageObserver.h"

namespace tools
{
    template< typename T > class MessageController_ABC;
}

namespace sword
{
    class SimToClient_Content;
    class ControlBeginTick;
}
namespace plugins
{
namespace hla
{

class SimulationTimeManager
    : public SimulationTimeManager_ABC
    , private tools::MessageObserver< sword::ControlBeginTick >
{
public:
    //! @name Contructors destructors
    //@{
    SimulationTimeManager( tools::MessageController_ABC< sword::SimToClient_Content >& messageController );
    virtual ~SimulationTimeManager();
    //@}

    //! @name Operations
    //@{
    virtual const std::string& getSimulationTime( ) const;
    virtual unsigned int getSimulationtick() const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::ControlBeginTick& message, int );
    //@}

    //! @name Contructors destructors
    //@{
    tools::MessageController_ABC< sword::SimToClient_Content >& messageController_;
    std::string simulationTime_;
    unsigned int simulationTick_;
    //@}
};

}
}

#endif // plugins_hla_SimulationTimeManager_h
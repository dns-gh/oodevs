// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_PropagationManager_h
#define plugins_hla_PropagationManager_h

#include "PropagationManager_ABC.h"
#include "tools/MessageObserver.h"
#include "ObjectListener_ABC.h"

namespace tools
{
    class ExerciseConfig;
    template< typename T > class MessageController_ABC;
}
namespace dispatcher
{
    class SimulationPublisher_ABC;
    class Logger_ABC;
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

class PropagationManager
    : public PropagationManager_ABC
    , private tools::MessageObserver< sword::ControlBeginTick >
{
public:
    //! @name Contructors destructors
    //@{
    PropagationManager( dispatcher::Logger_ABC& logger, const tools::ExerciseConfig& conf,
            tools::MessageController_ABC< sword::SimToClient_Content >& messageController );
    virtual ~PropagationManager();
    //@}

    //! @name Operations
    //@{
    virtual void saveDataFile( const std::string& identifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
            int col, int lig, double xll, double yll, double cellsize );
    //@}

private://! @name Operations
    //@{
    void saveProjectionFile( const std::string& identifier );
    virtual void Notify( const sword::ControlBeginTick& message, int  );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::vector< std::string > > T_PropagationTimes;
    //@}

    //! @name Attributes
    //@{
    dispatcher::Logger_ABC& logger_;
    const tools::ExerciseConfig& exerciseConfig_;
    std::string simulationTime_;
    T_PropagationTimes propagationTimes_;
    //@}
};

} // namespace hla
} // namespace plugins
#endif // plugins_hla_PropagationManager_h
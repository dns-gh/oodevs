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
#include "ObjectListener_ABC.h"
#include <boost/noncopyable.hpp>

namespace tools
{
    class ExerciseConfig;
}
namespace dispatcher
{
    class SimulationPublisher_ABC;
    class Logger_ABC;
}
namespace sword
{
    class SimToClient_Content;
}

namespace plugins
{
namespace hla
{

class SimulationTimeManager_ABC;
class PropagationManager
    : public PropagationManager_ABC
    , private boost::noncopyable
{
public:
    //! @name Contructors destructors
    //@{
    PropagationManager( dispatcher::Logger_ABC& logger, const tools::ExerciseConfig& conf, const SimulationTimeManager_ABC& timeManager );
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
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::vector< std::string > > T_PropagationTimes;
    //@}

    //! @name Attributes
    //@{
    dispatcher::Logger_ABC& logger_;
    const SimulationTimeManager_ABC& timeManager_;
    const tools::ExerciseConfig& exerciseConfig_;
    T_PropagationTimes propagationTimes_;
    //@}
};

} // namespace hla
} // namespace plugins
#endif // plugins_hla_PropagationManager_h
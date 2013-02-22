// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ActionsLogger_h_
#define __ActionsLogger_h_

#include <map>
#include <memory>
#include <string>

#include <boost/noncopyable.hpp>

namespace sword
{
    class AutomatOrder;
    class CrowdOrder;
    class UnitOrder;
    class FragOrder;
    class ClientToSim;
}

namespace dispatcher
{
    class Model_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
    class Time_ABC;
}

namespace tools
{
    class SessionConfig;
}

namespace boost
{
    template< typename T > class function;
namespace posix_time
{
    class ptime;
}
}

namespace plugins
{
namespace logger
{
// =============================================================================
/** @class  ActionsLogger
    @brief  ActionsLogger
*/
// Created: SBO 2010-05-11
// =============================================================================
class ActionsLogger : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionsLogger( const tools::SessionConfig& config,
                            const dispatcher::Model_ABC& model,
                            const kernel::Time_ABC& timer );
    virtual ~ActionsLogger();
    //@}

    //! @name Operations
    //@{
    void Log( const sword::UnitOrder& message );
    void Log( const sword::AutomatOrder& message );
    void Log( const sword::CrowdOrder& message );
    void Log( const sword::FragOrder& message );
    void Close();
    //@}

    //! @name Checkpoint
    //@{
    void SaveCheckpointActiveMissions( std::string name );
    //@}

    //! @name Typedef helpers
    //@{
    typedef std::map< boost::posix_time::ptime, sword::ClientToSim > T_Actions;
    typedef boost::function< bool( const sword::ClientToSim& ) > T_Filter;
    //@}

private:
    //! @name Helpers
    //@{
    void SaveTo( const std::string& filename, const T_Filter& filter ) const;
    void Commit() const;
    void LoadOrdersIfCheckpoint();
    template< typename T, typename U >
    void LogAction( const T& message, const U& mutator );
    //@}

private:
    //! @name Member data
    //@{
    const tools::SessionConfig&                      config_;
    const kernel::Time_ABC&                          timer_;
    std::auto_ptr< kernel::EntityResolver_ABC >      entities_;
    std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    T_Actions                                        actions_;
    bool                                             loaded_;
    //@}
};

}
}

#endif // __ActionsLogger_h_

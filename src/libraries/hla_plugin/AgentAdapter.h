// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AGENTADAPTER_H_
#define plugins_hla_AGENTADAPTER_H_

#include "dispatcher/Observer.h"
#include "tic/PlatformVisitor_ABC.h"

#include <boost/shared_ptr.hpp>

namespace tic
{
    class PlatformDelegate_ABC;
    class PlatformDelegateFactory_ABC;
}

namespace dispatcher
{
    class Agent_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}
namespace sword
{
    class UnitAttributes;
}

namespace plugins
{
namespace hla
{

class Agent_ABC;
class PlatformProxy;

// =============================================================================
/** @class  AgentAdapter
    @brief  AgentAdapter
*/
// Created: AHC 2012-07-27
// =============================================================================
class AgentAdapter : private dispatcher::Observer< sword::UnitAttributes >
    , private tic::PlatformVisitor_ABC
{
public:
    //! @name Public types
    //@{
    typedef std::function< void( Agent_ABC&, dispatcher::Agent_ABC&, const tic::Platform_ABC&, int childIndex ) > T_NotificationCallback;
    //@}

    //! @name Constructors/Destructor
    //@{
    AgentAdapter( tic::PlatformDelegateFactory_ABC& factory, const kernel::CoordinateConverter_ABC& converter,
            dispatcher::Agent_ABC& agent, T_NotificationCallback cb );
    //@}

private:
    //! @name Private operations
    //@{
    virtual void Notify( const sword::UnitAttributes& attributes );
    virtual void AddPlatform( const tic::Platform_ABC& platform );
    //@}

    //! @name types
    //@{
    typedef std::map< const tic::Platform_ABC*, boost::shared_ptr< PlatformProxy > > T_Proxies;
    //@}

    //! @name Attributes
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    dispatcher::Agent_ABC& agent_;
    T_NotificationCallback callback_;
    int childIndex_;
    std::unique_ptr< tic::PlatformDelegate_ABC > platforms_;
    T_Proxies proxies_;
    //@}
};

}
}
#endif // plugins_hla_AGENTADAPTER_H_

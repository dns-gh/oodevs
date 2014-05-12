// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentProxy_h_
#define __AgentProxy_h_

#include "dispatcher/Observer.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/ForceIdentifier.h"
#include "tic/PlatformVisitor_ABC.h"
#include <memory>

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Agent;
}

namespace rpr
{
    class EntityTypeResolver;
}

namespace sword
{
    class UnitAttributes;
}

namespace tic
{
    class PlatformDelegate_ABC;
}

namespace plugins
{
namespace dis
{
    class UdpNetwork;
    class Time_ABC;
    class IdentifierFactory_ABC;

// =============================================================================
/** @class  AgentProxy
    @brief  AgentProxy
*/
// Created: AGE 2008-03-10
// =============================================================================
class AgentProxy : private dispatcher::Observer< sword::UnitAttributes >
                 , private tic::PlatformVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentProxy( const Time_ABC& time, IdentifierFactory_ABC& id, const kernel::CoordinateConverter_ABC& converter
                       , UdpNetwork& network, const rpr::EntityTypeResolver& resolver, dispatcher::Agent& holder
                       , unsigned char exercise, bool lagAFrame, std::unique_ptr< tic::PlatformDelegate_ABC > platforms );
    virtual ~AgentProxy();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Notify( const sword::UnitAttributes& attributes );
    virtual void AddPlatform( const tic::Platform_ABC& platform );
    //@}

    //! @name Types
    //@{
    typedef std::map< const tic::Platform_ABC*, rpr::EntityIdentifier > T_Identifiers;
    //@}

private:
    //! @name Member data
    //@{
    const Time_ABC&        time_;
    IdentifierFactory_ABC& id_;
    const kernel::CoordinateConverter_ABC& converter_;
    UdpNetwork&            network_;
    const rpr::EntityTypeResolver& resolver_;
    dispatcher::Agent&     holder_;
    rpr::ForceIdentifier   forceId_;
    const unsigned char    exercise_;
    bool                   lagAFrame_;
    T_Identifiers          ids_;
    std::unique_ptr< tic::Platform_ABC > adapted_;
    std::unique_ptr< tic::PlatformDelegate_ABC > platforms_;
    //@}
};

}
}

#endif // __DisExtension_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __dis_plugin_Model_h_
#define __dis_plugin_Model_h_

#include "dispatcher/ExtensionFactory_ABC.h"
#include "IdentifierFactory_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Agent;
}

namespace rpr
{
    class EntityTypeResolver;
}

namespace tic
{
    class PlatformDelegateFactory_ABC;
}

namespace plugins
{
namespace dis
{
    class AgentProxy;
    class Time_ABC;
    class UdpNetwork;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: AGE 2008-03-10
// =============================================================================
class Model : private boost::noncopyable
            , public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
            , private IdentifierFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Model( UdpNetwork& network, const Time_ABC& time, const kernel::CoordinateConverter_ABC& converter, const rpr::EntityTypeResolver& resolver, xml::xistream& xis, tic::PlatformDelegateFactory_ABC& factory );
    virtual ~Model();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent& entity );
    virtual rpr::EntityIdentifier CreateNewIdentifier();
    //@}

    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< AgentProxy > > T_Agents;
    //@}

private:
    //! @name Member data
    //@{
    UdpNetwork& network_;
    const Time_ABC& time_;
    const kernel::CoordinateConverter_ABC& converter_;
    const rpr::EntityTypeResolver& resolver_;
    tic::PlatformDelegateFactory_ABC& factory_;
    const unsigned short site_;
    const unsigned short application_;
    const unsigned char  exercise_;
    const bool lag_;
    unsigned short id_;
    T_Agents agents_;
    //@}
};

}
}

#endif // __dis_plugin_Model_h_

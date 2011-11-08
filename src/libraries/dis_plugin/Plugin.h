// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __dis_Plugin_h_
#define __dis_Plugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "Time_ABC.h"
#include <memory>

namespace dispatcher
{
    class Config;
    class Logger_ABC;
    class Model_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace rpr
{
    class EntityTypeResolver;
}

namespace xml
{
    class xistream;
}

namespace tic
{
    class PlatformDelegateFactory_ABC;
}

namespace plugins
{
namespace dis
{
    class FireManager;
    class Model;
    class UdpNetwork;

// =============================================================================
/** @class  Plugin
    @brief  Plugin
*/
// Created: AGE 2008-03-10
// =============================================================================
class Plugin : public dispatcher::Plugin_ABC
             , public Time_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Plugin( dispatcher::Model_ABC& model, const dispatcher::Config& config, dispatcher::Logger_ABC& logger, xml::xistream& xis );
    virtual ~Plugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive                  ( const sword::SimToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    virtual unsigned long GetTime() const;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& model_;
    std::auto_ptr< UdpNetwork > network_;
    std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    const unsigned long timeStep_;
    std::auto_ptr< tic::PlatformDelegateFactory_ABC > platforms_;
    std::auto_ptr< rpr::EntityTypeResolver > resolver_;
    std::auto_ptr< Model > factory_;
    std::auto_ptr< FireManager > fires_;
    unsigned long time_;
    //@}
};

}
}

#endif // __dis_Plugin_h_

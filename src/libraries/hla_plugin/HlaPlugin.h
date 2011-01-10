// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HlaPlugin_h_
#define __HlaPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "protocol/Protocol.h"
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Model_ABC;
    class Config;
}

namespace plugins
{
namespace hla
{
    class FederateFacade;
    class RtiAmbassadorFactory_ABC;

// =============================================================================
/** @class  HlaPlugin
    @brief  Hla plugin
*/
// Created: SBO 2008-02-18
// =============================================================================
class HlaPlugin : public dispatcher::Plugin_ABC, private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             HlaPlugin( dispatcher::Model_ABC& model, const dispatcher::Config& config, xml::xistream& xis );
    virtual ~HlaPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< RtiAmbassadorFactory_ABC > pFactory_;
    std::auto_ptr< FederateFacade > federate_;
    //@}
};

}
}

#endif // __HlaPlugin_h_

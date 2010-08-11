// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __HlaPlugin_h_
#define __HlaPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "protocol/protocol.h"

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Model;
    class Config;
}

namespace plugins
{
namespace hla
{
    class AggregateEntityClass;
    class FederateFacade;
    class ExtensionFactory;

// =============================================================================
/** @class  HlaPlugin
    @brief  Hla plugin
*/
// Created: SBO 2008-02-18
// =============================================================================
class HlaPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             HlaPlugin( dispatcher::Model& model, const dispatcher::Config& config, xml::xistream& xis );
    virtual ~HlaPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    HlaPlugin( const HlaPlugin& );            //!< Copy constructor
    HlaPlugin& operator=( const HlaPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model& model_;
    std::auto_ptr< AggregateEntityClass > agentClass_;
    std::auto_ptr< ExtensionFactory > factory_;
    std::auto_ptr< FederateFacade > federate_;
    //@}
};

}
}

#endif // __HlaPlugin_h_

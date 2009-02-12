// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EsriPlugin_h_
#define __EsriPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "dispatcher/NoopPublisher.h"
#include "game_asn/Messenger.h"

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Model;
    class Config;
    class SimulationPublisher_ABC;
    class LinkResolver_ABC;
    class CompositeRegistrable;
}

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace plugins
{
namespace esri
{
    class DatabasePublisher;

// =============================================================================
/** @class  EsriPlugin
    @brief  EsriPlugin
*/
// Created: JCR 2007-08-29
// =============================================================================
class EsriPlugin : public dispatcher::Plugin_ABC
                 , public dispatcher::NoopClientPublisher
{

public:
    //! @name Constructors/Destructor
    //@{             
             EsriPlugin( const dispatcher::Config& config, xml::xistream& xis, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& publisher, tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& links, dispatcher::CompositeRegistrable& registrables );
    virtual ~EsriPlugin();
    //@}

    //! @name Operations
    //@{    
    virtual void Receive( const ASN1T_MsgsSimToClient& asnMsg );
    virtual void Send   ( const ASN1T_MsgsMessengerToClient& msg ); 

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EsriPlugin( const EsriPlugin& );            //!< Copy constructor
    EsriPlugin& operator=( const EsriPlugin& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< DatabasePublisher > databasePublisher_;
    std::auto_ptr< dispatcher::Plugin_ABC > messenger_;
    //@}
};

}
}

#endif // __EsriPlugin_h_

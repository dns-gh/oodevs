// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AarPlugin_h_
#define __AarPlugin_h_

#include "Plugin_ABC.h"

namespace tools
{
    class MessageDispatcher_ABC;
}

class FunctionFactory;
class Task;

namespace dispatcher
{
    class Config;
    class MessageLoader;
    class LinkResolver_ABC;

// =============================================================================
/** @class  AarPlugin
    @brief  AAR plugin
*/
// Created: AGE 2007-09-17
// =============================================================================
class AarPlugin : public Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AarPlugin( tools::MessageDispatcher_ABC& dispatcher, LinkResolver_ABC& resolver, const Config& config, const std::string& records );
    virtual ~AarPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& message );
    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& profile );
    virtual void NotifyClientLeft         ( ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AarPlugin( const AarPlugin& );            //!< Copy constructor
    AarPlugin& operator=( const AarPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void OnReceive( const std::string&, const ASN1T_MsgsClientToAar& message );
    void OnReceiveIndicatorRequest( const std::string& client, unsigned context, const ASN1T_MsgIndicatorRequest& request );
    //@}

private:
    //! @name Member data
    //@{
    LinkResolver_ABC& resolver_;
    std::auto_ptr< FunctionFactory > factory_;
    std::auto_ptr< MessageLoader >   messages_;
    //@}
};

}

#endif // __AarPlugin_h_

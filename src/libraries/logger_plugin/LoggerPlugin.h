// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LoggerPlugin_h_
#define __LoggerPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "reports/ReportFactory.h"
#include "RcEntityResolver.h"
#include <fstream>

namespace tools
{
    class SessionConfig;
}

namespace dispatcher
{
    class Model;
    class Services;
}

namespace plugins
{
namespace logger
{
// =============================================================================
/** @class  LoggerPlugin
    @brief  LoggerPlugin
*/
// Created: LDC 2010-03-17
// =============================================================================
class LoggerPlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LoggerPlugin( const dispatcher::Model& model, const tools::SessionConfig& config, const dispatcher::Services& services );
    virtual ~LoggerPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client );
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LoggerPlugin( const LoggerPlugin& );            //!< Copy constructor
    LoggerPlugin& operator=( const LoggerPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void FormatMission( const char* name, int id, int mission );
    bool Initialize();
    //@}

private:
    //! @name Member data
    //@{
    std::string                 filename_;
    std::ofstream*              file_;
    kernel::ObjectTypes         objectTypes_;
    RcEntityResolver            resolver_;
    ReportFactory               factory_;
    const dispatcher::Model&    model_;
    const dispatcher::Services& services_;
    std::string                 date_;
    bool                        enabled_;
    bool                        initialized_;
    //@}
};
}
}

#endif // __LoggerPlugin_h_

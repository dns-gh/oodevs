// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __plugins_bml_PluginProcessHandler_h_
#define __plugins_bml_PluginProcessHandler_h_

#pragma warning( disable: 4996 ) // Function call with parameters that may be unsafe
#define _WIN32_WINNT 0x0501
#include <exception>
#include <memory>
#include <string>

#include "dispatcher/Plugin_ABC.h"

namespace dispatcher
{
    class Config;
    class Logger_ABC;
}

namespace xml
{
    class xistream;
}

namespace plugins 
{
namespace bml
{

// =============================================================================
/** @class  PluginProcessHandler
    @brief  PluginProcessHandler
*/
// Created: JCR 2011-10-27
// =============================================================================
class PluginProcessHandler : public dispatcher::Plugin_ABC
{
public: 
    //! @name Constructors/Destructor
    //@{
             PluginProcessHandler( const dispatcher::Config& config, const std::string& process_name, dispatcher::Logger_ABC& logger, xml::xistream& xis );        
    virtual ~PluginProcessHandler();
    //@}

    //! @name Process management
    //@{
    void Start();
    void Stop();
    //@}

    //! @name Inherited from Plugin_ABC
    //@{
    void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ ) {};
    void NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ ) {};
    void Receive( const sword::SimToClient& /*message*/ ) {};
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PluginProcessHandler( const PluginProcessHandler& );            //!< Copy constructor
    PluginProcessHandler& operator=( const PluginProcessHandler& ); //!< Assignment operator
    //@}

    //! @name Process arguments
    //@{
    void LoadSimulationConfig( const dispatcher::Config& config );
    void LoadPluginConfig( xml::xistream& xis, const dispatcher::Config& config );
    void AddArgument( const std::string& arg );
    //@}
    
    struct InternalData;
    class ThreadDelayed;

private:
    std::string commandLine_;
    std::string workingDir_;
    std::auto_ptr< InternalData > internal_;
    std::auto_ptr< ThreadDelayed > thread_;
};

}
}

#endif // __plugins_bml_PluginProcessHandler_h_

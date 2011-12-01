// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __plugins_edxl_PluginProcessHandler_h_
#define __plugins_edxl_PluginProcessHandler_h_

#define LOG_MESSAGE( e ) \
	std::string( "[Sword EDXL Service]: " ) + e

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
namespace edxl
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

    //! @name Inherited from Plugin_ABC
    //@{
    void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ ) {};
    void NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ ) {};
    void Receive( const sword::SimToClient& message );
    //@}

	void SetLoginProfile( const std::string& profile, const std::string& password );

private:
    //! @name Copy/Assignment
    //@{
    PluginProcessHandler( const PluginProcessHandler& );            //!< Copy constructor
    PluginProcessHandler& operator=( const PluginProcessHandler& ); //!< Assignment operator
    //@}

    //! @name Process management
    //@{
    void Start();
    void Stop();
    //@}

    //! @name Process arguments
    //@{
    class PluginConfig;
	void LoadLoginProfile();
	void LoadSimulationConfig( const PluginConfig& config );
    void LoadPluginConfig( xml::xistream& xis, const PluginConfig& config );
    void AddArgument( const std::string& arg );
    //@}
    
    struct InternalData;

private:
    dispatcher::Logger_ABC& logger_;
	std::string processName_;
    std::string workingDir_;
	std::string sessionDir_;
	std::string physicalDir_;
    std::string commandLine_;

	std::auto_ptr< PluginConfig > config_;
	std::auto_ptr< xml::xistream > xis_;
    std::auto_ptr< InternalData > internal_;
};

}
}

#endif // __plugins_bml_PluginProcessHandler_h_

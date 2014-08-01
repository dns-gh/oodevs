// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef GAMING_GAMING_CONFIG_H
#define GAMING_GAMING_CONFIG_H

#include "tools/SessionConfig.h"

class Network;

namespace gui
{
    struct GamingCommonConfig;
}

// =============================================================================
/** @class  GamingConfig
    @brief  GamingConfig
*/
// Created: NLD 2007-01-12
// =============================================================================
class GamingConfig : public tools::SessionConfig
{
public:
    //! @name Constructors/Destructor
    //@{
             GamingConfig( int argc, char** argv );
    virtual ~GamingConfig();
    //@}

    //! @name Operations
    //@{
    void Connect( Network& network ) const;
    const std::string& GetLogin() const;
    const std::string& GetPassword() const;
    tools::Path GetOrderFile() const;
    bool IsLoginInCommandLine() const;
    unsigned long GetNetworkTimeOut() const;

    virtual bool HasTimeline() const;
    virtual std::string GetTimelineUrl() const;
    virtual int GetTimelineDebugPort() const;
    // Disabled if empty.
    tools::Path GetTimelineClientLogFile() const;
    // Disabled if empty
    tools::Path GetCefLogFile() const;
    bool HasMapnik() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadSession();
    //@}

private:
    //! @name Member data
    //@{
    std::string host_;
    std::string login_;
    std::string password_;
    tools::Path orderFile_;
    bool isLoginInCommandLine_;
    unsigned long networkTimeOut_;
    bool hasTimeline_;
    std::string timelineUrl_;
    int timelineDebugPort_;
    tools::Path timelineLogFile_;
    tools::Path cefLogFile_;
    std::unique_ptr< gui::GamingCommonConfig > common_;
    //@}
};

#endif // GAMING_GAMING_CONFIG_H

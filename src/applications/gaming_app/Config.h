// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#include "tools/SessionConfig.h"

namespace tools
{
    class RealFileLoaderObserver_ABC;
}

class Network;

// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: NLD 2007-01-12
// =============================================================================
class Config : public tools::SessionConfig
{
public:
    //! @name Constructors/Destructor
    //@{
             Config( int argc, char** argv, tools::RealFileLoaderObserver_ABC& observer );
    virtual ~Config();
    //@}

    //! @name Operations
    //@{
    void Connect( Network& network ) const;
    std::string GetLogin() const;
    tools::Path GetOrderFile() const;
    bool IsLoginInCommandLine() const;
    unsigned long GetNetworkTimeOut() const;

    virtual bool HasTimeline() const;
    virtual std::string GetTimelineUrl() const;
    virtual int GetTimelineDebugPort() const;
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
    tools::Path orderFile_;
    bool isLoginInCommandLine_;
    unsigned long networkTimeOut_;
    bool hasTimeline_;
    std::string timelineUrl_;
    int timelineDebugPort_;
    //@}
};

#endif // __Config_h_

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

namespace dispatcher
{
// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: NLD 2007-01-10
// =============================================================================
class Config : public virtual tools::SessionConfig
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Config( tools::RealFileLoaderObserver_ABC& observer );
    virtual ~Config();
    //@}

    //! @name Operations
    //@{
    virtual void Parse( int argc, char** argv );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetNetworkSimulationParameters() const;
    unsigned short     GetNetworkClientsParameters() const;
    unsigned short     GetNetworkShieldParameters() const;
    bool               UseShieldUtf8Encoding() const;
    unsigned int       GetKeyFramesFrequency() const;
    unsigned int       GetReplayFragmentsFrequency() const;
    unsigned int       GetTickDuration() const;
    unsigned int       GetLogFiles() const;
    unsigned int       GetLogSize() const;
    unsigned int       GetShieldLogFiles() const;
    unsigned int       GetShieldLogSize() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string    networkSimulationParameters_;
    unsigned short networkSimulationPort_;
    unsigned short networkClientsParameters_;
    unsigned short networkShieldParameters_;
    bool           useShieldUtf8Encoding_;
    unsigned int   keyFramesFrequency_;
    unsigned int   replayFragmentsFrequency_;
    unsigned int   timeStep_;
    unsigned int   logFiles_;
    unsigned int   logSize_;
    unsigned int   shieldLogFiles_;
    unsigned int   shieldLogSize_;
    //@}
};

}

#endif // __Config_h_

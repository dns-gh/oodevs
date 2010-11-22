// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ProcessService_h_
#define __ProcessService_h_

#include "frontend/ProcessObserver_ABC.h"
#include "protocol/LauncherSenders.h"
#pragma warning( push, 0 )
#pragma warning( disable : 4244 )
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#pragma warning( pop )
#include <boost/noncopyable.hpp>
#include <boost/weak_ptr.hpp>

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    class ProcessWrapper;
}

namespace launcher
{

// =============================================================================
/** @class  ProcessService
    @brief  ProcessService
*/
// Created: SBO 2010-09-30
// =============================================================================
class ProcessService : private boost::enable_shared_from_this< ProcessService >
                     , private boost::noncopyable
                     , public frontend::ProcessObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ProcessService( const tools::GeneralConfig& config );
    virtual ~ProcessService();
    //@}

    //! @name Operations
    //@{
    void SendExerciseList( MsgsLauncherToAdmin::MsgExercicesListResponse& message );
    MsgsLauncherToAdmin::MsgControlStartAck::ErrorCode StartExercise( const MsgsAdminToLauncher::MsgControlStart& message );
    MsgsLauncherToAdmin::MsgControlStopAck::ErrorCode StopExercise( const MsgsAdminToLauncher::MsgControlStop& message );
    void SendProfileList( MsgsAuthenticationToClient::MsgProfileDescriptionList& message );
    bool IsRunning( const std::string& exercise ) const;
    virtual void ProcessStopped();
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    std::map< std::string, boost::weak_ptr< frontend::ProcessWrapper > > processes_;
    boost::recursive_mutex mutex_;
    //@}
};

}

#endif // __ProcessService_h_

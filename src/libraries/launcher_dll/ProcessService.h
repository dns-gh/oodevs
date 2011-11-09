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

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

namespace launcher
{

class SwordFacade;
class LauncherService;
class Config;

// =============================================================================
/** @class  ProcessService
    @brief  Process service
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
             ProcessService( const launcher::Config& config, const tools::Loader_ABC& fileLoader, const LauncherService& server );
    virtual ~ProcessService();
    //@}

    //! @name Operations
    //@{
    void SendExerciseList( sword::ExerciseListResponse& message );
    void SendSessionList( sword::SessionListResponse& message );
    void SendProfileList( sword::ProfileListResponse& message );
    void SendCheckpointList( sword::CheckpointListResponse& message, const std::string& exercice, const std::string& session );
    void RemoveCheckpoint( sword::CheckpointDeleteResponse& message, const std::vector< std::string >& checkpoints,
                           const std::string& exercice, const std::string& session );
    sword::SessionStartResponse::ErrorCode StartSession( const std::string& endpoint, const sword::SessionStartRequest& message );
    sword::SessionStopResponse::ErrorCode StopSession( const sword::SessionStopRequest& message );
    bool IsRunning( const std::string& exercise, const std::string& session ) const;
    void ExecuteCommand( const std::string& endpoint, const sword::SessionCommandExecutionRequest& message );
    void ChangeParameter( const std::string& endpoint, const sword::SessionParameterChangeRequest& message );
    void SendConnectedProfiles( const std::string& endpoint, const sword::ConnectedProfileListRequest& message );
    void SendSessionsStatuses( const std::string& endpoint );
    virtual void NotifyStopped();
    virtual void NotifyError( const std::string& error, std::string commanderEndpoint = "" );
    void Update();
    void SendRunningExercices( const std::string& endpoint ) const;
    void SendSessionNotification( const sword::SessionNotificationRequest& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ExecutePauseResume( const std::string& endpoint, const std::string& exercise, const std::string& session,
                             bool running, int context, SwordFacade& facade );
    void ExecuteChangeTime( const std::string& endpoint, const std::string& exercise, const std::string& session,
                            const std::string& date, int context, SwordFacade& facade );
    void SaveCheckpoint( const std::string& name, SwordFacade& facade );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::pair< std::string, std::string >, boost::shared_ptr< SwordFacade > > ProcessContainer;
    //@}

private:
    //! @name Member data
    //@{
    const launcher::Config& config_;
    const tools::Loader_ABC& fileLoader_;
    const LauncherService& server_;
    ProcessContainer processes_;
    boost::recursive_mutex mutex_;
    //@}
};

}

#endif // __ProcessService_h_

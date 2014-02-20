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
#include <tools/Path.h>
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
class LauncherPublisher;
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
    bool IsRunning( const tools::Path& exercise, const tools::Path& session ) const;
    void SendSessionsStatuses( const std::string& endpoint );
    virtual void NotifyStopped();
    virtual void NotifyError( const std::string& error, const std::string& commanderEndpoint = "" );
    void Update();
    void SendRunningExercices( const std::string& endpoint ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< std::pair< tools::Path, tools::Path >, boost::shared_ptr< SwordFacade > > ProcessContainer;
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

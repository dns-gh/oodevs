// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************


#ifndef __LAUNCHER_TEST_APP_RESPONSEHANDLER_H__
#define __LAUNCHER_TEST_APP_RESPONSEHANDLER_H__

#include "frontend/ResponseHandler_ABC.h"
#include <iosfwd>

namespace boost
{
    class mutex;
    class condition_variable;
}
namespace sword
{
    class ExerciseListResponse;
    class SessionStartResponse;
    class SessionStopResponse;
    class ProfileListResponse;
    class SessionCommandExecutionResponse;
    class SessionNotification;
    class SessionParameterChangeResponse;
    class SessionStatus;
    class ConnectedProfileListResponse;
}
namespace launcher_test_app
{

// =============================================================================
/** @class  ConnectionHandler
    @brief  ConnectionHandler
*/
// Created: AHC 2011-05-27
// =============================================================================
class ResponseHandler : public frontend::ResponseHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ResponseHandler( std::ostream& os, boost::mutex& mutex, boost::condition_variable& cond );
    virtual ~ResponseHandler();
    //@}

    //! @name Accessors
    //@{
    int MessagesReceived() const { return messagesReceived_; }
    //@}
    //! @name Operations
    //@{
    void Handle( const sword::ExerciseListResponse& message );
    void Handle( const sword::SessionStartResponse& message );
    void Handle( const sword::SessionStopResponse& message );
    void Handle( const sword::ProfileListResponse& message );
    void Handle( const sword::SessionCommandExecutionResponse& message );
    void Handle( const sword::SessionNotification& message );
    void Handle( const sword::SessionParameterChangeResponse& message );
    void Handle( const sword::SessionStatus& message );
    void Handle( const sword::ConnectedProfileListResponse& message );
    void Handle( const sword::SessionListResponse& message );
    void Handle( const sword::CheckpointListResponse& message );
    void Handle( const sword::CheckpointDeleteResponse& message );
    //@}

private:
   void Notify();

private:
   std::ostream& os_;
   boost::mutex& mutex_;
   boost::condition_variable& cond_;
   int messagesReceived_;
};

}

#endif // __LAUNCHER_TEST_APP_RESPONSEHANDLER_H__

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __RemoteHost_h_
#define __RemoteHost_h_

#include "ExerciseIdentifierFactory_ABC.h"
#include "ResponseHandler_ABC.h"
#include "Host_ABC.h"
#include <boost/shared_ptr.hpp>
#include <map>

namespace kernel
{
    class Controller;
}

namespace frontend
{
    class Exercise_ABC;
    class LauncherPublisher;

// =============================================================================
/** @class  RemoteHost
    @brief  RemoteHost
*/
// Created: SBO 2010-10-21
// =============================================================================
class RemoteHost : public Host_ABC
                 , private ExerciseIdentifierFactory_ABC
                 , public ResponseHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteHost( LauncherPublisher& publisher, const std::string& host, kernel::Controller& controller );
    virtual ~RemoteHost();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetId() const;
    virtual void StartSimulation( const tools::Path& exercise, const tools::Path& session ) const;
    virtual void StartDispatcher( const tools::Path& exercise, const tools::Path& session, const T_Parameters& parameters ) const;
    virtual void StartReplay( const tools::Path& exercise, const tools::Path& session ) const;
    virtual void StopSession( const tools::Path& exercise, const tools::Path& session ) const;
    //@}

    //! @name Operation
    //@{
    virtual void Handle( const sword::ExerciseListResponse& message );
    virtual void Handle( const sword::SessionStartResponse& message );
    virtual void Handle( const sword::SessionStopResponse& message );
    virtual void Handle( const sword::SessionListResponse& message );
    virtual void Handle( const sword::SessionNotification& message );
    virtual void Handle( const sword::SessionParameterChangeResponse& message );
    virtual void Handle( const sword::SessionStatus& message );
    virtual void Handle( const sword::CheckpointListResponse& message );
    virtual void Handle( const sword::CheckpointDeleteResponse& message );
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string CreateIdentifier( const tools::Path& exercise ) const;
    //@}

private:
    //! @name Member data
    //@{
    LauncherPublisher& publisher_;
    const std::string id_;
    kernel::Controller& controller_;
    std::map< tools::Path, boost::shared_ptr< Exercise_ABC > > exercises_;
    //@}
};

}

#endif // __RemoteHost_h_

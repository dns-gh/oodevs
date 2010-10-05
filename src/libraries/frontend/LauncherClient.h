// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LauncherClient_h_
#define __LauncherClient_h_

#include "tools/ClientNetworker.h"

namespace MsgsLauncherToAdmin
{
    class MsgLauncherToAdmin;
}

namespace kernel
{
    class Controller;
}

namespace frontend
{
    class Exercises;
    class LauncherPublisher;

// =============================================================================
/** @class  LauncherClient
    @brief  LauncherClient
*/
// Created: SBO 2010-09-29
// =============================================================================
class LauncherClient : public tools::ClientNetworker
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LauncherClient( kernel::Controller& controller );
    virtual ~LauncherClient();
    //@}

    //! @name Operations
    //@{
    void Connect( const std::string& host, unsigned int port );
    bool Connected() const;
    void QueryExerciseList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LauncherClient( const LauncherClient& );            //!< Copy constructor
    LauncherClient& operator=( const LauncherClient& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionError( const std::string& address, const std::string& error );
    void HandleLauncherToAdmin( const std::string& endpoint, const MsgsLauncherToAdmin::MsgLauncherToAdmin& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    std::auto_ptr< Exercises > exercises_;
    std::auto_ptr< LauncherPublisher > publisher_;
    bool connected_;
    //@}
};

}

#endif // __LauncherClient_h_

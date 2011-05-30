// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************


#ifndef __LAUNCHER_TEST_APP_APP_H__
#define __LAUNCHER_TEST_APP_APP_H__

#include <string>
#include <list>
#include <fstream>
#include <boost/thread.hpp>
#include <qapplication.h>

#include "clients_kernel/Controllers.h"
#include "frontend/LauncherClient.h"
#include "frontend/LauncherPublisher.h"

#include "ConnectionHandler.h"

namespace frontend
{
    class ResponseHandler_ABC;
}

namespace google
{
namespace protobuf
{
    class Message;
}
}

namespace xml
{
    class xistream;
}
namespace launcher_test_app
{
    class ResponseHandler;

// =============================================================================
/** @class  App
    @brief  App
*/
// Created: AHC 2011-05-27
// =============================================================================
class App
{
public:
    //! @name Constructors/Destructor
    //@{
    App( int argc, char* argv[] );
    virtual ~App();
    //@}

    //! @name Operations
    //@{
    void Connect();
    // use Wait(-1) for infinte wait
    void Wait(int messages = 1);
    template <typename M> void Send(M& msg)
    {
        client_.Send( client_.Endpoint(), msg );
    }
    void Send(google::protobuf::Message* msg);
    void ProcessMessages();
    //@}

private:
    void ReadMessage(xml::xistream& xis);

private:
    std::string host_;
    unsigned int port_;
    boost::mutex mutex_;
    boost::condition_variable cond_;
    QApplication app;
    std::ofstream outputFile_;
    ConnectionHandler connectionHandler_;
    boost::shared_ptr<ResponseHandler> responseHandler_;
    kernel::Controllers controllers_;
    frontend::LauncherClient client_;
    std::auto_ptr< boost::thread > updateThread_;
    std::list< boost::shared_ptr< google::protobuf::Message > > messages_;
};

}

#endif // __LAUNCHER_TEST_APP_APP_H__

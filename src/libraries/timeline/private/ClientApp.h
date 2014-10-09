// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef CLIENT_APP_H__
#define CLIENT_APP_H__

#pragma warning( push, 0 )
#include <cef_app.h>
#pragma warning( pop )

#include <boost/noncopyable.hpp>

namespace timeline
{
    class Engine;
namespace controls
{
    struct ClientHandler_ABC;
}
}

namespace timeline
{
class ClientApp : public CefApp
                , public CefRenderProcessHandler
                , public boost::noncopyable
{
public:
             ClientApp( controls::ClientHandler_ABC& handler, CefRefPtr< Engine > );
    virtual ~ClientApp();

    /// CefApp methods
    virtual CefRefPtr< CefRenderProcessHandler > GetRenderProcessHandler();

    /// CefRenderProcessHandler methods
    virtual void OnContextCreated( CefRefPtr< CefBrowser > browser,
                                   CefRefPtr< CefFrame > frame,
                                   CefRefPtr< CefV8Context > context );
    virtual void OnContextReleased( CefRefPtr< CefBrowser > browser,
                                    CefRefPtr< CefFrame > frame,
                                    CefRefPtr< CefV8Context > context );
    virtual bool OnProcessMessageReceived( CefRefPtr< CefBrowser > browser,
                                           CefProcessId pid,
                                           CefRefPtr< CefProcessMessage > message );

protected:
    IMPLEMENT_REFCOUNTING( ClientApp );
    IMPLEMENT_LOCKING( ClientApp );

private:
    controls::ClientHandler_ABC& handler_;
    CefRefPtr< Engine > engine_;
};

}

#endif

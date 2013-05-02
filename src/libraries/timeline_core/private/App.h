// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef APP_H__
#define APP_H__

#pragma warning( push, 0 )
#include <cef_app.h>
#pragma warning( pop )

namespace timeline
{
namespace core
{
    struct Configuration;
    class Engine;
}
}

namespace timeline
{
namespace core
{
class App : public CefApp
          , public CefRenderProcessHandler
{
public:
             App( const Configuration& cfg );
    virtual ~App();

    /// CefApp methods
    virtual CefRefPtr< CefRenderProcessHandler > GetRenderProcessHandler();

    /// CefRenderProcessHandler methods
    virtual void OnContextCreated( CefRefPtr< CefBrowser >   browser,
                                   CefRefPtr< CefFrame >     frame,
                                   CefRefPtr< CefV8Context > context );

protected:
    IMPLEMENT_REFCOUNTING( App );

private:
    CefRefPtr< Engine > engine_;
};
}
}

#endif//CLIENT_H__

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
    explicit App( const Configuration& cfg, CefRefPtr< Engine > engine );
    virtual ~App();

    /// CefApp methods
    virtual void OnBeforeCommandLineProcessing( const CefString& process_type,
                                                CefRefPtr< CefCommandLine > command_line );
    virtual CefRefPtr< CefRenderProcessHandler > GetRenderProcessHandler();

    /// CefRenderProcessHandler methods
    virtual void OnContextCreated(  CefRefPtr< CefBrowser >   browser,
                                    CefRefPtr< CefFrame >     frame,
                                    CefRefPtr< CefV8Context > context );
    virtual void OnContextReleased( CefRefPtr< CefBrowser >   browser,
                                    CefRefPtr< CefFrame >     frame,
                                    CefRefPtr< CefV8Context > context );

protected:
    IMPLEMENT_REFCOUNTING( App );
    IMPLEMENT_LOCKING( App );

private:
    CefRefPtr< Engine > engine_;
};
}
}

#endif//CLIENT_H__

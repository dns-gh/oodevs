// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef SERVER_APP_H__
#define SERVER_APP_H__

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <cef_app.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <tools/Path.h>

#include <boost/noncopyable.hpp>

namespace timeline
{
// ServerApp wraps:
// * CefApp: CEF entry point, given to CefInitialize. Allows access to
//   capability queries, like GetBrowserProcessHandler(), which enable us to
//   return our custom CefBrowserProcessHandler
// * CefBrowserProcessHandler: Allow customizing the command line arguments
//   given to CEF sub-processes
//   Note that this is a common pattern, the CefApp returns itself on
//   capabilities, like CefBrowserProcessHandler or CefRenderProcessHandler
class ServerApp : public CefApp
                , public CefBrowserProcessHandler
                , public boost::noncopyable
{
public:
             ServerApp( int debug_port,
                        const tools::Path& client_log,
                        const tools::Path& cef_log );
    virtual ~ServerApp();

    /// CefApp methods
    virtual void OnBeforeCommandLineProcessing( const CefString& process_type,
                                                CefRefPtr< CefCommandLine > command_line );
    virtual CefRefPtr< CefBrowserProcessHandler > GetBrowserProcessHandler();

    /// CefBrowserProcessHandler methods
    virtual void OnBeforeChildProcessLaunch( CefRefPtr< CefCommandLine > command_line );

protected:
    IMPLEMENT_REFCOUNTING( ServerApp );
    IMPLEMENT_LOCKING( ServerApp );

private:
    const tools::Path client_log_;
};

}

#endif

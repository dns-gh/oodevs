// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef BROWSER_H__
#define BROWSER_H__

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <cef_client.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif
#include <boost/noncopyable.hpp>

namespace timeline
{
    class Server;
}

namespace timeline
{
// Browser wraps a CEF browser object, and manage its life span through the
// CefLifeSpanHandler. This class is responsible for creating a cef browser
// window, resizing this window, and pumping messages to/from the renderer
// process
class Browser : public boost::noncopyable
              , public CefClient
              , public CefLifeSpanHandler
{
public:
             Browser( Server& server, HWND hwnd, const std::string& url );
    virtual ~Browser();

    /// CefClient methods
    virtual CefRefPtr< CefLifeSpanHandler > GetLifeSpanHandler();
    virtual bool OnProcessMessageReceived( CefRefPtr< CefBrowser >,
                                           CefProcessId,
                                           CefRefPtr< CefProcessMessage > message );

    /// CefLifeSpanHandler methods
    virtual void OnAfterCreated( CefRefPtr< CefBrowser > browser );
    virtual void OnBeforeClose ( CefRefPtr< CefBrowser > browser );

    /// Public methods
    void Start();
    void Resize();
    void Reload();
    void Load( const std::string& url );
    void Post( CefRefPtr< CefProcessMessage > msg );

protected:
    IMPLEMENT_REFCOUNTING( Browser );
    IMPLEMENT_LOCKING( Browser );

private:
    const HWND hwnd_;
    Server& server_;
    std::string url_;
    std::string load_;
    CefRefPtr< CefBrowser > cef_;
};
}

#endif

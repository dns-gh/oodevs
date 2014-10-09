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
namespace core
{
    class Engine;
}
}

namespace timeline
{
namespace core
{
class Browser : public boost::noncopyable
              , public CefClient
              , public CefLifeSpanHandler
{
public:
             Browser( HWND hwnd, const std::string& url );
    virtual ~Browser();
    static CefRefPtr< Browser > Factory( HWND hwnd, const std::string& url );

    /// CefClient methods
    virtual CefRefPtr< CefLifeSpanHandler > GetLifeSpanHandler();

    /// CefLifeSpanHandler methods
    virtual void OnAfterCreated( CefRefPtr< CefBrowser > browser );
    virtual void OnBeforeClose ( CefRefPtr< CefBrowser > browser );

    /// Public methods
    void Start();
    void UpdateSize();
    void Reload();
    void Load( const std::string& url );

protected:
    IMPLEMENT_REFCOUNTING( Browser );
    IMPLEMENT_LOCKING( Browser );

private:
    const HWND hwnd_;
    std::string url_;
    std::string load_;
    CefRefPtr< CefBrowser > cef_;
};
}
}

#endif//CLIENT_H__

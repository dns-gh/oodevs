// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Browser.h"
#include "Engine.h"
#include <timeline/api.h>

using namespace timeline::core;

Browser::Browser( HWND hwnd, const std::string& url )
    : hwnd_( hwnd )
    , url_ ( url )
{
    // NOTHING
}

CefRefPtr< Browser > Browser::Factory( HWND hwnd, const std::string& url )
{
    return new Browser( hwnd, url );
}

void Browser::Start()
{
    CefWindowInfo info;
    info.SetAsChild( hwnd_, RECT() );
    CefBrowserHost::CreateBrowser( info,
        static_cast< CefRefPtr< CefClient > >( this ),
        std::string(), CefBrowserSettings() );
}

Browser::~Browser()
{
    // NOTHING
}

CefRefPtr< CefLifeSpanHandler > Browser::GetLifeSpanHandler()
{
    return this;
}

void Browser::OnAfterCreated( CefRefPtr< CefBrowser > cef )
{
    std::string url;
    {
        AutoLock lock( this );
        cef_ = cef;
        url = url_;
    }
    UpdateSize();
    Load( url );
}

void Browser::OnBeforeClose( CefRefPtr< CefBrowser > )
{
    AutoLock lock( this );
    cef_ = 0;
}

void Browser::UpdateSize()
{
    RECT rect;
    const bool valid = !!GetClientRect( hwnd_, &rect );
    if( !valid )
        return;
    AutoLock lock( this );
    if( !cef_ )
        return;
    auto hwnd = cef_->GetHost()->GetWindowHandle();
    HDWP hdwp = BeginDeferWindowPos( 1 );
    hdwp = DeferWindowPos( hdwp, hwnd, NULL, rect.left, rect.top,
        rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER );
    EndDeferWindowPos( hdwp );
}

void Browser::Load( const std::string& url )
{
    if( url.empty() )
        return;
    AutoLock lock( this );
    url_ = url;
    if( !cef_ || url_ == load_ )
        return;
    cef_->GetMainFrame()->LoadURL( url_.c_str() );
    load_ = url_;
}

void Browser::Reload()
{
    AutoLock lock( this );
    if( cef_ )
        cef_->Reload();
}

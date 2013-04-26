// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Context.h"

#include <tools/IpcDevice.h>

#pragma warning( push, 0 )
#include <cef_app.h>
#include <cef_client.h>
#pragma warning( pop )
#include <boost/thread.hpp>

using namespace timeline::core;
namespace ipc = tools::ipc;

namespace
{
    HWND GetHwnd( int value )
    {
        const HWND hwnd = reinterpret_cast< HWND >( value );
        if( !IsWindow( hwnd ) )
            throw std::exception( "invalid hwnd value" );
        return hwnd;
    }
}

struct Context::Handler : public boost::noncopyable
                       , public CefClient
                       , public CefLifeSpanHandler
{
    Handler( ipc::Device& device, HWND hwnd, const std::string& url )
        : device_ ( device )
        , hwnd_   ( hwnd )
        , aborted_( false )
        , width_  ( 0 )
        , height_ ( 0 )
        , load_   ( url )
    {
        gui_.reset( new boost::thread( boost::bind( &Handler::Run, this ) ) );
    }

    virtual ~Handler()
    {
        Abort();
        gui_->join();
    }

    void Abort()
    {
        AutoLock lock( this );
        aborted_ = true;
    }

    bool IsFinished()
    {
        AutoLock lock( this );
        return aborted_;
    }

    virtual CefRefPtr< CefLifeSpanHandler > GetLifeSpanHandler()
    {
        return this;
    }

    virtual void OnAfterCreated( CefRefPtr< CefBrowser > browser )
    {
        std::string url;
        int width, height;
        Update();
        {
            AutoLock lock( this );
            browser_ = browser;
            url = load_;
            width = width_;
            height = height_;
        }
        Load( url );
        Resize( width, height );
    }

    virtual void OnBeforeClose( CefRefPtr< CefBrowser > browser )
    {
        AutoLock lock( this );
        browser_ = 0;
    }

    void Resize( int width, int height )
    {
        AutoLock lock( this );
        width_ = width;
        height_ = height;
        if( !browser_ )
            return;
        HDWP hdwp = BeginDeferWindowPos( 1 );
        hdwp = DeferWindowPos( hdwp, browser_->GetWindowHandle(), NULL, 0, 0, width_, height_, SWP_NOZORDER );
        EndDeferWindowPos( hdwp );
    }

    void Load( const std::string& url )
    {
        if( url.empty() )
            return;
        AutoLock lock( this );
        url_ = url;
        if( !browser_ || url_ == load_ )
            return;
        browser_->GetMainFrame()->LoadURL( url_.c_str() );
        load_ = url_;
    }

    void Update()
    {
        RECT rect;
        const bool valid = !!GetClientRect( hwnd_, &rect );
        if( valid )
            Resize( rect.right - rect.left, rect.bottom - rect.top );
    }

    void Run()
    {
        std::vector< char > buffer( ipc::DEFAULT_MAX_PACKET_SIZE );
        while( !IsFinished() )
            if( device_.Read( &buffer[0], buffer.size(), boost::posix_time::milliseconds( 50 ) ) )
                Update();
    }

protected:
    IMPLEMENT_REFCOUNTING( Handler );
    IMPLEMENT_LOCKING( Handler );

private:
    ipc::Device& device_;
    bool aborted_;
    const HWND hwnd_;
    int width_;
    int height_;
    std::string url_;
    std::string load_;
    CefRefPtr< CefBrowser > browser_;
    std::auto_ptr< boost::thread > gui_;
};

Context::Context( const Configuration& cfg )
    : cfg_    ( cfg )
    , device_ ( new ipc::Device( cfg_.uuid, false, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , handler_( new Handler( *device_, GetHwnd( cfg_.wid ), cfg_.url ) )
{
    CefWindowInfo info;
    info.SetAsChild( GetHwnd( cfg_.wid ), RECT() );
    CefBrowser::CreateBrowser( info,
        static_cast< CefRefPtr< CefClient > >( handler_ ),
        cfg_.url.c_str(),
        CefBrowserSettings() );
    handler_->Load( cfg_.url );
}

std::auto_ptr< Context_ABC > timeline::core::MakeContext( const Configuration& cfg )
{
    return std::auto_ptr< Context_ABC >( new Context( cfg ) );
}

Context::~Context()
{
    // NOTHING
}

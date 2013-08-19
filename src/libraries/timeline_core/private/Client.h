// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef CLIENT_H__
#define CLIENT_H__

#include <timeline_core/api.h>
#include <controls/controls.h>
#pragma warning( push, 0 )
#include <cef_base.h>
#pragma warning( pop )

namespace tools
{
namespace ipc
{
    class Device;
}
}

namespace timeline
{
namespace core
{
    class App;
    class Browser;
    class Engine;
}
}

namespace timeline
{
namespace core
{
class Client : public Client_ABC
             , public controls::ClientHandler_ABC
{
public:
    explicit Client( const Configuration& cfg );
    virtual ~Client();

    /// Client_ABC methods
    virtual int Run();

    /// controls::Handler_ABC methods
    virtual void OnResizeClient();
    virtual void OnQuitClient();
    virtual void OnReloadClient();
    virtual void OnCenterClient();
    virtual void OnLoadClient( const std::string& url );
    virtual void OnCreateEvent( const Event& event );
    virtual void OnReadEvents();
    virtual void OnReadEvent( const std::string& uuid );
    virtual void OnUpdateEvent( const Event& event );
    virtual void OnDeleteEvent( const std::string& uuid );
    virtual void OnLoadEvents( const std::string& events );
    virtual void OnSaveEvents();

private:
    const Configuration cfg_;
    std::auto_ptr< tools::ipc::Device > read_;
    std::auto_ptr< tools::ipc::Device > write_;
    CefRefPtr< Engine > engine_;
    CefRefPtr< App > app_;
    CefRefPtr< Browser > browser_;
    bool quit_;
};
}
}

#endif//CLIENT_H__

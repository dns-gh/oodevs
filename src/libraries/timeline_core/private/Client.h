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

namespace boost
{
    class mutex;
}

namespace tools
{
    class Ofstream;
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
    virtual void OnUpdateQuery( const std::map< std::string, std::string >& query );
    virtual void OnCreateEvents( const Events& events );
    virtual void OnSelectEvent( const std::string& uuid );
    virtual void OnReadEvents();
    virtual void OnReadEvent( const std::string& uuid );
    virtual void OnUpdateEvent( const Event& event );
    virtual void OnDeleteEvents( const std::vector< std::string >& uuids );
    virtual void OnLoadEvents( const std::string& events );
    virtual void OnSaveEvents();

private:
    virtual void Log( const std::string& msg, bool read );

private:
    const Configuration cfg_;
    std::unique_ptr< boost::mutex > lock_;
    std::unique_ptr< tools::Ofstream > log_;
    std::unique_ptr< tools::ipc::Device > read_;
    std::unique_ptr< tools::ipc::Device > write_;
    CefRefPtr< Engine > engine_;
    CefRefPtr< App > app_;
    CefRefPtr< Browser > browser_;
    bool quit_;
};
}
}

#endif//CLIENT_H__

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

#include <timeline/api.h>
#include <controls/controls.h>
#pragma warning( push, 0 )
#include <internal/cef_ptr.h>
#pragma warning( pop )

namespace timeline
{
    class ClientApp;
    class Engine;
}

namespace boost
{
    class mutex;
}

namespace tools
{
    class Ofstream;
}

namespace timeline
{
class Client : public controls::ClientHandler_ABC
{
public:
             Client( int argc, const char* argv[] );
    virtual ~Client();

    /// public methods
    bool Start();

    /// controls::ClientHandler_ABC methods
    virtual void OnCenterClient();
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
    virtual void Log( const std::string& msg );
    
private:
    const tools::Path client_log_;
    const controls::T_Logger logger_;
    std::unique_ptr< boost::mutex > lock_;
    std::unique_ptr< tools::Ofstream > log_;
    CefRefPtr< Engine > engine_;
    CefRefPtr< ClientApp > app_;
};
}

#endif//CLIENT_H__

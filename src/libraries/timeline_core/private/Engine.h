// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef ENGINE_H__
#define ENGINE_H__

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <cef_v8.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif
#include <boost/noncopyable.hpp>
#include <functional>

namespace tools
{
namespace ipc
{
    class Device;
}
}

namespace timeline
{
    struct Event;
    struct Error;
    typedef std::vector< Event > Events;
}

namespace timeline
{
namespace core
{
class Engine : public boost::noncopyable
{
public:
    typedef std::function< void( const std::string& ) > T_Logger;
             Engine( tools::ipc::Device& device, const T_Logger& log );
    virtual ~Engine();

    /// Public methods
    void Register   ( CefRefPtr< CefV8Context > context );
    void Unregister ();
    void CenterClient();
    void UpdateQuery( const std::map< std::string, std::string >& query );
    void CreateEvents( const Events& events );
    void SelectEvent( const std::string& uuid );
    void ReadEvents ();
    void ReadEvent  ( const std::string& uuid );
    void UpdateEvent( const Event& event );
    void DeleteEvents( const std::vector< std::string >& uuids );
    void LoadEvents( const std::string& events );
    void SaveEvents();

private:
    void SendCreatedEvents( const Events& events, const Error& err );
    void SendReadEvents   ( const Events& events, const Error& err );
    void SendReadEvent    ( const Event& event, const Error& err );
    void SendUpdatedEvent ( const Event& event, const Error& err );
    void SendDeletedEvents( const std::vector< std::string >& uuids, const Error& err );
    void SendLoadedEvents ( const Error& err );
    void SendSavedEvents  ( const std::string& events, const Error& err );

    // V8 handlers
    CefRefPtr< CefV8Value > OnReady                ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnCreatedEvent         ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnCreatedEvents        ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnReadEvents           ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnReadEvent            ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnUpdatedEvent         ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnDeletedEvent         ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnLoadedEvents         ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnSavedEvents          ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnSelectEvent          ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnDeselectEvent        ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnActivateEvent        ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnContextMenuEvent     ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnContextMenuBackground( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnKeyDown              ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnKeyPress             ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnKeyUp                ( const CefV8ValueList& args );

protected:
    IMPLEMENT_REFCOUNTING( Engine );

private:
    const T_Logger log_;
    CefRefPtr< CefV8Context > ctx_;
    tools::ipc::Device& device_;
};
}
}

#endif//ENGINE_H__
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
}

namespace timeline
{
namespace core
{
class Engine : public boost::noncopyable
{
public:
             Engine( tools::ipc::Device& device );
    virtual ~Engine();

    /// Public methods
    void Register   ( CefRefPtr< CefV8Context > context );
    void Unregister ();
    void CreateEvent( const Event& event );
    void DeleteEvent( const std::string& uuid );

private:
    void CreatedEvent( const Event& event, const Error& err );
    void DeletedEvent( const std::string& uuid, const Error& err );

    // V8 handlers
    CefRefPtr< CefV8Value > OnReady        ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnCreatedEvent ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnSelectEvent  ( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnDeselectEvent( const CefV8ValueList& args );
    CefRefPtr< CefV8Value > OnDeletedEvent ( const CefV8ValueList& args );

protected:
    IMPLEMENT_REFCOUNTING( Engine );

private:
    CefRefPtr< CefV8Context > ctx_;
    tools::ipc::Device& device_;
};
}
}

#endif//ENGINE_H__
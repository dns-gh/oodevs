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

namespace timeline
{
    struct Event;
}

namespace timeline
{
namespace core
{
class Engine : public boost::noncopyable
             , public CefV8Handler
{
public:
             Engine();
    virtual ~Engine();

    /// CefV8Handler methods
    bool Execute( const CefString&         name,
                  CefRefPtr< CefV8Value >  object,
                  const CefV8ValueList&    args,
                  CefRefPtr< CefV8Value >& retval,
                  CefString&               exception );

    /// Public methods
    void Register   ( CefRefPtr< CefV8Context > context );
    void Unregister ();
    void CreateEvent( const Event& event );

protected:
    IMPLEMENT_REFCOUNTING( Engine );

private:
    CefRefPtr< CefV8Context > ctx_;
};
}
}

#endif//ENGINE_H__
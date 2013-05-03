// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Engine.h"

#include <timeline/api.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace timeline::core;

Engine::Engine()
{
    // NOTHING
}

Engine::~Engine()
{
    // NOTHING
}

bool Engine::Execute( const CefString&         /*name*/,
                      CefRefPtr< CefV8Value >  /*object*/,
                      const CefV8ValueList&    /*args*/,
                      CefRefPtr< CefV8Value >& /*retval*/,
                      CefString&               /*exception*/ )
{
    return false;
}

namespace
{
    CefRefPtr< CefV8Value > SetValue( CefRefPtr< CefV8Value >& dst, const std::string& key, CefRefPtr< CefV8Value > src )
    {
        dst->SetValue( key, src, V8_PROPERTY_ATTRIBUTE_NONE );
        return src;
    }

    CefRefPtr< CefV8Value > SetValue( CefRefPtr< CefV8Value >& dst, const std::string& key )
    {
        return SetValue( dst, key, CefV8Value::CreateObject( 0 ) );
    }

    CefRefPtr< CefV8Value > SetValue( CefRefPtr< CefV8Value >& dst, const std::string& key, const std::string& value )
    {
        return SetValue( dst, key, CefV8Value::CreateString( value ) );
    }

    CefRefPtr< CefV8Value > SetValue( CefRefPtr< CefV8Value >& dst, const std::string& key, bool value )
    {
        return SetValue( dst, key, CefV8Value::CreateBool( value ) );
    }
}

void Engine::Register( CefRefPtr< CefV8Context > context )
{
    ctx_ = context;
    auto window = context->GetGlobal();
    auto gaming = SetValue( window, "gaming" );
    SetValue( gaming, "enabled", true );
}

void Engine::Unregister()
{
    ctx_ = 0;
}

namespace
{
    struct Gate
    {
        Gate()
        {
            if( !CefCurrentlyOn( TID_RENDERER ) )
                throw std::runtime_error( "expecting renderer thread" );
        }

        ~Gate()
        {
            if( ctx_.get() )
                ctx_->Exit();
        }

        bool Acquire( CefRefPtr< CefV8Context > context )
        {
            if( !context.get() )
                return false;
            frame_ = context->GetFrame();
            url_ = frame_->GetURL();
            if( !context->Enter() )
            {
                Alert( "Unable to enter context" );
                return false;
            }
            ctx_ = context;
            return true;
        }

        void Alert( const std::string& msg )
        {
            frame_->ExecuteJavaScript( "alert('" + msg + "');", url_, 0 );
        }

        bool Execute( CefRefPtr< CefV8Value >& operand, const CefV8ValueList& args )
        {
            return !!operand->ExecuteFunctionWithContext( ctx_, 0, args );
        }

    private:
        CefRefPtr< CefFrame > frame_;
        std::string url_;
        CefRefPtr< CefV8Context > ctx_;
    };

    CefRefPtr< CefV8Value > GetValue( CefRefPtr< CefV8Context >& ctx, const std::string& key )
    {
        std::deque< std::string > tokens;
        boost::algorithm::split( tokens, key, boost::is_any_of( "." ) );
        auto root = ctx->GetGlobal();
        while( root.get() && !tokens.empty() )
        {
            root = root->GetValue( tokens.front() );
            tokens.pop_front();
        }
        return root;
    }

    void SetEvent( CefRefPtr< CefV8Value >& data, const timeline::Event& event )
    {
        if( !event.uuid.empty() )
            SetValue( data, "uuid", event.uuid );
        SetValue( data, "name", event.name );
        SetValue( data, "info", event.info );
        SetValue( data, "begin", event.begin );
        if( !event.end.empty() )
            SetValue( data, "end", event.end );
        SetValue( data, "done", event.done );
        if( event.action.target.empty() )
            return;
        auto action = SetValue( data, "action" );
        SetValue( action, "target", event.action.target );
        SetValue( action, "apply", event.action.apply );
        SetValue( action, "payload", event.action.payload );
    }
}

void Engine::CreateEvent( const timeline::Event& event )
{
    Gate gate;
    if( !gate.Acquire( ctx_ ) )
        return;
    auto create_event = GetValue( ctx_, "gaming.create_event" );
    if( !create_event )
        return gate.Alert( "unable to find gaming.create_event" );
    auto data = CefV8Value::CreateObject( 0 );
    SetEvent( data, event );
    CefV8ValueList args;
    args.push_back( data );
    if( !gate.Execute( create_event, args ) )
        return gate.Alert( "unable to execute create_event" );
}
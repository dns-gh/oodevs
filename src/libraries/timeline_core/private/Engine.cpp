// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Engine.h"

#include "controls/controls.h"
#include "timeline/api.h"

#include <tools/IpcDevice.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

using namespace timeline::core;

Engine::Engine( tools::ipc::Device& device )
    : device_( device )
{
    // NOTHING
}

Engine::~Engine()
{
    // NOTHING
}

namespace
{
    struct Adapter : public CefV8Handler
                   , public boost::noncopyable
    {
        typedef boost::function< CefRefPtr< CefV8Value >( const CefV8ValueList& args ) > T_Operand;

        Adapter( const std::string& name, size_t argc, const T_Operand& operand )
            : name_   ( name )
            , argc_   ( argc )
            , operand_( operand )

        {
            // NOTHING
        }

        ~Adapter()
        {
            // NOTHING
        }

        bool Execute( const CefString&         name,
                      CefRefPtr< CefV8Value >  /*object*/,
                      const CefV8ValueList&    args,
                      CefRefPtr< CefV8Value >& retval,
                      CefString&               exception )
        {
            if( name != name_ )
                return false;
            try
            {
                if( argc_ != args.size() )
                    throw std::exception( "invalid number of arguments" );
                retval = operand_( args );
            }
            catch( const std::exception& err )
            {
                exception = err.what();
            }
            return true;
        }

    protected:
        IMPLEMENT_REFCOUNTING( Adapter );

    private:
        const std::string name_;
        const size_t argc_;
        const T_Operand operand_;
    };

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

    CefRefPtr< CefV8Value > SetValue( CefRefPtr< CefV8Value >& dst, const std::string& key, size_t argc, const Adapter::T_Operand& operand )
    {
        CefRefPtr< CefV8Handler > handler( new Adapter( key, argc, operand ) );
        return SetValue( dst, key, CefV8Value::CreateFunction( key, handler ) );
    }

    std::string GetString( const CefRefPtr< CefV8Value > src, const std::string& key )
    {
        if( !src->HasValue( key ) )
            return std::string();
        return src->GetValue( key )->GetStringValue();
    }

    bool GetBool( const CefRefPtr< CefV8Value > src, const std::string& key )
    {
        if( !src->HasValue( key ) )
            return false;
        return src->GetValue( key )->GetBoolValue();
    }

    int GetInteger( const CefRefPtr< CefV8Value > src, const std::string& key )
    {
        if( !src->HasValue( key ) )
            return 0;
        return src->GetValue( key )->GetIntValue();
    }
}

void Engine::Register( CefRefPtr< CefV8Context > context )
{
    ctx_ = context;
    auto window = context->GetGlobal();
    auto gaming = SetValue( window, "gaming" );
    SetValue( gaming, "enabled", true );
    SetValue( gaming, "created_event",  2, boost::bind( &Engine::OnCreatedEvent,  this, _1 ) );
    SetValue( gaming, "select_event",   1, boost::bind( &Engine::OnSelectEvent,   this, _1 ) );
    SetValue( gaming, "deselect_event", 0, boost::bind( &Engine::OnDeselectEvent, this, _1 ) );
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
                return false;
            ctx_ = context;
            return true;
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

    timeline::Event GetEvent( const CefRefPtr< CefV8Value > src )
    {
        timeline::Event dst;
        dst.uuid  = GetString( src, "uuid" );
        dst.name  = GetString( src, "name" );
        dst.info  = GetString( src, "info" );
        dst.begin = GetString( src, "begin" );
        dst.end   = GetString( src, "end" );
        dst.done  = GetBool( src, "done" );
        if( !src->HasValue( "action" ) )
            return dst;
        auto sub       = src->GetValue( "action" );
        auto& action   = dst.action;
        action.target  = GetString( sub, "action.target" );
        action.apply   = GetBool( sub, "action.apply" );
        action.payload = GetString( sub, "action.payload" );
        return dst;
    }

    timeline::Error GetError( const CefRefPtr< CefV8Value > src )
    {
        timeline::Error dst;
        dst.code = GetInteger( src, "code" );
        dst.text = GetString( src, "text" );
        return dst;
    }
}

void Engine::CreateEvent( const timeline::Event& event )
{
    Gate gate;
    if( !gate.Acquire( ctx_ ) )
        return CreatedEvent( event, Error( EC_INTERNAL_SERVER_ERROR, "unable to acquire v8 context" ) );
    auto create_event = GetValue( ctx_, "gaming.create_event" );
    if( !create_event )
        return CreatedEvent( event, Error( EC_INTERNAL_SERVER_ERROR, "unable to find gaming.create_event" ) );
    auto data = CefV8Value::CreateObject( 0 );
    SetEvent( data, event );
    CefV8ValueList args;
    args.push_back( data );
    if( !gate.Execute( create_event, args ) )
        return CreatedEvent( event, Error( EC_INTERNAL_SERVER_ERROR, "unable to execute gaming.create_event" ) );
}

void Engine::CreatedEvent( const timeline::Event& event, const timeline::Error& error )
{
    std::vector< uint8_t > buffer( controls::CreatedEvent( 0, 0, event, error ) );
    controls::CreatedEvent( &buffer[0], buffer.size(), event, error );
    device_.Write( &buffer[0], buffer.size(), boost::posix_time::seconds( 1 ) );
}

CefRefPtr< CefV8Value > Engine::OnCreatedEvent( const CefV8ValueList& args )
{
    CreatedEvent( GetEvent( args[0] ), GetError( args[1] ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnSelectEvent( const CefV8ValueList& args )
{
    const Event event = GetEvent( args[0] );
    std::vector< uint8_t > buffer( controls::SelectedEvent( 0, 0, event ) );
    controls::SelectedEvent( &buffer[0], buffer.size(), event );
    device_.Write( &buffer[0], buffer.size(), boost::posix_time::seconds( 1 ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnDeselectEvent( const CefV8ValueList& /*args*/ )
{
    std::vector< uint8_t > buffer( controls::DeselectedEvent( 0, 0 ) );
    controls::DeselectedEvent( &buffer[0], buffer.size() );
    device_.Write( &buffer[0], buffer.size(), boost::posix_time::seconds( 1 ) );
    return 0;
}
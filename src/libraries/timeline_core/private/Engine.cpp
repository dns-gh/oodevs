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

#include <tools/Base64Converters.h>
#include <tools/IpcDevice.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/bind.hpp>

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
    SetValue( gaming, "ready",                  0, boost::bind( &Engine::OnReady, this, _1 ) );
    SetValue( gaming, "created_event",          2, boost::bind( &Engine::OnCreatedEvent,          this, _1 ) );
    SetValue( gaming, "get_read_events",        2, boost::bind( &Engine::OnReadEvents,            this, _1 ) );
    SetValue( gaming, "get_read_event",         2, boost::bind( &Engine::OnReadEvent,             this, _1 ) );
    SetValue( gaming, "updated_event",          2, boost::bind( &Engine::OnUpdatedEvent,          this, _1 ) );
    SetValue( gaming, "deleted_event",          2, boost::bind( &Engine::OnDeletedEvent,          this, _1 ) );
    SetValue( gaming, "select_event",           1, boost::bind( &Engine::OnSelectEvent,           this, _1 ) );
    SetValue( gaming, "deselect_event",         0, boost::bind( &Engine::OnDeselectEvent,         this, _1 ) );
    SetValue( gaming, "activate_event",         1, boost::bind( &Engine::OnActivateEvent,         this, _1 ) );
    SetValue( gaming, "contextmenu_event",      1, boost::bind( &Engine::OnContextMenuEvent,      this, _1 ) );
    SetValue( gaming, "contextmenu_background", 1, boost::bind( &Engine::OnContextMenuBackground, this, _1 ) );
    SetValue( gaming, "keydown",                1, boost::bind( &Engine::OnKeyDown,               this, _1 ) );
    SetValue( gaming, "keypress",               1, boost::bind( &Engine::OnKeyPress,              this, _1 ) );
    SetValue( gaming, "keyup",                  1, boost::bind( &Engine::OnKeyUp,                 this, _1 ) );
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
        SetValue( action, "payload", tools::BinaryToBase64( event.action.payload ) );
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
        auto action = src->GetValue( "action" );
        if( !action )
            return dst;
        dst.action.target  = GetString( action, "target" );
        dst.action.apply   = GetBool( action, "apply" );
        dst.action.payload = tools::Base64ToBinary( GetString( action, "payload" ) );
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
        return SendCreatedEvent( event, Error( EC_INTERNAL_SERVER_ERROR, "unable to acquire v8 context" ) );
    auto create_event = GetValue( ctx_, "gaming.create_event" );
    if( !create_event )
        return SendCreatedEvent( event, Error( EC_INTERNAL_SERVER_ERROR, "unable to find gaming.create_event" ) );
    auto data = CefV8Value::CreateObject( 0 );
    SetEvent( data, event );
    CefV8ValueList args;
    args.push_back( data );
    if( !gate.Execute( create_event, args ) )
        return SendCreatedEvent( event, Error( EC_INTERNAL_SERVER_ERROR, "unable to execute gaming.create_event" ) );
}

void Engine::SendCreatedEvent( const timeline::Event& event, const timeline::Error& error )
{
    Write( device_, boost::bind( &controls::CreatedEvent, _1, _2, event, error ) );
}

CefRefPtr< CefV8Value > Engine::OnReady( const CefV8ValueList& /*args*/ )
{
    Write( device_, &controls::ReadyServer );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnCreatedEvent( const CefV8ValueList& args )
{
    SendCreatedEvent( GetEvent( args[0] ), GetError( args[1] ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnSelectEvent( const CefV8ValueList& args )
{
    const Event event = GetEvent( args[0] );
    Write( device_, boost::bind( &controls::SelectedEvent, _1, _2, event ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnDeselectEvent( const CefV8ValueList& /*args*/ )
{
    Write( device_, &controls::DeselectedEvent );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnActivateEvent( const CefV8ValueList& args )
{
    const Event event = GetEvent( args[0] );
    Write( device_, boost::bind( &controls::ActivatedEvent, _1, _2, event ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnContextMenuEvent( const CefV8ValueList& args )
{
    const Event event = GetEvent( args[0] );
    Write( device_, boost::bind( &controls::ContextMenuEvent, _1, _2, event ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnContextMenuBackground( const CefV8ValueList& args )
{
    const std::string time = args[0]->GetStringValue();
    Write( device_, boost::bind( &controls::ContextMenuBackground, _1, _2, time ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnKeyDown( const CefV8ValueList& args )
{
    int key = args[0]->GetIntValue();
    Write( device_, boost::bind( &controls::KeyDown, _1, _2, key ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnKeyPress( const CefV8ValueList& args )
{
    int key = args[0]->GetIntValue();
    Write( device_, boost::bind( &controls::KeyPress, _1, _2, key ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnKeyUp( const CefV8ValueList& args )
{
    int key = args[0]->GetIntValue();
    Write( device_, boost::bind( &controls::KeyUp, _1, _2, key ) );
    return 0;
}

void Engine::DeleteEvent( const std::string& uuid )
{
    Gate gate;
    if( !gate.Acquire( ctx_ ) )
        return SendDeletedEvent( uuid, Error( EC_INTERNAL_SERVER_ERROR, "unable to acquire v8 context" ) );
    auto delete_event = GetValue( ctx_, "gaming.delete_event" );
    if( !delete_event )
        return SendDeletedEvent( uuid, Error( EC_INTERNAL_SERVER_ERROR, "unable to find gaming.delete_event" ) );
    CefV8ValueList args;
    args.push_back( CefV8Value::CreateString( uuid ) );
    if( !gate.Execute( delete_event, args ) )
        return SendDeletedEvent( uuid, Error( EC_INTERNAL_SERVER_ERROR, "unable to execute gaming.delete_event" ) );
}

CefRefPtr< CefV8Value > Engine::OnDeletedEvent( const CefV8ValueList& args )
{
    SendDeletedEvent( args[0]->GetStringValue(), GetError( args[1] ) );
    return 0;
}

void Engine::SendDeletedEvent( const std::string& uuid, const timeline::Error& error )
{
    Write( device_, boost::bind( &controls::DeletedEvent, _1, _2, uuid, error ) );
}

void Engine::ReadEvents()
{
    Gate gate;
    if( !gate.Acquire( ctx_ ) )
        return SendReadEvents( Events(), Error( EC_INTERNAL_SERVER_ERROR, "unable to acquire v8 context" ) );
    auto read_events = GetValue( ctx_, "gaming.read_events" );
    if( !read_events )
        return SendReadEvents( Events(), Error( EC_INTERNAL_SERVER_ERROR, "unable to find gaming.read_events" ) );
    if( !gate.Execute( read_events, CefV8ValueList() ) )
        return SendReadEvents( Events(), Error( EC_INTERNAL_SERVER_ERROR, "unable to execute gaming.read_events" ) );
}

CefRefPtr< CefV8Value > Engine::OnReadEvents( const CefV8ValueList& args )
{
    Events rpy;
    const auto& list = args[0];
    for( int i = 0; i < list->GetArrayLength(); ++i )
        rpy.push_back( GetEvent( list->GetValue( i ) ) );
    SendReadEvents( rpy, GetError( args[1] ) );
    return 0;
}

void Engine::SendReadEvents( const timeline::Events& events, const timeline::Error& error )
{
    Write( device_, boost::bind( &controls::ReadEvents, _1, _2, events, error ) );
}

void Engine::ReadEvent( const std::string& uuid )
{
    Gate gate;
    if( !gate.Acquire( ctx_ ) )
        return SendReadEvent( Event(), Error( EC_INTERNAL_SERVER_ERROR, "unable to acquire v8 context" ) );
    auto read_event = GetValue( ctx_, "gaming.read_event" );
    if( !read_event )
        return SendReadEvent( Event(), Error( EC_INTERNAL_SERVER_ERROR, "unable to find gaming.read_event" ) );
    CefV8ValueList args;
    args.push_back( CefV8Value::CreateString( uuid ) );
    if( !gate.Execute( read_event, args ) )
        return SendReadEvent( Event(), Error( EC_INTERNAL_SERVER_ERROR, "unable to execute gaming.read_event" ) );
}

CefRefPtr< CefV8Value > Engine::OnReadEvent( const CefV8ValueList& args )
{
    SendReadEvent( GetEvent( args[0] ), GetError( args[1] ) );
    return 0;
}

void Engine::SendReadEvent( const timeline::Event& event, const timeline::Error& error )
{
    Write( device_, boost::bind( &controls::ReadEvent, _1, _2, event, error ) );
}

void Engine::UpdateEvent( const timeline::Event& event )
{
    Gate gate;
    if( !gate.Acquire( ctx_ ) )
        return SendUpdatedEvent( Event(), Error( EC_INTERNAL_SERVER_ERROR, "unable to acquire v8 context" ) );
    auto update_event = GetValue( ctx_, "gaming.update_event" );
    if( !update_event )
        return SendUpdatedEvent( Event(), Error( EC_INTERNAL_SERVER_ERROR, "unable to find gaming.update event" ) );
    auto data = CefV8Value::CreateObject( 0 );
    SetEvent( data, event );
    CefV8ValueList args;
    args.push_back( data );
    if( !gate.Execute( update_event, args ) )
        return SendUpdatedEvent( Event(), Error( EC_INTERNAL_SERVER_ERROR, "unable to execute gaming.update_event" ) );
}

CefRefPtr< CefV8Value > Engine::OnUpdatedEvent( const CefV8ValueList& args )
{
    SendUpdatedEvent( GetEvent( args[0] ), GetError( args[1] ) );
    return 0;
}

void Engine::SendUpdatedEvent( const timeline::Event& event, const timeline::Error& error )
{
    Write( device_, boost::bind( &controls::UpdatedEvent, _1, _2, event, error ) );
}
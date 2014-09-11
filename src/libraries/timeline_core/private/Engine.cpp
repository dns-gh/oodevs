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

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>

using namespace timeline::core;

Engine::Engine( tools::ipc::Device& device, const T_Logger& log )
    : log_   ( log )
    , device_( device )
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

    CefRefPtr< CefV8Value > AddValue( CefRefPtr< CefV8Value >& dst, int index, CefRefPtr< CefV8Value > src )
    {
        dst->SetValue( index, src );
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

    CefRefPtr< CefV8Value > SetValue( CefRefPtr< CefV8Value >& dst, const std::string& key, int value )
    {
        return SetValue( dst, key, CefV8Value::CreateInt( value ) );
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
    SetValue( gaming, "ready",                  0, boost::bind( &Engine::OnReady,                 this, _1 ) );
    SetValue( gaming, "created_event",          2, boost::bind( &Engine::OnCreatedEvent,          this, _1 ) );
    SetValue( gaming, "created_events",         2, boost::bind( &Engine::OnCreatedEvents,         this, _1 ) );
    SetValue( gaming, "get_read_events",        2, boost::bind( &Engine::OnReadEvents,            this, _1 ) );
    SetValue( gaming, "get_read_event",         2, boost::bind( &Engine::OnReadEvent,             this, _1 ) );
    SetValue( gaming, "updated_event",          2, boost::bind( &Engine::OnUpdatedEvent,          this, _1 ) );
    SetValue( gaming, "deleted_event",          2, boost::bind( &Engine::OnDeletedEvent,          this, _1 ) );
    SetValue( gaming, "loaded_events",          1, boost::bind( &Engine::OnLoadedEvents,          this, _1 ) );
    SetValue( gaming, "saved_events",           2, boost::bind( &Engine::OnSavedEvents,           this, _1 ) );
    SetValue( gaming, "selected_event",         1, boost::bind( &Engine::OnSelectEvent,           this, _1 ) );
    SetValue( gaming, "deselected_event",       0, boost::bind( &Engine::OnDeselectEvent,         this, _1 ) );
    SetValue( gaming, "activated_event",        1, boost::bind( &Engine::OnActivateEvent,         this, _1 ) );
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
        SetValue( data, "end", event.end );
        SetValue( data, "parent", event.parent );
        SetValue( data, "error_text", event.error_text );
        SetValue( data, "error_code", event.error_code );
        SetValue( data, "metadata", event.metadata );
        SetValue( data, "read_only", event.read_only );
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
        dst.uuid   = GetString( src, "uuid" );
        dst.name   = GetString( src, "name" );
        dst.info   = GetString( src, "info" );
        dst.begin  = GetString( src, "begin" );
        dst.end    = GetString( src, "end" );
        dst.parent = GetString( src, "parent" );
        dst.error_text = GetString( src, "error_text" );
        dst.error_code = GetInteger( src, "error_code" );
        dst.metadata = GetString( src, "metadata" );
        dst.read_only  = GetBool( src, "read_only" );
        dst.done       = GetBool( src, "done" );
        auto action = src->GetValue( "action" );
        if( !action )
            return dst;
        dst.action.target  = GetString( action, "target" );
        dst.action.apply   = GetBool( action, "apply" );
        dst.action.payload = tools::Base64ToBinary( GetString( action, "payload" ) );
        return dst;
    }

    timeline::Events GetEvents( const CefRefPtr< CefV8Value > src )
    {
        timeline::Events dst;
        for( int i = 0; i < src->GetArrayLength(); ++i )
            dst.push_back( GetEvent( src->GetValue( i ) ) );
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

void Engine::CenterClient()
{
    Gate gate;
    if( gate.Acquire( ctx_ ) )
        if( auto center = GetValue( ctx_, "gaming.center_view" ) )
            gate.Execute( center, CefV8ValueList() );
}

void Engine::UpdateQuery( const std::map< std::string, std::string >& parameters )
{
    Gate gate;
    if( gate.Acquire( ctx_ ) )
    {
        auto data = CefV8Value::CreateObject( 0 );
        for( auto it = parameters.begin(); it != parameters.end(); ++it )
            SetValue( data, it->first, it->second );

        auto update_query = GetValue( ctx_, "gaming.update_query" );
        CefV8ValueList args;
        args.push_back( data );
        gate.Execute( update_query, args );
    }
}

void Engine::CreateEvents( const timeline::Events& events )
{
    Gate gate;
    if( !gate.Acquire( ctx_ ) )
        return SendCreatedEvents( events, Error( EC_INTERNAL_SERVER_ERROR, "unable to acquire v8 context" ) );
    auto create_events = GetValue( ctx_, "gaming.create_events" );
    if( !create_events )
        return SendCreatedEvents( events, Error( EC_INTERNAL_SERVER_ERROR, "unable to find gaming.create_events" ) );
    auto data = CefV8Value::CreateArray( events.size() );
    size_t idx = 0;
    for( auto it = events.begin(); it != events.end(); ++it )
    {
        auto v = AddValue( data, idx++, CefV8Value::CreateObject( 0 ) );
        SetEvent( v, *it );
    }
    CefV8ValueList args;
    args.push_back( data );
    if( !gate.Execute( create_events, args ) )
        return SendCreatedEvents( events, Error( EC_INTERNAL_SERVER_ERROR, "unable to execute gaming.create_event" ) );
}

void Engine::SelectEvent( const std::string& uuid )
{
    Gate gate;
    if( gate.Acquire( ctx_ ) )
    {
        CefV8ValueList args;
        args.push_back( CefV8Value::CreateString( uuid ) );
        if( auto select = GetValue( ctx_, "gaming.select_event" ) )
            gate.Execute( select, args );
    }
}

void Engine::SendCreatedEvents( const timeline::Events& events, const timeline::Error& error )
{
    controls::CreatedEvents( device_, log_, events, error );
}

CefRefPtr< CefV8Value > Engine::OnReady( const CefV8ValueList& /*args*/ )
{
    controls::ReadyServer( device_, log_ );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnCreatedEvent( const CefV8ValueList& args )
{
    SendCreatedEvents( boost::assign::list_of( GetEvent( args[0] ) ), GetError( args[1] ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnCreatedEvents( const CefV8ValueList& args )
{
    SendCreatedEvents( GetEvents( args[0] ), GetError( args[1] ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnSelectEvent( const CefV8ValueList& args )
{
    controls::SelectedEvent( device_, log_, GetEvent( args[0] ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnDeselectEvent( const CefV8ValueList& /*args*/ )
{
    controls::DeselectedEvent( device_, log_ );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnActivateEvent( const CefV8ValueList& args )
{
    controls::ActivatedEvent( device_, log_, GetEvent( args[0] ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnContextMenuEvent( const CefV8ValueList& args )
{
    controls::ContextMenuEvent( device_, log_, GetEvent( args[0] ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnContextMenuBackground( const CefV8ValueList& args )
{
    controls::ContextMenuBackground( device_, log_, args[0]->GetStringValue() );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnKeyDown( const CefV8ValueList& args )
{
    controls::KeyDown( device_, log_, args[0]->GetIntValue() );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnKeyPress( const CefV8ValueList& args )
{
    controls::KeyPress( device_, log_, args[0]->GetIntValue() );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnKeyUp( const CefV8ValueList& args )
{
    controls::KeyUp( device_, log_, args[0]->GetIntValue() );
    return 0;
}

void Engine::DeleteEvents( const std::vector< std::string >& uuids )
{
    Gate gate;
    if( !gate.Acquire( ctx_ ) )
        return SendDeletedEvents( uuids, Error( EC_INTERNAL_SERVER_ERROR, "unable to acquire v8 context" ) );
    auto delete_events = GetValue( ctx_, "gaming.delete_events" );
    if( !delete_events )
        return SendDeletedEvents( uuids, Error( EC_INTERNAL_SERVER_ERROR, "unable to find gaming.delete_events" ) );
    auto data = CefV8Value::CreateArray( uuids.size() );
    size_t idx = 0;
    for( auto it = uuids.begin(); it != uuids.end(); ++it )
        AddValue( data, idx++, CefV8Value::CreateString( *it ) );
    CefV8ValueList args;
    args.push_back( data );
    if( !gate.Execute( delete_events, args ) )
        return SendDeletedEvents( uuids, Error( EC_INTERNAL_SERVER_ERROR, "unable to execute gaming.delete_events" ) );
}

void Engine::LoadEvents( const std::string& events )
{
    Gate gate;
    if( !gate.Acquire( ctx_ ) )
        return SendLoadedEvents( Error( EC_INTERNAL_SERVER_ERROR, "unable to acquire v8 context" ) );
    auto load_events = GetValue( ctx_, "gaming.load_events" );
    if( !load_events )
        return SendLoadedEvents( Error( EC_INTERNAL_SERVER_ERROR, "unable to find gaming.load_events" ) );
    CefV8ValueList args;
    args.push_back( CefV8Value::CreateString( events ) );
    if( !gate.Execute( load_events, args ) )
        return SendLoadedEvents( Error( EC_INTERNAL_SERVER_ERROR, "unable to execute gaming.load_events" ) );
}

void Engine::SaveEvents()
{
    Gate gate;
    if( !gate.Acquire( ctx_ ) )
        return SendSavedEvents( std::string(), Error( EC_INTERNAL_SERVER_ERROR, "unable to acquire v8 context" ) );
    auto save_events = GetValue( ctx_, "gaming.save_events" );
    if( !save_events )
        return SendSavedEvents( std::string(), Error( EC_INTERNAL_SERVER_ERROR, "unable to find gaming.save_events" ) );
    if( !gate.Execute( save_events, CefV8ValueList() ) )
        return SendSavedEvents( std::string(), Error( EC_INTERNAL_SERVER_ERROR, "unable to execute gaming.save_events" ) );
}

CefRefPtr< CefV8Value > Engine::OnDeletedEvent( const CefV8ValueList& args )
{
    SendDeletedEvents( boost::assign::list_of( args[0]->GetStringValue() ), GetError( args[1] ) );
    return 0;
}

void Engine::SendDeletedEvents( const std::vector< std::string >& uuids, const timeline::Error& error )
{
    controls::DeletedEvents( device_, log_, uuids, error );
}

void Engine::SendLoadedEvents( const timeline::Error& err )
{
    controls::LoadedEvents( device_, log_, err );
}

void Engine::SendSavedEvents( const std::string& events, const timeline::Error& err )
{
    controls::SavedEvents( device_, log_, events, err );
}

CefRefPtr< CefV8Value > Engine::OnLoadedEvents( const CefV8ValueList& args )
{
    SendLoadedEvents( GetError( args[0] ) );
    return 0;
}

CefRefPtr< CefV8Value > Engine::OnSavedEvents( const CefV8ValueList& args )
{
    SendSavedEvents( args[0]->GetStringValue(), GetError( args[1] ) );
    return 0;
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
    controls::ReadEvents( device_, log_, events, error );
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
    controls::ReadEvent( device_, log_, event, error );
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
    controls::UpdatedEvent( device_, log_, event, error );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_TESTER_API_FIXTURE_H
#define MODULE_TESTER_API_FIXTURE_H

#include "MakeModel.h"
#include "Tools.h"
#include "MockModuleApi.h"
#include <module_api/Version.h>
#include <module_api/Module.h>
#include <core/ModuleLoader.h>
#include <core/Commands.h>
#include <core/Module.h>
#include <core/Convert.h>
#include <core/Model.h>
#include <core/Hooks.h>
#include <core/Logger_ABC.h>
#include <core/MakeModel.h>
#include <core/NamedChildrenVisitor.h>
#include <core/IdentifiedChildrenVisitor.h>
#include <turtle/mock.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

#define TRY try {
#define CATCH } \
    catch( std::exception& e ) \
    { \
        BOOST_ERROR( e.what() ); \
    } catch( ... ) \
    { \
        BOOST_ERROR( "unknown exception" ); \
    }

namespace core
{
    struct ConfigureFixture
    {
        ConfigureFixture()
        {
            MOCK_EXPECT( api.Configure ).once().calls( boost::bind( &ConfigureFixture::Configure, boost::ref( loader ), _1, _2 ) );
        }
        MOCK_STATIC_METHOD( SWORD_RegisterCommand, 6, void( const char* name, SWORD_CommandConstructor constructor, SWORD_CommandExecutor executor, \
                                                            SWORD_CommandExecutor pausedExecutor, SWORD_CommandDestructor destructor, void* userData ), RegisterCommand )
        MOCK_STATIC_METHOD( SWORD_PostEvent, 2, void( const char* name, SWORD_Model* event ), PostEvent )
        MOCK_STATIC_METHOD( SWORD_RegisterHook, 5, void( SWORD_Hook* destination, SWORD_Hook* previous, const SWORD_Hook hook, const char* key, const char* signature ), RegisterHook )
        MOCK_STATIC_METHOD( SWORD_UseHook, 3, void( SWORD_Hook* destination, const char* key, const char* signature ), UseHook )
        MOCK_STATIC_METHOD( SWORD_CreateEvent, 0, SWORD_Model*(), CreateEvent )
        MOCK_STATIC_METHOD( SWORD_CreateEffect, 0, SWORD_Model*(), CreateEffect )

        static void SWORD_PostEffect( SWORD_Model* effect, const SWORD_Model* target )
        {
            TRY
                BOOST_REQUIRE( effect );
                BOOST_REQUIRE( target );
                PostEffect( effect, core::Convert( target )->Context() );
            CATCH
        }
        MOCK_STATIC_METHOD( PostEffect, 2, void( SWORD_Model* effect, const std::string& target ), PostEffect )
        void ExpectEffect( const core::Model& target, const sword::test::ModelBuilder& effect ) // $$$$ MCO 2012-04-11: invert parameters to match PostEffect
        {
            models_.push_back( boost::make_shared< core::Model >() );
            MOCK_EXPECT( CreateEffect ).once().returns( Convert( models_.back().get() ) );
            MOCK_EXPECT( PostEffect ).once().with( effect, target.Context() );
        }
        void ExpectEffect( const core::Model& target )
        {
            models_.push_back( boost::make_shared< core::Model >() );
            MOCK_EXPECT( CreateEffect ).once().returns( Convert( models_.back().get() ) );
            MOCK_EXPECT( PostEffect ).once().with( mock::any, target.Context() );
        }
        void ExpectEvent( const std::string& name, const sword::test::ModelBuilder& event )
        {
            models_.push_back( boost::make_shared< core::Model >() );
            MOCK_EXPECT( CreateEvent ).once().returns( Convert( models_.back().get() ) );
            MOCK_EXPECT( PostEvent ).once().with( name, event );
        }
        void ExpectEvent( const std::string& name )
        {
            models_.push_back( boost::make_shared< core::Model >() );
            MOCK_EXPECT( CreateEvent ).once().returns( Convert( models_.back().get() ) );
            MOCK_EXPECT( PostEvent ).once().with( name, mock::any );
        }

        static void SWORD_PostRemove( const SWORD_Model* target )
        {
            TRY
                BOOST_REQUIRE( target );
                PostRemove( core::Convert( target )->Context() );
            CATCH
        }
        MOCK_STATIC_METHOD( PostRemove, 1, void( const std::string& target ), PostRemove )
        void ExpectRemove( const core::Model& target )
        {
            MOCK_EXPECT( PostRemove ).once().with( target.Context() );
        }

        #define DECLARE_GET_SET( TYPE_NAME, TYPE ) \
        static int SWORD_Get##TYPE_NAME( const SWORD_Model* node, TYPE* value ) \
        { \
            TRY \
                BOOST_REQUIRE( node ); \
                BOOST_REQUIRE( value ); \
                *value = *core::Convert( node ); \
                return true; \
            CATCH \
            return false; \
        } \
        static int SWORD_Set##TYPE_NAME( SWORD_Model* node, TYPE value ) \
        { \
            TRY \
                BOOST_REQUIRE( node ); \
                *core::Convert( node ) = value; \
                return true; \
            CATCH \
            return false; \
        }
        DECLARE_GET_SET( Int8,   int8_t )
        DECLARE_GET_SET( Int16,  int16_t )
        DECLARE_GET_SET( Int32,  int32_t )
        DECLARE_GET_SET( Int64,  int64_t )
        DECLARE_GET_SET( Uint8,  uint8_t )
        DECLARE_GET_SET( Uint16, uint16_t )
        DECLARE_GET_SET( Uint32, uint32_t )
        DECLARE_GET_SET( Uint64, uint64_t )
        DECLARE_GET_SET( Float,  float )
        DECLARE_GET_SET( Double, double )
        #undef DECLARE_GET_SET

        static int SWORD_GetBool( const SWORD_Model* node, int* value )
        {
            TRY
                BOOST_REQUIRE( node );
                BOOST_REQUIRE( value );
                *value = *core::Convert( node );
                return true;
            CATCH
            return false;
        }
        static int SWORD_GetText( const SWORD_Model* node, const char** value, size_t* size )
        {
            TRY
                BOOST_REQUIRE( node );
                BOOST_REQUIRE( value );
                BOOST_REQUIRE( size );
                const std::string& text = *Convert( node );
                *value = text.c_str();
                *size = text.size();
                return true;
            CATCH
            return false;
        }
        static int SWORD_SetText( SWORD_Model* node, const char* value, size_t size )
        {
            TRY
                BOOST_REQUIRE( node );
                BOOST_REQUIRE( value );
                *Convert( node ) = std::string( value, size );
                return true;
            CATCH
            return false;
        }
        static int SWORD_CopyValue( const SWORD_Model* from, SWORD_Model* to )
        {
            TRY
                BOOST_REQUIRE( from );
                BOOST_REQUIRE( to );
                *Convert( to ) = *Convert( from );
                return true;
            CATCH
            return false;
        }
        static int SWORD_GetSize( const SWORD_Model* node, size_t* size )
        {
            TRY
                BOOST_REQUIRE( node );
                BOOST_REQUIRE( size );
                *size = core::Convert( node )->GetSize();
                return true;
            CATCH
            return false;
        }
        static const SWORD_Model* SWORD_GetElement( const SWORD_Model* node, size_t index )
        {
            TRY
                BOOST_REQUIRE( node );
                return core::Convert( &core::Convert( node )->GetElement( index ) );
            CATCH
            return 0;
        }
        static SWORD_Model* SWORD_AddElement( SWORD_Model* parent )
        {
            TRY
                BOOST_REQUIRE( parent );
                return core::Convert( &core::Convert( parent )->AddElement() );
            CATCH
            return 0;
        }
        static const SWORD_Model* SWORD_GetNamedChild( const SWORD_Model* node, const char* key )
        {
            TRY
                BOOST_REQUIRE( node );
                BOOST_REQUIRE( key );
                return core::Convert( &(*core::Convert( node ))[ key ] );
            CATCH
            return 0;
        }
        static SWORD_Model* SWORD_SetNamedChild( SWORD_Model* node, const char* key )
        {
            TRY
                BOOST_REQUIRE( node );
                BOOST_REQUIRE( key );
                return core::Convert( &(*core::Convert( node ))[ key ] );
            CATCH
            return 0;
        }
        static int SWORD_VisitNamedChildren( const SWORD_Model* parent, SWORD_NamedChildrenVisitor visitor, void* userData )
        {
            TRY
                BOOST_REQUIRE( parent );
                BOOST_REQUIRE( visitor );
                core:: NamedChildrenVisitor( *core::Convert( parent ), visitor, userData );
                return true;
            CATCH
            return false;
        }
        static int SWORD_VisitIdentifiedChildren( const SWORD_Model* parent, SWORD_IdentifiedChildrenVisitor visitor, void* userData )
        {
            TRY
                BOOST_REQUIRE( parent );
                BOOST_REQUIRE( visitor );
                core::IdentifiedChildrenVisitor( *core::Convert( parent ), visitor, userData );
                return true;
            CATCH
            return false;
        }
        static const SWORD_Model* SWORD_GetIdentifiedChild( const SWORD_Model* parent, size_t key )
        {
            TRY
                BOOST_REQUIRE( parent );
                return core::Convert( &(*core::Convert( parent ))[ key ] );
            CATCH
            return 0;
        }
        static SWORD_Model* SWORD_SetIdentifiedChild( SWORD_Model* parent, size_t key )
        {
            TRY
                BOOST_REQUIRE( parent );
                return core::Convert( &(*core::Convert( parent ))[ key ] );
            CATCH
            return 0;
        }
        static int SWORD_GetUserData( const SWORD_Model* node, void** value )
        {
            TRY
                BOOST_REQUIRE( node );
                BOOST_REQUIRE( value );
                (*value) = core::Convert( node )->GetData()->Get();
                return true;
            CATCH
            return false;
        }
        static int SWORD_CopyModel( const SWORD_Model* source, SWORD_Model* target )
        {
            TRY
                BOOST_REQUIRE( source );
                BOOST_REQUIRE( target );
                *Convert( target ) = *Convert( source );
                return true;
            CATCH
                return false;
        }
        MOCK_STATIC_METHOD( SWORD_Log, 2, void( SWORD_LogLevel level, const char* message ), Log )

        static void Configure( core::ModuleLoader& loader, void* module, const std::string& name )
        {
            SWORD_Api api;
#define API_SETTER( function, result, parameters ) api.function = function;
    SWORD_FUNCTION_LIST( API_SETTER )
#undef API_SETTER
            loader.LoadFunction< void (*)( SWORD_Api* ) >( module, name, "SWORD_InitializeApi" )( &api );
        }
        core::MockModuleApi api;
        core::ModuleLoader loader;
        std::list< boost::shared_ptr< core::Model > > models_;
    };
    struct LoadFixture : public ConfigureFixture
    {
        LoadFixture( const std::string& name )
            : module( loader, api, name, SWORD_API_VERSION )
        {}
        virtual ~LoadFixture()
        {
            models_.clear();
            module.Deinitialize();
        }
        core::Module module;
    };
    struct ApiFixture : public LoadFixture, core::Logger_ABC
    {
        template< typename T >
        static void RegisterHookHelper( core::Hooks& hooks, SWORD_Hook* destination, SWORD_Hook* previous, const SWORD_Hook hook, const char* key, const char* signature )
        {
            hooks.RegisterHook( reinterpret_cast< T* >( destination ), reinterpret_cast< T* >( previous ), reinterpret_cast< T >( hook ), key, signature );
        }
        template< typename T >
        static void UseHookHelper( core::Hooks& hooks, SWORD_Hook* destination, const char* key, const char* signature )
        {
            hooks.UseHook( reinterpret_cast< T* >( destination ), key, signature );
        }

        ApiFixture( const std::string& name )
            : LoadFixture( name )
            , commands( model )
        {
            model[ "profiling" ] = false;
        }
        virtual ~ApiFixture()
        {
            if( ! std::uncaught_exception() )
                mock::verify();
            mock::reset();
        }
        void ExpectRegisterCommand( const std::string& command )
        {
            MOCK_EXPECT( RegisterCommand )
                .once().with( command, mock::any, mock::any, mock::any, mock::any, mock::any )
                .calls( boost::bind( &core::Commands::Register, &commands, _1, _2, _3, _4, _5, _6 ) );
        }
        virtual void Log( core::Level /*level*/, const std::string& message ) const
        {
            BOOST_FAIL( message );
        }
        void PostCommand( const std::string& name, const Model& parameters )
        {
            commands.Post( "default", name, parameters );
        }
        size_t StartCommand( const std::string& name, const Model& parameters )
        {
            return commands.Start( "default", name, parameters );
        }
        void StopCommand( std::size_t command )
        {
            commands.Stop( command );
        }
        void ExecuteCommands()
        {
            commands.Execute( "default" );
        }
        core::Model model;
        core::Commands commands;
        core::Hooks hooks;
    };
}

#define HOOKS( APPLY ) \
    REGISTERED_HOOKS( APPLY ) \
    USED_HOOKS( APPLY ) \
    REGISTERED_AND_USED_HOOKS( APPLY )

#define MOCK_HOOK( hook, arity, result, parameters ) \
    typedef result ( *hook##Hook ) parameters; \
    hook##Hook hook##_; \
    MOCK_STATIC_METHOD( hook, arity, result parameters, hook )

#define EXPECT_REGISTER_HOOK( hook, arity, result, parameters ) \
    MOCK_EXPECT( RegisterHook ).at_least( 1 ).with( mock::any, mock::any, mock::any, #hook, #result#parameters ).calls( boost::bind( &RegisterHookHelper< hook##Hook >, boost::ref( hooks ), _1, _2, _3, _4, _5 ) );

#define EXPECT_USE_HOOK( hook, arity, result, parameters ) \
    MOCK_EXPECT( UseHook ).at_least( 1 ).with( mock::any, #hook, #result#parameters ).calls( boost::bind( &UseHookHelper< hook##Hook >, boost::ref( hooks ), _1, _2, _3 ) );

#define REGISTER_HOOK( hook, arity, result, parameters ) \
    hooks.RegisterHook< hook##Hook >( 0, &hook##_, hook, #hook, #result#parameters );

#define FORWARD_HOOK( hook, arity, result, parameters ) \
    if( hook##_ ) MOCK_EXPECT( hook ).calls( boost::ref( hook##_ ) );

#define INITIALIZE_HOOK( hook, number, result, parameters ) \
    , hook##_( 0 )

#define HOOK_FIXTURE( APPLY_HOOKS ) \
namespace core \
{ \
struct HookFixture : core::ApiFixture \
{ \
    HookFixture( const std::string& name, const std::vector< std::string >& commands ) \
        : ApiFixture( name ) \
        APPLY_HOOKS( INITIALIZE_HOOK ) \
    { \
        REGISTERED_HOOKS( EXPECT_REGISTER_HOOK ) \
        USED_HOOKS( EXPECT_USE_HOOK ) \
        REGISTERED_AND_USED_HOOKS( EXPECT_REGISTER_HOOK ) \
        REGISTERED_AND_USED_HOOKS( EXPECT_USE_HOOK ) \
        BOOST_FOREACH( const std::string& name, commands ) \
            ExpectRegisterCommand( name ); \
        module.Initialize( model ); \
        mock::verify(); \
        mock::reset(); \
        APPLY_HOOKS( REGISTER_HOOK ) \
        hooks.Resolve( *this ); \
        APPLY_HOOKS( FORWARD_HOOK ) \
    } \
    APPLY_HOOKS( MOCK_HOOK ) \
};\
}

#endif // MODULE_TESTER_API_FIXTURE_H

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ScriptPlugin.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include "dispatcher/Profile_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"
#include "directia/Brain.h"
#include "dispatcher/Registrable_ABC.h"
#include "Actions.h"
#include "Script.h"
#include "MiscEvents.h"
#include "ConditionFacade.h"
#include "AgentManipulator.h"
#include "AutomatManipulator.h"
#include "KnowledgeManipulator.h"
#include "SimulationCommands.h"
#include "ClientCommands.h"
#include "ScriptCommands.h"
#include "ExtensionFactory.h"
#include "tools/MessageDispatcher_ABC.h"
#include "dispatcher/CompositeRegistrable.h"

#include "MT/MT_Logger/MT_Logger_lib.h"
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>

namespace bfs = boost::filesystem;
using namespace script;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ScriptPlugin constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
ScriptPlugin::ScriptPlugin( Model& model, const Config& config, SimulationPublisher_ABC& publisher, tools::MessageDispatcher_ABC& dispatcher, ClientPublisher_ABC& clients, LinkResolver_ABC& resolver, CompositeRegistrable& registrables )
    : model_     ( model )
    , config_    ( config )
    , registrables_( registrables )
    , controller_( new kernel::Controller() )
    , converter_ ( new kernel::CoordinateConverter( config ) )
    , factory_   ( new ExtensionFactory( *controller_, *converter_ ) )
    , time_      ( -1 )
    , reset_     ( true )
{
    model_.RegisterFactory( *factory_ );
    registrables_.Add( new RegistrableProxy( *this ) );
    registrables_.Add( new Actions( *controller_, config, model, publisher ) );
    registrables_.Add( new ConditionFacade( *controller_, *converter_, model_ ) );
    registrables_.Add( new AgentManipulator::Registrar() );
    registrables_.Add( new AutomatManipulator::Registrar() );
    registrables_.Add( new KnowledgeManipulator::Registrar() );
    registrables_.Add( new SimulationCommands( publisher, *converter_ ) );
    registrables_.Add( new ClientCommands( clients, resolver ) );
    registrables_.Add( new ScriptCommands( *controller_ ) );
    dispatcher.RegisterMessage( *this, &ScriptPlugin::OnReceiveClientToMessenger );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
ScriptPlugin::~ScriptPlugin()
{
    model_.UnregisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::Receive
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void ScriptPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    if( message.msg.t == T_MsgsSimToClient_msg_msg_control_begin_tick )
        controller_->Update( events::TickEnded( message.msg.u.msg_control_end_tick->current_tick ) );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::NotifyClientAuthenticated
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void ScriptPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& profile )
{
    controller_->Update( events::ClientJoined( client.GetEndpoint(), profile.GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::NotifyClientLeft
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void ScriptPlugin::NotifyClientLeft( ClientPublisher_ABC& client )
{
    controller_->Update( events::ClientLeft( client.GetEndpoint() ) );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::OnReceiveClientToMessenger
// Created: SBO 2008-06-27
// -----------------------------------------------------------------------------
void ScriptPlugin::OnReceiveClientToMessenger( const std::string&, const ASN1T_MsgsClientToMessenger& message )
{
    switch( message.t )
    {
        case T_MsgsClientToMessenger_msg_text_message:
            MT_LOG_INFO_MSG( message.u.msg_text_message->message );
            controller_->Update( *message.u.msg_text_message ); break;
        default:
            break;
    }
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::Update
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
void ScriptPlugin::Update()
{
    if( reset_ )
    {
        pending_.clear();
        scripts_.clear();
        reset_ = false;
        LoadScripts();
    }

    ApplyPendings();

    long newTime = clock();
    if( time_ > 0 )
    {
        const float delta = float( newTime - time_ ) / float( CLOCKS_PER_SEC );
        controller_->Update( events::TimeFlowed( delta ) );
    }
    time_ = newTime;
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::LoadScripts
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void ScriptPlugin::LoadScripts()
{
    const bfs::path dir( config_.BuildExerciseChildFile( "scripts" ), bfs::native );
    if( ! bfs::exists( dir ) )
        return;
    for( bfs::directory_iterator it( dir ); it !=  bfs::directory_iterator(); ++it )
    {
        const bfs::path& file = *it;
        if( !bfs::is_directory( file ) && bfs::extension( file ) == ".lua" )
            LoadScript( file.native_file_string() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::LoadScript
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void ScriptPlugin::LoadScript( const std::string& file )
{
    try
    {
        scripts_.push_back( new Script( file, registrables_ ) );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error reading script '" << file << "' : " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::RegisterIn
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
void ScriptPlugin::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "plugin", this );
    brain.RegisterFunction( "Reset", &ScriptPlugin::Reset );
    brain.RegisterFunction( "Later", &ScriptPlugin::Later );
    brain.RegisterObject( "coord", this );
    brain.RegisterFunction( "ToUtm", &ScriptPlugin::ToUtm );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::Reset
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
void ScriptPlugin::Reset()
{
    reset_ = true;
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::Later
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void ScriptPlugin::Later( const directia::WrappedScriptFunction& function )
{
    pending_.push_back( new directia::WrappedScriptFunction( function ) );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::ToUtm
// Created: AGE 2008-07-17
// -----------------------------------------------------------------------------
std::string ScriptPlugin::ToUtm( const dispatcher::Position& position )
{
    return converter_->ConvertToMgrs( dispatcher::ToPoint( position ) );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::ApplyPendings
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void ScriptPlugin::ApplyPendings()
{
    boost::ptr_vector< directia::WrappedScriptFunction > pending;
    pending.swap( pending_ );
    std::for_each( pending.begin(), pending.end(), boost::apply<bool>() );
}


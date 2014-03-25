// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ScriptPlugin.h"
#include "Actions.h"
#include "Script.h"
#include "MiscEvents.h"
#include "ConditionFacade.h"
#include "AgentManipulator.h"
#include "AutomatManipulator.h"
#include "KnowledgeManipulator.h"
#include "ObjectManipulator.h"
#include "ObjectKnowledgeManipulator.h"
#include "PopulationManipulator.h"
#include "PopulationKnowledgeManipulator.h"
#include "SimulationCommands.h"
#include "ClientCommands.h"
#include "ScriptCommands.h"
#include "ExtensionFactory.h"
#include "ModelFacade.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"
#include "dispatcher/CompositeRegistrable.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Profile_ABC.h"
#include "dispatcher/Registrable_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "tools/FileWrapper.h"
#include "tools/MessageDispatcher_ABC.h"
#include <tools/Path.h>
#include "MT_Tools/MT_Logger.h"
#include <directia/brain/Brain.h>
#include <boost/bind/apply.hpp>

using namespace plugins::script;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ScriptPlugin constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
ScriptPlugin::ScriptPlugin( Model_ABC& model,
                            const Config& config,
                            SimulationPublisher_ABC& publisher,
                            tools::MessageDispatcher_ABC& dispatcher,
                            ClientPublisher_ABC& clients,
                            const dispatcher::LinkResolver_ABC& resolver,
                            CompositeRegistrable& registrables )
    : model_       ( model )
    , config_      ( config )
    , controller_  ( new kernel::Controller() )
    , registrables_( registrables )
    , converter_   ( new kernel::CoordinateConverter( config ) )
    , factory_     ( new ExtensionFactory( *controller_, *converter_, publisher ) )
    , time_        ( -1 )
    , reset_       ( true )
    , loaded_      ( false )
    , tickDuration_( 10 )
{
    model_.RegisterFactory( *factory_ );
    registrables_.Add( new RegistrableProxy( *this ) );
    registrables_.Add( new Actions( *controller_, config_, model, publisher ) );
    registrables_.Add( new ConditionFacade( *controller_, *converter_, model_, config_ ) );
    registrables_.Add( new AgentManipulator::Registrar() );
    registrables_.Add( new AutomatManipulator::Registrar() );
    registrables_.Add( new KnowledgeManipulator::Registrar() );
    registrables_.Add( new ObjectManipulator::Registrar() );
    registrables_.Add( new ObjectKnowledgeManipulator::Registrar() );
    registrables_.Add( new PopulationManipulator::Registrar() );
    registrables_.Add( new PopulationKnowledgeManipulator::Registrar() );
    registrables_.Add( new SimulationCommands( publisher, *converter_ ) );
    registrables_.Add( new ClientCommands( clients, resolver ) );
    registrables_.Add( new ScriptCommands( *controller_ ) );
    registrables_.Add( new ModelFacade( model_ ) );
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
void ScriptPlugin::Receive( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_control_information() )
    {
        if( wrapper.message().control_information().has_tick_duration() )
            tickDuration_ = wrapper.message().control_information().tick_duration();
    }
    else if( wrapper.message().has_control_send_current_state_begin() )
    {
        loaded_ = true;
        Update();
    }
    else if( wrapper.message().has_control_begin_tick() )
        controller_->Update( events::SimulationTimeChanged( wrapper.message().control_begin_tick().date_time().data() ) );
    else if( wrapper.message().has_control_end_tick() )
        controller_->Update( events::TickEnded( wrapper.message().control_end_tick().current_tick(), tickDuration_ ) );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::Receive
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
void ScriptPlugin::Receive( const sword::AarToClient& wrapper )
{
    if( wrapper.message().has_indicator() )
        controller_->Update( events::IndicatorChanged( wrapper.message().indicator().name(), wrapper.message().indicator().value() ) );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::NotifyClientAuthenticated
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void ScriptPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& /*client*/, const std::string& link,
                                              Profile_ABC& profile, unsigned int /*clientId*/, bool uncounted )
{
    if( uncounted )
        return;
    controller_->Update( events::ClientJoined( link, profile.GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::NotifyClientLeft
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void ScriptPlugin::NotifyClientLeft( ClientPublisher_ABC& /*client*/, const std::string& link, bool uncounted )
{
    if( uncounted )
        return;
    controller_->Update( events::ClientLeft( link ) );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::OnReceiveClientToMessenger
// Created: SBO 2008-06-27
// -----------------------------------------------------------------------------
void ScriptPlugin::OnReceiveClientToMessenger( const std::string&, const sword::ClientToMessenger& wrapper )
{
    if( wrapper.message().has_text_message() )
    {
        MT_LOG_INFO_MSG( wrapper.message().text_message().message() );
        controller_->Update( wrapper.message().text_message() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::Update
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
void ScriptPlugin::Update()
{
    if( loaded_ && reset_ )
    {
        pending_.clear();
        scripts_.clear();
        reset_ = false;
        LoadScripts();
    }

    if ( loaded_ )
    {
        ApplyPendings();

        long newTime = clock();
        if( time_ > 0 )
        {
            const float delta = float( newTime - time_ ) / float( CLOCKS_PER_SEC );
            controller_->Update( events::TimeFlowed( delta ) );
        }
        time_ = newTime;
    }
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::LoadScripts
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void ScriptPlugin::LoadScripts()
{
    const tools::Path dir( config_.BuildExerciseChildFile( "scripts" ) );
    if( dir.Exists() )
    {
        for( auto it = dir.begin(); it != dir.end(); ++it )
        {
            const tools::Path& file = *it;
            if( !file.IsDirectory() && file.Extension() == ".lua" )
                LoadScript( file );
        }
    }
    const std::vector< tools::Path >& orderFiles = config_.GetStartupOrderFiles();
    if( !orderFiles.empty() )
    {
        try
        {
            tools::Path luaFile = GenerateOrdersScript( orderFiles );
            LoadScript( luaFile );
            luaFile.Remove();
        }
        catch( ... )
        {
            // NOTHING
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::LoadScript
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void ScriptPlugin::LoadScript( const tools::Path& file )
{
    try
    {
        scripts_.push_back( new Script( file, registrables_ ) );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error reading script '" << file << "': " << tools::GetExceptionMsg( e ) );
    }
}

namespace directia
{
    void UsedByDIA( ScriptPlugin* ) {}
    void ReleasedByDIA( ScriptPlugin* ) {}
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::RegisterIn
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
void ScriptPlugin::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "plugin" ] = this;
    brain.Register( "Reset", &ScriptPlugin::Reset );
    brain.Register( "Later", &ScriptPlugin::Later );
    brain[ "coord" ] = this;
    brain.Register( "ToUtm", &ScriptPlugin::ToUtm );
    brain.Register( "UtmPosition", &ScriptPlugin::UtmPosition );
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
void ScriptPlugin::Later( const directia::tools::binders::ScriptRef& function )
{
    pending_.push_back( new directia::tools::binders::ScriptRef( function ) );
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
// Name: ScriptPlugin::FromUtm
// Created: SBO 2009-08-21
// -----------------------------------------------------------------------------
dispatcher::Position ScriptPlugin::UtmPosition( const std::string& utm )
{
    return dispatcher::ToPosition( converter_->ConvertToXY( utm ) );
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::GenerateOrdersScript
// Created: JSR 2012-03-02
// -----------------------------------------------------------------------------
tools::Path ScriptPlugin::GenerateOrdersScript( const std::vector< tools::Path >& files )
{
    tools::Path templateFile = config_.BuildResourceChildFile( "StartupOrdersTemplate.lua" );
    tools::Ifstream file( templateFile );
    const tools::Path dest = config_.BuildExerciseChildFile( "StartupOrders.lua" );
    tools::Ofstream destFile( dest );
    std::string line;
    while( destFile.good() && std::getline( file, line ) )
    {
        if( line != "$$$$" )
            destFile << line << '\n';
        else
        {
            for( auto it = files.begin(); it != files.end(); ++it )
            {
                tools::Path orderFile = *it;
                orderFile.ReplaceExtension();
                destFile << "actions:StartScheduler( \"../../" << orderFile.ToUTF8() << "\" )\n";
            }
        }
    }
    return dest;
}

// -----------------------------------------------------------------------------
// Name: ScriptPlugin::ApplyPendings
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void ScriptPlugin::ApplyPendings()
{
    boost::ptr_vector< directia::tools::binders::ScriptRef > pending;
    pending.swap( pending_ );
    std::for_each( pending.begin(), pending.end(), boost::apply<void>() );
}

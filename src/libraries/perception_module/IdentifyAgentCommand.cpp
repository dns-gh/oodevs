// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "IdentifyAgentCommand.h"
#include "PerceptionLevel.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include "wrapper/Hook.h"

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( GetAgentListWithinLocalisation, void, ( const SWORD_Model* root, const TER_Localisation* localization,
                                                      void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )

// -----------------------------------------------------------------------------
// Name: IdentifyAgentCommand constructor
// Created: SLI 2012-03-29
// -----------------------------------------------------------------------------
IdentifyAgentCommand::IdentifyAgentCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_  ( parameters[ "identifier" ] )
    , localization_( static_cast< const TER_Localisation* >( parameters[ "localization" ].GetUserData() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdentifyAgentCommand destructor
// Created: SLI 2012-03-29
// -----------------------------------------------------------------------------
IdentifyAgentCommand::~IdentifyAgentCommand()
{
    // NOTHING
}

struct AgentVisitor : private boost::noncopyable
{
    AgentVisitor( wrapper::Effect& effect, bool recordMode )
        : effect_    ( effect )
        , recordMode_( recordMode )
    {}
    static void Add( const SWORD_Model* agent, void* userData )
    {
        wrapper::Node notification = static_cast< AgentVisitor* >( userData )->effect_.AddElement();
        notification[ "target" ] = reinterpret_cast< size_t >( wrapper::View( agent )[ "pion" ].GetUserData() );
        notification[ "level" ] = PerceptionLevel::identified_.GetID();
        notification[ "recorded" ] = static_cast< AgentVisitor* >( userData )->recordMode_;
    }
    wrapper::Effect& effect_;
    bool recordMode_;
};

// -----------------------------------------------------------------------------
// Name: IdentifyAgentCommand::Execute
// Created: SLI 2012-03-29
// -----------------------------------------------------------------------------
void IdentifyAgentCommand::Execute( const wrapper::View& model ) const
{
    const wrapper::View& entity = model[ "entities" ][ identifier_ ];
    wrapper::Effect effect( entity[ "perceptions/notifications/agents-in-zone" ] );
    AgentVisitor visitor( effect, entity[ "perceptions/record-mode/activated" ] );
    GET_HOOK( GetAgentListWithinLocalisation )( model, localization_, &AgentVisitor::Add, &visitor );
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: IdentifyAgentCommand::ExecutePaused
// Created: SLI 2012-03-29
// -----------------------------------------------------------------------------
void IdentifyAgentCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}

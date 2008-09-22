// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "SimpleEntityCondition.h"
#include "AgentManipulator.h"
#include "KnowledgeManipulator.h"
#include "AutomatManipulator.h"
#include "dispatcher/Agent.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/Automat.h"

using namespace script;

// -----------------------------------------------------------------------------
// Name: EntityConditionBase::Trigger
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void EntityConditionBase::Trigger( BaseCondition& that, const dispatcher::Agent& entity )
{
    if( const AgentManipulator* manipulator = entity.Retrieve< AgentManipulator >() )
        that.Trigger( manipulator );
}

// -----------------------------------------------------------------------------
// Name: EntityConditionBase::Trigger
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void EntityConditionBase::Trigger( BaseCondition& that, const dispatcher::AgentKnowledge& entity )
{
    const KnowledgeManipulator* k = entity.Retrieve< KnowledgeManipulator >();
    const AgentManipulator* a = entity.agent_.Retrieve< AgentManipulator >();
    if( k && a )
        that.Trigger( k, a );
}

// -----------------------------------------------------------------------------
// Name: AgentConditionBase::Trigger
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void EntityConditionBase::Trigger( BaseCondition& that, const dispatcher::Automat& entity )
{
    if( const AutomatManipulator* manipulator = entity.Retrieve< AutomatManipulator >() )
        that.Trigger( manipulator );
}
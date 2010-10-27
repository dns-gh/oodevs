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
#include "ObjectManipulator.h"
#include "ObjectKnowledgeManipulator.h"
#include "PopulationManipulator.h"
#include "PopulationKnowledgeManipulator.h"
#include "dispatcher/Agent.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Object_ABC.h"
#include "dispatcher/ObjectKnowledge.h"
#include "dispatcher/Population.h"
#include "dispatcher/PopulationKnowledge.h"

using namespace plugins::script;

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
    const AgentManipulator* a = entity.GetEntity() ? entity.GetEntity()->Retrieve< AgentManipulator >() : 0;
    if( k && a )
        that.Trigger( k, a );
}

// -----------------------------------------------------------------------------
// Name: EntityConditionBase::Trigger
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void EntityConditionBase::Trigger( BaseCondition& that, const dispatcher::Automat& entity )
{
    if( const AutomatManipulator* manipulator = entity.Retrieve< AutomatManipulator >() )
        that.Trigger( manipulator );
}

// -----------------------------------------------------------------------------
// Name: EntityConditionBase::Trigger
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
void EntityConditionBase::Trigger( BaseCondition& that, const dispatcher::Object_ABC& entity )
{
    if( const ObjectManipulator* manipulator = entity.Retrieve< ObjectManipulator >() )
        that.Trigger( manipulator );
}

// -----------------------------------------------------------------------------
// Name: EntityConditionBase::Trigger
// Created: DSO 2010-07-27
// -----------------------------------------------------------------------------
void EntityConditionBase::Trigger( BaseCondition& that, const dispatcher::ObjectKnowledge& entity )
{
    const ObjectKnowledgeManipulator* k = entity.Retrieve< ObjectKnowledgeManipulator >();
    const ObjectManipulator* o = entity.GetEntity() ? entity.GetEntity()->Retrieve< ObjectManipulator >() : 0;
    if( k && o )
        that.Trigger( k, o );
}

// -----------------------------------------------------------------------------
// Name: EntityConditionBase::Trigger
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
void EntityConditionBase::Trigger( BaseCondition& that, const dispatcher::Population& entity )
{
    if( const PopulationManipulator* manipulator = entity.Retrieve< PopulationManipulator >() )
        that.Trigger( manipulator );
}

// -----------------------------------------------------------------------------
// Name: EntityConditionBase::Trigger
// Created: DSO 2010-07-27
// -----------------------------------------------------------------------------
void EntityConditionBase::Trigger( BaseCondition& that, const dispatcher::PopulationKnowledge& entity )
{
    const PopulationKnowledgeManipulator* k = entity.Retrieve< PopulationKnowledgeManipulator >();
    const PopulationManipulator* p = entity.GetEntity() ? entity.GetEntity()->Retrieve< PopulationManipulator >() : 0;
    if( k && p )
        that.Trigger( k, p );
}

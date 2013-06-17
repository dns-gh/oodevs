// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamPopulationKnowledge.h"
#include "actions/PopulationKnowledge.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::ParamPopulationKnowledge( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameter< kernel::PopulationKnowledge_ABC >( builder, parameter )
    , converter_( builder.GetAgentKnowledgeConverter() )
    , agent_( builder.GetControllers() )
{
    assert( converter_ != 0 );
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::~ParamPopulationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    if( !agent_ )
        return;
    const kernel::PopulationKnowledge_ABC* knowledge = converter_->Find( entity, *agent_ );
    if( knowledge )
        EntityParameter< kernel::PopulationKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< kernel::PopulationKnowledge_ABC > > param( new actions::parameters::PopulationKnowledge( parameter_, controller_ ) );
    EntityParameter< kernel::PopulationKnowledge_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::BuildInterface
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
QWidget* ParamPopulationKnowledge::BuildInterface( const QString& objectName, QWidget* parent )
{
    QWidget* result = EntityParameter< kernel::PopulationKnowledge_ABC >::BuildInterface( objectName, parent );
    group_->setEnabled( false );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::SetEntity
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::SetEntity( const kernel::Entity_ABC* entity )
{
    agent_ = entity;
    if( group_ )
        group_->setEnabled( IsInParam() || entity != 0 );
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::Visit( const actions::parameters::PopulationKnowledge& param )
{
    InternalVisit( param );
}

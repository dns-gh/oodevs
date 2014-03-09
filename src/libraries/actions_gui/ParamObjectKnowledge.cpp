// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamObjectKnowledge.h"
#include "actions/ObjectKnowledge.h"
#include "InterfaceBuilder_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/ObjectType.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamObjectKnowledge::ParamObjectKnowledge( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameter< kernel::ObjectKnowledge_ABC >( builder, parameter )
    , converter_( builder.GetObjectKnowledgeConverter() )
    , agent_( builder.GetControllers() )
{
    assert( converter_ != 0 );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledge::~ParamObjectKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    if( agent_ && !entity.GetType().IsUrban() && parameter_.HasGenObject( entity.GetType().GetType() ) )
    {
        const kernel::Hierarchies* hierarchies = agent_->Retrieve< kernel::CommunicationHierarchies >();
        if( hierarchies )
            if( const kernel::Entity_ABC* superior = hierarchies->GetSuperior() )
                if( const kernel::ObjectKnowledge_ABC* knowledge = converter_->Find( entity, *superior ) )
                    EntityParameter< kernel::ObjectKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< kernel::ObjectKnowledge_ABC > > param( new actions::parameters::ObjectKnowledge( parameter_, controllers_.controller_ ) );
    EntityParameter< kernel::ObjectKnowledge_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::BuildInterface
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
QWidget* ParamObjectKnowledge::BuildInterface( const QString& objectName, QWidget* parent )
{
    QWidget* result = EntityParameter< kernel::ObjectKnowledge_ABC >::BuildInterface( objectName, parent );
    group_->setEnabled( false );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::SetEntity
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::SetEntity( const kernel::Entity_ABC* entity )
{
    agent_ = entity;
    if( group_ )
        group_->setEnabled( IsInParam() || entity != 0 );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::Visit( const actions::parameters::ObjectKnowledge& param )
{
    InternalVisit( param );
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledge::Purge
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
void ParamObjectKnowledge::Purge()
{
    EntityParameter< kernel::ObjectKnowledge_ABC >::Purge();
    agent_ = 0;
}

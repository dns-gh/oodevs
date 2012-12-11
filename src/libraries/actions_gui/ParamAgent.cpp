// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamAgent.h"
#include "moc_ParamAgent.cpp"
#include "ListParameter.h"
#include "moc_ListParameter.cpp"
#include "actions/Agent.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "MissionInterface_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamAgent constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamAgent::ParamAgent( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameter< kernel::Agent_ABC >( builder, parameter )
    , superior_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgent destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgent::~ParamAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgent::NotifyContextMenu
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
void ParamAgent::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    if( IsInList() && parentList_->IsPotential( this ) )
    {
        superior_ = &entity;
        kernel::ContextMenu* listMenu = menu.SubMenu( paramInterface_.Title().toStdString(), parentList_->GetName(), true, paramInterface_.GetIndex( parentList_ ) );
        listMenu->insertItem( tools::translate( "Param_ABC", "Add automat's agents" ), this, SLOT( AddHierarchy() ), 0, -1, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamAgent::NotifyContextMenu
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
void ParamAgent::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    if( IsInList() && parentList_->IsPotential( this ) )
    {
        superior_ = &entity;
        kernel::ContextMenu* listMenu = menu.SubMenu( paramInterface_.Title().toStdString(), parentList_->GetName(), true, paramInterface_.GetIndex( parentList_ ) );
        listMenu->insertItem( tools::translate( "Param_ABC", "Add formation's agents" ), this, SLOT( AddHierarchy() ), 0, -1, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamAgent::AddHierarchy
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
void ParamAgent::AddHierarchy()
{
    assert( IsInList() );
    if( superior_ )
        AddHierarchy( *superior_ );
}

// -----------------------------------------------------------------------------
// Name: ParamAgent::AddHierarchy
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
void ParamAgent::AddHierarchy( const kernel::Entity_ABC& superior )
{
    assert( IsInList() );
    if( const kernel::Agent_ABC* agent = dynamic_cast< const kernel::Agent_ABC* >( &superior ) )
    {
        Param_ABC* param = parentList_->CreateElement();
        if( param == 0 )
            throw MASA_EXCEPTION( "ListParameter doesn't create a ParamAgent." );
        if( ParamAgent* paramAgent = dynamic_cast< ParamAgent* >( param ) )
            paramAgent->SetPotential( *agent );
        else
            throw MASA_EXCEPTION( "ListParameter doesn't create a ParamAgent." );
        param->OnMenuClick();
        parentList_->AddElement( *param );
    }
    else if( const kernel::Hierarchies* hierarchy = superior.Retrieve< kernel::TacticalHierarchies >() )
    {
        tools::Iterator< const kernel::Entity_ABC& > children = hierarchy->CreateSubordinateIterator();
        while( children.HasMoreElements() )
            AddHierarchy( children.NextElement() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamAgent::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ParamAgent::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< kernel::Agent_ABC > > param( new actions::parameters::Agent( parameter_, controller_ ) );
    EntityParameter< kernel::Agent_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

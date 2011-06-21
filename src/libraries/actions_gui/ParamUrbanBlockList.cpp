// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamUrbanBlock.h"
#include "ParamUrbanBlockList.h"
#include "actions/ParameterList.h"
#include "actions/UrbanBlock.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlockList constructor
// Created: FPO 2011-05-31
// -----------------------------------------------------------------------------
ParamUrbanBlockList::ParamUrbanBlockList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller )
    : EntityListParameter< kernel::Object_ABC >( parent, parameter, actions, controller )
    , parameter_( parameter )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlockList destructor
// Created: FPO 2011-05-31
// -----------------------------------------------------------------------------
ParamUrbanBlockList::~ParamUrbanBlockList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlockList::AddToMenu
// Created: FPO 2011-05-31
// -----------------------------------------------------------------------------
void ParamUrbanBlockList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tools::translate( "ListParameter", "%1: add item" ).arg( GetName() ), menu );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter< kernel::Object_ABC >* ParamUrbanBlockList::CreateElement
// Created: FPO 2011-05-31
// -----------------------------------------------------------------------------
EntityParameter< kernel::Object_ABC >* ParamUrbanBlockList::CreateElement( const kernel::Object_ABC& potential )
{
    return new ParamUrbanBlock( this, kernel::OrderParameter( tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ).ascii(), "urban block", false ), potential, controller_ );
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlockList::SetName
// Created: FPO 2011-05-31
// -----------------------------------------------------------------------------
void ParamUrbanBlockList::SetName( const QString& name )
{
    this->name_ = name;
    ListParameter::SetLabel( name );
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlockList::CommitTo
// Created: FPO 2011-05-31
// -----------------------------------------------------------------------------
void ParamUrbanBlockList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::ParameterList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}
